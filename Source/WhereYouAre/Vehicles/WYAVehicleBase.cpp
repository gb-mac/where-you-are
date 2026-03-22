#include "Vehicles/WYAVehicleBase.h"
#include "WYACharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"

AWYAVehicleBase::AWYAVehicleBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // Spring arm for third-person vehicle camera
    CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
    CameraArm->SetupAttachment(GetMesh());
    CameraArm->TargetArmLength         = 800.f;
    CameraArm->SocketOffset             = FVector(0.f, 0.f, 200.f);
    CameraArm->bUsePawnControlRotation  = true;
    CameraArm->bInheritPitch            = false; // vehicle camera stays level-ish
    CameraArm->bDoCollisionTest         = true;

    VehicleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCamera"));
    VehicleCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
    VehicleCamera->bUsePawnControlRotation = false;
}

void AWYAVehicleBase::BeginPlay()
{
    Super::BeginPlay();
}

void AWYAVehicleBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// ── Enter / Exit ──────────────────────────────────────────────────────────────

void AWYAVehicleBase::Enter(APlayerController* PC, AWYACharacter* Character)
{
    if (bOccupied || !PC || !Character) return;

    OccupantPC        = PC;
    OccupantCharacter = Character;
    bOccupied         = true;

    // Hide character — they're "sitting inside"
    Character->SetActorHiddenInGame(true);
    Character->SetActorEnableCollision(false);

    // Hand control to this vehicle
    PC->Possess(this);

    UE_LOG(LogTemp, Log, TEXT("WYAVehicle: %s entered %s"),
        *Character->GetName(), *GetName());
}

void AWYAVehicleBase::Exit()
{
    if (!bOccupied || !OccupantPC || !OccupantCharacter) return;

    // Place character beside the driver door
    const FVector ExitPos = GetActorLocation()
        + GetActorRotation().RotateVector(ExitOffset)
        + FVector(0.f, 0.f, 100.f);

    OccupantCharacter->SetActorLocation(ExitPos, false, nullptr, ETeleportType::TeleportPhysics);
    OccupantCharacter->SetActorHiddenInGame(false);
    OccupantCharacter->SetActorEnableCollision(true);

    // Give control back to character
    OccupantPC->Possess(OccupantCharacter);

    UE_LOG(LogTemp, Log, TEXT("WYAVehicle: exited %s"), *GetName());

    OccupantPC        = nullptr;
    OccupantCharacter = nullptr;
    bOccupied         = false;
}

// ── Input ─────────────────────────────────────────────────────────────────────

void AWYAVehicleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AWYAVehicleBase::MoveThrottle);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"),   this, &AWYAVehicleBase::MoveSteering);
    PlayerInputComponent->BindAxis(TEXT("LookUp"),      this, &AWYAVehicleBase::AddControllerPitchInput);
    PlayerInputComponent->BindAxis(TEXT("LookRight"),   this, &AWYAVehicleBase::AddControllerYawInput);

    PlayerInputComponent->BindAction(TEXT("Jump"),    IE_Pressed,  this, &AWYAVehicleBase::ApplyBrake);
    PlayerInputComponent->BindAction(TEXT("Jump"),    IE_Released, this, &AWYAVehicleBase::ReleaseBrake);
}

void AWYAVehicleBase::MoveThrottle(float Value)
{
    if (auto* VehicleMove = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        VehicleMove->SetThrottleInput(FMath::Max(0.f, Value));
        VehicleMove->SetBrakeInput(FMath::Max(0.f, -Value));  // reverse = brake + throttle backwards
        VehicleMove->SetReverseInput(Value < -0.1f);
    }
}

void AWYAVehicleBase::MoveSteering(float Value)
{
    if (auto* VehicleMove = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        VehicleMove->SetSteeringInput(Value);
    }
}

void AWYAVehicleBase::ApplyBrake()
{
    if (auto* VehicleMove = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        VehicleMove->SetHandbrakeInput(true);
    }
}

void AWYAVehicleBase::ReleaseBrake()
{
    if (auto* VehicleMove = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        VehicleMove->SetHandbrakeInput(false);
    }
}
