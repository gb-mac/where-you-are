#include "Contracts/WYAContractSubsystem.h"
#include "Characters/AWYANamedTargetCharacter.h"
#include "Characters/AWYASecurityCharacter.h"
#include "Actors/AWYAExfilPoint.h"
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

    // Start run state tracking
    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        FWYAContractRunState& RunState = RunStates.Add(Taken.ID);
        RunState.ContractID    = Taken.ID;
        RunState.StartTimeSecs = World->GetTimeSeconds();
    }

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

    const FString AbandonedID = (*Active)[Idx].ID;
    Active->RemoveAt(Idx);
    RunStates.Remove(AbandonedID);

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

// ── Save / load ───────────────────────────────────────────────────────────────

void UWYAContractSubsystem::GetSaveData(APlayerController* PC,
                                         TArray<FWYAContract>& OutContracts,
                                         TArray<FWYAContractRunState>& OutStates) const
{
    OutContracts.Empty();
    OutStates.Empty();

    const TArray<FWYAContract>* Active = ActiveContracts.Find(PC);
    if (!Active || Active->IsEmpty()) return;

    OutContracts = *Active;

    for (const FWYAContract& C : *Active)
    {
        if (const FWYAContractRunState* State = RunStates.Find(C.ID))
        {
            OutStates.Add(*State);
        }
    }
}

void UWYAContractSubsystem::LoadSavedContracts(APlayerController* PC,
                                                const TArray<FWYAContract>& Contracts,
                                                const TArray<FWYAContractRunState>& States)
{
    if (!PC || Contracts.IsEmpty()) return;

    TArray<FWYAContract>& Active = ActiveContracts.FindOrAdd(PC);
    Active = Contracts;

    for (const FWYAContractRunState& State : States)
    {
        RunStates.Add(State.ContractID, State);
    }

    UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: restored %d active contract(s) for %s"),
        Contracts.Num(), *GetNameSafe(PC));

    OnBoardRefreshed.Broadcast(Board); // update board UI in case it's open
}

// ── Security / damage / exfil hooks ──────────────────────────────────────────

void UWYAContractSubsystem::NotifySecurityAlerted()
{
    for (auto& Pair : RunStates)
    {
        Pair.Value.AlertCount++;
    }
    UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: security alerted — Ghost bonus voided for all active runs"));
}

void UWYAContractSubsystem::NotifyPlayerTookDamage(APlayerController* PC)
{
    if (!PC) return;

    const TArray<FWYAContract>* Active = ActiveContracts.Find(PC);
    if (!Active) return;

    for (const FWYAContract& C : *Active)
    {
        if (FWYAContractRunState* RunState = RunStates.Find(C.ID))
        {
            if (!RunState->bTookDamage)
            {
                RunState->bTookDamage = true;
                UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: %s took damage — Clean bonus voided for '%s'"),
                    *GetNameSafe(PC), *C.TargetName);
            }
        }
    }
}

void UWYAContractSubsystem::ReachExfil(APlayerController* PC, const FString& ContractID)
{
    if (!PC) return;

    TArray<FWYAContract>* Active = ActiveContracts.Find(PC);
    if (!Active) return;

    const int32 Idx = Active->IndexOfByPredicate(
        [&ContractID](const FWYAContract& C){ return C.ID == ContractID; });

    if (Idx == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("WYAContractSubsystem: ReachExfil — contract '%s' not in %s active list"),
            *ContractID, *GetNameSafe(PC));
        return;
    }

    const FWYAContract Contract = (*Active)[Idx];
    Active->RemoveAt(Idx);

    // Grab (and remove) run state
    FWYAContractRunState RunState;
    if (FWYAContractRunState* Found = RunStates.Find(ContractID))
    {
        Found->bExfilReached = true;
        RunState = *Found;
        RunStates.Remove(ContractID);
    }
    else
    {
        RunState.ContractID    = ContractID;
        RunState.bExfilReached = true;
    }

    const int32 TotalGold = CalculateAndAwardBonus(PC, Contract, RunState);

    OnContractCompleted.Broadcast(PC, Contract);

    UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: %s exfil'd '%s' — total %dG awarded"),
        *GetNameSafe(PC), *Contract.TargetName, TotalGold);
}

int32 UWYAContractSubsystem::CalculateAndAwardBonus(APlayerController* PC,
                                                     const FWYAContract& Contract,
                                                     const FWYAContractRunState& RunState)
{
    const int32 Base  = Contract.GoldReward;
    int32       Bonus = 0;

    // Ghost: no security alerted this run → +50%
    if (RunState.AlertCount == 0)
    {
        Bonus += FMath::RoundToInt(Base * 0.5f);
        UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: Ghost bonus +%dG"), FMath::RoundToInt(Base * 0.5f));
    }

    // Swift: completed under tier time limit → +25%
    float SwiftLimit = SwiftLimit_Standard;
    switch (Contract.Tier)
    {
    case EWYAContractTier::Priority:  SwiftLimit = SwiftLimit_Priority;  break;
    case EWYAContractTier::HighTable: SwiftLimit = SwiftLimit_HighTable; break;
    default: break;
    }

    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        const float Elapsed = World->GetTimeSeconds() - RunState.StartTimeSecs;
        if (RunState.StartTimeSecs > 0.f && Elapsed <= SwiftLimit)
        {
            Bonus += FMath::RoundToInt(Base * 0.25f);
            UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: Swift bonus +%dG (%.0fs of %.0fs)"),
                FMath::RoundToInt(Base * 0.25f), Elapsed, SwiftLimit);
        }
    }

    // Clean: no damage taken → +25%
    if (!RunState.bTookDamage)
    {
        Bonus += FMath::RoundToInt(Base * 0.25f);
        UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: Clean bonus +%dG"), FMath::RoundToInt(Base * 0.25f));
    }

    const int32 Total = Base + Bonus;

    if (CurrencySub)
    {
        const int32 NewBalance = CurrencySub->AwardGold(Total);
        UE_LOG(LogTemp, Log, TEXT("WYAContractSubsystem: paid %dG (base %d + bonus %d) — balance %dG"),
            Total, Base, Bonus, NewBalance);
    }

    return Total;
}
