#include "Characters/AWYANamedTargetCharacter.h"
#include "Contracts/WYAContractSubsystem.h"
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

    // ── Spawn guards ─────────────────────────────────────────────────────────
    SpawnGuards();

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

// ── Contract completion ───────────────────────────────────────────────────────

void AWYANamedTargetCharacter::HandleContractTargetDowned(AActor* Attacker)
{
    if (ContractID.IsEmpty()) return;

    UGameInstance* GI = GetGameInstance();
    if (!GI) return;

    UWYAContractSubsystem* ContractSub = GI->GetSubsystem<UWYAContractSubsystem>();
    if (!ContractSub) return;

    // Complete for any player who holds this contract — supports co-op
    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && ContractSub->HasActiveContract(PC))
        {
            // Check if this specific contract is in their active list
            for (const FWYAContract& C : ContractSub->GetActiveContracts(PC))
            {
                if (C.ID == ContractID)
                {
                    ContractSub->CompleteContract(PC, ContractID);
                    UE_LOG(LogTemp, Log, TEXT("AWYANamedTargetCharacter: contract '%s' completed for %s"),
                        *TargetDisplayName, *GetNameSafe(PC));
                    break;
                }
            }
        }
    }
}
