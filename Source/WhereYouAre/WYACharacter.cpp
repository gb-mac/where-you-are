#include "WYACharacter.h"
#include "Combat/WYACombatComponent.h"
#include "Engine/OverlapResult.h"
#include "Location/WYAGeoTypes.h"
#include "Location/WYALocationSubsystem.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"

AWYACharacter::AWYACharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    Combat = CreateDefaultSubobject<UWYACombatComponent>(TEXT("Combat"));

    // Mannequin mesh — Third Person content pack
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannequinMesh(
        TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
    if (MannequinMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(MannequinMesh.Object);
        GetMesh()->SetRelativeLocationAndRotation(
            FVector(0.f, 0.f, -90.f),
            FRotator(0.f, -90.f, 0.f));
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(
        TEXT("/Game/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));
    if (AnimBP.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBP.Class);
    }

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;

    // Spring arm — camera follows at distance, rotates with controller
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength          = 400.0f;
    CameraBoom->bUsePawnControlRotation  = true;
    CameraBoom->SocketOffset             = FVector(0.0f, 50.0f, 70.0f); // over-the-shoulder

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Character rotates to face movement direction
    GetCharacterMovement()->bOrientRotationToMovement   = true;
    GetCharacterMovement()->RotationRate                = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed                = WalkSpeed;
    GetCharacterMovement()->MinAnalogWalkSpeed          = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking  = 2000.0f;
    GetCharacterMovement()->JumpZVelocity               = 700.0f;
    GetCharacterMovement()->AirControl                  = 0.35f;
    GetCharacterMovement()->NavAgentProps.bCanCrouch    = true;
}

void AWYACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AWYACharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight",   this, &AWYACharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp",      this, &AWYACharacter::LookUp);
    PlayerInputComponent->BindAxis("LookRight",   this, &AWYACharacter::LookRight);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed,  this, &AWYACharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AWYACharacter::StopSprint);

    PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AWYACharacter::OnAttack);
}

void AWYACharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

FWYAGeoCoord AWYACharacter::GetCurrentGeoCoord() const
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UWYALocationSubsystem* Sub = GI->GetSubsystem<UWYALocationSubsystem>())
        {
            return Sub->WorldToGeo(GetActorLocation());
        }
    }
    return FWYAGeoCoord{};
}

void AWYACharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Yaw(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Value);
    }
}

void AWYACharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Yaw(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Value);
    }
}

void AWYACharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AWYACharacter::LookRight(float Value)
{
    AddControllerYawInput(Value);
}

void AWYACharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AWYACharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AWYACharacter::OnAttack()
{
    if (!Combat || !Combat->IsAlive()) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // Sphere sweep: 60cm radius, 120cm forward at torso height.
    // No animation yet — Blueprint subclass can play punch montage before/after.
    const FVector Center = GetActorLocation()
        + GetActorForwardVector() * 120.f
        + FVector(0.f, 0.f, 50.f);

    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (!World->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity,
        ECC_Pawn, FCollisionShape::MakeSphere(60.f), Params))
    {
        return;
    }

    for (const FOverlapResult& Overlap : Overlaps)
    {
        if (UWYACombatComponent* TargetCombat = Overlap.GetActor()->FindComponentByClass<UWYACombatComponent>())
        {
            // Don't hit self (already ignored in query params, but guard the component case)
            if (Overlap.GetActor() == this) continue;

            TargetCombat->ApplyDamage(25.f, this);
            break; // one target per punch
        }
    }
}
