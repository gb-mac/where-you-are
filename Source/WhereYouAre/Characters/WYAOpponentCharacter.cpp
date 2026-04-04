#include "Characters/WYAOpponentCharacter.h"
#include "Combat/WYACombatComponent.h"
#include "Loot/AWYALootActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "UObject/ConstructorHelpers.h"

AWYAOpponentCharacter::AWYAOpponentCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Combat = CreateDefaultSubobject<UWYACombatComponent>(TEXT("Combat"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannequinMesh(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (MannequinMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MannequinMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(
			FVector(0.f, 0.f, -90.f),
			FRotator(0.f, -90.f, 0.f));
	}

	// Default loot table: scrap always, ammo 50% of the time
	FWYALootEntry ScrapEntry;
	ScrapEntry.ItemType   = EWYACarriedItemType::Scrap;
	ScrapEntry.Quantity   = 1;
	ScrapEntry.DropChance = 0.8f;

	FWYALootEntry AmmoEntry;
	AmmoEntry.ItemType   = EWYACarriedItemType::Ammo;
	AmmoEntry.Quantity   = 2;
	AmmoEntry.DropChance = 0.5f;

	LootTable.Add(ScrapEntry);
	LootTable.Add(AmmoEntry);
}

void AWYAOpponentCharacter::BeginPlay()
{
	Super::BeginPlay();
	Combat->OnDowned.AddDynamic(this, &AWYAOpponentCharacter::HandleDowned);
}

void AWYAOpponentCharacter::DoMeleeAttack()
{
	UWorld* World = GetWorld();
	if (!World) return;

	// 80cm sphere, 120cm in front at torso height
	const FVector Center = GetActorLocation()
		+ GetActorForwardVector() * 120.f
		+ FVector(0.f, 0.f, 50.f);

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (!World->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity,
		ECC_Pawn, FCollisionShape::MakeSphere(80.f), Params))
	{
		return;
	}

	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* Target = Overlap.GetActor();
		if (!Target) continue;

		// Never hit other opponents — opponents don't fight each other yet
		if (Cast<AWYAOpponentCharacter>(Target)) continue;

		if (UWYACombatComponent* TargetCombat = Target->FindComponentByClass<UWYACombatComponent>())
		{
			TargetCombat->ApplyDamage(15.f, this);
			break; // one target per swing
		}
	}
}

void AWYAOpponentCharacter::HandleDowned(AActor* Attacker)
{
	// Drop loot before the actor is cleaned up
	UWorld* World = GetWorld();
	if (World)
	{
		for (const FWYALootEntry& Entry : LootTable)
		{
			if (FMath::FRand() <= Entry.DropChance)
			{
				const FVector DropLocation = GetActorLocation()
					+ FVector(
						FMath::RandRange(-50.f, 50.f),
						FMath::RandRange(-50.f, 50.f),
						20.f);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AWYALootActor* LootActor = World->SpawnActor<AWYALootActor>(
					AWYALootActor::StaticClass(),
					DropLocation,
					FRotator::ZeroRotator,
					SpawnParams);

				if (LootActor)
				{
					LootActor->ItemType = Entry.ItemType;
					LootActor->Quantity = Entry.Quantity;
				}
			}
		}
	}

	// Stop all movement
	GetCharacterMovement()->DisableMovement();

	// Remove collision so corpse doesn't block navigation
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Ragdoll if a physics asset is assigned — Blueprint can override with proper death anim
	if (GetMesh() && GetMesh()->GetPhysicsAsset())
	{
		GetMesh()->SetSimulatePhysics(true);
	}

	// Notify Blueprint for animation / VFX
	BP_OnDowned(Attacker);

	// Clean up after 5s — keeps the scene from filling with corpses
	SetLifeSpan(5.f);
}
