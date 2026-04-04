#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWYAWorkbench.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * Home-base workbench where the player installs fix-him repair components.
 *
 * The workbench checks inventory order: Mobility → Power → Comms.
 * Processing (Stage 3) is not installable here — it comes from Dr. Osei.
 *
 * Place one of these in the home-base level. Interact via E key —
 * AWYAPlayerController::OnInteract() finds the closest workbench first
 * and calls TryInstallComponent().
 */
UCLASS()
class WHEREYOUARE_API AWYAWorkbench : public AActor
{
    GENERATED_BODY()

public:
    AWYAWorkbench();

    /**
     * Check the player's inventory for an installable fix-him component and
     * install it if repair prerequisites are met.
     *
     * Install order:
     *   1. Mobility  — no prerequisite
     *   2. Power     — requires Mobility + Processing (Dr. Osei) done
     *   3. Comms     — requires Power done
     *
     * On success: removes item from inventory, fires the corresponding
     * UWYAFixHimQuestSubsystem callback, and saves the game.
     *
     * Returns false if nothing is installable.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|HomeBase")
    bool TryInstallComponent(APlayerController* PC);

    /**
     * Returns the interaction label shown in the HUD.
     * Examples: "Install mobility chassis", "Complete earlier repairs first",
     * "Nothing to install".
     */
    UFUNCTION(BlueprintPure, Category = "WYA|HomeBase")
    FText GetInteractionPrompt(APlayerController* PC) const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|HomeBase")
    TObjectPtr<USphereComponent> ProximityVolume;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|HomeBase")
    TObjectPtr<UStaticMeshComponent> Mesh;
};
