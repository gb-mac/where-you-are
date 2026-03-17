#pragma once

#include "CoreMinimal.h"
#include "WYAGeoTypes.generated.h"

UENUM(BlueprintType)
enum class EWYALocationSource : uint8
{
    None        UMETA(DisplayName = "None"),
    Cached      UMETA(DisplayName = "Cached SaveGame"),
    NativeGPS   UMETA(DisplayName = "Native GPS (Mobile)"),
    IPGeolocate UMETA(DisplayName = "IP Geolocation"),
    Manual      UMETA(DisplayName = "Manual / Config"),
    Default     UMETA(DisplayName = "Hardcoded Default"),
};

USTRUCT(BlueprintType)
struct FWYAGeoCoord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double Latitude = 0.0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double Longitude = 0.0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    double Altitude = 0.0; // meters above sea level

    UPROPERTY(BlueprintReadOnly)
    EWYALocationSource Source = EWYALocationSource::None;

    bool IsValid() const
    {
        return Latitude != 0.0 || Longitude != 0.0;
    }
};
