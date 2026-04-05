#include "Characters/AWYASecurityCharacter.h"
#include "Contracts/WYAContractSubsystem.h"
#include "Combat/WYACombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "EngineUtils.h"

AWYASecurityCharacter::AWYASecurityCharacter()
{
    FactionTag = TEXT("Security");
    PrimaryActorTick.bCanEverTick = false;
}

void AWYASecurityCharacter::BeginPlay()
{
    Super::BeginPlay(); // registers base OnDowned

    SpawnLocation = GetActorLocation();

    // Override the base downed handler to add alert notification
    Combat->OnDowned.AddDynamic(this, &AWYASecurityCharacter::OnSecurityDowned);

    // Start patrol loop when Unaware
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(PatrolTimerHandle,
            this, &AWYASecurityCharacter::PatrolTick,
            PatrolInterval, /*bLooping=*/true,
            FMath::RandRange(0.f, PatrolInterval)); // stagger initial patrol
    }
}

// ── Patrol ────────────────────────────────────────────────────────────────────

void AWYASecurityCharacter::PatrolTick()
{
    if (AlertState != EWYAAlertState::Unaware) return;
    if (Combat->IsDown()) return;

    // Pick a random point within PatrolRadius of spawn
    const float Angle = FMath::RandRange(0.f, 360.f);
    const float Dist  = FMath::RandRange(200.f, PatrolRadius);

    const FVector Target = SpawnLocation + FVector(
        FMath::Cos(FMath::DegreesToRadians(Angle)) * Dist,
        FMath::Sin(FMath::DegreesToRadians(Angle)) * Dist,
        0.f);

    // Use movement input toward target — the AI controller handles pathfinding
    // when a NavMesh exists; this direct input works on Cesium terrain without one.
    const FVector Dir = (Target - GetActorLocation()).GetSafeNormal2D();
    GetCharacterMovement()->AddInputVector(Dir * 0.6f); // patrol speed (~250 UU/s)
}

// ── Alert ─────────────────────────────────────────────────────────────────────

void AWYASecurityCharacter::RaiseAlert(EWYAAlertState NewState)
{
    if (static_cast<uint8>(NewState) <= static_cast<uint8>(AlertState)) return; // no downgrade
    if (Combat->IsDown()) return;

    const EWYAAlertState Previous = AlertState;
    AlertState = NewState;

    if (NewState == EWYAAlertState::Suspicious)
    {
        BP_OnSuspicious();

        // Auto-escalate to Alerted after 3s if still Suspicious
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().SetTimer(SuspicionTimerHandle,
                FTimerDelegate::CreateLambda([this]()
                {
                    if (AlertState == EWYAAlertState::Suspicious)
                        RaiseAlert(EWYAAlertState::Alerted);
                }),
                3.f, false);
        }
    }
    else if (NewState == EWYAAlertState::Alerted)
    {
        BP_OnAlerted();

        // Stop patrol timer
        if (UWorld* World = GetWorld())
            World->GetTimerManager().ClearTimer(PatrolTimerHandle);

        // Propagate to nearby security
        UWorld* World = GetWorld();
        if (World)
        {
            for (TActorIterator<AWYASecurityCharacter> It(World); It; ++It)
            {
                AWYASecurityCharacter* Other = *It;
                if (!Other || Other == this || Other->Combat->IsDown()) continue;

                const float Dist = FVector::Dist(GetActorLocation(), Other->GetActorLocation());
                if (Dist <= AlertPropagationRadius)
                {
                    Other->RaiseAlert(EWYAAlertState::Alerted);
                }
            }
        }

        // Notify ContractSubsystem — voids Ghost bonus for all active runs
        if (UGameInstance* GI = GetGameInstance())
        {
            if (UWYAContractSubsystem* ContractSub = GI->GetSubsystem<UWYAContractSubsystem>())
            {
                ContractSub->NotifySecurityAlerted();
            }
        }
    }
}

// ── Down ──────────────────────────────────────────────────────────────────────

void AWYASecurityCharacter::OnSecurityDowned(AActor* Attacker)
{
    // If we were Unaware, no alert — stealth kill
    // If we were Suspicious/Alerted, the alert already propagated

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(PatrolTimerHandle);
        World->GetTimerManager().ClearTimer(SuspicionTimerHandle);
    }
}
