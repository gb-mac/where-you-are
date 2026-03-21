#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Location/WYAGeoTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "WYALocationNameResolver.generated.h"

/** Human-readable place context for feeding into AI prompts. */
USTRUCT(BlueprintType)
struct FWYALocationContext
{
	GENERATED_BODY()

	/**
	 * Prompt-ready display string, e.g. "Prince's Island Park, Calgary".
	 * Falls back to "Calgary" (city only) then to raw coords if Nominatim
	 * returns nothing useful.
	 */
	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;

	/** Neighbourhood or suburb name, e.g. "Prince's Island Park". Empty if not found. */
	UPROPERTY(BlueprintReadOnly)
	FString Neighbourhood;

	/** City or town name. */
	UPROPERTY(BlueprintReadOnly)
	FString City;

	bool IsValid() const { return !DisplayName.IsEmpty(); }
};

/**
 * Reverse-geocodes GPS coordinates to human-readable place names via Nominatim (OSM).
 * Results are cached by coordinate cell (~100m resolution) for the session lifetime.
 *
 * Usage:
 *   GetGameInstance()->GetSubsystem<UWYALocationNameResolver>()
 *       ->Resolve(Coord, [](bool bOk, FWYALocationContext Ctx) { ... });
 *
 * Nominatim fair-use policy: max 1 req/s, no bulk use.
 * Suitable for single player-origin lookups — do not call in a tight loop.
 */
UCLASS()
class WHEREYOUARE_API UWYALocationNameResolver : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Resolve a GPS coordinate to a location context.
	 * Callback fires on the game thread. If Nominatim is unreachable, bOk=false
	 * and Context.DisplayName will be a formatted coordinate fallback string.
	 */
	void Resolve(FWYAGeoCoord Coord, TFunction<void(bool, FWYALocationContext)> Callback);

private:
	void OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnected,
	                    FWYAGeoCoord Coord, TFunction<void(bool, FWYALocationContext)> Callback);

	FWYALocationContext ParseResponse(const FString& Json) const;
	FWYALocationContext FallbackContext(FWYAGeoCoord Coord) const;

	/** Cache key: coords rounded to 3 decimal places (~100m cell). */
	FString CacheKey(FWYAGeoCoord Coord) const;

	TMap<FString, FWYALocationContext> Cache;
};
