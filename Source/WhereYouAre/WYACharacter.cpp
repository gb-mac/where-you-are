#include "WYACharacter.h"
#include "Combat/WYACombatComponent.h"
#include "Inventory/WYAInventoryComponent.h"
#include "Inventory/WYAInventoryTypes.h"
#include "Engine/OverlapResult.h"
#include "Location/WYAGeoTypes.h"
#include "Location/WYALocationSubsystem.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

AWYACharacter::AWYACharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    Combat    = CreateDefaultSubobject<UWYACombatComponent>(TEXT("Combat"));
    Inventory = CreateDefaultSubobject<UWYAInventoryComponent>(TEXT("Inventory"));

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

void AWYACharacter::BeginPlay()
{
    Super::BeginPlay();

    if (Combat)
    {
        Combat->OnWoundStateChanged.AddDynamic(this, &AWYACharacter::OnWoundStateChanged);
        Combat->OnHealthChanged.AddDynamic(this, &AWYACharacter::OnHealthChanged);
        Combat->OnDowned.AddDynamic(this, &AWYACharacter::HandleDowned);
    }
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
    if (!Combat) { GetCharacterMovement()->MaxWalkSpeed = 1200.f; return; }

    switch (Combat->GetWoundState())
    {
    case EWYAWoundState::Healthy:
    case EWYAWoundState::FieldStable:
        GetCharacterMovement()->MaxWalkSpeed = 1200.f;
        break;
    case EWYAWoundState::NeedsTreatment:
        GetCharacterMovement()->MaxWalkSpeed = 550.f;
        break;
    case EWYAWoundState::Down:
        // No sprint while down
        break;
    }
}

void AWYACharacter::StopSprint()
{
    // Restore to the walk speed for current wound state
    if (!Combat) { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; return; }

    switch (Combat->GetWoundState())
    {
    case EWYAWoundState::Healthy:
        GetCharacterMovement()->MaxWalkSpeed = 600.f;
        break;
    case EWYAWoundState::FieldStable:
        GetCharacterMovement()->MaxWalkSpeed = 480.f;
        break;
    case EWYAWoundState::NeedsTreatment:
        GetCharacterMovement()->MaxWalkSpeed = 300.f;
        break;
    case EWYAWoundState::Down:
        GetCharacterMovement()->MaxWalkSpeed = 0.f;
        break;
    }
}

void AWYACharacter::OnWoundStateChanged(EWYAWoundState NewState)
{
    UCharacterMovementComponent* Movement = GetCharacterMovement();

    switch (NewState)
    {
    case EWYAWoundState::Healthy:
        Movement->MaxWalkSpeed = 600.f;
        break;
    case EWYAWoundState::FieldStable:
        Movement->MaxWalkSpeed = 480.f;
        break;
    case EWYAWoundState::NeedsTreatment:
        Movement->MaxWalkSpeed = 300.f;
        break;
    case EWYAWoundState::Down:
        {
            APlayerController* PC = Cast<APlayerController>(GetController());
            DisableInput(PC);
            Movement->DisableMovement();
        }
        break;
    }

    BP_OnWoundStateChanged(NewState);
}

void AWYACharacter::OnHealthChanged(float NewHealth, float MaxHealth)
{
    // Notify Blueprint for HUD updates etc.
    // Currently a stub — BP_OnPlayerHitReact is fired contextually from combat.
}

void AWYACharacter::HandleDowned(AActor* Attacker)
{
    BP_OnPlayerDowned(Attacker);

    APlayerController* PC = Cast<APlayerController>(GetController());
    DisableInput(PC);

    GetWorld()->GetTimerManager().SetTimer(
        RecoveryHandle,
        this,
        &AWYACharacter::RecoverFromDown,
        30.f,
        false);
}

void AWYACharacter::RecoverFromDown()
{
    if (!Combat) return;

    Combat->Revive(25.f);

    APlayerController* PC = Cast<APlayerController>(GetController());
    EnableInput(PC);

    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AWYACharacter::OnAttack()
{
    if (!Combat || !Combat->IsAlive()) return;

    if (!Inventory)
    {
        DoMeleeSwing(false);
        return;
    }

    if (Inventory->HasItem(EWYACarriedItemType::RangedWeapon)
        && Inventory->HasItem(EWYACarriedItemType::Ammo))
    {
        DoRangedFire();
        return;
    }

    const bool bArmed = Inventory->HasItem(EWYACarriedItemType::MeleeWeapon);
    DoMeleeSwing(bArmed);
}

void AWYACharacter::DoMeleeSwing(bool bArmed)
{
    UWorld* World = GetWorld();
    if (!World) return;

    const float Radius = bArmed ? 90.f  : 60.f;
    const float Reach  = bArmed ? 150.f : 120.f;
    const float Damage = bArmed ? 40.f  : 25.f;

    const FVector Center = GetActorLocation()
        + GetActorForwardVector() * Reach
        + FVector(0.f, 0.f, 50.f);

    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (!World->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity,
        ECC_Pawn, FCollisionShape::MakeSphere(Radius), Params))
    {
        BP_OnMeleeSwing(bArmed);
        return;
    }

    for (const FOverlapResult& Overlap : Overlaps)
    {
        AActor* Target = Overlap.GetActor();
        if (!Target || Target == this) continue;

        if (UWYACombatComponent* TargetCombat = Target->FindComponentByClass<UWYACombatComponent>())
        {
            TargetCombat->ApplyDamage(Damage, this);
            break; // one target per swing
        }
    }

    BP_OnMeleeSwing(bArmed);
}

void AWYACharacter::DoRangedFire()
{
    UWorld* World = GetWorld();
    if (!World || !FollowCamera) return;

    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End   = Start + FollowCamera->GetForwardVector() * 5000.f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = false;

    if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
    {
        if (AActor* HitActor = Hit.GetActor())
        {
            if (UWYACombatComponent* TargetCombat = HitActor->FindComponentByClass<UWYACombatComponent>())
            {
                TargetCombat->ApplyDamage(35.f, this);
                bHit = true;
            }
        }
    }

    // Consume one round
    if (Inventory)
    {
        Inventory->RemoveItem(EWYACarriedItemType::Ammo, 1);

        if (Inventory->GetQuantity(EWYACarriedItemType::Ammo) == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("AWYACharacter: Out of ammo."));
        }
    }

    BP_OnRangedFire(bHit);
}
