#pragma once

#include "CoreMinimal.h"
#include "Api/WYAApiTypes.h"
#include "WYAFactionTypes.generated.h"

// ── Zone control ──────────────────────────────────────────────────────────────

/**
 * A single territory zone — one OSM block or land-use area.
 * The server scores it against faction affinity rules and assigns control.
 *
 * Latitude/longitude bounds define the zone polygon. For game purposes
 * we store the centroid and a radius approximation for fast point queries.
 */
USTRUCT(BlueprintType)
struct FWYATerritoryZone
{
    GENERATED_BODY()

    /** Unique zone id — stable hash of OSM feature id + region key. */
    UPROPERTY(BlueprintReadOnly) FString ZoneId;

    /** Controlling faction. Fractured = no control. Contested = two factions close. */
    UPROPERTY(BlueprintReadOnly) EWYAFaction PrimaryFaction = EWYAFaction::Fractured;

    /**
     * For Contested zones, the two factions fighting for it.
     * Both are None if zone is Fractured.
     */
    UPROPERTY(BlueprintReadOnly) EWYAFaction ContestantA = EWYAFaction::None;
    UPROPERTY(BlueprintReadOnly) EWYAFaction ContestantB = EWYAFaction::None;

    /** Zone centroid. */
    UPROPERTY(BlueprintReadOnly) double CentroidLat = 0.0;
    UPROPERTY(BlueprintReadOnly) double CentroidLon = 0.0;

    /** Approximate radius for point-in-zone queries. Metres. */
    UPROPERTY(BlueprintReadOnly) float RadiusMetres = 200.0f;

    /** Faction score that determined control (0–1). Below threshold = Fractured. */
    UPROPERTY(BlueprintReadOnly) float ControlScore = 0.0f;

    /** How strongly the two contestants are competing (0 = clear, 1 = dead even). */
    UPROPERTY(BlueprintReadOnly) float ContestStrength = 0.0f;

    /** OSM land-use / building type that drove this assignment. Debug/display. */
    UPROPERTY(BlueprintReadOnly) FString PrimaryOsmTag;

    /** ISO-8601 timestamp from the server — when this zone was last scored. */
    UPROPERTY(BlueprintReadOnly) FString ScoredAt;

    bool IsValid() const { return !ZoneId.IsEmpty(); }
    bool IsContested() const { return PrimaryFaction == EWYAFaction::Contested; }
    bool IsFractured() const { return PrimaryFaction == EWYAFaction::Fractured; }
};

// ── Territory region cache ────────────────────────────────────────────────────

/**
 * The full territory map for a geographic region.
 * One region covers a circular area around the player's spawn origin.
 * Server-authoritative — all players in the same region share this map.
 */
USTRUCT(BlueprintType)
struct FWYATerritoryRegion
{
    GENERATED_BODY()

    /** Region key — geohash of origin coordinate at precision 5 (~4km cells). */
    UPROPERTY(BlueprintReadOnly) FString RegionKey;

    /** All territory zones within this region. */
    UPROPERTY(BlueprintReadOnly) TArray<FWYATerritoryZone> Zones;

    /** Server timestamp — when this region was last generated from OSM. */
    UPROPERTY(BlueprintReadOnly) FString GeneratedAt;

    /** True if data was loaded from local cache (may be stale). */
    bool bFromLocalCache = false;

    bool IsValid() const { return !RegionKey.IsEmpty() && Zones.Num() > 0; }
};

// ── Faction color palette ─────────────────────────────────────────────────────

/**
 * Returns the canonical overlay color for a faction.
 * Used by AR overlay and desktop territory render.
 * Alpha encodes zone state: full control = 0.7, contested = 0.4, fractured = 0.2.
 */
inline FLinearColor WYAFactionColor(EWYAFaction F, bool bContested = false, bool bFractured = false)
{
    if (bFractured)  return FLinearColor(0.15f, 0.15f, 0.15f, 0.2f);
    if (bContested)  return FLinearColor(0.8f,  0.7f,  0.1f,  0.4f); // contested = yellow bleed

    switch (F)
    {
        case EWYAFaction::Holdouts:    return FLinearColor(0.18f, 0.52f, 0.25f, 0.7f); // muted green
        case EWYAFaction::IronCompact: return FLinearColor(0.20f, 0.30f, 0.55f, 0.7f); // steel blue
        case EWYAFaction::Wardens:     return FLinearColor(0.55f, 0.40f, 0.15f, 0.7f); // oak brown
        case EWYAFaction::Signal:      return FLinearColor(0.10f, 0.65f, 0.75f, 0.7f); // cyan
        case EWYAFaction::Covenant:    return FLinearColor(0.55f, 0.10f, 0.60f, 0.7f); // deep violet
        case EWYAFaction::Hollow:      return FLinearColor(0.45f, 0.45f, 0.45f, 0.7f); // industrial grey
        case EWYAFaction::Reckoners:   return FLinearColor(0.75f, 0.30f, 0.10f, 0.7f); // rust orange
        case EWYAFaction::Coherent:    return FLinearColor(0.90f, 0.85f, 0.20f, 0.7f); // electric yellow
        default:                       return FLinearColor(0.10f, 0.10f, 0.10f, 0.1f);
    }
}
