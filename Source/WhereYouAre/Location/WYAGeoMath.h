#pragma once

#include "CoreMinimal.h"
#include "WYAGeoTypes.h"

/**
 * Pure static math utilities for geodetic <-> UE5 world space conversion.
 * Uses ENU (East-North-Up) local tangent plane anchored at the session origin.
 *
 * UE5 axis mapping:
 *   X = North (Forward)
 *   Y = East  (Right)
 *   Z = Up
 *
 * Scale: 1 UU = 1 cm, so meters are multiplied by 100.
 */
struct WHEREYOUARE_API FWYAGeoMath
{
    // WGS84 ellipsoid constants
    static constexpr double WGS84_A  = 6378137.0;          // semi-major axis (m)
    static constexpr double WGS84_E2 = 0.00669437999014;   // eccentricity squared

    /** Convert GPS coordinate to UE5 world space relative to an origin coordinate.
     *  Returns FVector::ZeroVector when Target == Origin. */
    static FVector GeoToWorld(const FWYAGeoCoord& Target, const FWYAGeoCoord& Origin);

    /** Convert UE5 world position back to a GPS coordinate given the session origin. */
    static FWYAGeoCoord WorldToGeo(const FVector& WorldPos, const FWYAGeoCoord& Origin);

    /** Straight-line distance in meters between two GPS coordinates (ignores altitude). */
    static double HaversineDistance(const FWYAGeoCoord& A, const FWYAGeoCoord& B);

private:
    struct FECEF { double X, Y, Z; };

    static FECEF GeoToECEF(const FWYAGeoCoord& Coord);
    static FVector ECEFDeltaToENU(const FECEF& Delta, const FWYAGeoCoord& Origin);
};
