#include "Contracts/WYAContractSubsystem.h"
#include "Characters/AWYANamedTargetCharacter.h"
#include "AI/WYAAISubsystem.h"
#include "Economy/WYACurrencySubsystem.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UWYAContractSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Collection.InitializeDependency<UWYAAISubsystem>();
    Collection.InitializeDependency<UWYACurrencySubsystem>();
    Super::Initialize(Collection);

    AISub       = GetGameInstance()->GetSubsystem<UWYAAISubsystem>();
    CurrencySub = GetGameInstance()->GetSubsystem<UWYACurrencySubsystem>();

    // Initial fill attempt — may be empty on first session (AI still generating).
    TryFillBoard();

    // Poll every 5 seconds while the board has open slots.
    // Stops naturally when the board is full; restarts on TakeContract().
    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        World->GetTimerManager().SetTimer(BoardFillTimerHandle,
            FTimerDelegate::CreateUObject(this, &UWYAContractSubsystem::TryFillBoard),
            5.f, /*bLooping=*/true);
    }

    UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: initialized (board=%d slots)"), BoardSize);
}

// ── Board management ──────────────────────────────────────────────────────────

void UWYAContractSubsystem::TryFillBoard()
{
    if (!AISub) return;

    bool bChanged = false;

    while (Board.Num() < BoardSize)
    {
        FWYAContract Contract;
        if (!AISub->TryPopPregeneratedContract(Contract)) break;

        Board.Add(Contract);
        bChanged = true;

        UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: board +1 — '%s' (%s, %dG)"),
            *Contract.TargetName,
            Contract.Tier == EWYAContractTier::HighTable ? TEXT("High Table") :
            Contract.Tier == EWYAContractTier::Priority  ? TEXT("Priority")   : TEXT("Standard"),
            Contract.GoldReward);
    }

    if (bChanged)
    {
        OnBoardRefreshed.Broadcast(Board);
    }
}

// ── Take / complete / abandon ─────────────────────────────────────────────────

bool UWYAContractSubsystem::TakeContract(APlayerController* PC, const FString& ContractID)
{
    if (!PC) return false;

    // Check active limit
    TArray<FWYAContract>& Active = ActiveContracts.FindOrAdd(PC);
    if (Active.Num() >= MaxActiveContracts)
    {
        UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: %s already at max active contracts (%d)"),
            *GetNameSafe(PC), MaxActiveContracts);
        return false;
    }

    // Find contract on board
    const int32 BoardIdx = Board.IndexOfByPredicate(
        [&ContractID](const FWYAContract& C){ return C.ID == ContractID; });

    if (BoardIdx == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("WYAContractSubsystem: TakeContract — ID '%s' not on board"),
            *ContractID);
        return false;
    }

    FWYAContract Taken = Board[BoardIdx];
    Board.RemoveAt(BoardIdx);
    Active.Add(Taken);

    UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: %s took contract '%s' (%dG)"),
        *GetNameSafe(PC), *Taken.TargetName, Taken.GoldReward);

    OnContractTaken.Broadcast(PC, Taken);
    OnBoardRefreshed.Broadcast(Board);

    // Spawn the named target 50–150m from the player
    if (APawn* Pawn = PC->GetPawn())
    {
        if (UWorld* World = GetGameInstance()->GetWorld())
        {
            const float Angle = FMath::RandRange(0.f, 360.f);
            const float Dist  = FMath::RandRange(5000.f, 15000.f); // 50-150m in UU

            const FVector SpawnOffset(
                FMath::Cos(FMath::DegreesToRadians(Angle)) * Dist,
                FMath::Sin(FMath::DegreesToRadians(Angle)) * Dist,
                200.f);

            const FVector SpawnLoc = Pawn->GetActorLocation() + SpawnOffset;

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            AWYANamedTargetCharacter* Target = World->SpawnActor<AWYANamedTargetCharacter>(
                AWYANamedTargetCharacter::StaticClass(), SpawnLoc, FRotator::ZeroRotator, SpawnParams);

            if (Target)
            {
                Target->ContractID        = Taken.ID;
                Target->TargetDisplayName = Taken.TargetName;
                Target->Tier              = Taken.Tier;
                Target->FactionTag        = Taken.TargetRole; // role text as faction context for AI personality

                UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: spawned named target '%s' %.0fm from player"),
                    *Taken.TargetName, Dist / 100.f);
            }
        }
    }

    // Refill the open board slot from the pre-gen buffer
    TryFillBoard();

    return true;
}

bool UWYAContractSubsystem::CompleteContract(APlayerController* PC, const FString& ContractID)
{
    if (!PC) return false;

    TArray<FWYAContract>* Active = ActiveContracts.Find(PC);
    if (!Active) return false;

    const int32 Idx = Active->IndexOfByPredicate(
        [&ContractID](const FWYAContract& C){ return C.ID == ContractID; });

    if (Idx == INDEX_NONE) return false;

    const FWYAContract Completed = (*Active)[Idx];
    Active->RemoveAt(Idx);

    // Award Gold
    if (CurrencySub)
    {
        const int32 NewBalance = CurrencySub->AwardGold(Completed.GoldReward);
        UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: contract '%s' complete — awarded %dG (balance: %dG)"),
            *Completed.TargetName, Completed.GoldReward, NewBalance);
    }

    OnContractCompleted.Broadcast(PC, Completed);
    return true;
}

bool UWYAContractSubsystem::AbandonContract(APlayerController* PC, const FString& ContractID)
{
    if (!PC) return false;

    TArray<FWYAContract>* Active = ActiveContracts.Find(PC);
    if (!Active) return false;

    const int32 Idx = Active->IndexOfByPredicate(
        [&ContractID](const FWYAContract& C){ return C.ID == ContractID; });

    if (Idx == INDEX_NONE) return false;

    UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: %s abandoned contract '%s'"),
        *GetNameSafe(PC), *(*Active)[Idx].TargetName);

    Active->RemoveAt(Idx);
    return true;
}

TArray<FWYAContract> UWYAContractSubsystem::GetActiveContracts(APlayerController* PC) const
{
    const TArray<FWYAContract>* Found = ActiveContracts.Find(PC);
    return Found ? *Found : TArray<FWYAContract>{};
}

bool UWYAContractSubsystem::HasActiveContract(APlayerController* PC) const
{
    const TArray<FWYAContract>* Found = ActiveContracts.Find(PC);
    return Found && Found->Num() > 0;
}
