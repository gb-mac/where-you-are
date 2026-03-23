#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Http.h"
#include "Api/WYAApiTypes.h"
#include "WYAApiClient.generated.h"

/**
 * HTTP gateway to the Where You Are backend API.
 *
 * On Initialize, loads (or generates) a stable DeviceUUID, then registers or logs in
 * to obtain a JWT. Once authenticated, exposes REST wrappers for items.
 *
 * All callbacks fire on the game thread.
 * Access: GetGameInstance()->GetSubsystem<UWYAApiClient>()
 */
UCLASS()
class WHEREYOUARE_API UWYAApiClient : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** True once the JWT has been obtained and requests can be made. */
	bool IsAuthenticated() const { return !JWT.IsEmpty(); }

	/** Account data from the most recent successful auth. */
	const FWYAAccountData& GetAccount() const { return Account; }

	// ── Item API ──────────────────────────────────────────────────────────────

	/** GET /v1/items/nearby — fires Callback(bSuccess, Items). */
	void GetNearbyItems(
		double Lat, double Lon, float RadiusMetres,
		TFunction<void(bool, TArray<FWYAItemData>)> Callback);

	/** PATCH /v1/items/:id/claim — fires Callback(bSuccess, UpdatedItem). */
	void ClaimItem(
		const FString& ItemId, double ClaimerLat, double ClaimerLon,
		TFunction<void(bool, FWYAItemData)> Callback);

	/** POST /v1/items — fires Callback(bSuccess, NewItem). */
	void PlaceItem(
		EWYAItemType Type, double Lat, double Lon, double Alt,
		TFunction<void(bool, FWYAItemData)> Callback);

	// ── Territory API ──────────────────────────────────────────────────────────

	/**
	 * GET /v1/territory?lat=X&lon=Y
	 * Returns the territory region JSON for the geohash cell containing (Lat, Lon).
	 * Server generates or returns cached OSM-scored territory for that region.
	 * Fires Callback(bSuccess, RegionJsonObject).
	 */
	void GetTerritoryRegion(
		double Lat, double Lon,
		TFunction<void(bool, TSharedPtr<FJsonObject>)> Callback);

	/** Delegate fired once authentication succeeds (or permanently fails). */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAuthReady, bool /*bSuccess*/);
	FOnAuthReady OnAuthReady;

private:
	void EnsureAuth();
	void TryRegister();
	void TryLogin();
	void OnAuthSuccess(const FString& InJWT, const FWYAAccountData& InAccount);

	void SendRequest(
		const FString& Verb, const FString& Path,
		const FString& Body,
		TFunction<void(bool, TSharedPtr<FJsonObject>)> Callback);

	void OnRequestComplete(
		FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnected,
		TFunction<void(bool, TSharedPtr<FJsonObject>)> Callback);

	// Queued calls that arrived before auth completed
	void FlushPendingCalls();

	static const FString SaveSlot;
	static const FString BaseURL;

	FString JWT;
	FString DeviceUUID;
	FWYAAccountData Account;

	bool bAuthInProgress = false;
	bool bAuthFailed     = false;

	TArray<TFunction<void()>> PendingCalls;
};
