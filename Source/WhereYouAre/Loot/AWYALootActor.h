#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/WYAInventoryTypes.h"
#include "AWYALootActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AWYACharacter;

/**
 * A physical loot pickup placed in the world.
 * Press E when nearby to call TryPickup() — adds the item to the character's
 * inventory and destroys this actor on success.
 *
 * AWYAPlayerController::FindClosestLootActor() / SetFocusedLootActor() handle
 * detection and HUD prompt display.
 */
UCLASS()
class WHEREYOUARE_API AWYALootActor : public AActor
{
    GENERATED_BODY()

public:
    AWYALootActor();

    /**
     * Attempt to add ItemType × Quantity to Char's inventory.
     * Destroys self and returns true on success.
     * Returns false if inventory is full or Char has no inventory component.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Loot")
    bool TryPickup(AWYACharacter* Char);

    /** Returns "[E] Pick up <DisplayName>" for use in the HUD prompt. */
    UFUNCTION(BlueprintPure, Category = "WYA|Loot")
    FText GetPickupPrompt() const;

    /** Maps EWYACarriedItemType to a human-readable display string. */
    static FText GetTypeDisplayName(EWYACarriedItemType Type);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Loot")
    TObjectPtr<USphereComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Loot")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WYA|Loot")
    EWYACarriedItemType ItemType = EWYACarriedItemType::Scrap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WYA|Loot")
    int32 Quantity = 1;
};
