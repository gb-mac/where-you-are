#include "Location/WYAGeoMath.h"

FWYAGeoMath::FECEF FWYAGeoMath::GeoToECEF(const FWYAGeoCoord& Coord)
{
    const double LatRad = FMath::DegreesToRadians(Coord.Latitude);
    const double LonRad = FMath::DegreesToRadians(Coord.Longitude);

    const double SinLat = FMath::Sin(LatRad);
    const double CosSqLat = 1.0 - WGS84_E2 * SinLat * SinLat;
    const double N = WGS84_A / FMath::Sqrt(CosSqLat);

    const double R = N + Coord.Altitude;
    return {
        R * FMath::Cos(LatRad) * FMath::Cos(LonRad),
        R * FMath::Cos(LatRad) * FMath::Sin(LonRad),
        (N * (1.0 - WGS84_E2) + Coord.Altitude) * FMath::Sin(LatRad)
    };
}

FVector FWYAGeoMath::ECEFDeltaToENU(const FECEF& Delta, const FWYAGeoCoord& Origin)
{
    const double LatRad = FMath::DegreesToRadians(Origin.Latitude);
    const double LonRad = FMath::DegreesToRadians(Origin.Longitude);

    const double SinLat = FMath::Sin(LatRad);
    const double CosLat = FMath::Cos(LatRad);
    const double SinLon = FMath::Sin(LonRad);
    const double CosLon = FMath::Cos(LonRad);

    // ENU components (meters)
    const double East  = -SinLon * Delta.X + CosLon * Delta.Y;
    const double North = -SinLat * CosLon * Delta.X - SinLat * SinLon * Delta.Y + CosLat * Delta.Z;
    const double Up    =  CosLat * CosLon * Delta.X + CosLat * SinLon * Delta.Y + SinLat * Delta.Z;

    // Map ENU -> UE5 axes, convert meters to UU (1m = 100 UU)
    return FVector(
        static_cast<float>(North * 100.0),  // X = Forward = North
        static_cast<float>(East  * 100.0),  // Y = Right   = East
        static_cast<float>(Up    * 100.0)   // Z = Up       = Up
    );
}

FVector FWYAGeoMath::GeoToWorld(const FWYAGeoCoord& Target, const FWYAGeoCoord& Origin)
{
    const FECEF TargetECEF = GeoToECEF(Target);
    const FECEF OriginECEF = GeoToECEF(Origin);

    const FECEF Delta = {
        TargetECEF.X - OriginECEF.X,
        TargetECEF.Y - OriginECEF.Y,
        TargetECEF.Z - OriginECEF.Z
    };

    return ECEFDeltaToENU(Delta, Origin);
}

FWYAGeoCoord FWYAGeoMath::WorldToGeo(const FVector& WorldPos, const FWYAGeoCoord& Origin)
{
    // Convert UU back to meters; reverse the ENU->UE5 axis mapping
    const double North = WorldPos.X / 100.0;
    const double East  = WorldPos.Y / 100.0;
    const double Up    = WorldPos.Z / 100.0;

    const double LatRad = FMath::DegreesToRadians(Origin.Latitude);
    const double LonRad = FMath::DegreesToRadians(Origin.Longitude);

    const double SinLat = FMath::Sin(LatRad);
    const double CosLat = FMath::Cos(LatRad);
    const double SinLon = FMath::Sin(LonRad);
    const double CosLon = FMath::Cos(LonRad);

    // ENU -> ECEF delta
    const FECEF OriginECEF = GeoToECEF(Origin);
    const FECEF Delta = {
        -SinLon * East - SinLat * CosLon * North + CosLat * CosLon * Up,
        CosLon  * East - SinLat * SinLon * North + CosLat * SinLon * Up,
        CosLat  * North + SinLat * Up
    };

    const FECEF TargetECEF = {
        OriginECEF.X + Delta.X,
        OriginECEF.Y + Delta.Y,
        OriginECEF.Z + Delta.Z
    };

    // ECEF -> Geodetic (iterative)
    const double P   = FMath::Sqrt(TargetECEF.X * TargetECEF.X + TargetECEF.Y * TargetECEF.Y);
    const double Lon = FMath::Atan2(TargetECEF.Y, TargetECEF.X);

    double Lat = FMath::Atan2(TargetECEF.Z, P * (1.0 - WGS84_E2));
    for (int32 i = 0; i < 5; ++i)
    {
        const double SinLatI = FMath::Sin(Lat);
        const double N = WGS84_A / FMath::Sqrt(1.0 - WGS84_E2 * SinLatI * SinLatI);
        Lat = FMath::Atan2(TargetECEF.Z + WGS84_E2 * N * SinLatI, P);
    }

    const double SinLatF = FMath::Sin(Lat);
    const double N = WGS84_A / FMath::Sqrt(1.0 - WGS84_E2 * SinLatF * SinLatF);
    const double Alt = P / FMath::Cos(Lat) - N;

    FWYAGeoCoord Result;
    Result.Latitude  = FMath::RadiansToDegrees(Lat);
    Result.Longitude = FMath::RadiansToDegrees(Lon);
    Result.Altitude  = Alt;
    return Result;
}

double FWYAGeoMath::HaversineDistance(const FWYAGeoCoord& A, const FWYAGeoCoord& B)
{
    const double dLat = FMath::DegreesToRadians(B.Latitude  - A.Latitude);
    const double dLon = FMath::DegreesToRadians(B.Longitude - A.Longitude);
    const double aVal = FMath::Sin(dLat / 2) * FMath::Sin(dLat / 2)
                      + FMath::Cos(FMath::DegreesToRadians(A.Latitude))
                      * FMath::Cos(FMath::DegreesToRadians(B.Latitude))
                      * FMath::Sin(dLon / 2) * FMath::Sin(dLon / 2);
    return WGS84_A * 2.0 * FMath::Atan2(FMath::Sqrt(aVal), FMath::Sqrt(1.0 - aVal));
}
