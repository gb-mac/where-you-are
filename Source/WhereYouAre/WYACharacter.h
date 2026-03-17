#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WYACharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWYALocationSubsystem;

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

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

    /** Get the character's current real-world GPS coordinate. */
    UFUNCTION(BlueprintCallable, Category = "Location")
    FWYAGeoCoord GetCurrentGeoCoord() const;

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

    static constexpr float WalkSpeed   = 600.0f;
    static constexpr float SprintSpeed = 1200.0f;
};
