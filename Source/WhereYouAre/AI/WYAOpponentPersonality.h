#pragma once

#include "CoreMinimal.h"
#include "WYAOpponentPersonality.generated.h"

/** Preferred engagement range — maps to BT branch selection. */
UENUM(BlueprintType)
enum class EWYAPreferredRange : uint8
{
	Close  UMETA(DisplayName = "Close"),  // melee / shotgun
	Mid    UMETA(DisplayName = "Mid"),    // SMG / assault rifle
	Long   UMETA(DisplayName = "Long"),   // rifle / sniper
};

/**
 * LLM-generated personality profile for an opponent.
 * Produced once at spawn by UWYAAISubsystem::GenerateOpponentPersonality.
 * Applied to the AIController's Blackboard — the Behavior Tree reads these values
 * to parameterise combat decisions (range preference, aggression, retreat, flanking).
 *
 * The BT must not start making personality-gated decisions until bReady = true.
 * Until then it uses the fallback defaults defined below.
 */
USTRUCT(BlueprintType)
struct FWYAOpponentPersonality
{
	GENERATED_BODY()

	/** 0 = passive / defensive, 1 = berserker. Affects attack frequency and cover use. */
	UPROPERTY(BlueprintReadOnly)
	float AggressionLevel = 0.5f;

	/** Preferred engagement distance. Drives cover selection and weapon choice. */
	UPROPERTY(BlueprintReadOnly)
	EWYAPreferredRange PreferredRange = EWYAPreferredRange::Mid;

	/**
	 * Health fraction at which the opponent retreats.
	 * 0 = fights to the death, 1 = retreats immediately.
	 */
	UPROPERTY(BlueprintReadOnly)
	float RetreatThreshold = 0.25f;

	/**
	 * Tendency to flank rather than advance directly.
	 * 0 = straight charge, 1 = always flanks.
	 */
	UPROPERTY(BlueprintReadOnly)
	float FlankingTendency = 0.5f;

	/** 2–3 short combat callout lines for audio/subtitles. */
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> TauntLines;

	/** Faction tag used to generate this personality, e.g. "Scavenger". */
	UPROPERTY(BlueprintReadOnly)
	FString FactionTag;

	/** False until the LLM response has been parsed. BT should use defaults until true. */
	UPROPERTY(BlueprintReadOnly)
	bool bReady = false;
};
