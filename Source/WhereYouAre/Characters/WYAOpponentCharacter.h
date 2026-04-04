#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory/WYAInventoryTypes.h"
#include "WYAOpponentCharacter.generated.h"

class UWYACombatComponent;

/**
 * One entry in the opponent's loot table.
 * On death each entry rolls independently against its DropChance.
 */
USTRUCT(BlueprintType)
struct FWYALootEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Loot")
	EWYACarriedItemType ItemType = EWYACarriedItemType::Scrap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Loot")
	int32 Quantity = 1;

	/** 0.0 = never drops, 1.0 = always drops. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Loot", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DropChance = 1.0f;
};

/**
 * Generic opponent character. Possessed by AWYAOpponentController.
 *
 * Melee range is handled by DoMeleeAttack() — a sphere sweep in front that
 * deals damage to the first non-opponent hit target.
 *
 * No death state — characters go Down. Ragdoll fires on HandleDowned and the
 * actor is cleaned up after a short delay.
 *
 * Blueprint subclass (BP_Opponent_*) sets mesh, physics asset, faction tag,
 * and implements BP_OnHitReact / BP_OnDowned for visual feedback.
 */
UCLASS()
class WHEREYOUARE_API AWYAOpponentCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWYAOpponentCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Combat")
	TObjectPtr<UWYACombatComponent> Combat;

	/** Faction identity — forwarded to WYAOpponentController for LLM personality. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|AI")
	FString FactionTag = TEXT("Scavenger");

	/**
	 * Items this opponent drops on death. Each entry rolls independently.
	 * Default: Scrap (80% chance) + 2x Ammo (50% chance).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Loot")
	TArray<FWYALootEntry> LootTable;

	/**
	 * Sphere-sweep melee attack. Called by the controller state machine or a BT task.
	 * Deals 15 damage to first non-opponent target in a 0.8m arc, 1.2m forward.
	 * Blueprint can play the attack animation before calling this.
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|Combat")
	void DoMeleeAttack();

protected:
	virtual void BeginPlay() override;

	/** Override in Blueprint to play hit-react animation. */
	UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
	void BP_OnHitReact(AActor* Attacker);

	/** Override in Blueprint to play downed animation or ragdoll setup. */
	UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Combat")
	void BP_OnDowned(AActor* Attacker);

private:
	UFUNCTION()
	void HandleDowned(AActor* Attacker);
};
