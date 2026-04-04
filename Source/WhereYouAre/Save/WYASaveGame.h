#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Inventory/WYAInventoryTypes.h"
#include "Api/WYAApiTypes.h"
#include "WYASaveGame.generated.h"

/**
 * Persistent save data for Where You Are.
 * Covers fix-him questline state, player inventory, and faction affiliation.
 */
UCLASS()
class WHEREYOUARE_API UWYASaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    // ── Fix-Him questline state ───────────────────────────────────────────────

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bMobilityRepaired = false;

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bProcessingRepaired = false;

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bPowerRepaired = false;

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bCommsRepaired = false;

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bQuestlineActive = false;

    // ── Player inventory ──────────────────────────────────────────────────────

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    TArray<FWYAInventoryItem> SavedInventory;

    // ── Faction ───────────────────────────────────────────────────────────────

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    EWYAFaction PlayerFaction = EWYAFaction::None;

    // ── Playtime tracking & questline trigger ─────────────────────────────────

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    float TotalPlaytimeSecs = 0.f;

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    int32 SessionCount = 0;

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bFixHimQuestlineTriggered = false;

    // ── Currency ──────────────────────────────────────────────────────────────

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    int32 SavedGold = 0;

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    int32 SavedSilver = 0;

    // ── Survival ──────────────────────────────────────────────────────────────

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    float SavedWater = 100.f;

    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    float SavedFood = 100.f;

    // ── Slot metadata ─────────────────────────────────────────────────────────

    static const FString SlotName;
    static const int32   UserIndex;
};
