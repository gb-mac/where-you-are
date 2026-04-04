#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WYACurrencySubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChanged,   int32, NewBalance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSilverChanged, int32, NewBalance);

/**
 * Authoritative in-process ledger for Gold and Silver.
 *
 * All mutations go through this subsystem so the HUD and other systems
 * can bind to balance change delegates in one place.
 *
 * Gold: earned via contracts/milestones, no cap, not purchasable.
 * Silver: earned via barter drip + real-money purchase (IAP handled elsewhere).
 *
 * Gold ↔ Silver exchange is blocked at the subsystem level — there is no
 * conversion path. Any code attempting cross-exchange should be rejected here.
 *
 * Note: This is the local ledger. Backend validation of transactions against
 * server state is out of scope until the vertical slice economy ships.
 *
 * Access: GetGameInstance()->GetSubsystem<UWYACurrencySubsystem>()
 */
UCLASS()
class WHEREYOUARE_API UWYACurrencySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // ── Gold ─────────────────────────────────────────────────────────────────

    /**
     * Award Gold (contract payout, faction rank-up, milestone).
     * Amount must be > 0. Returns new balance.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Economy")
    int32 AwardGold(int32 Amount);

    /**
     * Spend Gold. Returns true and deducts if the player has enough.
     * Returns false without mutating balance if insufficient.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Economy")
    bool SpendGold(int32 Amount);

    UFUNCTION(BlueprintPure, Category = "WYA|Economy")
    int32 GetGold() const { return Gold; }

    UFUNCTION(BlueprintPure, Category = "WYA|Economy")
    bool HasGold(int32 Amount) const { return Gold >= Amount; }

    // ── Silver ────────────────────────────────────────────────────────────────

    /**
     * Award Silver (barter drip, volume milestone, reputation bonus).
     * Also called by IAP flow after purchase validation.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Economy")
    int32 AwardSilver(int32 Amount);

    /**
     * Spend Silver (safe zone entry, contract posting, cosmetics).
     * Returns false without mutating balance if insufficient.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Economy")
    bool SpendSilver(int32 Amount);

    UFUNCTION(BlueprintPure, Category = "WYA|Economy")
    int32 GetSilver() const { return Silver; }

    UFUNCTION(BlueprintPure, Category = "WYA|Economy")
    bool HasSilver(int32 Amount) const { return Silver >= Amount; }

    // ── Delegates ─────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "WYA|Economy")
    FOnGoldChanged OnGoldChanged;

    UPROPERTY(BlueprintAssignable, Category = "WYA|Economy")
    FOnSilverChanged OnSilverChanged;

    // ── Save/load ─────────────────────────────────────────────────────────────

    void GetSaveData(int32& OutGold, int32& OutSilver) const;
    void ApplySaveData(int32 SavedGold, int32 SavedSilver);

private:
    int32 Gold   = 0;
    int32 Silver = 0;
};
