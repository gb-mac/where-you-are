#include "Faction/WYAFactionGeoSubsystem.h"
#include "Location/WYALocationSubsystem.h"
#include "Api/WYAApiClient.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogFactionGeo, Log, All);

const FString UWYAFactionGeoSubsystem::CacheSubdir = TEXT("WYATerritoryCache");

// ── Geohash (precision 5, ~4km cells) ────────────────────────────────────────
// Simple base32 geohash implementation — enough for region keying.

static const TCHAR* GeoHashChars = TEXT("0123456789bcdefghjkmnpqrstuvwxyz");

static FString ComputeGeohash(double Lat, double Lon, int32 Precision)
{
    double MinLat = -90.0,  MaxLat = 90.0;
    double MinLon = -180.0, MaxLon = 180.0;

    FString Hash;
    Hash.Reserve(Precision);

    int32 Bits = 0;
    int32 Bit  = 0;
    int32 Ch   = 0;
    bool  bEven = true;

    while (Hash.Len() < Precision)
    {
        if (bEven)
        {
            double Mid = (MinLon + MaxLon) / 2.0;
            if (Lon >= Mid) { Ch |= (1 << (4 - Bit)); MinLon = Mid; }
            else            { MaxLon = Mid; }
        }
        else
        {
            double Mid = (MinLat + MaxLat) / 2.0;
            if (Lat >= Mid) { Ch |= (1 << (4 - Bit)); MinLat = Mid; }
            else            { MaxLat = Mid; }
        }
        bEven = !bEven;

        if (++Bit == 5)
        {
            Hash.AppendChar(GeoHashChars[Ch]);
            Bit = 0;
            Ch  = 0;
        }
    }
    return Hash;
}

// ── Lifecycle ─────────────────────────────────────────────────────────────────

void UWYAFactionGeoSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Collection.InitializeDependency<UWYALocationSubsystem>();

    auto* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
    if (LocSys)
    {
        if (LocSys->IsResolved())
        {
            // Location already available (e.g. cached from previous session)
            OnLocationResolved(LocSys->GetOrigin(), true);
        }
        else
        {
            LocationResolvedHandle = LocSys->OnLocationResolved.AddUObject(
                this, &UWYAFactionGeoSubsystem::OnLocationResolved);
        }
    }
}

void UWYAFactionGeoSubsystem::Deinitialize()
{
    if (auto* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>())
    {
        LocSys->OnLocationResolved.Remove(LocationResolvedHandle);
    }
    Super::Deinitialize();
}

// ── Location callback ─────────────────────────────────────────────────────────

void UWYAFactionGeoSubsystem::OnLocationResolved(FWYAGeoCoord Coord, bool bSuccess)
{
    if (!bSuccess || !Coord.IsValid())
    {
        UE_LOG(LogFactionGeo, Warning, TEXT("Location resolve failed — territory unavailable"));
        OnTerritoryReady.Broadcast(false);
        return;
    }

    FetchRegion(Coord.Latitude, Coord.Longitude);
}

// ── Territory fetch ───────────────────────────────────────────────────────────

void UWYAFactionGeoSubsystem::FetchRegion(double Lat, double Lon)
{
    const FString RegionKey = ComputeRegionKey(Lat, Lon);
    UE_LOG(LogFactionGeo, Log, TEXT("Fetching territory for region %s (%.4f, %.4f)"),
        *RegionKey, Lat, Lon);

    // Try local cache first
    FWYATerritoryRegion Cached;
    if (LoadRegionCache(RegionKey, Cached))
    {
        CurrentRegion = Cached;
        bTerritoryReady = true;
        UE_LOG(LogFactionGeo, Log, TEXT("Territory loaded from cache — %d zones"), Cached.Zones.Num());
        OnTerritoryReady.Broadcast(true);
        // Still fetch from server to refresh (fire-and-forget update)
    }

    // Fetch from server — GET /v1/territory?lat=X&lon=Y
    auto* Api = GetGameInstance()->GetSubsystem<UWYAApiClient>();
    if (!Api) return;

    Api->GetTerritoryRegion(Lat, Lon,
        [this](bool bOk, TSharedPtr<FJsonObject> Json)
        {
            OnRegionResponse(bOk, Json);
        });
}

void UWYAFactionGeoSubsystem::OnRegionResponse(bool bSuccess, const TSharedPtr<FJsonObject>& Json)
{
    if (!bSuccess || !Json.IsValid())
    {
        if (!bTerritoryReady)
        {
            UE_LOG(LogFactionGeo, Warning, TEXT("Territory fetch failed and no cache available"));
            OnTerritoryReady.Broadcast(false);
        }
        return;
    }

    FWYATerritoryRegion NewRegion;
    if (!ParseRegion(Json, NewRegion))
    {
        UE_LOG(LogFactionGeo, Warning, TEXT("Territory parse failed"));
        if (!bTerritoryReady) OnTerritoryReady.Broadcast(false);
        return;
    }

    // Diff zones for change events (only if we already had data)
    if (bTerritoryReady)
    {
        for (const FWYATerritoryZone& NewZone : NewRegion.Zones)
        {
            const FWYATerritoryZone* OldZone = GetZoneAtCoord(NewZone.CentroidLat, NewZone.CentroidLon);
            if (OldZone && OldZone->PrimaryFaction != NewZone.PrimaryFaction)
            {
                UE_LOG(LogFactionGeo, Log, TEXT("Zone %s changed: %s → %s"),
                    *NewZone.ZoneId,
                    *WYAFactionToString(OldZone->PrimaryFaction),
                    *WYAFactionToString(NewZone.PrimaryFaction));
                OnZoneControlChanged.Broadcast(NewZone.ZoneId, NewZone.PrimaryFaction);
            }
        }
    }

    CurrentRegion = NewRegion;
    SaveRegionCache(NewRegion);

    if (!bTerritoryReady)
    {
        bTerritoryReady = true;
        UE_LOG(LogFactionGeo, Log, TEXT("Territory ready — %d zones in region %s"),
            NewRegion.Zones.Num(), *NewRegion.RegionKey);
        OnTerritoryReady.Broadcast(true);
    }
}

// ── JSON parsing ──────────────────────────────────────────────────────────────

bool UWYAFactionGeoSubsystem::ParseRegion(const TSharedPtr<FJsonObject>& Json,
                                           FWYATerritoryRegion& Out) const
{
    if (!Json.IsValid()) return false;

    Json->TryGetStringField(TEXT("regionKey"),   Out.RegionKey);
    Json->TryGetStringField(TEXT("generatedAt"), Out.GeneratedAt);

    if (Out.RegionKey.IsEmpty()) return false;

    const TArray<TSharedPtr<FJsonValue>>* ZoneArray = nullptr;
    if (!Json->TryGetArrayField(TEXT("zones"), ZoneArray)) return false;

    Out.Zones.Reserve(ZoneArray->Num());
    for (const TSharedPtr<FJsonValue>& ZoneVal : *ZoneArray)
    {
        const TSharedPtr<FJsonObject>* ZoneObj = nullptr;
        if (!ZoneVal->TryGetObject(ZoneObj)) continue;

        FWYATerritoryZone Zone;
        if (ParseZone(*ZoneObj, Zone))
        {
            Out.Zones.Add(MoveTemp(Zone));
        }
    }

    return Out.Zones.Num() > 0;
}

bool UWYAFactionGeoSubsystem::ParseZone(const TSharedPtr<FJsonObject>& Obj,
                                         FWYATerritoryZone& Out) const
{
    if (!Obj.IsValid()) return false;

    Obj->TryGetStringField(TEXT("zoneId"),      Out.ZoneId);
    Obj->TryGetStringField(TEXT("scoredAt"),    Out.ScoredAt);
    Obj->TryGetStringField(TEXT("primaryOsmTag"), Out.PrimaryOsmTag);
    Obj->TryGetNumberField(TEXT("centroidLat"), Out.CentroidLat);
    Obj->TryGetNumberField(TEXT("centroidLon"), Out.CentroidLon);

    double RadiusD = 200.0;
    Obj->TryGetNumberField(TEXT("radiusMetres"), RadiusD);
    Out.RadiusMetres = static_cast<float>(RadiusD);

    double ScoreD = 0.0, ContestD = 0.0;
    Obj->TryGetNumberField(TEXT("controlScore"),    ScoreD);
    Obj->TryGetNumberField(TEXT("contestStrength"), ContestD);
    Out.ControlScore    = static_cast<float>(ScoreD);
    Out.ContestStrength = static_cast<float>(ContestD);

    FString FactionStr, ContestantAStr, ContestantBStr;
    Obj->TryGetStringField(TEXT("primaryFaction"), FactionStr);
    Obj->TryGetStringField(TEXT("contestantA"),    ContestantAStr);
    Obj->TryGetStringField(TEXT("contestantB"),    ContestantBStr);
    Out.PrimaryFaction = WYAFactionFromString(FactionStr);
    Out.ContestantA    = WYAFactionFromString(ContestantAStr);
    Out.ContestantB    = WYAFactionFromString(ContestantBStr);

    return Out.IsValid();
}

// ── Queries ───────────────────────────────────────────────────────────────────

int32 UWYAFactionGeoSubsystem::FindZoneIndex(double Lat, double Lon) const
{
    // Simple radius check — good enough for zone sizes in the game.
    // A spatial index (quadtree) can replace this when zone counts get large.
    const double DegPerMetre = 1.0 / 111320.0;

    for (int32 i = 0; i < CurrentRegion.Zones.Num(); ++i)
    {
        const FWYATerritoryZone& Z = CurrentRegion.Zones[i];
        const double DLat = (Lat - Z.CentroidLat);
        const double DLon = (Lon - Z.CentroidLon) * FMath::Cos(FMath::DegreesToRadians(Lat));
        const double DistMetres = FMath::Sqrt(DLat * DLat + DLon * DLon) / DegPerMetre;

        if (DistMetres <= static_cast<double>(Z.RadiusMetres))
        {
            return i;
        }
    }
    return INDEX_NONE;
}

EWYAFaction UWYAFactionGeoSubsystem::GetFactionAtCoord(double Lat, double Lon) const
{
    if (!bTerritoryReady) return EWYAFaction::None;
    const int32 Idx = FindZoneIndex(Lat, Lon);
    return Idx != INDEX_NONE ? CurrentRegion.Zones[Idx].PrimaryFaction : EWYAFaction::Fractured;
}

EWYAFaction UWYAFactionGeoSubsystem::GetFactionAtWorldPos(const FVector& WorldPos) const
{
    if (!bTerritoryReady) return EWYAFaction::None;
    auto* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
    if (!LocSys) return EWYAFaction::None;
    const FWYAGeoCoord Coord = LocSys->WorldToGeo(WorldPos);
    return GetFactionAtCoord(Coord.Latitude, Coord.Longitude);
}

const FWYATerritoryZone* UWYAFactionGeoSubsystem::GetZoneAtCoord(double Lat, double Lon) const
{
    if (!bTerritoryReady) return nullptr;
    const int32 Idx = FindZoneIndex(Lat, Lon);
    return Idx != INDEX_NONE ? &CurrentRegion.Zones[Idx] : nullptr;
}

const FWYATerritoryZone* UWYAFactionGeoSubsystem::GetZoneAtWorldPos(const FVector& WorldPos) const
{
    if (!bTerritoryReady) return nullptr;
    auto* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
    if (!LocSys) return nullptr;
    const FWYAGeoCoord Coord = LocSys->WorldToGeo(WorldPos);
    return GetZoneAtCoord(Coord.Latitude, Coord.Longitude);
}

TArray<FWYATerritoryZone> UWYAFactionGeoSubsystem::GetZonesByFaction(EWYAFaction Filter) const
{
    TArray<FWYATerritoryZone> Result;
    if (!bTerritoryReady) return Result;

    for (const FWYATerritoryZone& Zone : CurrentRegion.Zones)
    {
        if (Zone.PrimaryFaction == Filter) Result.Add(Zone);
    }
    return Result;
}

void UWYAFactionGeoSubsystem::RequestTerritoryRefresh()
{
    auto* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
    if (LocSys && LocSys->IsResolved())
    {
        const FWYAGeoCoord& Origin = LocSys->GetOrigin();
        FetchRegion(Origin.Latitude, Origin.Longitude);
    }
}

// ── Local cache ───────────────────────────────────────────────────────────────

FString UWYAFactionGeoSubsystem::ComputeRegionKey(double Lat, double Lon)
{
    return ComputeGeohash(Lat, Lon, 5);
}

void UWYAFactionGeoSubsystem::SaveRegionCache(const FWYATerritoryRegion& Region) const
{
    const FString Dir = FPaths::Combine(FPaths::ProjectSavedDir(), CacheSubdir);
    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    PF.CreateDirectoryTree(*Dir);

    const FString FilePath = FPaths::Combine(Dir, Region.RegionKey + TEXT(".json"));

    // Reconstruct JSON for storage — reuse the wire format
    TSharedRef<FJsonObject> Root = MakeShared<FJsonObject>();
    Root->SetStringField(TEXT("regionKey"),   Region.RegionKey);
    Root->SetStringField(TEXT("generatedAt"), Region.GeneratedAt);

    TArray<TSharedPtr<FJsonValue>> ZoneValues;
    for (const FWYATerritoryZone& Z : Region.Zones)
    {
        TSharedRef<FJsonObject> ZObj = MakeShared<FJsonObject>();
        ZObj->SetStringField(TEXT("zoneId"),         Z.ZoneId);
        ZObj->SetStringField(TEXT("primaryFaction"),  WYAFactionToString(Z.PrimaryFaction));
        ZObj->SetStringField(TEXT("contestantA"),     WYAFactionToString(Z.ContestantA));
        ZObj->SetStringField(TEXT("contestantB"),     WYAFactionToString(Z.ContestantB));
        ZObj->SetNumberField(TEXT("centroidLat"),     Z.CentroidLat);
        ZObj->SetNumberField(TEXT("centroidLon"),     Z.CentroidLon);
        ZObj->SetNumberField(TEXT("radiusMetres"),    Z.RadiusMetres);
        ZObj->SetNumberField(TEXT("controlScore"),    Z.ControlScore);
        ZObj->SetNumberField(TEXT("contestStrength"), Z.ContestStrength);
        ZObj->SetStringField(TEXT("primaryOsmTag"),   Z.PrimaryOsmTag);
        ZObj->SetStringField(TEXT("scoredAt"),        Z.ScoredAt);
        ZoneValues.Add(MakeShared<FJsonValueObject>(ZObj));
    }
    Root->SetArrayField(TEXT("zones"), ZoneValues);

    FString JsonStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
    FJsonSerializer::Serialize(Root, Writer);
    FFileHelper::SaveStringToFile(JsonStr, *FilePath);
}

bool UWYAFactionGeoSubsystem::LoadRegionCache(const FString& RegionKey,
                                               FWYATerritoryRegion& Out) const
{
    const FString FilePath = FPaths::Combine(
        FPaths::ProjectSavedDir(), CacheSubdir, RegionKey + TEXT(".json"));

    FString JsonStr;
    if (!FFileHelper::LoadFileToString(JsonStr, *FilePath)) return false;

    TSharedPtr<FJsonObject> Root;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) return false;

    if (!ParseRegion(Root, Out)) return false;
    Out.bFromLocalCache = true;
    return true;
}
