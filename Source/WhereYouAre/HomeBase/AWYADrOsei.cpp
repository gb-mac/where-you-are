#include "HomeBase/AWYADrOsei.h"
#include "Quest/WYAFixHimQuestSubsystem.h"
#include "Save/WYASaveSubsystem.h"
#include "Inventory/WYAInventoryComponent.h"
#include "WYACharacter.h"
#include "Components/SphereComponent.h"

AWYADrOsei::AWYADrOsei()
{
    PrimaryActorTick.bCanEverTick = false;

    ProximityVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityVolume"));
    ProximityVolume->InitSphereRadius(300.f);
    SetRootComponent(ProximityVolume);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

bool AWYADrOsei::TryInteract(APlayerController* PC)
{
    if (!PC) return false;

    UGameInstance* GI = PC->GetGameInstance();
    if (!GI) return false;

    UWYAFixHimQuestSubsystem* FixHim = GI->GetSubsystem<UWYAFixHimQuestSubsystem>();
    if (!FixHim) return false;

    // Already done — no-op
    if (FixHim->IsProcessingRepaired()) return false;

    // Mobility is the prerequisite for either phase
    if (!FixHim->IsMobilityRepaired())
    {
        UE_LOG(LogTemp, Log, TEXT("AWYADrOsei: Mobility not yet repaired — cannot start diagnostic"));
        return false;
    }

    // ── Phase 1: first visit — issue the diagnostic log objective ─────────────
    if (!FixHim->IsOseiDiagnosticRequested())
    {
        FixHim->SetOseiDiagnosticRequested(true);

        if (UWYASaveSubsystem* SaveSub = GI->GetSubsystem<UWYASaveSubsystem>())
            SaveSub->SaveGame();

        UE_LOG(LogTemp, Log,
            TEXT("AWYADrOsei: Phase 1 — diagnostic requested, player needs DiagnosticLogPackage"));
        return true;
    }

    // ── Phase 2: player returns with log package — run diagnostic, repair ─────
    AWYACharacter* Char = Cast<AWYACharacter>(PC->GetPawn());
    UWYAInventoryComponent* Inv = Char ? Char->Inventory : nullptr;
    if (!Inv) return false;

    if (!Inv->HasItem(EWYACarriedItemType::FixHim_DiagnosticLogPackage))
    {
        UE_LOG(LogTemp, Log, TEXT("AWYADrOsei: Phase 2 — waiting for DiagnosticLogPackage"));
        return false;
    }

    Inv->RemoveItem(EWYACarriedItemType::FixHim_DiagnosticLogPackage);
    FixHim->OnProcessingRepairCompleted();

    if (UWYASaveSubsystem* SaveSub = GI->GetSubsystem<UWYASaveSubsystem>())
        SaveSub->SaveGame();

    UE_LOG(LogTemp, Log, TEXT("AWYADrOsei: Phase 2 — processing repair complete (Stage 3 entered)"));
    return true;
}

FText AWYADrOsei::GetInteractionPrompt(APlayerController* PC) const
{
    if (!PC) return FText::FromString(TEXT("Dr. Osei"));

    UGameInstance* GI = PC->GetGameInstance();
    UWYAFixHimQuestSubsystem* FixHim = GI ? GI->GetSubsystem<UWYAFixHimQuestSubsystem>() : nullptr;
    if (!FixHim) return FText::FromString(TEXT("Dr. Osei"));

    if (FixHim->IsProcessingRepaired())
        return FText::FromString(TEXT("Processing core restored"));

    if (!FixHim->IsMobilityRepaired())
        return FText::FromString(TEXT("Complete earlier repairs first"));

    if (!FixHim->IsOseiDiagnosticRequested())
        return FText::FromString(TEXT("Speak with Dr. Osei"));

    // Diagnostic has been requested — check whether player has the package
    AWYACharacter* Char = Cast<AWYACharacter>(PC->GetPawn());
    UWYAInventoryComponent* Inv = Char ? Char->Inventory : nullptr;

    if (Inv && Inv->HasItem(EWYACarriedItemType::FixHim_DiagnosticLogPackage))
        return FText::FromString(TEXT("Deliver diagnostic logs"));

    return FText::FromString(TEXT("Retrieve diagnostic log package"));
}
