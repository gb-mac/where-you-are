#include "Inventory/WYAInventoryComponent.h"

UWYAInventoryComponent::UWYAInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UWYAInventoryComponent::AddItem(EWYACarriedItemType Type, int32 Quantity)
{
    if (Type == EWYACarriedItemType::None || Quantity <= 0) return false;

    const int32 SlotIdx = FindSlot(Type);
    if (SlotIdx != INDEX_NONE)
    {
        // Stack onto existing slot
        Items[SlotIdx].Quantity += Quantity;
        OnInventoryChanged.Broadcast();
        return true;
    }

    // New slot — check capacity
    if (Items.Num() >= MaxSlots) return false;

    FWYAInventoryItem NewItem;
    NewItem.Type     = Type;
    NewItem.Quantity = Quantity;
    Items.Add(NewItem);
    OnInventoryChanged.Broadcast();
    return true;
}

bool UWYAInventoryComponent::RemoveItem(EWYACarriedItemType Type, int32 Quantity)
{
    if (Type == EWYACarriedItemType::None || Quantity <= 0) return false;

    const int32 SlotIdx = FindSlot(Type);
    if (SlotIdx == INDEX_NONE) return false;
    if (Items[SlotIdx].Quantity < Quantity) return false;

    Items[SlotIdx].Quantity -= Quantity;
    if (Items[SlotIdx].Quantity == 0)
    {
        Items.RemoveAtSwap(SlotIdx);
    }

    OnInventoryChanged.Broadcast();
    return true;
}

bool UWYAInventoryComponent::HasItem(EWYACarriedItemType Type, int32 Quantity) const
{
    return GetQuantity(Type) >= Quantity;
}

int32 UWYAInventoryComponent::GetQuantity(EWYACarriedItemType Type) const
{
    const int32 SlotIdx = FindSlot(Type);
    return SlotIdx != INDEX_NONE ? Items[SlotIdx].Quantity : 0;
}

int32 UWYAInventoryComponent::FindSlot(EWYACarriedItemType Type) const
{
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].Type == Type) return i;
    }
    return INDEX_NONE;
}
