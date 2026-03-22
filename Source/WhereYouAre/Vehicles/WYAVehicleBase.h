#pragma once

#include "CoreMinimal.h"
#include "ChaosVehicleMovementComponent.h"
#include "WheeledVehiclePawn.h"
#include "WYAVehicleBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class AWYACharacter;

UENUM(BlueprintType)
enum class EWYAVehicleType : uint8
{
    Sedan    UMETA(DisplayName = "Sedan"),
    SUV      UMETA(DisplayName = "SUV"),
    Pickup   UMETA(DisplayName = "Pickup"),
    Sports   UMETA(DisplayName = "Sports"),
};

/**
 * GTA-style driveable vehicle.
 *
 * C++ base — subclass in Blueprint to assign SkeletalMesh, PhysicsAsset, WheelSetup.
 * Enter: player presses F near vehicle → PC possesses this pawn, character hidden.
 * Exit:  player presses F while driving → character reappears beside door, PC repossesses character.
 *
 * Requires ChaosVehicles plugin (already ships with UE5).
 */
UCLASS(Abstract)
class WHEREYOUARE_API AWYAVehicleBase : public AWheeledVehiclePawn
{
    GENERATED_BODY()

public:
    AWYAVehicleBase();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** Called by WYAPlayerController when F is pressed near this vehicle. */
    void Enter(APlayerController* PC, AWYACharacter* Character);

    /** Called by WYAPlayerController when F is pressed while driving. */
    void Exit();

    bool IsOccupied() const { return bOccupied; }

    UPROPERTY(EditDefaultsOnly, Category = "Vehicle")
    EWYAVehicleType VehicleType = EWYAVehicleType::Sedan;

    /** Radius in which the player can enter (UU — 300 = 3 metres). */
    UPROPERTY(EditDefaultsOnly, Category = "Vehicle")
    float EnterRadius = 300.f;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> CameraArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> VehicleCamera;

    /** Offset from driver door where character reappears on exit. */
    UPROPERTY(EditDefaultsOnly, Category = "Vehicle")
    FVector ExitOffset = FVector(-50.f, -200.f, 0.f);

private:
    UPROPERTY() TObjectPtr<APlayerController> OccupantPC;
    UPROPERTY() TObjectPtr<AWYACharacter>     OccupantCharacter;
    bool bOccupied = false;

    void SetupVehicleInput();
    void MoveThrottle(float Value);
    void MoveSteering(float Value);
    void ApplyBrake();
    void ReleaseBrake();
};
