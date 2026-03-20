#include "WYAPlayerController.h"
#include "UI/WYAInteractionWidget.h"
#include "Items/AWYAWorldItem.h"
#include "Items/WYAItemSubsystem.h"
#include "Api/WYAApiClient.h"
#include "Location/WYALocationSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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
}

void AWYAPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AWYAWorldItem* Closest = FindClosestItem();
    if (Closest != FocusedItem.Get())
    {
        SetFocusedItem(Closest);
    }
}

// ── Interaction ───────────────────────────────────────────────────────────────

void AWYAPlayerController::OnInteract()
{
    AWYAWorldItem* Item = FocusedItem.Get();
    if (!Item || !Item->IsAvailable()) return;

    UWYAApiClient* Api = GetGameInstance()->GetSubsystem<UWYAApiClient>();
    UWYALocationSubsystem* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
    if (!Api || !LocSys || !LocSys->IsResolved()) return;

    const FWYAGeoCoord& Origin = LocSys->GetOrigin();
    // Player is always at world origin → GPS origin
    const FString ItemId = Item->GetItemData().Id;

    Api->ClaimItem(ItemId, Origin.Latitude, Origin.Longitude,
        [this](bool bOk, FWYAItemData UpdatedItem)
    {
        if (!HUDWidget) return;
        const FText Msg = bOk
            ? FText::FromString(TEXT("Claimed!"))
            : FText::FromString(TEXT("Claim failed — too far or already taken"));
        HUDWidget->ShowClaimResult(bOk, Msg);
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
