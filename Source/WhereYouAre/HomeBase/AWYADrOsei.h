#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWYADrOsei.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * Dr. Osei — the NPC who fixes the assistant's processing core (Stage 2 → 3).
 *
 * Unlike the workbench (which takes physical components), this is a person.
 * The player finds him, he runs diagnostics, the processing core comes back online.
 * No inventory item consumed — the interaction IS the repair.
 *
 * Prerequisite: Mobility must already be repaired (Stage 2 reached).
 * One-time: after the interaction completes, the prompt switches to "Repair complete"
 * and further interactions are no-ops.
 *
 * Place one of these in the level at the clinic / facility location.
 * AWYAPlayerController picks him up in the interaction priority chain:
 *   Workbench → DrOsei → LootActor → GPS item
 */
UCLASS()
class WHEREYOUARE_API AWYADrOsei : public AActor
{
    GENERATED_BODY()

public:
    AWYADrOsei();

    /**
     * Attempt the Stage 3 repair interaction.
     *
     * Prereqs: Mobility repaired, Processing not yet done.
     * On success: calls OnProcessingRepairCompleted(), saves, returns true.
     * Returns false (no-op) if prereqs not met or already done.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|FixHim")
    bool TryInteract(APlayerController* PC);

    /**
     * Context-sensitive interaction label for the HUD.
     * "Speak with Dr. Osei"      — ready for repair
     * "Complete earlier repairs first" — Mobility not done yet
     * "Processing core restored" — already done
     */
    UFUNCTION(BlueprintPure, Category = "WYA|FixHim")
    FText GetInteractionPrompt(APlayerController* PC) const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|FixHim")
    TObjectPtr<USphereComponent> ProximityVolume;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|FixHim")
    TObjectPtr<UStaticMeshComponent> Mesh;
    // Narrative flags (bOseiOfferedDirectContact, bOseiRequestedStage4Presence) live in
    // UWYAFixHimQuestSubsystem so they persist through the save system correctly.
};
