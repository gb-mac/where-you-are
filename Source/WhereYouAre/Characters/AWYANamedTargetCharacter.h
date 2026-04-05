#pragma once

#include "CoreMinimal.h"
#include "Characters/WYAOpponentCharacter.h"
#include "Contracts/WYAContractTypes.h"
#include "AWYANamedTargetCharacter.generated.h"

/**
 * A contract target — a named opponent generated from a hit contract.
 *
 * Significantly harder than a regular opponent:
 *   Standard  — 300 HP, 1 bodyguard,  always drops decent loot
 *   Priority  — 500 HP, 2 bodyguards, better loot
 *   HighTable — 800 HP, 3 bodyguards, best loot
 *
 * On death: fires WYAContractSubsystem::CompleteContract() for any
 * player controller that holds the matching ContractID.
 *
 * Retreat: at 30% health the target breaks from engagement and
 * moves away from the nearest player at sprint speed. Guards continue
 * to fight. After 5 seconds the target re-engages — this gives the
 * player a moment to finish off guards before the target comes back.
 */
UCLASS()
class WHEREYOUARE_API AWYANamedTargetCharacter : public AWYAOpponentCharacter
{
    GENERATED_BODY()

public:
    AWYANamedTargetCharacter();

    // ── Contract identity ─────────────────────────────────────────────────────

    /** Set by WYAContractSubsystem when spawning this target. */
    UPROPERTY(BlueprintReadWrite, Category = "WYA|Contract")
    FString ContractID;

    UPROPERTY(BlueprintReadWrite, Category = "WYA|Contract")
    FString TargetDisplayName;

    UPROPERTY(BlueprintReadWrite, Category = "WYA|Contract")
    EWYAContractTier Tier = EWYAContractTier::Standard;

    // ── State ─────────────────────────────────────────────────────────────────

    /** True while the target is breaking from engagement. */
    UPROPERTY(BlueprintReadOnly, Category = "WYA|Combat")
    bool bRetreating = false;

    // ── Blueprint hooks ───────────────────────────────────────────────────────

    /** Override to show target name plate / distinctive visual. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Contract")
    void BP_OnTargetSpawned(const FString& Name, EWYAContractTier InTier);

    /** Override to play retreat VO / VFX. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
    void BP_OnRetreat();

    /** Override to play re-engage VO / VFX. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
    void BP_OnReengage();

protected:
    virtual void BeginPlay() override;

private:
    // ── Internal ──────────────────────────────────────────────────────────────

    /** Spawn bodyguards in a close ring around self. Count determined by Tier. */
    void SpawnGuards();

    /** Spawn security patrols in a wider perimeter ring. Count determined by Tier. */
    void SpawnSecurity();

    /** Triggered by OnHealthChanged when health falls below RetreatThreshold. */
    UFUNCTION()
    void OnHealthUpdated(float NewHealth, float InMaxHealth);

    /** Fires ContractSubsystem completion on Down. */
    UFUNCTION()
    void HandleContractTargetDowned(AActor* Attacker);

    void BeginRetreat();
    void EndRetreat();

    /**
     * Starts polling for nearby living security after the target goes down.
     * First guard to walk within BodyDiscoveryRadius fires NotifyBodyDiscovered.
     */
    void StartBodyDiscoveryWatch();

    /** Timer callback — checks for guards near the body location. */
    void CheckBodyDiscovery();

    // ── Tier config ───────────────────────────────────────────────────────────

    static constexpr float RetreatHealthFraction = 0.30f;

    /** Radius (UU) within which a living security NPC discovers the body. ~8m. */
    static constexpr float BodyDiscoveryRadius = 800.f;

    /** True once retreat has been triggered this life (one retreat per fight). */
    bool bHasRetreated = false;

    FTimerHandle RetreatTimerHandle;
    FTimerHandle BodyDiscoveryTimerHandle;
};
