#include "Save/WYASaveSubsystem.h"
#include "Save/WYASaveGame.h"
#include "Quest/WYAFixHimQuestSubsystem.h"
#include "Inventory/WYAInventoryComponent.h"
#include "WYACharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UWYASaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Load immediately so fix-him state is available before SpawnPlayer fires.
    LoadGame();
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

    // Inventory — gather from local player pawn
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

    if (CachedSave)
    {
        // Restore fix-him state without triggering dialogue
        if (UWYAFixHimQuestSubsystem* FixHim = GetGameInstance()->GetSubsystem<UWYAFixHimQuestSubsystem>())
        {
            FixHim->LoadRepairState(
                CachedSave->bMobilityRepaired,
                CachedSave->bProcessingRepaired,
                CachedSave->bPowerRepaired,
                CachedSave->bCommsRepaired);
        }

        UE_LOG(LogTemp, Log, TEXT("WYASaveSubsystem: save loaded (mobility=%d processing=%d power=%d comms=%d)"),
            CachedSave->bMobilityRepaired,
            CachedSave->bProcessingRepaired,
            CachedSave->bPowerRepaired,
            CachedSave->bCommsRepaired);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("WYASaveSubsystem: no save found — fresh start"));
    }

    OnSaveLoaded.Broadcast();
}

bool UWYASaveSubsystem::HasSaveData() const
{
    return IsValid(CachedSave);
}
