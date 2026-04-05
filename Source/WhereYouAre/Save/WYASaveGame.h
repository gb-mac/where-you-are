#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Inventory/WYAInventoryTypes.h"
#include "Api/WYAApiTypes.h"
#include "Contracts/WYAContractTypes.h"
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

    // ── Dr. Osei / Stage 3 narrative state ───────────────────────────────────

    /** True after Phase 1 interaction — player has the diagnostic log objective. */
    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bOseiDiagnosticRequested = false;

    /** Player chose "He'd probably want to hear that from you" (OSEI_FH_B05_FROM_YOU). */
    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bOseiOfferedDirectContact = false;

    /** Player chose "He said you'd want to know he's still carrying it" (OSEI_FH_C02_CARRYING). */
    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    bool bOseiRequestedStage4Presence = false;

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

    // ── Active contracts ──────────────────────────────────────────────────────

    /** Contracts currently in the player's active list — restored on load. */
    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    TArray<FWYAContract> SavedActiveContracts;

    /** Run state for each active contract — preserves Ghost/Swift/Clean tracking. */
    UPROPERTY(SaveGame, BlueprintReadWrite, Category = "WYA|Save")
    TArray<FWYAContractRunState> SavedContractRunStates;

    // ── Slot metadata ─────────────────────────────────────────────────────────

    static const FString SlotName;
    static const int32   UserIndex;
};
