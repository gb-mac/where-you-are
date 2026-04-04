#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WYAPlayerController.generated.h"

class UWYAInteractionWidget;
class AWYAWorldItem;
class AWYAVehicleBase;
class AWYAWorkbench;

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

    /** Radius (UU) in which items are interactable. 50m = 5000 UU. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|Items")
    float InteractionRadius = 5000.f;

private:
    void OnInteract();
    void OnPlaceItem();
    void OnVehicle();   // F key — enter nearby vehicle or exit current

    AWYAWorkbench*   FindClosestWorkbench() const;
    AWYAWorldItem*   FindClosestItem()      const;
    AWYAVehicleBase* FindClosestVehicle()   const;
    void             SetFocusedItem(AWYAWorldItem* Item);

    /** Radius (UU) in which vehicles are enterable. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|Vehicles")
    float VehicleEnterRadius = 300.f;

    UPROPERTY() TObjectPtr<UWYAInteractionWidget> HUDWidget;
    UPROPERTY() TWeakObjectPtr<AWYAWorldItem>     FocusedItem;
    UPROPERTY() TWeakObjectPtr<AWYAVehicleBase>   CurrentVehicle;
};
