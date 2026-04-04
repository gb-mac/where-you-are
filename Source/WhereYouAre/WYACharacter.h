#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/WYACombatComponent.h"
#include "Survival/WYASurvivalComponent.h"
#include "WYACharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWYALocationSubsystem;
class UWYAInventoryComponent;

/**
 * Third-person character for Where You Are.
 * Camera uses a spring arm for over-the-shoulder perspective.
 * Real-world GPS position is derived from UE5 world location via the location subsystem.
 */
UCLASS()
class WHEREYOUARE_API AWYACharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AWYACharacter();

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

    /** Get the character's current real-world GPS coordinate. */
    UFUNCTION(BlueprintCallable, Category = "Location")
    FWYAGeoCoord GetCurrentGeoCoord() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Combat")
    TObjectPtr<UWYACombatComponent> Combat;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Inventory")
    TObjectPtr<UWYAInventoryComponent> Inventory;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Survival")
    TObjectPtr<UWYASurvivalComponent> Survival;

    /** Called when the player goes Down — implement ragdoll / camera in Blueprint. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
    void BP_OnPlayerDowned(AActor* Attacker);

    /** Called on every hit — implement screen shake / vignette in Blueprint. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
    void BP_OnPlayerHitReact(AActor* Attacker, float Damage);

    /** Called when wound state changes — drive animation blend in Blueprint. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
    void BP_OnWoundStateChanged(EWYAWoundState NewState);

    /** Called on melee swing — play punch/weapon animation in Blueprint. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
    void BP_OnMeleeSwing(bool bArmed);

    /** Called on ranged fire — play fire animation / muzzle flash in Blueprint. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
    void BP_OnRangedFire(bool bHit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void LookRight(float Value);
    void StartSprint();
    void StopSprint();
    void OnAttack();

    /** Weapon-aware attack dispatch. */
    void DoMeleeSwing(bool bArmed);
    void DoRangedFire();

    /** Wound-state callbacks bound in BeginPlay. */
    UFUNCTION()
    void OnWoundStateChanged(EWYAWoundState NewState);

    UFUNCTION()
    void OnHealthChanged(float NewHealth, float MaxHealth);

    /** Bound to Combat->OnDowned via AddDynamic. */
    UFUNCTION()
    void HandleDowned(AActor* Attacker);

    /** Called 30 seconds after going Down — revives with minimal health. */
    void RecoverFromDown();

    FTimerHandle RecoveryHandle;

    static constexpr float WalkSpeed = 600.0f;
};
