#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Faction/WYAFactionTypes.h"
#include "Location/WYAGeoTypes.h"
#include "WYAFactionGeoSubsystem.generated.h"

/**
 * Faction territory system — procedural geo assignment from OSM data.
 *
 * On location resolve, fetches or generates territory data for the player's
 * region from the backend. Territory is server-authoritative and shared by
 * all players in the same geographic region.
 *
 * Usage:
 *   auto* FactionGeo = GetGameInstance()->GetSubsystem<UWYAFactionGeoSubsystem>();
 *   FactionGeo->OnTerritoryReady.AddUObject(this, &AMyActor::OnTerritoryReady);
 *
 * Query:
 *   EWYAFaction F = FactionGeo->GetFactionAtCoord(Lat, Lon);
 *   const FWYATerritoryZone* Zone = FactionGeo->GetZoneAtCoord(Lat, Lon);
 */
UCLASS()
class WHEREYOUARE_API UWYAFactionGeoSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // ── State ─────────────────────────────────────────────────────────────────

    bool IsTerritoryReady() const { return bTerritoryReady; }
    const FWYATerritoryRegion& GetRegion() const { return CurrentRegion; }

    // ── Queries ───────────────────────────────────────────────────────────────

    /**
     * Returns the controlling faction at a GPS coordinate.
     * Returns EWYAFaction::None if territory is not yet loaded.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Faction")
    EWYAFaction GetFactionAtCoord(double Lat, double Lon) const;

    /**
     * Returns the controlling faction at a UE5 world position.
     * Converts via WYALocationSubsystem::WorldToGeo internally.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Faction")
    EWYAFaction GetFactionAtWorldPos(const FVector& WorldPos) const;

    /**
     * Returns a pointer to the zone containing (Lat, Lon), or nullptr if not found.
     * Pointer is valid until the territory region is refreshed.
     */
    const FWYATerritoryZone* GetZoneAtCoord(double Lat, double Lon) const;
    const FWYATerritoryZone* GetZoneAtWorldPos(const FVector& WorldPos) const;

    /**
     * All zones that the controlling faction matches Filter.
     * Contested/Fractured zones can be queried by passing those enum values.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Faction")
    TArray<FWYATerritoryZone> GetZonesByFaction(EWYAFaction Filter) const;

    // ── Events ────────────────────────────────────────────────────────────────

    /** Fired once when the territory region is ready (fetched or from cache). */
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnTerritoryReady, bool /*bSuccess*/);
    FOnTerritoryReady OnTerritoryReady;

    /**
     * Fired when a zone's controlling faction changes (player actions shifting lines).
     * Only fires on updates received after the initial load.
     */
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnZoneControlChanged,
        const FString& /*ZoneId*/, EWYAFaction /*NewFaction*/);
    FOnZoneControlChanged OnZoneControlChanged;

    // ── Force refresh ─────────────────────────────────────────────────────────

    /** Pull latest territory state from server. Replaces cached region. */
    void RequestTerritoryRefresh();

private:
    void OnLocationResolved(FWYAGeoCoord Coord, bool bSuccess);
    void FetchRegion(double Lat, double Lon);
    void OnRegionResponse(bool bSuccess, const TSharedPtr<class FJsonObject>& Json);
    bool ParseRegion(const TSharedPtr<class FJsonObject>& Json, FWYATerritoryRegion& Out) const;
    bool ParseZone(const TSharedPtr<class FJsonObject>& ZoneObj, FWYATerritoryZone& Out) const;

    void SaveRegionCache(const FWYATerritoryRegion& Region) const;
    bool LoadRegionCache(const FString& RegionKey, FWYATerritoryRegion& Out) const;

    /** Geohash of origin at precision 5 — region bucket key (~4km cells). */
    static FString ComputeRegionKey(double Lat, double Lon);

    /** Returns index of zone containing (Lat, Lon), or INDEX_NONE. */
    int32 FindZoneIndex(double Lat, double Lon) const;

    FWYATerritoryRegion CurrentRegion;
    bool bTerritoryReady = false;

    FDelegateHandle LocationResolvedHandle;

    // Cache file: Saved/WYATerritoryCache/{RegionKey}.json
    static const FString CacheSubdir;
};
