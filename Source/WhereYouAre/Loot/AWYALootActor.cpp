#include "Loot/AWYALootActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "WYACharacter.h"
#include "Inventory/WYAInventoryComponent.h"

AWYALootActor::AWYALootActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
    Root->InitSphereRadius(150.f);
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Root);
}

bool AWYALootActor::TryPickup(AWYACharacter* Char)
{
    if (!Char || !Char->Inventory) return false;

    const bool bAdded = Char->Inventory->AddItem(ItemType, Quantity);
    if (bAdded)
    {
        Destroy();
        return true;
    }

    return false;
}

FText AWYALootActor::GetPickupPrompt() const
{
    return FText::FromString(
        FString::Printf(TEXT("[E] Pick up %s"), *GetTypeDisplayName(ItemType).ToString()));
}

// static
FText AWYALootActor::GetTypeDisplayName(EWYACarriedItemType Type)
{
    switch (Type)
    {
        case EWYACarriedItemType::FixHim_MobilityParts: return FText::FromString(TEXT("Mobility Parts"));
        case EWYACarriedItemType::FixHim_PowerCore:     return FText::FromString(TEXT("Power Core"));
        case EWYACarriedItemType::FixHim_CommHardware:  return FText::FromString(TEXT("Comm Hardware"));
        case EWYACarriedItemType::MeleeWeapon:          return FText::FromString(TEXT("Melee Weapon"));
        case EWYACarriedItemType::RangedWeapon:         return FText::FromString(TEXT("Ranged Weapon"));
        case EWYACarriedItemType::Ammo:                 return FText::FromString(TEXT("Ammo"));
        case EWYACarriedItemType::Food:                 return FText::FromString(TEXT("Food"));
        case EWYACarriedItemType::Water:                return FText::FromString(TEXT("Water"));
        case EWYACarriedItemType::Scrap:                return FText::FromString(TEXT("Scrap"));
        default:                                        return FText::FromString(TEXT("Unknown Item"));
    }
}
