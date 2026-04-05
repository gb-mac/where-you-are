#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Contracts/WYAContractTypes.h"
#include "WYAContractSubsystem.generated.h"

class UWYAAISubsystem;
class UWYACurrencySubsystem;
class AWYANamedTargetCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnContractBoardRefreshed, const TArray<FWYAContract>& /*Board*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnContractTaken,     APlayerController*, const FWYAContract&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnContractCompleted, APlayerController*, const FWYAContract&);

/**
 * Contract board — constantly refilled from the AI pre-gen pipeline.
 *
 * Six slots on the board. Players can take up to MaxActiveContracts (3) at once.
 * Taking a contract removes it from the board and a fresh one is queued to replace it.
 * Completing a contract awards Gold via UWYACurrencySubsystem.
 *
 * Completion is currently player-triggered (CompleteContract call) — kill detection
 * against named targets will be wired in a later pass.
 *
 * Access: GetGameInstance()->GetSubsystem<UWYAContractSubsystem>()
 */
UCLASS()
class WHEREYOUARE_API UWYAContractSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // ── Board ─────────────────────────────────────────────────────────────────

    /** All contracts currently available to take. Up to BoardSize entries. */
    UFUNCTION(BlueprintPure, Category = "WYA|Contracts")
    const TArray<FWYAContract>& GetBoardContracts() const { return Board; }

    /** True if the board has at least one contract ready. */
    UFUNCTION(BlueprintPure, Category = "WYA|Contracts")
    bool HasBoardContracts() const { return Board.Num() > 0; }

    // ── Take / complete / abandon ─────────────────────────────────────────────

    /**
     * Take a contract from the board — moves it to the player's active list
     * and spawns the named target in the world 50–150m from the player.
     * Returns false if the player already has MaxActiveContracts active,
     * or if ContractID is not on the board.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Contracts")
    bool TakeContract(APlayerController* PC, const FString& ContractID);

    /**
     * Mark an active contract as complete — awards Gold and removes from active list.
     * Returns false if ContractID is not in the player's active list.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Contracts")
    bool CompleteContract(APlayerController* PC, const FString& ContractID);

    /**
     * Abandon an active contract without reward.
     * Contract is discarded — does not return to the board.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Contracts")
    bool AbandonContract(APlayerController* PC, const FString& ContractID);

    /**
     * Called by AWYASecurityCharacter when it reaches Alerted state.
     * Increments AlertCount on all active run states — voids Ghost bonus.
     */
    void NotifySecurityAlerted();

    /**
     * Called by AWYAExfilPoint when a player enters the exfil trigger.
     * Marks the run as extracted, calculates bonuses, awards Gold.
     */
    void ReachExfil(APlayerController* PC, const FString& ContractID);

    /**
     * Called by WYACombatComponent (via player character) when the player takes damage
     * during an active contract run. Voids the Clean bonus.
     */
    void NotifyPlayerTookDamage(APlayerController* PC);

    /** All active contracts for this player. Empty array if none. */
    UFUNCTION(BlueprintPure, Category = "WYA|Contracts")
    TArray<FWYAContract> GetActiveContracts(APlayerController* PC) const;

    /** True if the player has at least one active contract. */
    UFUNCTION(BlueprintPure, Category = "WYA|Contracts")
    bool HasActiveContract(APlayerController* PC) const;

    // ── Events ────────────────────────────────────────────────────────────────

    /** Fired whenever the board gains or loses a contract. */
    FOnContractBoardRefreshed OnBoardRefreshed;

    FOnContractTaken     OnContractTaken;
    FOnContractCompleted OnContractCompleted;

    // ── Config ────────────────────────────────────────────────────────────────

    /** Maximum contracts visible on the board at any one time. */
    static constexpr int32 BoardSize = 6;

    /** Maximum contracts a player can hold simultaneously. */
    static constexpr int32 MaxActiveContracts = 3;

private:
    /** Try to pop from AI pre-gen buffer and fill any open board slots. */
    void TryFillBoard();

    /**
     * Calculate and award bonus Gold based on run state.
     * Ghost (no alerts): +50%. Swift (under time limit): +25%. Clean (no damage): +25%.
     * Returns total Gold awarded.
     */
    int32 CalculateAndAwardBonus(APlayerController* PC, const FWYAContract& Contract,
                                  const FWYAContractRunState& RunState);

    TArray<FWYAContract> Board;

    /** Active contracts per player controller. */
    TMap<APlayerController*, TArray<FWYAContract>> ActiveContracts;

    /** Live run state per contract ID. */
    TMap<FString, FWYAContractRunState> RunStates;

    UPROPERTY()
    TObjectPtr<UWYAAISubsystem> AISub;

    UPROPERTY()
    TObjectPtr<UWYACurrencySubsystem> CurrencySub;

    /** Polls TryFillBoard() every few seconds while the board has open slots. */
    FTimerHandle BoardFillTimerHandle;

    // ── Swift time limits by tier (seconds) ───────────────────────────────────
    static constexpr float SwiftLimit_Standard  = 180.f; // 3 min
    static constexpr float SwiftLimit_Priority  = 300.f; // 5 min
    static constexpr float SwiftLimit_HighTable = 480.f; // 8 min
};
