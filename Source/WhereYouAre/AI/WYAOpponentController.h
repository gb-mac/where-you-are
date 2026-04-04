#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/WYAOpponentPersonality.h"
#include "WYAOpponentController.generated.h"

class AWYAOpponentCharacter;

/**
 * AIController for all opponent characters.
 *
 * Two-path AI:
 *
 * 1) C++ state machine (active now) — Idle → Chase → Attack, driven by a
 *    0.2s think timer using direct movement input. No NavMesh required.
 *    Works on Cesium terrain immediately.
 *
 * 2) Behavior Tree (when BB/BT assets exist in Content/AI/) — set
 *    BehaviorTreeAsset on this controller or a Blueprint subclass. The
 *    state machine shuts off once the BT is running.
 *
 * Personality from WYAAISubsystem writes to the Blackboard and is read
 * by both paths. BB_PersonalityReady gates personality-driven branches.
 *
 * Blackboard key names — match exactly in BB_Opponent.uasset:
 *   AggressionLevel   Float
 *   PreferredRange    Int  (0=Close, 1=Mid, 2=Long)
 *   RetreatThreshold  Float
 *   FlankingTendency  Float
 *   PersonalityReady  Bool
 *   TargetActor       Object
 */
UCLASS()
class WHEREYOUARE_API AWYAOpponentController : public AAIController
{
	GENERATED_BODY()

public:
	static const FName BB_AggressionLevel;
	static const FName BB_PreferredRange;
	static const FName BB_RetreatThreshold;
	static const FName BB_FlankingTendency;
	static const FName BB_PersonalityReady;
	static const FName BB_TargetActor;

	virtual void OnPossess(APawn* InPawn)   override;
	virtual void OnUnPossess()              override;

	UFUNCTION(BlueprintCallable, Category = "WYA|AI")
	void AssignPersonality(const FWYAOpponentPersonality& Personality);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|AI")
	FString FactionTag = TEXT("Scavenger");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|AI")
	FString LocationContext;

private:
	// ── State machine ─────────────────────────────────────────────────────

	enum class EOpponentState : uint8 { Idle, Chase, Attack };

	void Think();
	void EnterAttack(AWYAOpponentCharacter* Opp);
	void LeaveAttack();

	FTimerHandle ThinkHandle;
	FTimerHandle AttackHandle;
	EOpponentState AIState = EOpponentState::Idle;

	static constexpr float DetectRadius   = 2000.f;  // 20 m
	static constexpr float AttackRadius   =  200.f;  // 2 m — close combat range
	static constexpr float LoseRadius     = 4000.f;  // 40 m — give up chase
	static constexpr float ThinkInterval  =    0.2f; // seconds between AI decisions
	static constexpr float AttackInterval =    1.2f; // seconds between melee swings

	// ── Personality / Blackboard ───────────────────────────────────────────

	UFUNCTION()
	void OnPersonalityReady(bool bSuccess, const FWYAOpponentPersonality& Personality);
	void WriteToBlackboard(const FWYAOpponentPersonality& Personality);
};
