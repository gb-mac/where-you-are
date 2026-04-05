#pragma once

#include "CoreMinimal.h"
#include "WYAInventoryTypes.generated.h"

/**
 * Carried item types — physical inventory the player holds.
 * Distinct from world-placed GPS items (EWYAItemType).
 */
UENUM(BlueprintType)
enum class EWYACarriedItemType : uint8
{
    None                    UMETA(DisplayName = "None"),

    // ── Fix-Him repair components ─────────────────────────────────────────────
    FixHim_MobilityParts       UMETA(DisplayName = "Chassis Joint Actuators"),    // installs via workbench → Stage 2
    FixHim_DiagnosticLogPackage UMETA(DisplayName = "Diagnostic Log Package"),    // delivered to Dr. Osei → Stage 3
    FixHim_PowerCore           UMETA(DisplayName = "Industrial Capacitor Array"), // installs via workbench → Stage 4
    FixHim_CommHardware        UMETA(DisplayName = "Communication Coprocessor"),  // installs via workbench → Stage 5

    // ── General loot stubs ────────────────────────────────────────────────────
    MeleeWeapon             UMETA(DisplayName = "Melee Weapon"),
    RangedWeapon            UMETA(DisplayName = "Ranged Weapon"),
    Ammo                    UMETA(DisplayName = "Ammo"),
    Food                    UMETA(DisplayName = "Food"),
    Water                   UMETA(DisplayName = "Water"),
    Scrap                   UMETA(DisplayName = "Scrap"),

    // ── Intelligence ─────────────────────────────────────────────────────────
    /**
     * A fragment of pre-Cascade intelligence — data shards, encrypted logs,
     * physical dossiers. Not craftable, not tradeable.
     * Triggers assistant dialogue when the player returns home.
     * Drops from HighTable named targets and special world locations (Antarctica).
     */
    IntelFragment           UMETA(DisplayName = "Intel Fragment"),
};

/** A single inventory entry. Same type stacks — Quantity accumulates. */
USTRUCT(BlueprintType)
struct FWYAInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Inventory")
    EWYACarriedItemType Type = EWYACarriedItemType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Inventory")
    int32 Quantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Inventory")
    FString DisplayName;
};
