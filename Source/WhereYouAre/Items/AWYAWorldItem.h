#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Api/WYAApiTypes.h"
#include "AWYAWorldItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UWidgetComponent;

/**
 * An item placed in the physical world at a GPS coordinate.
 * Spawned and managed by UWYAItemSubsystem.
 *
 * Override in Blueprint (BP_WYAWorldItem) to set the mesh and widget class.
 */
UCLASS()
class WHEREYOUARE_API AWYAWorldItem : public AActor
{
	GENERATED_BODY()

public:
	AWYAWorldItem();

	/** Update this actor from fresh backend data. Repositions and refreshes state. */
	void SetItemData(const FWYAItemData& Data, const struct FWYAGeoCoord& WorldOrigin);

	const FWYAItemData& GetItemData() const { return ItemData; }

	/** True if the item can still be claimed. */
	bool IsAvailable() const { return ItemData.IsAvailable(); }

	/** Distance in centimetres from WorldPos to this actor. */
	float DistanceTo(const FVector& WorldPos) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> RootSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> NamePlate;

	/** Called after ItemData is updated so Blueprint subclasses can react. */
	UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Items")
	void OnItemDataUpdated();

	UPROPERTY(BlueprintReadOnly, Category = "WYA|Items")
	FWYAItemData ItemData;

	/** Retries terrain placement until Cesium collision is ready. */
	void RetryTerrainPlacement(FVector ComputedWorldPos, int32 AttemptsLeft);

	FTimerHandle TerrainRetryHandle;
};
