#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Contracts/WYAContractTypes.h"
#include "AWYAContractBoard.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * Physical contract board — player interacts to view and take AI-generated hits.
 *
 * Place one in the level (e.g. at the Meridian or a safe house).
 * AWYAPlayerController::OnInteract() dispatches to InteractWithBoard().
 * TakeContract() is called by the UI (WBP_ContractBoard) when the player
 * selects a contract from the board display.
 *
 * Blueprint subclass BP_ContractBoard implements:
 *   BP_OnBoardInteracted  — open the contract list widget
 *   BP_OnBoardRefreshed   — update the live board display
 *   BP_OnContractTaken    — feedback (sound, animation, close widget)
 */
UCLASS()
class WHEREYOUARE_API AWYAContractBoard : public AActor
{
    GENERATED_BODY()

public:
    AWYAContractBoard();

    // ── Player interaction ────────────────────────────────────────────────────

    /**
     * Called by AWYAPlayerController when the player presses E near this board.
     * Fires BP_OnBoardInteracted with the current board contract list.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Contracts")
    void InteractWithBoard(APlayerController* PC);

    /**
     * Called by WBP_ContractBoard when the player selects a contract.
     * Wraps WYAContractSubsystem::TakeContract() and saves the game.
     * Returns false if the take failed (player at max active, contract gone, etc.)
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Contracts")
    bool TakeContract(APlayerController* PC, const FString& ContractID);

    /** Returns "[E] View Contracts" or "[E] Board Empty" for the HUD prompt. */
    UFUNCTION(BlueprintPure, Category = "WYA|Contracts")
    FText GetInteractionPrompt() const;

    // ── Blueprint hooks ───────────────────────────────────────────────────────

    /**
     * Fired when the player presses E.
     * Use this to open WBP_ContractBoard and populate it with Contracts.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Contracts")
    void BP_OnBoardInteracted(APlayerController* PC, const TArray<FWYAContract>& Contracts);

    /**
     * Fired whenever the board gains or loses a contract (target taken or
     * replaced by the pre-gen buffer).
     * Use this to keep an open WBP_ContractBoard widget up to date.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Contracts")
    void BP_OnBoardRefreshed(const TArray<FWYAContract>& Contracts);

    /**
     * Fired after TakeContract() resolves.
     * bSuccess = false if the take failed (already at max, contract gone).
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Contracts")
    void BP_OnContractTaken(const FWYAContract& Contract, bool bSuccess);

    // ── Components ────────────────────────────────────────────────────────────

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Contracts")
    TObjectPtr<USphereComponent> ProximityVolume;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Contracts")
    TObjectPtr<UStaticMeshComponent> Mesh;

protected:
    virtual void BeginPlay() override;

private:
    /** Bound to WYAContractSubsystem::OnBoardRefreshed — forwards to BP event. */
    void OnBoardRefreshed(const TArray<FWYAContract>& Board);
};
