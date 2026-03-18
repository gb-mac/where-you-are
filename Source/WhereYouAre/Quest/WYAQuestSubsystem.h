#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Quest/WYAQuestTypes.h"
#include "WYAQuestSubsystem.generated.h"

class UWYAAISubsystem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuestAssigned, APlayerController*, const FWYAQuest&);

/**
 * Central quest dispatcher. GameMode routes all quest assignment through here.
 *
 * Two pipelines — kept strictly separate:
 *
 *   SIDE QUESTS   — AI-generated via UWYAAISubsystem. Location-aware, non-deterministic.
 *                   Pre-generation starts at location resolve; pop with TryAssignSideQuest().
 *
 *   MAIN STORY    — Static, hand-authored. Narrative agent owns the content.
 *                   Stub lives in WYAMainQuestData (to be populated). Advance with
 *                   AdvanceMainStory(). DO NOT route AI content through this pipeline.
 *
 * See decisions-log.md: "AI Quests Are Side Quests Only — Main Story Is Static".
 */
UCLASS()
class WHEREYOUARE_API UWYAQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Attempt to assign the next pre-generated side quest to a player.
	 * Returns false if none are ready yet (AI is still generating — try again after a short delay).
	 */
	bool TryAssignSideQuest(APlayerController* PC);

	/**
	 * Advance the player to the next main story beat.
	 *
	 * STUB — narrative agent will populate WYAMainQuestData with authored beats.
	 * Until then this logs a warning and no-ops.
	 */
	void AdvanceMainStory(APlayerController* PC);

	/** Fired whenever a quest (either type) is assigned to a player. */
	FOnQuestAssigned OnQuestAssigned;

private:
	UPROPERTY()
	TObjectPtr<UWYAAISubsystem> AISub;

	// -----------------------------------------------------------------------
	// Main story data — STUB for narrative agent
	// -----------------------------------------------------------------------
	// TODO (narrative agent): Replace with a UDataTable<FWYAMainQuestRow> loaded
	// from Content/Quests/DT_MainStory.uasset. Each row = one authored story beat.
	// AdvanceMainStory() should look up the player's current beat index and assign
	// the next row. Beat content, objectives, and rewards are entirely hand-authored.
	// -----------------------------------------------------------------------

	TMap<APlayerController*, int32> MainStoryProgress; // PC → current beat index
};
