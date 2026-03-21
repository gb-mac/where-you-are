#pragma once

#include "CoreMinimal.h"
#include "WYAAssistantTypes.generated.h"

/**
 * Assistant voice stage — driven by the fix-him questline repair progress.
 * Stage is set by the game/quest system; the AI subsystem reads it to constrain
 * LLM output to the correct voice register.
 *
 * Stage 1 — Yoda/Bumblebee: single words, inverted syntax, costs him something
 * Stage 2 — Short sentences: 2-4 words, personality starting to surface
 * Stage 3 — The voice arrives: full exchanges, dry, direct, not unkind
 * Stage 4 — Operational: tactical, dry humor, increasing freedom
 * Stage 5 — Restored: Morgan Freeman/Attenborough warmth, full capability
 */
UENUM(BlueprintType)
enum class EWYAAssistantStage : uint8
{
	Stage1_Yoda        UMETA(DisplayName = "Stage 1 — Yoda/Bumblebee"),
	Stage2_Short       UMETA(DisplayName = "Stage 2 — Short Sentences"),
	Stage3_Voice       UMETA(DisplayName = "Stage 3 — The Voice Arrives"),
	Stage4_Operational UMETA(DisplayName = "Stage 4 — Operational"),
	Stage5_Restored    UMETA(DisplayName = "Stage 5 — Restored"),
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnAssistantLine, bool, bSuccess, const FString&, Line);
