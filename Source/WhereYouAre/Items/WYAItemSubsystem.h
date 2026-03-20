#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Api/WYAApiTypes.h"
#include "WYAItemSubsystem.generated.h"

class AWYAWorldItem;
class UWYAApiClient;
class UWYALocationSubsystem;

/**
 * Manages the pool of live AWYAWorldItem actors, polling the backend every 30s.
 * Access: GetGameInstance()->GetSubsystem<UWYAItemSubsystem>()
 */
UCLASS()
class WHEREYOUARE_API UWYAItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Manually trigger a refresh (also called by the timer). */
	void FetchNearbyItems();

	/** All currently spawned world item actors. */
	const TArray<TWeakObjectPtr<AWYAWorldItem>>& GetWorldItems() const { return WorldItems; }

	/** Fired after every successful fetch. */
	DECLARE_MULTICAST_DELEGATE(FOnItemsRefreshed);
	FOnItemsRefreshed OnItemsRefreshed;

private:
	void OnLocationResolved(struct FWYAGeoCoord Coord, bool bSuccess);
	void ReconcileActors(const TArray<FWYAItemData>& Items);
	AWYAWorldItem* SpawnWorldItem(const FWYAItemData& Data);

	UPROPERTY() TArray<TObjectPtr<AWYAWorldItem>> OwnedItems;
	TArray<TWeakObjectPtr<AWYAWorldItem>>         WorldItems;

	FTimerHandle PollHandle;
	static constexpr float PollIntervalSeconds = 30.f;

	bool bStarted = false;
};
