#pragma once

#include "CoreMinimal.h"
#include "Characters/WYAOpponentCharacter.h"
#include "AWYASecurityCharacter.generated.h"

/**
 * Alert state for security NPCs.
 *
 *   Unaware    — on patrol, no knowledge of player
 *   Suspicious — heard something / saw movement, investigating
 *   Alerted    — confirmed player contact, full combat + propagation
 */
UENUM(BlueprintType)
enum class EWYAAlertState : uint8
{
    Unaware    UMETA(DisplayName = "Unaware"),
    Suspicious UMETA(DisplayName = "Suspicious"),
    Alerted    UMETA(DisplayName = "Alerted"),
};

/**
 * Security guard — patrols an area and escalates through alert states.
 *
 * Distinct from bodyguards (AWYAOpponentCharacter) in two ways:
 *   1. Patrols a radius around spawn point when Unaware
 *   2. Propagates alerts to nearby security on Alerted
 *
 * When any security reaches Alerted, WYAContractSubsystem is notified
 * so the Ghost bonus is voided for all active contract runs.
 *
 * Security does NOT auto-follow the named target — they hold position
 * or patrol independently. This lets the player pick off perimeter
 * guards before engaging the target.
 *
 * Blueprint subclass BP_Security_* sets mesh, patrol radius, faction tag.
 */
UCLASS()
class WHEREYOUARE_API AWYASecurityCharacter : public AWYAOpponentCharacter
{
    GENERATED_BODY()

public:
    AWYASecurityCharacter();

    // ── Alert state ───────────────────────────────────────────────────────────

    UPROPERTY(BlueprintReadOnly, Category = "WYA|Security")
    EWYAAlertState AlertState = EWYAAlertState::Unaware;

    /** Raise alert state — propagates to nearby security when reaching Alerted. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Security")
    void RaiseAlert(EWYAAlertState NewState);

    // ── Patrol config ─────────────────────────────────────────────────────────

    /** Radius around spawn point that this guard patrols when Unaware. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Security")
    float PatrolRadius = 1500.f; // 15m

    /** Seconds between patrol moves when Unaware. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Security")
    float PatrolInterval = 4.f;

    // ── Blueprint hooks ───────────────────────────────────────────────────────

    /** Override to play suspicion VO / animation (e.g. guard looks around). */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Security")
    void BP_OnSuspicious();

    /** Override to play alert VO / animation (e.g. "Contact!"). */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Security")
    void BP_OnAlerted();

protected:
    virtual void BeginPlay() override;

private:
    void PatrolTick();
    void OnSecurityDowned(AActor* Attacker);

    FVector SpawnLocation;
    FTimerHandle PatrolTimerHandle;
    FTimerHandle SuspicionTimerHandle;

    /** Radius within which this guard propagates alerts to other security. */
    static constexpr float AlertPropagationRadius = 3000.f; // 30m
};
