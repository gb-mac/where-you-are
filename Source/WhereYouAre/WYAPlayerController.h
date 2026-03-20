#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WYAPlayerController.generated.h"

class UWYAInteractionWidget;
class AWYAWorldItem;

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

    AWYAWorldItem* FindClosestItem() const;
    void           SetFocusedItem(AWYAWorldItem* Item);

    UPROPERTY() TObjectPtr<UWYAInteractionWidget> HUDWidget;
    UPROPERTY() TWeakObjectPtr<AWYAWorldItem>     FocusedItem;
};
