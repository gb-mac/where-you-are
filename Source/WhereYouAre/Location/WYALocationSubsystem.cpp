#include "Location/WYALocationSubsystem.h"
#include "Location/WYALocationProvider.h"
#include "Location/WYAIPGeolocator.h"
#include "Location/WYAGeoMath.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CommandLine.h"
#include "WYALocationSave.h"

static const FString LocationSaveSlot = TEXT("WYALocation");
static constexpr int32 LocationSaveUserIndex = 0;
static constexpr int32 CacheMaxAgeDays = 30;

// ---------------------------------------------------------------------------

void UWYALocationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Build provider list in priority order
    Providers.Add(NewObject<UWYAIPGeolocator>(this));
    // Future: Providers.Insert(NewObject<UWYANativeGPSProvider>(this), 0); // mobile
}

void UWYALocationSubsystem::Deinitialize()
{
    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        World->GetTimerManager().ClearTimer(ProviderTimeoutHandle);
    }
    for (UWYALocationProvider* Provider : Providers)
    {
        if (Provider) Provider->Cancel();
    }
    Super::Deinitialize();
}

void UWYALocationSubsystem::RequestLocation()
{
    if (bResolved) return;

    // 1. Try cached coord first (instant, no async)
    FWYAGeoCoord Cached;
    if (TryLoadCachedCoord(Cached))
    {
        UE_LOG(LogTemp, Log, TEXT("WYALocation: Using cached coordinate (%.4f, %.4f)"),
            Cached.Latitude, Cached.Longitude);
        WorldOrigin = Cached;
        bResolved = true;
        OnLocationResolved.Broadcast(WorldOrigin, true);
        return;
    }

    CurrentProviderIndex = 0;
    TryNextProvider();
}

void UWYALocationSubsystem::TryNextProvider()
{
    if (CurrentProviderIndex >= Providers.Num())
    {
        // All providers exhausted — use manual / default fallback (never fails)
        FWYAGeoCoord Fallback = GetManualOrDefaultCoord();
        UE_LOG(LogTemp, Warning, TEXT("WYALocation: All providers failed, using fallback (%.4f, %.4f)"),
            Fallback.Latitude, Fallback.Longitude);
        WorldOrigin = Fallback;
        bResolved = true;
        OnLocationResolved.Broadcast(WorldOrigin, false);
        return;
    }

    UWYALocationProvider* Provider = Providers[CurrentProviderIndex];
    if (!Provider)
    {
        ++CurrentProviderIndex;
        TryNextProvider();
        return;
    }

    Provider->OnLocationResult.AddUObject(this, &UWYALocationSubsystem::OnProviderResult);

    // Timeout guard
    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        World->GetTimerManager().SetTimer(ProviderTimeoutHandle,
            this, &UWYALocationSubsystem::OnProviderTimeout,
            ProviderTimeoutSeconds, false);
    }

    Provider->RequestLocation();
}

void UWYALocationSubsystem::OnProviderResult(FWYAGeoCoord Coord, bool bSuccess)
{
    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        World->GetTimerManager().ClearTimer(ProviderTimeoutHandle);
    }

    if (bSuccess)
    {
        SaveCachedCoord(Coord);
        WorldOrigin = Coord;
        bResolved = true;
        OnLocationResolved.Broadcast(WorldOrigin, true);
    }
    else
    {
        ++CurrentProviderIndex;
        TryNextProvider();
    }
}

void UWYALocationSubsystem::OnProviderTimeout()
{
    UE_LOG(LogTemp, Warning, TEXT("WYALocation: Provider %d timed out"), CurrentProviderIndex);
    if (CurrentProviderIndex < Providers.Num())
    {
        Providers[CurrentProviderIndex]->Cancel();
    }
    ++CurrentProviderIndex;
    TryNextProvider();
}

FVector UWYALocationSubsystem::GeoToWorld(const FWYAGeoCoord& Coord) const
{
    return FWYAGeoMath::GeoToWorld(Coord, WorldOrigin);
}

FWYAGeoCoord UWYALocationSubsystem::WorldToGeo(const FVector& WorldPos) const
{
    return FWYAGeoMath::WorldToGeo(WorldPos, WorldOrigin);
}

void UWYALocationSubsystem::SaveCachedCoord(const FWYAGeoCoord& Coord)
{
    UWYALocationSave* Save = Cast<UWYALocationSave>(
        UGameplayStatics::CreateSaveGameObject(UWYALocationSave::StaticClass()));
    Save->Latitude  = Coord.Latitude;
    Save->Longitude = Coord.Longitude;
    Save->Altitude  = Coord.Altitude;
    Save->Timestamp = FDateTime::UtcNow();
    UGameplayStatics::SaveGameToSlot(Save, LocationSaveSlot, LocationSaveUserIndex);
}

bool UWYALocationSubsystem::TryLoadCachedCoord(FWYAGeoCoord& OutCoord)
{
    if (!UGameplayStatics::DoesSaveGameExist(LocationSaveSlot, LocationSaveUserIndex))
        return false;

    UWYALocationSave* Save = Cast<UWYALocationSave>(
        UGameplayStatics::LoadGameFromSlot(LocationSaveSlot, LocationSaveUserIndex));
    if (!Save) return false;

    const FTimespan Age = FDateTime::UtcNow() - Save->Timestamp;
    if (Age.GetDays() > CacheMaxAgeDays) return false;

    OutCoord.Latitude  = Save->Latitude;
    OutCoord.Longitude = Save->Longitude;
    OutCoord.Altitude  = Save->Altitude;
    OutCoord.Source    = EWYALocationSource::Cached;
    return OutCoord.IsValid();
}

FWYAGeoCoord UWYALocationSubsystem::GetManualOrDefaultCoord()
{
    FWYAGeoCoord Coord;

    // Check command line args: -StartLat=51.5 -StartLon=-0.12
    double Lat = 0.0, Lon = 0.0;
    if (FParse::Value(FCommandLine::Get(), TEXT("StartLat="), Lat) &&
        FParse::Value(FCommandLine::Get(), TEXT("StartLon="), Lon))
    {
        Coord.Latitude  = Lat;
        Coord.Longitude = Lon;
        Coord.Source    = EWYALocationSource::Manual;
        UE_LOG(LogTemp, Log, TEXT("WYALocation: Using command-line coord (%.4f, %.4f)"), Lat, Lon);
        return Coord;
    }

    // Check DefaultGame.ini [WYALocationSettings]
    double IniLat = 0.0, IniLon = 0.0;
    GConfig->GetDouble(TEXT("WYALocationSettings"), TEXT("DefaultLatitude"),  IniLat, GGameIni);
    GConfig->GetDouble(TEXT("WYALocationSettings"), TEXT("DefaultLongitude"), IniLon, GGameIni);
    if (IniLat != 0.0 || IniLon != 0.0)
    {
        Coord.Latitude  = IniLat;
        Coord.Longitude = IniLon;
        Coord.Source    = EWYALocationSource::Manual;
        return Coord;
    }

    // Hardcoded default: London
    Coord.Latitude  = 51.5074;
    Coord.Longitude = -0.1278;
    Coord.Source    = EWYALocationSource::Default;
    return Coord;
}
