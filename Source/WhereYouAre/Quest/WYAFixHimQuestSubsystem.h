#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AI/WYAAssistantTypes.h"
#include "WYAFixHimQuestSubsystem.generated.h"

class UWYAAssistantSubsystem;

/** Fired after each fix-him stage advance. NewStage is the stage just entered. */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRepairStageChanged, EWYAAssistantStage);

/**
 * Tracks the fix-him repair questline and drives UWYAAssistantSubsystem stage advances.
 *
 * Four repair stages, each unlocking the next assistant voice register:
 *
 *   Mobility     → Stage 2 (short sentences, personality surfaces)
 *   Processing   → Stage 3 (full voice arrives)
 *   Power        → Stage 4 (operational, dry humor, home management)
 *   Comms        → Stage 5 (restored, delivers the Vael warning)
 *
 * Call the On*RepairCompleted() methods when the player installs the corresponding
 * component at the home base. These are the only correct call sites for SetStage().
 *
 * TriggerQuestline() fires the "Fix." conversation moment (Stage 1 voice asking for
 * the repair). Call this once after sufficient in-game time has passed (weeks of
 * real-world play time, not session time). It fires only once — re-calling is a no-op.
 *
 * See docs/narrative/assistant-stage-triggers.md for full trigger specification.
 * See docs/narrative/quests/questline-fix-him.md for the full questline design.
 */
UCLASS()
class WHEREYOUARE_API UWYAFixHimQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// -----------------------------------------------------------------------
	// Questline activation
	// -----------------------------------------------------------------------

	/**
	 * Fire the "Fix." conversation — the assistant's request for repair.
	 * A domestic machine fails mid-task. He watches it. He asks.
	 *
	 * Call when: sufficient in-game time has passed and the player is at home base.
	 * Fires only once regardless of how many times this is called.
	 * Has no effect if any repair has already been completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|FixHim")
	void TriggerQuestline();

	/** True once TriggerQuestline() has been called successfully. */
	UFUNCTION(BlueprintPure, Category = "WYA|FixHim")
	bool IsQuestlineActive() const { return bQuestlineActive; }

	// -----------------------------------------------------------------------
	// Repair completion — call these when the player installs the component
	// -----------------------------------------------------------------------

	/** Stage 1 → 2. Mobility components installed. He's in the garden when you log back in. */
	UFUNCTION(BlueprintCallable, Category = "WYA|FixHim")
	void OnMobilityRepairCompleted();

	/** Stage 2 → 3. Processing core installed. "Six days. You look terrible. Sit down." */
	UFUNCTION(BlueprintCallable, Category = "WYA|FixHim")
	void OnProcessingRepairCompleted();

	/** Stage 3 → 4. Power systems restored. He built something you didn't ask for. */
	UFUNCTION(BlueprintCallable, Category = "WYA|FixHim")
	void OnPowerRepairCompleted();

	/** Stage 4 → 5. Communication hardware restored. "Give me tonight." */
	UFUNCTION(BlueprintCallable, Category = "WYA|FixHim")
	void OnCommunicationRepairCompleted();

	// -----------------------------------------------------------------------
	// State accessors — used by save system
	// -----------------------------------------------------------------------

	UFUNCTION(BlueprintPure, Category = "WYA|FixHim")
	bool IsMobilityRepaired()   const { return bMobilityRepaired; }

	UFUNCTION(BlueprintPure, Category = "WYA|FixHim")
	bool IsProcessingRepaired() const { return bProcessingRepaired; }

	UFUNCTION(BlueprintPure, Category = "WYA|FixHim")
	bool IsPowerRepaired()      const { return bPowerRepaired; }

	UFUNCTION(BlueprintPure, Category = "WYA|FixHim")
	bool IsCommsRepaired()      const { return bCommsRepaired; }

	/**
	 * Restore repair state from a save game without firing stage transition dialogue.
	 * Call this before SpawnPlayer() during load so the assistant starts at the
	 * correct stage.
	 */
	void LoadRepairState(bool bMobility, bool bProcessing, bool bPower, bool bComms);

	/** Fired after each stage advance. NewStage is the stage that was just entered. */
	FOnRepairStageChanged OnRepairStageChanged;

private:
	UPROPERTY()
	TObjectPtr<UWYAAssistantSubsystem> AssistantSub;

	bool bQuestlineActive    = false;
	bool bMobilityRepaired   = false;
	bool bProcessingRepaired = false;
	bool bPowerRepaired      = false;
	bool bCommsRepaired      = false;

	/** Advance assistant to NewStage, log it, fire the delegate. */
	void AdvanceStage(EWYAAssistantStage NewStage, const TCHAR* RepairName);
};
