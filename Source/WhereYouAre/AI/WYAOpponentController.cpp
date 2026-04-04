#include "AI/WYAOpponentController.h"
#include "AI/WYAAISubsystem.h"
#include "Characters/WYAOpponentCharacter.h"
#include "Combat/WYACombatComponent.h"
#include "WYACharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"

const FName AWYAOpponentController::BB_AggressionLevel  = TEXT("AggressionLevel");
const FName AWYAOpponentController::BB_PreferredRange   = TEXT("PreferredRange");
const FName AWYAOpponentController::BB_RetreatThreshold = TEXT("RetreatThreshold");
const FName AWYAOpponentController::BB_FlankingTendency = TEXT("FlankingTendency");
const FName AWYAOpponentController::BB_PersonalityReady = TEXT("PersonalityReady");
const FName AWYAOpponentController::BB_TargetActor      = TEXT("TargetActor");

// ── Possession ────────────────────────────────────────────────────────────────

void AWYAOpponentController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Write fallback defaults so the BT (when it exists) has valid values from tick 1
	WriteToBlackboard(FWYAOpponentPersonality{});

	// Request LLM personality — overwrites defaults when it arrives
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
			if (Blackboard) Blackboard->SetValueAsBool(BB_PersonalityReady, true);
		}
	}

	// Start think loop — state machine runs until BT takes over
	// (If a BT asset is assigned this controller still ticks, but BT handles movement/attack.
	//  Future: gate Think() behind !IsBotLogicStarted() to avoid double-driving.)
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(ThinkHandle,
			this, &AWYAOpponentController::Think,
			ThinkInterval, /*bLoop=*/true);
	}
}

void AWYAOpponentController::OnUnPossess()
{
	LeaveAttack();
	if (UWorld* World = GetWorld())
		World->GetTimerManager().ClearTimer(ThinkHandle);

	Super::OnUnPossess();
}

// ── State machine ─────────────────────────────────────────────────────────────

void AWYAOpponentController::Think()
{
	AWYAOpponentCharacter* Opp = Cast<AWYAOpponentCharacter>(GetPawn());
	if (!Opp || !Opp->Combat->IsAlive()) return;

	// Find nearest player character
	AWYACharacter* Player  = nullptr;
	float          BestSq  = MAX_FLT;
	const FVector  OppLoc  = Opp->GetActorLocation();

	for (TActorIterator<AWYACharacter> It(GetWorld()); It; ++It)
	{
		if (!It->IsValidLowLevel()) continue;
		const float Sq = FVector::DistSquared(OppLoc, It->GetActorLocation());
		if (Sq < BestSq) { BestSq = Sq; Player = *It; }
	}

	if (!Player) return;

	// Keep BB_TargetActor current for BT tasks when they exist
	if (Blackboard) Blackboard->SetValueAsObject(BB_TargetActor, Player);

	const float    Dist      = FMath::Sqrt(BestSq);
	const FVector  PlayerLoc = Player->GetActorLocation();

	switch (AIState)
	{
	case EOpponentState::Idle:
		if (Dist < DetectRadius)
		{
			AIState = EOpponentState::Chase;
		}
		break;

	case EOpponentState::Chase:
		if (Dist > LoseRadius)
		{
			AIState = EOpponentState::Idle;
			break;
		}
		if (Dist < AttackRadius)
		{
			AIState = EOpponentState::Attack;
			EnterAttack(Opp);
			break;
		}
		{
			// Steer toward player using movement input — no NavMesh needed
			const FVector Dir = (PlayerLoc - OppLoc).GetSafeNormal2D();
			Opp->AddMovementInput(Dir, 1.0f);
			Opp->SetActorRotation(Dir.Rotation());
		}
		break;

	case EOpponentState::Attack:
		if (Dist > AttackRadius * 2.f)
		{
			AIState = EOpponentState::Chase;
			LeaveAttack();
			break;
		}
		// While in attack state, keep facing the player
		{
			const FVector Dir = (PlayerLoc - OppLoc).GetSafeNormal2D();
			Opp->SetActorRotation(Dir.Rotation());
		}
		break;
	}
}

void AWYAOpponentController::EnterAttack(AWYAOpponentCharacter* Opp)
{
	UWorld* World = GetWorld();
	if (!World) return;

	// Swing immediately on entry, then on interval
	if (Opp) Opp->DoMeleeAttack();

	World->GetTimerManager().SetTimer(AttackHandle,
		FTimerDelegate::CreateLambda([WeakOpp = TWeakObjectPtr<AWYAOpponentCharacter>(Opp)]()
		{
			if (AWYAOpponentCharacter* StrongOpp = WeakOpp.Get())
				StrongOpp->DoMeleeAttack();
		}),
		AttackInterval, /*bLoop=*/true);
}

void AWYAOpponentController::LeaveAttack()
{
	if (UWorld* World = GetWorld())
		World->GetTimerManager().ClearTimer(AttackHandle);
}

// ── Personality ───────────────────────────────────────────────────────────────

void AWYAOpponentController::AssignPersonality(const FWYAOpponentPersonality& Personality)
{
	WriteToBlackboard(Personality);
}

void AWYAOpponentController::OnPersonalityReady(bool bSuccess, const FWYAOpponentPersonality& Personality)
{
	WriteToBlackboard(Personality);
	UE_LOG(LogTemp, Log, TEXT("WYAOpponentController: personality applied — faction=%s aggression=%.2f range=%d"),
		*Personality.FactionTag, Personality.AggressionLevel, static_cast<int32>(Personality.PreferredRange));
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
