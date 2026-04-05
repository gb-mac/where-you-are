#include "Contracts/AWYAContractBoard.h"
#include "Contracts/WYAContractSubsystem.h"
#include "Save/WYASaveSubsystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AWYAContractBoard::AWYAContractBoard()
{
    PrimaryActorTick.bCanEverTick = false;

    ProximityVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityVolume"));
    ProximityVolume->InitSphereRadius(300.f);
    SetRootComponent(ProximityVolume);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

void AWYAContractBoard::BeginPlay()
{
    Super::BeginPlay();

    // Subscribe to board changes so BP_OnBoardRefreshed fires automatically
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UWYAContractSubsystem* ContractSub = GI->GetSubsystem<UWYAContractSubsystem>())
        {
            ContractSub->OnBoardRefreshed.AddUObject(this, &AWYAContractBoard::OnBoardRefreshed);
        }
    }
}

// ── Player interaction ────────────────────────────────────────────────────────

void AWYAContractBoard::InteractWithBoard(APlayerController* PC)
{
    if (!PC) return;

    UGameInstance* GI = GetGameInstance();
    if (!GI) return;

    UWYAContractSubsystem* ContractSub = GI->GetSubsystem<UWYAContractSubsystem>();
    if (!ContractSub) return;

    const TArray<FWYAContract>& Board = ContractSub->GetBoardContracts();

    UE_LOG(LogTemp, Log, TEXT("AWYAContractBoard: %s interacted — board has %d contract(s)"),
        *GetNameSafe(PC), Board.Num());

    BP_OnBoardInteracted(PC, Board);
}

bool AWYAContractBoard::TakeContract(APlayerController* PC, const FString& ContractID)
{
    if (!PC) return false;

    UGameInstance* GI = GetGameInstance();
    if (!GI) return false;

    UWYAContractSubsystem* ContractSub = GI->GetSubsystem<UWYAContractSubsystem>();
    if (!ContractSub) return false;

    const bool bOk = ContractSub->TakeContract(PC, ContractID);

    // Find the contract data for the BP feedback event (search board OR just broadcast empty on fail)
    FWYAContract TakenContract;
    for (const FWYAContract& C : ContractSub->GetActiveContracts(PC))
    {
        if (C.ID == ContractID)
        {
            TakenContract = C;
            break;
        }
    }

    BP_OnContractTaken(TakenContract, bOk);

    if (bOk)
    {
        // Persist immediately — active contract list changed
        if (UWYASaveSubsystem* SaveSub = GI->GetSubsystem<UWYASaveSubsystem>())
        {
            SaveSub->SaveGame();
        }
    }

    return bOk;
}

FText AWYAContractBoard::GetInteractionPrompt() const
{
    UGameInstance* GI = GetGameInstance();
    if (!GI) return FText::FromString(TEXT("Contract Board"));

    const UWYAContractSubsystem* ContractSub = GI->GetSubsystem<UWYAContractSubsystem>();
    if (!ContractSub) return FText::FromString(TEXT("Contract Board"));

    return ContractSub->HasBoardContracts()
        ? FText::FromString(TEXT("[E] View Contracts"))
        : FText::FromString(TEXT("[E] Board Empty"));
}

// ── Board refresh callback ────────────────────────────────────────────────────

void AWYAContractBoard::OnBoardRefreshed(const TArray<FWYAContract>& Board)
{
    BP_OnBoardRefreshed(Board);
}
