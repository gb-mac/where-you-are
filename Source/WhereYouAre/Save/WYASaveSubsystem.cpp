#include "Save/WYASaveSubsystem.h"
#include "Save/WYASaveGame.h"
#include "Quest/WYAFixHimQuestSubsystem.h"
#include "Inventory/WYAInventoryComponent.h"
#include "WYACharacter.h"
#include "Survival/WYASurvivalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UWYASaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    // Ensure fix-him subsystem is up before we touch its state.
    Collection.InitializeDependency<UWYAFixHimQuestSubsystem>();

    Super::Initialize(Collection);

    // Load immediately so fix-him state is available before SpawnPlayer fires.
    LoadGame();

    // Increment session count now that we have (or created) a save object.
    if (CachedSave)
    {
        CachedSave->SessionCount++;
        UE_LOG(LogTemp, Log, TEXT("WYASaveSubsystem: session %d started (%.0fs played so far)"),
            CachedSave->SessionCount, CachedSave->TotalPlaytimeSecs);
    }

    // Tick playtime every 60 seconds.
    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        World->GetTimerManager().SetTimer(
            PlaytimeTickHandle,
            this,
            &UWYASaveSubsystem::OnPlaytimeTick,
            60.f,
            /*bLooping=*/true);
    }
}

void UWYASaveSubsystem::SaveGame()
{
    UWYASaveGame* SaveObj = Cast<UWYASaveGame>(
        UGameplayStatics::CreateSaveGameObject(UWYASaveGame::StaticClass()));
    if (!SaveObj) return;

    // Fix-him state
    if (UWYAFixHimQuestSubsystem* FixHim = GetGameInstance()->GetSubsystem<UWYAFixHimQuestSubsystem>())
    {
        SaveObj->bMobilityRepaired   = FixHim->IsMobilityRepaired();
        SaveObj->bProcessingRepaired = FixHim->IsProcessingRepaired();
        SaveObj->bPowerRepaired      = FixHim->IsPowerRepaired();
        SaveObj->bCommsRepaired      = FixHim->IsCommsRepaired();
        SaveObj->bQuestlineActive    = FixHim->IsQuestlineActive();
    }

    // Carry forward playtime / session / trigger flag from cached save
    if (CachedSave)
    {
        SaveObj->TotalPlaytimeSecs        = CachedSave->TotalPlaytimeSecs;
        SaveObj->SessionCount             = CachedSave->SessionCount;
        SaveObj->bFixHimQuestlineTriggered = CachedSave->bFixHimQuestlineTriggered;
    }

    // Inventory and survival — gather from local player pawn
    if (UGameInstance* GI = GetGameInstance())
    {
        if (APlayerController* PC = GI->GetFirstLocalPlayerController())
        {
            if (AWYACharacter* Char = Cast<AWYACharacter>(PC->GetPawn()))
            {
                if (UWYAInventoryComponent* Inv = Char->Inventory)
                {
                    SaveObj->SavedInventory = Inv->GetItems();
                }

                if (UWYASurvivalComponent* Surv = Char->Survival)
                {
                    Surv->GetSaveData(SaveObj->SavedWater, SaveObj->SavedFood);
                }
            }
        }
    }

    CachedSave = SaveObj;

    UGameplayStatics::AsyncSaveGameToSlot(SaveObj, UWYASaveGame::SlotName, UWYASaveGame::UserIndex,
        FAsyncSaveGameToSlotDelegate::CreateLambda(
            [](const FString& /*Slot*/, const int32 /*UserIdx*/, bool bSuccess)
            {
                UE_LOG(LogTemp, Log, TEXT("WYASaveSubsystem: save %s"),
                    bSuccess ? TEXT("OK") : TEXT("FAILED"));
            }));
}

void UWYASaveSubsystem::LoadGame()
{
    if (UGameplayStatics::DoesSaveGameExist(UWYASaveGame::SlotName, UWYASaveGame::UserIndex))
    {
        CachedSave = Cast<UWYASaveGame>(
            UGameplayStatics::LoadGameFromSlot(UWYASaveGame::SlotName, UWYASaveGame::UserIndex));
    }

    if (!CachedSave)
    {
        // Fresh start — create an empty save object so the rest of Initialize() can
        // write to it (session count, playtime) without null-checks everywhere.
        CachedSave = Cast<UWYASaveGame>(
            UGameplayStatics::CreateSaveGameObject(UWYASaveGame::StaticClass()));

        UE_LOG(LogTemp, Log, TEXT("WYASaveSubsystem: no save found — fresh start"));
    }
    else
    {
        // Restore fix-him state without triggering dialogue
        if (UWYAFixHimQuestSubsystem* FixHim = GetGameInstance()->GetSubsystem<UWYAFixHimQuestSubsystem>())
        {
            FixHim->LoadRepairState(
                CachedSave->bMobilityRepaired,
                CachedSave->bProcessingRepaired,
                CachedSave->bPowerRepaired,
                CachedSave->bCommsRepaired);

            // If the questline was already triggered on a previous session, mark it
            // on the subsystem so we don't fire it again.
            if (CachedSave->bFixHimQuestlineTriggered && !FixHim->IsQuestlineActive())
            {
                // The questline has been triggered before but save state shows it's
                // not yet active (no repairs done yet on first play-through this
                // can happen if player triggered the conversation but hasn't repaired
                // anything). Call TriggerQuestline() silently — it guards against
                // double-fire internally.
                FixHim->TriggerQuestline();
            }
        }

        // Survival state is restored on the character pawn in OnSaveLoaded —
        // the pawn may not exist yet at load time, so we carry CachedSave forward
        // and apply it in ApplySurvivalToCharacter() after BeginPlay.

        UE_LOG(LogTemp, Log,
            TEXT("WYASaveSubsystem: save loaded (mobility=%d processing=%d power=%d comms=%d "
                 "playtime=%.0fs sessions=%d fixhim_triggered=%d water=%.0f food=%.0f)"),
            CachedSave->bMobilityRepaired,
            CachedSave->bProcessingRepaired,
            CachedSave->bPowerRepaired,
            CachedSave->bCommsRepaired,
            CachedSave->TotalPlaytimeSecs,
            CachedSave->SessionCount,
            CachedSave->bFixHimQuestlineTriggered ? 1 : 0,
            CachedSave->SavedWater,
            CachedSave->SavedFood);
    }

    OnSaveLoaded.Broadcast();
}

bool UWYASaveSubsystem::HasSaveData() const
{
    return IsValid(CachedSave);
}

// ---------------------------------------------------------------------------
// Playtime tracking
// ---------------------------------------------------------------------------

void UWYASaveSubsystem::OnPlaytimeTick()
{
    if (!CachedSave) return;

    CachedSave->TotalPlaytimeSecs += 60.f;
    PlaytimeTickCount++;

    CheckFixHimTrigger();

    // Persist to disk every 5th tick (every 5 minutes).
    if (PlaytimeTickCount % 5 == 0)
    {
        UE_LOG(LogTemp, Verbose, TEXT("WYASaveSubsystem: periodic save (%.0fs played)"),
            CachedSave->TotalPlaytimeSecs);
        SaveGame();
    }
}

void UWYASaveSubsystem::CheckFixHimTrigger()
{
    if (!CachedSave) return;

    if (CachedSave->bFixHimQuestlineTriggered) return;

    if (CachedSave->TotalPlaytimeSecs < FixHimTriggerPlaytimeThreshold) return;

    UWYAFixHimQuestSubsystem* FixHim = GetGameInstance()->GetSubsystem<UWYAFixHimQuestSubsystem>();
    if (!FixHim) return;

    // If it's already active (repairs started), just mark and return.
    if (FixHim->IsQuestlineActive())
    {
        CachedSave->bFixHimQuestlineTriggered = true;
        SaveGame();
        return;
    }

    UE_LOG(LogTemp, Log,
        TEXT("WYASaveSubsystem: playtime threshold reached (%.0fs) — triggering fix-him questline"),
        CachedSave->TotalPlaytimeSecs);

    FixHim->TriggerQuestline();

    CachedSave->bFixHimQuestlineTriggered = true;

    // Persist immediately so the trigger survives a crash or quit.
    SaveGame();
}
