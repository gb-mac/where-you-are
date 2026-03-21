#include "AI/WYALocationNameResolver.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

static constexpr double CacheResolution = 1000.0; // 3 decimal places → ~100m cell

void UWYALocationNameResolver::Resolve(FWYAGeoCoord Coord, TFunction<void(bool, FWYALocationContext)> Callback)
{
	const FString Key = CacheKey(Coord);
	if (const FWYALocationContext* Cached = Cache.Find(Key))
	{
		UE_LOG(LogTemp, Log, TEXT("WYALocationNameResolver: cache hit for %s → %s"), *Key, *Cached->DisplayName);
		Callback(true, *Cached);
		return;
	}

	const FString URL = FString::Printf(
		TEXT("https://nominatim.openstreetmap.org/reverse?lat=%.6f&lon=%.6f&format=jsonv2"),
		Coord.Latitude, Coord.Longitude);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("User-Agent"), TEXT("WhereYouAre/0.1"));
	Request->SetHeader(TEXT("Accept-Language"), TEXT("en"));

	Request->OnProcessRequestComplete().BindUObject(
		this, &UWYALocationNameResolver::OnHttpResponse, Coord, MoveTemp(Callback));

	Request->ProcessRequest();
	UE_LOG(LogTemp, Log, TEXT("WYALocationNameResolver: resolving %s"), *Key);
}

void UWYALocationNameResolver::OnHttpResponse(
	FHttpRequestPtr Request,
	FHttpResponsePtr Response,
	bool bConnected,
	FWYAGeoCoord Coord,
	TFunction<void(bool, FWYALocationContext)> Callback)
{
	if (!bConnected || !Response.IsValid() || Response->GetResponseCode() != 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("WYALocationNameResolver: Nominatim request failed"));
		Callback(false, FallbackContext(Coord));
		return;
	}

	FWYALocationContext Ctx = ParseResponse(Response->GetContentAsString());
	if (!Ctx.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("WYALocationNameResolver: could not parse Nominatim response"));
		Callback(false, FallbackContext(Coord));
		return;
	}

	const FString Key = CacheKey(Coord);
	Cache.Add(Key, Ctx);
	UE_LOG(LogTemp, Log, TEXT("WYALocationNameResolver: resolved %s → %s"), *Key, *Ctx.DisplayName);
	Callback(true, Ctx);
}

FWYALocationContext UWYALocationNameResolver::ParseResponse(const FString& Json) const
{
	TSharedPtr<FJsonObject> Root;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
	{
		return {};
	}

	const TSharedPtr<FJsonObject>* AddressObj;
	if (!Root->TryGetObjectField(TEXT("address"), AddressObj))
	{
		return {};
	}

	FWYALocationContext Ctx;

	// Neighbourhood: prefer "neighbourhood", fall back to "suburb", then "quarter"
	FString Neighbourhood;
	if (!(*AddressObj)->TryGetStringField(TEXT("neighbourhood"), Neighbourhood))
	{
		if (!(*AddressObj)->TryGetStringField(TEXT("suburb"), Neighbourhood))
		{
			(*AddressObj)->TryGetStringField(TEXT("quarter"), Neighbourhood);
		}
	}
	Ctx.Neighbourhood = Neighbourhood;

	// City: prefer "city", fall back to "town", "village", "municipality"
	FString City;
	if (!(*AddressObj)->TryGetStringField(TEXT("city"), City))
	{
		if (!(*AddressObj)->TryGetStringField(TEXT("town"), City))
		{
			if (!(*AddressObj)->TryGetStringField(TEXT("village"), City))
			{
				(*AddressObj)->TryGetStringField(TEXT("municipality"), City);
			}
		}
	}
	Ctx.City = City;

	// Build the display name the prompt will see
	if (!Neighbourhood.IsEmpty() && !City.IsEmpty())
	{
		Ctx.DisplayName = FString::Printf(TEXT("%s, %s"), *Neighbourhood, *City);
	}
	else if (!City.IsEmpty())
	{
		Ctx.DisplayName = City;
	}

	return Ctx;
}

FWYALocationContext UWYALocationNameResolver::FallbackContext(FWYAGeoCoord Coord) const
{
	FWYALocationContext Ctx;
	Ctx.DisplayName = FString::Printf(TEXT("%.4f, %.4f"), Coord.Latitude, Coord.Longitude);
	return Ctx;
}

FString UWYALocationNameResolver::CacheKey(FWYAGeoCoord Coord) const
{
	// Round to 3 decimal places — ~100m cell, avoids redundant fetches for nearby coords
	const int32 LatKey = FMath::RoundToInt(Coord.Latitude  * CacheResolution);
	const int32 LonKey = FMath::RoundToInt(Coord.Longitude * CacheResolution);
	return FString::Printf(TEXT("%d_%d"), LatKey, LonKey);
}
