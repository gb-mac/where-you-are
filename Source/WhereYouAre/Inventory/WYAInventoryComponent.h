#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/WYAInventoryTypes.h"
#include "WYAInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

/**
 * Carried inventory for any actor (player pawn, NPC, container).
 * Stacks items of the same type. Hard cap at MaxSlots distinct item types.
 */
UCLASS(ClassGroup=(WYA), meta=(BlueprintSpawnableComponent))
class WHEREYOUARE_API UWYAInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWYAInventoryComponent();

    /**
     * Add Quantity of Type.
     * Returns false if the inventory already has MaxSlots distinct item types
     * and this would be a new type.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Inventory")
    bool AddItem(EWYACarriedItemType Type, int32 Quantity = 1);

    /**
     * Remove Quantity of Type.
     * Returns false if the player doesn't have enough.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Inventory")
    bool RemoveItem(EWYACarriedItemType Type, int32 Quantity = 1);

    /** True if at least Quantity of Type is present. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Inventory")
    bool HasItem(EWYACarriedItemType Type, int32 Quantity = 1) const;

    /** Returns 0 if not present. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Inventory")
    int32 GetQuantity(EWYACarriedItemType Type) const;

    /** Read-only access to all stacks. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Inventory")
    const TArray<FWYAInventoryItem>& GetItems() const { return Items; }

    /** Broadcast after any successful Add or Remove. */
    UPROPERTY(BlueprintAssignable, Category = "WYA|Inventory")
    FOnInventoryChanged OnInventoryChanged;

    /** Maximum number of distinct item type slots. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WYA|Inventory")
    int32 MaxSlots = 20;

private:
    UPROPERTY()
    TArray<FWYAInventoryItem> Items;

    /** Returns index into Items for the given type, or INDEX_NONE. */
    int32 FindSlot(EWYACarriedItemType Type) const;
};
