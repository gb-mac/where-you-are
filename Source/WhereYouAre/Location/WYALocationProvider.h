#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Location/WYAGeoTypes.h"
#include "WYALocationProvider.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLocationResult, FWYAGeoCoord, bool /*bSuccess*/);

/**
 * Abstract base for all location backends (IP geolocation, native GPS, manual, cached).
 * Subclass and implement RequestLocation(). Fire OnLocationResult when done.
 */
UCLASS(Abstract)
class WHEREYOUARE_API UWYALocationProvider : public UObject
{
    GENERATED_BODY()

public:
    /** Begin async location acquisition. Fires OnLocationResult when complete or failed. */
    virtual void RequestLocation() PURE_VIRTUAL(UWYALocationProvider::RequestLocation, );

    /** Cancel any in-flight request. Safe to call if not active. */
    virtual void Cancel() {}

    FOnLocationResult OnLocationResult;

protected:
    void BroadcastResult(FWYAGeoCoord Coord, bool bSuccess)
    {
        OnLocationResult.Broadcast(Coord, bSuccess);
    }
};
