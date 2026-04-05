#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Quest/WYAQuestTypes.h"
#include "WYAPlayerController.generated.h"

class UWYAInteractionWidget;
class UWYAHUDWidget;
class AWYAWorldItem;
class AWYAVehicleBase;
class AWYAWorkbench;
class AWYADrOsei;
class AWYALootActor;
class AWYAContractBoard;

UCLASS()
class WHEREYOUARE_API AWYAPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AWYAPlayerController();

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    /** Blueprint must set this to WBP_Interaction. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|UI")
    TSubclassOf<UWYAInteractionWidget> InteractionWidgetClass;

    /** Blueprint must set this to WBP_WYAHud. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|UI")
    TSubclassOf<UWYAHUDWidget> HUDWidgetClass;

    /** Radius (UU) in which items are interactable. 50m = 5000 UU. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|Items")
    float InteractionRadius = 5000.f;

private:
    void OnInteract();
    void OnPlaceItem();
    void OnVehicle();   // F key — enter nearby vehicle or exit current

    AWYAWorkbench*    FindClosestWorkbench()    const;
    AWYADrOsei*       FindClosestDrOsei()       const;
    AWYAContractBoard* FindClosestContractBoard() const;
    AWYAWorldItem*    FindClosestItem()          const;
    AWYAVehicleBase*  FindClosestVehicle()       const;
    AWYALootActor*    FindClosestLootActor()     const;
    void              SetFocusedItem(AWYAWorldItem* Item);
    void              SetFocusedLootActor(AWYALootActor* Actor);

    /** Push current character/subsystem state to the persistent HUD. */
    void TickHUD(float DeltaSeconds);

    void OnQuestAssigned(APlayerController* PC, const FWYAQuest& Quest);
    void OnQuestCompleted(APlayerController* PC);

    /** Radius (UU) in which vehicles are enterable. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|Vehicles")
    float VehicleEnterRadius = 300.f;

    /** Seconds between HUD data polls. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|UI")
    float HUDUpdateInterval = 0.1f;

    float HUDUpdateAccumulator = 0.f;

    UPROPERTY() TObjectPtr<UWYAInteractionWidget> InteractionWidget;
    UPROPERTY() TObjectPtr<UWYAHUDWidget>         MainHUDWidget;
    UPROPERTY() TWeakObjectPtr<AWYAWorldItem>     FocusedItem;
    UPROPERTY() TWeakObjectPtr<AWYAVehicleBase>   CurrentVehicle;
    UPROPERTY() TWeakObjectPtr<AWYALootActor>     FocusedLootActor;
};
