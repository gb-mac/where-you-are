#include "Items/WYAItemSubsystem.h"
#include "Items/AWYAWorldItem.h"
#include "Api/WYAApiClient.h"
#include "Location/WYALocationSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

void UWYAItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UWYALocationSubsystem>();
	Collection.InitializeDependency<UWYAApiClient>();
	Super::Initialize(Collection);

	if (UWYALocationSubsystem* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>())
	{
		if (LocSys->IsResolved())
		{
			OnLocationResolved(LocSys->GetOrigin(), true);
		}
		else
		{
			LocSys->OnLocationResolved.AddUObject(this, &UWYAItemSubsystem::OnLocationResolved);
		}
	}
}

void UWYAItemSubsystem::Deinitialize()
{
	if (UWorld* World = GetWorld())
		World->GetTimerManager().ClearTimer(PollHandle);

	for (AWYAWorldItem* Item : OwnedItems)
	{
		if (IsValid(Item)) Item->Destroy();
	}
	OwnedItems.Empty();
	WorldItems.Empty();

	Super::Deinitialize();
}

void UWYAItemSubsystem::OnLocationResolved(FWYAGeoCoord /*Coord*/, bool bSuccess)
{
	// Start polling even on fallback location — a known coord is still usable.
	// bSuccess=false just means we fell back; items near that coord still load.
	if (bStarted) return;
	bStarted = true;

	FetchNearbyItems();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			PollHandle,
			this, &UWYAItemSubsystem::FetchNearbyItems,
			PollIntervalSeconds, /*bLoop=*/true);
	}
}

void UWYAItemSubsystem::FetchNearbyItems()
{
	UWYAApiClient* Api = GetGameInstance()->GetSubsystem<UWYAApiClient>();
	UWYALocationSubsystem* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
	if (!Api || !LocSys || !LocSys->IsResolved()) return;

	const FWYAGeoCoord& Origin = LocSys->GetOrigin();

	UE_LOG(LogTemp, Log, TEXT("WYAItems: fetching items near (%.4f, %.4f)"),
	       Origin.Latitude, Origin.Longitude);

	Api->GetNearbyItems(Origin.Latitude, Origin.Longitude, 500.f,
		[this](bool bOk, TArray<FWYAItemData> Items)
	{
		if (!bOk)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red,
				TEXT("WYAItems: fetch FAILED (check output log)"));
			return;
		}
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green,
			FString::Printf(TEXT("WYAItems: %d item(s) nearby"), Items.Num()));
		ReconcileActors(Items);
		OnItemsRefreshed.Broadcast();
	});
}

void UWYAItemSubsystem::ReconcileActors(const TArray<FWYAItemData>& Items)
{
	UWYALocationSubsystem* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
	if (!LocSys) return;

	// Build lookup from item ID → fresh data
	TMap<FString, FWYAItemData> FreshById;
	for (const FWYAItemData& D : Items) FreshById.Add(D.Id, D);

	// Remove actors no longer in the dataset
	for (int32 i = OwnedItems.Num() - 1; i >= 0; --i)
	{
		AWYAWorldItem* Actor = OwnedItems[i];
		if (!IsValid(Actor)) { OwnedItems.RemoveAt(i); continue; }

		if (!FreshById.Contains(Actor->GetItemData().Id))
		{
			Actor->Destroy();
			OwnedItems.RemoveAt(i);
		}
	}

	// Refresh existing / spawn new
	TSet<FString> ExistingIds;
	for (AWYAWorldItem* Actor : OwnedItems)
	{
		if (!IsValid(Actor)) continue;
		const FString Id = Actor->GetItemData().Id;
		ExistingIds.Add(Id);
		Actor->SetItemData(FreshById[Id], LocSys->GetOrigin());
	}

	for (const FWYAItemData& D : Items)
	{
		if (!ExistingIds.Contains(D.Id))
		{
			if (AWYAWorldItem* Actor = SpawnWorldItem(D))
			{
				Actor->SetItemData(D, LocSys->GetOrigin());
				OwnedItems.Add(Actor);
				WorldItems.Add(Actor);
			}
		}
	}

	// Prune stale weak pointers
	WorldItems.RemoveAll([](const TWeakObjectPtr<AWYAWorldItem>& W){ return !W.IsValid(); });
}

AWYAWorldItem* UWYAItemSubsystem::SpawnWorldItem(const FWYAItemData& Data)
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return World->SpawnActor<AWYAWorldItem>(
		AWYAWorldItem::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
}
