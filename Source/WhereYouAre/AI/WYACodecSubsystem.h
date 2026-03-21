#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WYACodecSubsystem.generated.h"

/**
 * One line or response branch in a codec call sequence.
 * The player's visible response options (if any) and which call they trigger next.
 */
USTRUCT(BlueprintType)
struct FWYACodecBranch
{
	GENERATED_BODY()

	/** What the player can say / do to continue. Empty = auto-advance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerPrompt;

	/** Tag of the next codec call this branch leads to. Empty = end of call. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NextCallTag;
};

/**
 * One codec call — a Burke monologue triggered by a world event.
 *
 * Burke does not use LLM. His voice is too specific, too ideologically precise,
 * and too plot-bearing for generative content. All codec content is authored.
 *
 * The narrative agent populates these via RegisterCall() at game init, or via
 * a DataTable asset (DT_CodecCalls) — see TODO below.
 *
 * Structure mirrors MGS codec: Burke's train of thought runs to completion.
 * Player response choices determine which trains run, not whether he's on them.
 */
USTRUCT(BlueprintType)
struct FWYACodecCall
{
	GENERATED_BODY()

	/**
	 * Unique tag for this call, e.g. "after_first_vael_encounter".
	 * Also used to chain calls via FWYACodecBranch::NextCallTag.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CallTag;

	/** The authored monologue lines — delivered in order, one after the other. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Lines;

	/**
	 * Player response options at the end of the monologue.
	 * Empty array = codec call ends with no player input.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWYACodecBranch> Branches;

	/**
	 * Whether this call can be replayed if the player visits Burke again.
	 * False = plays once. True = available on return.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRepeatable = false;
};

/** Fired when a codec call begins. UI layer subscribes to display the monologue. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCodecCallStarted, const FString&, CallTag, const FWYACodecCall&, Call);

/** Fired when a codec call ends (all lines delivered, no more branches). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCodecCallEnded, const FString&, CallTag);

/**
 * Burke's codec call delivery system.
 *
 * No LLM. All content is authored by the narrative agent.
 *
 * Usage (narrative agent):
 *   RegisterCall() for each authored codec call at game init.
 *
 * Usage (game/world systems):
 *   TriggerCodec("after_first_vael_encounter", PlayerController)
 *   to deliver the call associated with that world event.
 *
 * Usage (UI):
 *   Subscribe to OnCodecCallStarted and OnCodecCallEnded to drive the codec UI.
 *
 * TODO (Narrative agent): Populate all codec calls. Suggested structure:
 *   - One RegisterCall() block per authored monologue in WYAGameMode::InitGame,
 *     OR load from DT_CodecCalls.uasset DataTable (preferred for editor editing).
 *
 * TODO (Core agent): Create DT_CodecCalls.uasset in Content/Narrative/ and wire
 *   LoadCallsFromDataTable() on subsystem init. Build the codec UI widget that
 *   subscribes to OnCodecCallStarted/Ended and displays lines sequentially.
 */
UCLASS()
class WHEREYOUARE_API UWYACodecSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Register an authored codec call.
	 * Call from game init or load from DataTable.
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|Codec")
	void RegisterCall(const FWYACodecCall& Call);

	/**
	 * Trigger a codec call by event tag.
	 * No-ops silently if no call is registered for this tag, or if it has already
	 * played and bRepeatable=false.
	 *
	 * @param EventTag   Matches FWYACodecCall::CallTag
	 * @param PC         The player receiving the call (for future multiplayer routing)
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|Codec")
	void TriggerCodec(const FString& EventTag, APlayerController* PC);

	/** True if a codec call is currently active. */
	UFUNCTION(BlueprintPure, Category = "WYA|Codec")
	bool IsActive() const { return bCallActive; }

	/** Advance to the next line in the active call (call from UI on player input). */
	UFUNCTION(BlueprintCallable, Category = "WYA|Codec")
	void AdvanceLine();

	/** Select a branch response by index (0-based). */
	UFUNCTION(BlueprintCallable, Category = "WYA|Codec")
	void SelectBranch(int32 BranchIndex);

	UPROPERTY(BlueprintAssignable, Category = "WYA|Codec")
	FOnCodecCallStarted OnCodecCallStarted;

	UPROPERTY(BlueprintAssignable, Category = "WYA|Codec")
	FOnCodecCallEnded OnCodecCallEnded;

private:
	TMap<FString, FWYACodecCall> Calls;
	TSet<FString>                PlayedCalls; // non-repeatable calls already delivered

	bool          bCallActive    = false;
	FWYACodecCall ActiveCall;
	int32         ActiveLineIdx  = 0;

	void BeginCall(const FWYACodecCall& Call);
	void EndCall();
};
