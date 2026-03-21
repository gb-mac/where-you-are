#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/WYAOpponentPersonality.h"
#include "WYAOpponentController.generated.h"

/**
 * AIController for all opponent characters.
 *
 * On possession, requests a personality profile from UWYAAISubsystem and writes
 * the result to the Blackboard. The Behavior Tree reads these keys to parameterise
 * combat decisions.
 *
 * Blackboard key names are defined as static constants below — use the same names
 * when creating the BB asset in the editor.
 *
 * The BT should gate personality-driven decisions behind the BB_PersonalityReady
 * boolean so opponents behave sensibly (with fallback defaults) while generation
 * is in flight on slower hardware.
 *
 * TODO (Core agent): Create BB_Opponent.uasset and BT_Opponent.uasset in
 * Content/AI/ using the key names defined here. Assign BT_Opponent as the
 * BehaviorTreeAsset on this controller (or set it in Blueprint subclass).
 */
UCLASS()
class WHEREYOUARE_API AWYAOpponentController : public AAIController
{
	GENERATED_BODY()

public:
	// -----------------------------------------------------------------------
	// Blackboard key names — match exactly in BB_Opponent.uasset
	// -----------------------------------------------------------------------
	static const FName BB_AggressionLevel;    // Float
	static const FName BB_PreferredRange;     // Int  (0=Close, 1=Mid, 2=Long)
	static const FName BB_RetreatThreshold;   // Float
	static const FName BB_FlankingTendency;   // Float
	static const FName BB_PersonalityReady;   // Bool
	static const FName BB_TargetActor;        // Object  (set by perception/combat system)

	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * Directly assign a pre-built personality (e.g. for testing or scripted encounters).
	 * Skips the LLM request and writes immediately to the Blackboard.
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|AI")
	void AssignPersonality(const FWYAOpponentPersonality& Personality);

	/** The faction tag passed to the LLM. Set this on the spawned pawn before possession. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|AI")
	FString FactionTag = TEXT("Scavenger");

	/** Location context forwarded to the LLM prompt. Populated from WYALocationNameResolver. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|AI")
	FString LocationContext;

private:
	UFUNCTION()
	void OnPersonalityReady(bool bSuccess, const FWYAOpponentPersonality& Personality);
	void WriteToBlackboard(const FWYAOpponentPersonality& Personality);
};
