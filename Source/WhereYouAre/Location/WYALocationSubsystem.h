#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Location/WYAGeoTypes.h"
#include "WYALocationSubsystem.generated.h"

class UWYALocationProvider;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLocationResolved, FWYAGeoCoord, bool /*bSuccess*/);

/**
 * Session-scoped location service. Runs a prioritized fallback chain of providers
 * and broadcasts FOnLocationResolved once a coordinate is obtained.
 *
 * Fallback order:
 *   1. Cached SaveGame coordinate
 *   2. Native GPS (mobile, compile-guarded)
 *   3. IP geolocation (desktop)
 *   4. Manual coordinate (-StartLat / -StartLon args or DefaultGame.ini)
 *   5. Hardcoded default
 *
 * Access: GetGameInstance()->GetSubsystem<UWYALocationSubsystem>()
 */
UCLASS()
class WHEREYOUARE_API UWYALocationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    /** Start the provider chain. Call once from GameMode::InitGame. */
    void RequestLocation();

    /** Convert any GPS coordinate to UE5 world space using the session origin. */
    FVector GeoToWorld(const FWYAGeoCoord& Coord) const;

    /** Convert a UE5 world position back to GPS. */
    FWYAGeoCoord WorldToGeo(const FVector& WorldPos) const;

    /** True once a coordinate has been resolved. */
    bool IsResolved() const { return bResolved; }

    /** The resolved coordinate. Check IsResolved() first. */
    const FWYAGeoCoord& GetOrigin() const { return WorldOrigin; }

    /** Fired once when any provider succeeds (or all fail). */
    FOnLocationResolved OnLocationResolved;

private:
    UPROPERTY()
    TArray<TObjectPtr<UWYALocationProvider>> Providers;

    int32 CurrentProviderIndex = 0;
    bool bResolved = false;
    FWYAGeoCoord WorldOrigin;

    FTimerHandle ProviderTimeoutHandle;
    static constexpr float ProviderTimeoutSeconds = 8.0f;

    void TryNextProvider();
    void OnProviderResult(FWYAGeoCoord Coord, bool bSuccess);
    void OnProviderTimeout();

    void SaveCachedCoord(const FWYAGeoCoord& Coord);
    bool TryLoadCachedCoord(FWYAGeoCoord& OutCoord);

    FWYAGeoCoord GetManualOrDefaultCoord();
};
