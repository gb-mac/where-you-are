#include "Characters/AWYANamedTargetCharacter.h"
#include "Characters/AWYASecurityCharacter.h"
#include "Contracts/WYAContractSubsystem.h"
#include "Actors/AWYAExfilPoint.h"
#include "EngineUtils.h"
#include "Combat/WYACombatComponent.h"
#include "Loot/AWYALootActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

// ── Tier tables ───────────────────────────────────────────────────────────────

struct FWYANamedTargetTierConfig
{
    float MaxHealth;
    int32 GuardCount;
    float WalkSpeed;    // normal combat speed
    float RetreatSpeed; // speed during retreat burst
};

static constexpr FWYANamedTargetTierConfig TierConfigs[] =
{
    // Standard
    { 300.f, 1, 420.f, 650.f },
    // Priority
    { 500.f, 2, 450.f, 700.f },
    // HighTable
    { 800.f, 3, 450.f, 700.f },
};

// ── Construction ──────────────────────────────────────────────────────────────

AWYANamedTargetCharacter::AWYANamedTargetCharacter()
{
    // Default FactionTag — callers override for the actual faction context
    FactionTag = TEXT("NamedTarget");

    // Loot table is rebuilt in BeginPlay once Tier is known.
    LootTable.Empty();
}

void AWYANamedTargetCharacter::BeginPlay()
{
    Super::BeginPlay(); // registers OnDowned → HandleDowned (base class drops loot + ragdoll)

    const int32 TierIdx = FMath::Clamp(static_cast<int32>(Tier), 0, 2);
    const FWYANamedTargetTierConfig& Cfg = TierConfigs[TierIdx];

    // ── Scale health ─────────────────────────────────────────────────────────
    Combat->MaxHealth = Cfg.MaxHealth;
    Combat->Revive(Cfg.MaxHealth); // sets CurrentHealth to full without broadcasting Down

    // ── Movement speed ───────────────────────────────────────────────────────
    GetCharacterMovement()->MaxWalkSpeed = Cfg.WalkSpeed;

    // ── Override loot table — named targets always drop meaningfully ─────────
    LootTable.Empty();

    auto AddLoot = [this](EWYACarriedItemType Type, int32 Qty, float Chance = 1.f)
    {
        FWYALootEntry E;
        E.ItemType   = Type;
        E.Quantity   = Qty;
        E.DropChance = Chance;
        LootTable.Add(E);
    };

    switch (Tier)
    {
    case EWYAContractTier::HighTable:
        AddLoot(EWYACarriedItemType::RangedWeapon, 1);
        AddLoot(EWYACarriedItemType::Ammo,         8);
        AddLoot(EWYACarriedItemType::Scrap,        4);
        AddLoot(EWYACarriedItemType::Food,         2, 0.7f);
        break;
    case EWYAContractTier::Priority:
        AddLoot(EWYACarriedItemType::MeleeWeapon,  1);
        AddLoot(EWYACarriedItemType::Ammo,         5);
        AddLoot(EWYACarriedItemType::Scrap,        3);
        AddLoot(EWYACarriedItemType::Food,         1, 0.6f);
        break;
    default: // Standard
        AddLoot(EWYACarriedItemType::Ammo,  3);
        AddLoot(EWYACarriedItemType::Scrap, 2);
        AddLoot(EWYACarriedItemType::Food,  1, 0.5f);
        break;
    }

    // ── Bind health-change delegate for retreat trigger ───────────────────────
    Combat->OnHealthChanged.AddDynamic(this, &AWYANamedTargetCharacter::OnHealthUpdated);

    // ── Bind downed delegate for contract completion ──────────────────────────
    Combat->OnDowned.AddDynamic(this, &AWYANamedTargetCharacter::HandleContractTargetDowned);

    // ── Spawn guards and security perimeter ──────────────────────────────────
    SpawnGuards();
    SpawnSecurity();

    BP_OnTargetSpawned(TargetDisplayName, Tier);

    UE_LOG(LogTemp, Log, TEXT("AWYANamedTargetCharacter: '%s' spawned (tier=%d hp=%.0f guards=%d)"),
        *TargetDisplayName, TierIdx, Cfg.MaxHealth, Cfg.GuardCount);
}

// ── Guards ────────────────────────────────────────────────────────────────────

void AWYANamedTargetCharacter::SpawnGuards()
{
    UWorld* World = GetWorld();
    if (!World) return;

    const int32 TierIdx   = FMath::Clamp(static_cast<int32>(Tier), 0, 2);
    const int32 GuardCount = TierConfigs[TierIdx].GuardCount;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 i = 0; i < GuardCount; ++i)
    {
        const float Angle   = (360.f / GuardCount) * i + FMath::RandRange(-15.f, 15.f);
        const float Dist    = FMath::RandRange(250.f, 450.f); // 2.5–4.5m ring

        const FVector Offset(
            FMath::Cos(FMath::DegreesToRadians(Angle)) * Dist,
            FMath::Sin(FMath::DegreesToRadians(Angle)) * Dist,
            0.f);

        const FVector SpawnLoc = GetActorLocation() + Offset;

        AWYAOpponentCharacter* Guard = World->SpawnActor<AWYAOpponentCharacter>(
            AWYAOpponentCharacter::StaticClass(), SpawnLoc, FRotator::ZeroRotator, Params);

        if (Guard)
        {
            Guard->FactionTag = FactionTag; // guards share faction with the target
            UE_LOG(LogTemp, Log, TEXT("AWYANamedTargetCharacter: guard %d spawned for '%s'"),
                i + 1, *TargetDisplayName);
        }
    }
}

// ── Retreat ───────────────────────────────────────────────────────────────────

void AWYANamedTargetCharacter::OnHealthUpdated(float NewHealth, float InMaxHealth)
{
    if (bHasRetreated || bRetreating) return;
    if (InMaxHealth <= 0.f) return;

    if ((NewHealth / InMaxHealth) <= RetreatHealthFraction)
    {
        BeginRetreat();
    }
}

void AWYANamedTargetCharacter::BeginRetreat()
{
    bRetreating  = true;
    bHasRetreated = true;

    const int32 TierIdx = FMath::Clamp(static_cast<int32>(Tier), 0, 2);
    GetCharacterMovement()->MaxWalkSpeed = TierConfigs[TierIdx].RetreatSpeed;

    // Move away from the nearest player pawn
    APlayerController* NearestPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (NearestPC && NearestPC->GetPawn())
    {
        const FVector AwayDir = (GetActorLocation() - NearestPC->GetPawn()->GetActorLocation()).GetSafeNormal();
        const FVector RetreatTarget = GetActorLocation() + AwayDir * 1500.f;
        // The AI controller will pick this up via the Blackboard in the next tick.
        // For now, we move the character directly as a fallback.
        GetCharacterMovement()->AddInputVector(AwayDir, /*bForce=*/false);
    }

    BP_OnRetreat();

    UE_LOG(LogTemp, Log, TEXT("AWYANamedTargetCharacter: '%s' retreating at %.0f HP"),
        *TargetDisplayName, Combat->GetCurrentHealth());

    // Re-engage after 5 seconds
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(RetreatTimerHandle,
            this, &AWYANamedTargetCharacter::EndRetreat, 5.f, /*bLooping=*/false);
    }
}

void AWYANamedTargetCharacter::EndRetreat()
{
    if (!IsValid(this) || Combat->IsDown()) return;

    bRetreating = false;

    const int32 TierIdx = FMath::Clamp(static_cast<int32>(Tier), 0, 2);
    GetCharacterMovement()->MaxWalkSpeed = TierConfigs[TierIdx].WalkSpeed;

    BP_OnReengage();

    UE_LOG(LogTemp, Log, TEXT("AWYANamedTargetCharacter: '%s' re-engaging"), *TargetDisplayName);
}

// ── Security perimeter ────────────────────────────────────────────────────────

void AWYANamedTargetCharacter::SpawnSecurity()
{
    UWorld* World = GetWorld();
    if (!World) return;

    const int32 TierIdx = FMath::Clamp(static_cast<int32>(Tier), 0, 2);

    // Security count per tier: Standard=2, Priority=3, HighTable=5
    static constexpr int32 SecurityCounts[] = { 2, 3, 5 };
    const int32 SecurityCount = SecurityCounts[TierIdx];

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (int32 i = 0; i < SecurityCount; ++i)
    {
        const float Angle = FMath::RandRange(0.f, 360.f);
        const float Dist  = FMath::RandRange(800.f, 2000.f); // 8–20m patrol ring

        const FVector Offset(
            FMath::Cos(FMath::DegreesToRadians(Angle)) * Dist,
            FMath::Sin(FMath::DegreesToRadians(Angle)) * Dist,
            0.f);

        AWYASecurityCharacter* Guard = World->SpawnActor<AWYASecurityCharacter>(
            AWYASecurityCharacter::StaticClass(), GetActorLocation() + Offset, FRotator::ZeroRotator, Params);

        if (Guard)
        {
            UE_LOG(LogTemp, Log, TEXT("AWYANamedTargetCharacter: security %d/%d spawned for '%s'"),
                i + 1, SecurityCount, *TargetDisplayName);
        }
    }
}

// ── Body discovery watch ──────────────────────────────────────────────────────

void AWYANamedTargetCharacter::StartBodyDiscoveryWatch()
{
    UWorld* World = GetWorld();
    if (!World || ContractID.IsEmpty()) return;

    // Poll every second until a guard finds the body or the actor is destroyed.
    World->GetTimerManager().SetTimer(BodyDiscoveryTimerHandle,
        this, &AWYANamedTargetCharacter::CheckBodyDiscovery,
        1.f, /*bLooping=*/true);
}

void AWYANamedTargetCharacter::CheckBodyDiscovery()
{
    UWorld* World = GetWorld();
    if (!World) return;

    const FVector BodyLoc = GetActorLocation();

    for (TActorIterator<AWYASecurityCharacter> It(World); It; ++It)
    {
        AWYASecurityCharacter* Guard = *It;
        if (!IsValid(Guard) || Guard->Combat->IsDown()) continue;

        if (FVector::Dist(BodyLoc, Guard->GetActorLocation()) <= BodyDiscoveryRadius)
        {
            // Guard found the body — void Ghost for this contract
            if (UGameInstance* GI = GetGameInstance())
            {
                if (UWYAContractSubsystem* ContractSub = GI->GetSubsystem<UWYAContractSubsystem>())
                {
                    ContractSub->NotifyBodyDiscovered(ContractID);
                }
            }

            // Stop polling — discovery already fired
            World->GetTimerManager().ClearTimer(BodyDiscoveryTimerHandle);
            return;
        }
    }
}

// ── Contract completion (exfil spawn) ────────────────────────────────────────

void AWYANamedTargetCharacter::HandleContractTargetDowned(AActor* Attacker)
{
    if (ContractID.IsEmpty()) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // Start watching for body discovery — this is what voids Ghost, not alert state.
    StartBodyDiscoveryWatch();

    // Target is down — spawn the extraction point 100–200m away.
    // Gold is not awarded until the player reaches it.
    const float Angle = FMath::RandRange(0.f, 360.f);
    const float Dist  = FMath::RandRange(10000.f, 20000.f); // 100–200m in UU

    const FVector ExfilLoc = GetActorLocation() + FVector(
        FMath::Cos(FMath::DegreesToRadians(Angle)) * Dist,
        FMath::Sin(FMath::DegreesToRadians(Angle)) * Dist,
        200.f);

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AWYAExfilPoint* Exfil = World->SpawnActor<AWYAExfilPoint>(
        AWYAExfilPoint::StaticClass(), ExfilLoc, FRotator::ZeroRotator, SpawnParams);

    if (Exfil)
    {
        Exfil->ContractID = ContractID;
        UE_LOG(LogTemp, Log, TEXT("AWYANamedTargetCharacter: '%s' down — exfil spawned %.0fm away (contract %s)"),
            *TargetDisplayName, Dist / 100.f, *ContractID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AWYANamedTargetCharacter: failed to spawn exfil for contract '%s'"),
            *ContractID);
    }
}
