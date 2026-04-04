#include "WYAPlayerController.h"
#include "UI/WYAInteractionWidget.h"
#include "Loot/AWYALootActor.h"
#include "Items/AWYAWorldItem.h"
#include "Items/WYAItemSubsystem.h"
#include "Api/WYAApiClient.h"
#include "Location/WYALocationSubsystem.h"
#include "Vehicles/WYAVehicleBase.h"
#include "HomeBase/AWYAWorkbench.h"
#include "Inventory/WYAInventoryComponent.h"
#include "WYACharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

AWYAPlayerController::AWYAPlayerController()
{
    bShowMouseCursor = false;
    PrimaryActorTick.bCanEverTick = true;
}

void AWYAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (InteractionWidgetClass)
    {
        HUDWidget = CreateWidget<UWYAInteractionWidget>(this, InteractionWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
            HUDWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void AWYAPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction(TEXT("Interact"),  IE_Pressed, this, &AWYAPlayerController::OnInteract);
    InputComponent->BindAction(TEXT("PlaceItem"), IE_Pressed, this, &AWYAPlayerController::OnPlaceItem);
    InputComponent->BindAction(TEXT("Vehicle"),   IE_Pressed, this, &AWYAPlayerController::OnVehicle);
}

void AWYAPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AWYAWorldItem* Closest = FindClosestItem();
    if (Closest != FocusedItem.Get())
    {
        SetFocusedItem(Closest);
    }

    AWYALootActor* ClosestLoot = FindClosestLootActor();
    if (ClosestLoot != FocusedLootActor.Get())
    {
        SetFocusedLootActor(ClosestLoot);
    }
}

// ── Interaction ───────────────────────────────────────────────────────────────

void AWYAPlayerController::OnInteract()
{
    // Workbench takes priority over GPS item claiming.
    if (AWYAWorkbench* Bench = FindClosestWorkbench())
    {
        AWYACharacter* Char = Cast<AWYACharacter>(GetPawn());
        if (Char && Char->Inventory)
        {
            const bool bHasFixHimPart =
                Char->Inventory->HasItem(EWYACarriedItemType::FixHim_MobilityParts)
                || Char->Inventory->HasItem(EWYACarriedItemType::FixHim_PowerCore)
                || Char->Inventory->HasItem(EWYACarriedItemType::FixHim_CommHardware);

            if (bHasFixHimPart)
            {
                Bench->TryInstallComponent(this);
                return;
            }
        }
    }

    // Loot pickup (after workbench, before GPS item claiming)
    if (AWYALootActor* Loot = FocusedLootActor.Get())
    {
        if (AWYACharacter* Char = Cast<AWYACharacter>(GetPawn()))
        {
            Loot->TryPickup(Char);
            return;
        }
    }

    AWYAWorldItem* Item = FocusedItem.Get();
    if (!Item || !Item->IsAvailable()) return;

    UWYAApiClient* Api = GetGameInstance()->GetSubsystem<UWYAApiClient>();
    UWYALocationSubsystem* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
    if (!Api || !LocSys || !LocSys->IsResolved()) return;

    const FWYAGeoCoord& Origin = LocSys->GetOrigin();
    // Player is always at world origin → GPS origin
    const FString ItemId = Item->GetItemData().Id;

    Api->ClaimItem(ItemId, Origin.Latitude, Origin.Longitude,
        [this](EWYAClaimResult Result, FWYAItemData UpdatedItem)
    {
        if (!HUDWidget) return;
        FText Msg;
        switch (Result)
        {
        case EWYAClaimResult::Success:
            Msg = FText::FromString(TEXT("Claimed!"));
            break;
        case EWYAClaimResult::InsufficientFunds:
            Msg = FText::FromString(TEXT("Can't afford this — not enough Gold or Silver"));
            break;
        default:
            Msg = FText::FromString(TEXT("Claim failed — too far or already taken"));
            break;
        }
        HUDWidget->ShowClaimResult(Result == EWYAClaimResult::Success, Msg);
    });
}

void AWYAPlayerController::OnPlaceItem()
{
    UWYAApiClient* Api = GetGameInstance()->GetSubsystem<UWYAApiClient>();
    UWYALocationSubsystem* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
    if (!Api || !LocSys || !LocSys->IsResolved()) return;

    // Place a SupplyCache at the player's current GPS position.
    // In later builds this will open an item type picker.
    APawn* MyPawn = GetPawn();
    const FWYAGeoCoord PlaceGeo = MyPawn
        ? LocSys->WorldToGeo(MyPawn->GetActorLocation())
        : LocSys->GetOrigin();

    Api->PlaceItem(EWYAItemType::SupplyCache,
        PlaceGeo.Latitude, PlaceGeo.Longitude, PlaceGeo.Altitude,
        /*PriceAmount=*/0, /*PriceCurrency=*/TEXT(""),
        [this](bool bOk, FWYAItemData NewItem)
    {
        UE_LOG(LogTemp, Log, TEXT("WYAPlayerController: PlaceItem %s (id=%s)"),
               bOk ? TEXT("OK") : TEXT("FAILED"), *NewItem.Id);

        // Trigger an immediate refresh so the new item appears
        if (bOk)
        {
            if (UWYAItemSubsystem* ItemSys = GetGameInstance()->GetSubsystem<UWYAItemSubsystem>())
                ItemSys->FetchNearbyItems();
        }
    });
}

// ── Vehicle ───────────────────────────────────────────────────────────────────

void AWYAPlayerController::OnVehicle()
{
    // If already in a vehicle — exit it
    if (AWYAVehicleBase* Vehicle = CurrentVehicle.Get())
    {
        Vehicle->Exit();
        CurrentVehicle = nullptr;
        return;
    }

    // Find a nearby vehicle and enter it
    AWYAVehicleBase* Nearby = FindClosestVehicle();
    if (!Nearby) return;

    AWYACharacter* MyChar = Cast<AWYACharacter>(GetPawn());
    if (!MyChar) return;

    Nearby->Enter(this, MyChar);
    CurrentVehicle = Nearby;
}

AWYAVehicleBase* AWYAPlayerController::FindClosestVehicle() const
{
    APawn* MyPawn = GetPawn();
    if (!MyPawn) return nullptr;
    const FVector MyPos = MyPawn->GetActorLocation();

    AWYAVehicleBase* Best     = nullptr;
    float            BestDist = VehicleEnterRadius;

    for (TActorIterator<AWYAVehicleBase> It(GetWorld()); It; ++It)
    {
        AWYAVehicleBase* V = *It;
        if (!IsValid(V) || V->IsOccupied()) continue;

        const float Dist = FVector::Dist(MyPos, V->GetActorLocation());
        if (Dist < BestDist)
        {
            BestDist = Dist;
            Best     = V;
        }
    }

    return Best;
}

// ── Workbench ─────────────────────────────────────────────────────────────────

AWYAWorkbench* AWYAPlayerController::FindClosestWorkbench() const
{
    APawn* MyPawn = GetPawn();
    if (!MyPawn) return nullptr;
    const FVector MyPos = MyPawn->GetActorLocation();

    AWYAWorkbench* Best     = nullptr;
    float          BestDist = InteractionRadius;

    for (TActorIterator<AWYAWorkbench> It(GetWorld()); It; ++It)
    {
        AWYAWorkbench* Bench = *It;
        if (!IsValid(Bench)) continue;

        const float Dist = FVector::Dist(MyPos, Bench->GetActorLocation());
        if (Dist < BestDist)
        {
            BestDist = Dist;
            Best     = Bench;
        }
    }

    return Best;
}

// ── Focus ─────────────────────────────────────────────────────────────────────

AWYAWorldItem* AWYAPlayerController::FindClosestItem() const
{
    UWYAItemSubsystem* ItemSys = GetGameInstance()->GetSubsystem<UWYAItemSubsystem>();
    if (!ItemSys) return nullptr;

    APawn* MyPawn = GetPawn();
    if (!MyPawn) return nullptr;
    const FVector MyPos = MyPawn->GetActorLocation();

    AWYAWorldItem* Best     = nullptr;
    float          BestDist = InteractionRadius;

    for (const TWeakObjectPtr<AWYAWorldItem>& Weak : ItemSys->GetWorldItems())
    {
        AWYAWorldItem* Candidate = Weak.Get();
        if (!IsValid(Candidate) || !Candidate->IsAvailable()) continue;

        const float Dist = Candidate->DistanceTo(MyPos);
        if (Dist < BestDist)
        {
            BestDist = Dist;
            Best     = Candidate;
        }
    }

    return Best;
}

void AWYAPlayerController::SetFocusedItem(AWYAWorldItem* Item)
{
    FocusedItem = Item;

    if (!HUDWidget) return;

    if (!Item)
    {
        HUDWidget->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    HUDWidget->SetVisibility(ESlateVisibility::Visible);

    const FWYAItemData& Data = Item->GetItemData();
    FString TypeLabel;
    switch (Data.Type)
    {
        case EWYAItemType::SupplyCache:   TypeLabel = TEXT("Supply Cache");   break;
        case EWYAItemType::DeadDrop:      TypeLabel = TEXT("Dead Drop");      break;
        case EWYAItemType::WardenMarker:  TypeLabel = TEXT("Warden Marker");  break;
        case EWYAItemType::FactionCache:  TypeLabel = TEXT("Faction Cache");  break;
    }

    HUDWidget->UpdateDisplay(
        FText::FromString(TypeLabel),
        FText::FromString(TEXT("[E] Claim")));
}

// ── Loot Actor Focus ──────────────────────────────────────────────────────────

AWYALootActor* AWYAPlayerController::FindClosestLootActor() const
{
    APawn* MyPawn = GetPawn();
    if (!MyPawn) return nullptr;
    const FVector MyPos = MyPawn->GetActorLocation();

    AWYALootActor* Best     = nullptr;
    float          BestDist = InteractionRadius;

    for (TActorIterator<AWYALootActor> It(GetWorld()); It; ++It)
    {
        AWYALootActor* Candidate = *It;
        if (!IsValid(Candidate)) continue;

        const float Dist = FVector::Dist(MyPos, Candidate->GetActorLocation());
        if (Dist < BestDist)
        {
            BestDist = Dist;
            Best     = Candidate;
        }
    }

    return Best;
}

void AWYAPlayerController::SetFocusedLootActor(AWYALootActor* Actor)
{
    FocusedLootActor = Actor;

    if (!HUDWidget) return;

    // GPS item takes HUD priority — don't override its display
    if (FocusedItem.IsValid()) return;

    if (!Actor)
    {
        HUDWidget->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    HUDWidget->SetVisibility(ESlateVisibility::Visible);
    HUDWidget->UpdateDisplay(
        AWYALootActor::GetTypeDisplayName(Actor->ItemType),
        Actor->GetPickupPrompt());
}
