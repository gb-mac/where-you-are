#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AI/WYAAssistantTypes.h"
#include "WYAAssistantSubsystem.generated.h"

class UWYAAISubsystem;

/**
 * Central interface for all assistant dialogue.
 *
 * Two-tier architecture:
 *
 *   SCRIPTED (priority 1) — Authored lines registered via RegisterOverride().
 *   Plot-critical moments, stage transition beats, and anything that must land
 *   exactly right. These bypass the LLM entirely. Narrative agent owns this content.
 *
 *   LLM-GENERATED (priority 2) — Ambient, contextual, non-plot-bearing lines.
 *   Tactical field guidance, home base commentary, responses to player status.
 *   Only reached if no scripted override matches the trigger tag + stage.
 *
 * Stage is set externally by the quest/game system as the fix-him questline
 * progresses. Each stage has different voice constraints passed to the LLM.
 *
 * Usage:
 *   GetGameInstance()->GetSubsystem<UWYAAssistantSubsystem>()
 *       ->RequestLine("player_returned_injured", Context, Callback);
 *
 * TODO (Narrative agent): Populate scripted overrides for all authored moments.
 * TODO (Core agent): Call SetStage() at each fix-him questline repair completion.
 * TODO (Core agent): Wire to a DataTable asset (DT_AssistantOverrides) for
 *   editor-editable scripted content — call LoadOverridesFromDataTable() on init.
 */
UCLASS()
class WHEREYOUARE_API UWYAAssistantSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// -----------------------------------------------------------------------
	// Stage management
	// -----------------------------------------------------------------------

	/** Set by the quest system at each repair stage completion. */
	UFUNCTION(BlueprintCallable, Category = "WYA|Assistant")
	void SetStage(EWYAAssistantStage NewStage);

	UFUNCTION(BlueprintPure, Category = "WYA|Assistant")
	EWYAAssistantStage GetStage() const { return CurrentStage; }

	// -----------------------------------------------------------------------
	// Scripted override registry
	// -----------------------------------------------------------------------

	/**
	 * Register an authored line for a specific trigger tag and stage.
	 * Call this during game init (or load from DataTable) for all critical moments.
	 *
	 * @param TriggerTag   Identifies the moment, e.g. "player_returned_injured"
	 * @param Stage        Which stage this line applies to
	 * @param ScriptedLine The authored line to deliver verbatim
	 *
	 * If a tag+stage pair is registered more than once, the last registration wins.
	 * Register Stage5_Restored for any line that should also play at full restoration.
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|Assistant")
	void RegisterOverride(const FString& TriggerTag, EWYAAssistantStage Stage, const FString& ScriptedLine);

	// -----------------------------------------------------------------------
	// Line request
	// -----------------------------------------------------------------------

	/**
	 * Request a line for the given trigger and context.
	 * Checks the scripted override registry first. Falls through to LLM if no match.
	 *
	 * @param TriggerTag          Identifies the moment — used for override lookup
	 * @param ContextDescription  Human-readable description of the situation passed
	 *                            to the LLM if no scripted line matches, e.g.
	 *                            "The player just came home with visible injuries
	 *                             after being away for three days"
	 * @param OnComplete          Callback — always fires; bSuccess=false only if
	 *                            LLM path was taken and ollama was unreachable
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|Assistant")
	void RequestLine(
		const FString& TriggerTag,
		const FString& ContextDescription,
		const FOnAssistantLine& OnComplete);

private:
	/**
	 * Override table: TriggerTag → array of lines indexed by EWYAAssistantStage.
	 * Inner array has exactly 5 entries (one per stage). Empty string = no override.
	 */
	TMap<FString, TArray<FString>> Overrides;

	EWYAAssistantStage CurrentStage = EWYAAssistantStage::Stage1_Yoda;

	UPROPERTY()
	TObjectPtr<UWYAAISubsystem> AISub;

	bool TryGetOverride(const FString& TriggerTag, FString& OutLine) const;
};
