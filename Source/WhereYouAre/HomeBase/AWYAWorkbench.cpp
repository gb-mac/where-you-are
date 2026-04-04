#include "HomeBase/AWYAWorkbench.h"
#include "Quest/WYAFixHimQuestSubsystem.h"
#include "Save/WYASaveSubsystem.h"
#include "Inventory/WYAInventoryComponent.h"
#include "WYACharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"

AWYAWorkbench::AWYAWorkbench()
{
    PrimaryActorTick.bCanEverTick = false;

    ProximityVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityVolume"));
    ProximityVolume->InitSphereRadius(300.f);
    SetRootComponent(ProximityVolume);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

bool AWYAWorkbench::TryInstallComponent(APlayerController* PC)
{
    if (!PC) return false;

    AWYACharacter* Char = Cast<AWYACharacter>(PC->GetPawn());
    if (!Char) return false;

    UWYAInventoryComponent* Inv = Char->Inventory;
    if (!Inv) return false;

    UGameInstance* GI = PC->GetGameInstance();
    if (!GI) return false;

    UWYAFixHimQuestSubsystem* FixHim = GI->GetSubsystem<UWYAFixHimQuestSubsystem>();
    if (!FixHim) return false;

    // Install in prerequisite order: Mobility → Power → Comms
    // Processing is never installed here (Dr. Osei handles it).

    if (!FixHim->IsMobilityRepaired() && Inv->HasItem(EWYACarriedItemType::FixHim_MobilityParts))
    {
        Inv->RemoveItem(EWYACarriedItemType::FixHim_MobilityParts);
        FixHim->OnMobilityRepairCompleted();

        if (UWYASaveSubsystem* SaveSub = GI->GetSubsystem<UWYASaveSubsystem>())
            SaveSub->SaveGame();

        UE_LOG(LogTemp, Log, TEXT("AWYAWorkbench: Mobility repair installed"));
        return true;
    }

    if (FixHim->IsMobilityRepaired() && FixHim->IsProcessingRepaired()
        && !FixHim->IsPowerRepaired()
        && Inv->HasItem(EWYACarriedItemType::FixHim_PowerCore))
    {
        Inv->RemoveItem(EWYACarriedItemType::FixHim_PowerCore);
        FixHim->OnPowerRepairCompleted();

        if (UWYASaveSubsystem* SaveSub = GI->GetSubsystem<UWYASaveSubsystem>())
            SaveSub->SaveGame();

        UE_LOG(LogTemp, Log, TEXT("AWYAWorkbench: Power repair installed"));
        return true;
    }

    if (FixHim->IsPowerRepaired() && !FixHim->IsCommsRepaired()
        && Inv->HasItem(EWYACarriedItemType::FixHim_CommHardware))
    {
        Inv->RemoveItem(EWYACarriedItemType::FixHim_CommHardware);
        FixHim->OnCommunicationRepairCompleted();

        if (UWYASaveSubsystem* SaveSub = GI->GetSubsystem<UWYASaveSubsystem>())
            SaveSub->SaveGame();

        UE_LOG(LogTemp, Log, TEXT("AWYAWorkbench: Comms repair installed"));
        return true;
    }

    return false;
}

FText AWYAWorkbench::GetInteractionPrompt(APlayerController* PC) const
{
    if (!PC) return FText::FromString(TEXT("Workbench"));

    AWYACharacter* Char = Cast<AWYACharacter>(PC->GetPawn());
    UWYAInventoryComponent* Inv = Char ? Char->Inventory : nullptr;

    UGameInstance* GI = PC->GetGameInstance();
    UWYAFixHimQuestSubsystem* FixHim = GI ? GI->GetSubsystem<UWYAFixHimQuestSubsystem>() : nullptr;

    if (!Inv || !FixHim)
        return FText::FromString(TEXT("Workbench"));

    // Mobility — no prerequisite
    if (!FixHim->IsMobilityRepaired())
    {
        if (Inv->HasItem(EWYACarriedItemType::FixHim_MobilityParts))
            return FText::FromString(TEXT("Install mobility chassis"));

        // Player may have Power/Comms parts but not Mobility — guide them
        if (Inv->HasItem(EWYACarriedItemType::FixHim_PowerCore)
            || Inv->HasItem(EWYACarriedItemType::FixHim_CommHardware))
            return FText::FromString(TEXT("Complete earlier repairs first"));

        return FText::FromString(TEXT("Nothing to install"));
    }

    // Power — requires Mobility + Processing (Dr. Osei)
    if (!FixHim->IsPowerRepaired())
    {
        if (!FixHim->IsProcessingRepaired())
            return FText::FromString(TEXT("Find Dr. Osei — processing needs her help"));

        if (Inv->HasItem(EWYACarriedItemType::FixHim_PowerCore))
            return FText::FromString(TEXT("Install power core"));

        if (Inv->HasItem(EWYACarriedItemType::FixHim_CommHardware))
            return FText::FromString(TEXT("Complete earlier repairs first"));

        return FText::FromString(TEXT("Nothing to install"));
    }

    // Comms — requires Power
    if (!FixHim->IsCommsRepaired())
    {
        if (Inv->HasItem(EWYACarriedItemType::FixHim_CommHardware))
            return FText::FromString(TEXT("Install communication hardware"));

        return FText::FromString(TEXT("Nothing to install"));
    }

    return FText::FromString(TEXT("All repairs complete"));
}
