#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WYACombatComponent.generated.h"

/**
 * Wound state — not binary. Tracks the player's physical condition.
 * Field-stable means patched enough to keep moving, not properly treated.
 * Down means incapacitated — the player doesn't die, they go down.
 */
UENUM(BlueprintType)
enum class EWYAWoundState : uint8
{
    Healthy        UMETA(DisplayName = "Healthy"),
    FieldStable    UMETA(DisplayName = "Field Stable"),       // patched, performance affected
    NeedsTreatment UMETA(DisplayName = "Needs Treatment"),    // requires a doctor
    Down           UMETA(DisplayName = "Down"),               // incapacitated
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged,    float,          NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnWoundStateChanged, EWYAWoundState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnDowned,            AActor*,        Attacker);

/**
 * Health and wound state for any character — player or opponent.
 * Attach to AWYACharacter and AWYAOpponentCharacter.
 *
 * The player never truly dies — they go Down. The assistant and Moe handle recovery.
 * Opponents also go Down (not deleted mid-fight) but are cleaned up after a delay.
 */
UCLASS(ClassGroup=(WYA), meta=(BlueprintSpawnableComponent))
class WHEREYOUARE_API UWYACombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWYACombatComponent();

    /** Apply damage. Returns actual damage dealt. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Combat")
    float ApplyDamage(float Amount, AActor* Attacker);

    /**
     * Restore health. Does nothing if Down. Returns actual amount healed.
     * Updates wound state based on new health level.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Combat")
    float Heal(float Amount);

    /**
     * Revive from Down state — sets to NeedsTreatment, restores StartingHealth.
     * Broadcasts delegates. Called by the recovery system after being down.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Combat")
    void Revive(float StartingHealth = 25.f);

    UFUNCTION(BlueprintPure, Category = "WYA|Combat")
    bool IsAlive() const { return WoundState != EWYAWoundState::Down; }

    UFUNCTION(BlueprintPure, Category = "WYA|Combat")
    bool IsDown() const { return WoundState == EWYAWoundState::Down; }

    UFUNCTION(BlueprintPure, Category = "WYA|Combat")
    float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure, Category = "WYA|Combat")
    float GetHealthPercent() const { return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f; }

    UFUNCTION(BlueprintPure, Category = "WYA|Combat")
    EWYAWoundState GetWoundState() const { return WoundState; }

    UPROPERTY(BlueprintAssignable, Category = "WYA|Combat")
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "WYA|Combat")
    FOnWoundStateChanged OnWoundStateChanged;

    /** Broadcast when health reaches 0. Character should ragdoll/freeze, not delete. */
    UPROPERTY(BlueprintAssignable, Category = "WYA|Combat")
    FOnDowned OnDowned;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WYA|Combat")
    float MaxHealth = 100.f;

protected:
    virtual void BeginPlay() override;

private:
    float          CurrentHealth = 100.f;
    EWYAWoundState WoundState    = EWYAWoundState::Healthy;

    void SetWoundState(EWYAWoundState New);
};
