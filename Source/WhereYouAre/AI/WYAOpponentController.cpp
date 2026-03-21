#include "AI/WYAOpponentController.h"
#include "AI/WYAAISubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"

// Blackboard key name definitions — keep in sync with BB_Opponent.uasset
const FName AWYAOpponentController::BB_AggressionLevel  = TEXT("AggressionLevel");
const FName AWYAOpponentController::BB_PreferredRange   = TEXT("PreferredRange");
const FName AWYAOpponentController::BB_RetreatThreshold = TEXT("RetreatThreshold");
const FName AWYAOpponentController::BB_FlankingTendency = TEXT("FlankingTendency");
const FName AWYAOpponentController::BB_PersonalityReady = TEXT("PersonalityReady");
const FName AWYAOpponentController::BB_TargetActor      = TEXT("TargetActor");

void AWYAOpponentController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Write fallback defaults immediately so the BT has valid values from tick 1.
	// The LLM result will overwrite these when it arrives.
	WriteToBlackboard(FWYAOpponentPersonality{});

	// Request an LLM-generated personality asynchronously.
	if (UWYAAISubsystem* AISub = GetWorld()->GetGameInstance()->GetSubsystem<UWYAAISubsystem>())
	{
		if (AISub->IsAvailable())
		{
			FOnPersonalityGenerated Delegate;
			Delegate.BindDynamic(this, &AWYAOpponentController::OnPersonalityReady);
			AISub->GenerateOpponentPersonality(FactionTag, LocationContext, Delegate);
		}
		else
		{
			// ollama unreachable — defaults stay, mark ready so BT proceeds
			if (Blackboard)
			{
				Blackboard->SetValueAsBool(BB_PersonalityReady, true);
			}
			UE_LOG(LogTemp, Log, TEXT("WYAOpponentController: ollama unavailable, using fallback personality"));
		}
	}
}

void AWYAOpponentController::AssignPersonality(const FWYAOpponentPersonality& Personality)
{
	WriteToBlackboard(Personality);
}

void AWYAOpponentController::OnPersonalityReady(bool bSuccess, const FWYAOpponentPersonality& Personality)
{
	WriteToBlackboard(Personality);
	UE_LOG(LogTemp, Log, TEXT("WYAOpponentController: personality applied — faction=%s aggression=%.2f range=%d"),
	       *Personality.FactionTag,
	       Personality.AggressionLevel,
	       static_cast<int32>(Personality.PreferredRange));
}

void AWYAOpponentController::WriteToBlackboard(const FWYAOpponentPersonality& Personality)
{
	if (!Blackboard) return;

	Blackboard->SetValueAsFloat(BB_AggressionLevel,  Personality.AggressionLevel);
	Blackboard->SetValueAsInt  (BB_PreferredRange,   static_cast<int32>(Personality.PreferredRange));
	Blackboard->SetValueAsFloat(BB_RetreatThreshold, Personality.RetreatThreshold);
	Blackboard->SetValueAsFloat(BB_FlankingTendency, Personality.FlankingTendency);
	Blackboard->SetValueAsBool (BB_PersonalityReady, Personality.bReady);
}
