#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Http.h"
#include "Location/WYAGeoTypes.h"
#include "AI/WYALocationNameResolver.h"
#include "AI/WYAOpponentPersonality.h"
#include "AI/WYAAssistantTypes.h"
#include "Contracts/WYAContractTypes.h"
#include "WYAAISubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnQuestGenerated, bool, bSuccess, const FString&, QuestText);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnDialogueGenerated, bool, bSuccess, const FString&, DialogueText);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPersonalityGenerated, bool, bSuccess, const FWYAOpponentPersonality&, Personality);

/** A pre-generated side quest waiting to be assigned to the player. */
USTRUCT(BlueprintType)
struct FWYAPregeneratedQuest
{
	GENERATED_BODY()

	/** Short quest title, e.g. "Shadows Over the Ramble". */
	UPROPERTY(BlueprintReadOnly)
	FString Title;

	/** The quest body produced by the local LLM. */
	UPROPERTY(BlueprintReadOnly)
	FString Text;

	/** Location the quest was generated for. */
	UPROPERTY(BlueprintReadOnly)
	FWYAGeoCoord Origin;

	/** False until the async request completes. */
	UPROPERTY(BlueprintReadOnly)
	bool bReady = false;
};

/**
 * Async interface to the local ollama inference server.
 * Runs on the game instance — call via GetGameInstance()->GetSubsystem<UWYAAISubsystem>().
 *
 * On Initialize, auto-binds to UWYALocationSubsystem::OnLocationResolved and pre-generates
 * an opening side quest as soon as the player's GPS origin is known.
 *
 * All AI-generated content is SIDE QUESTS only.
 * Main story quests are static — see decisions-log.md.
 *
 * All requests are fire-and-forget HTTP; callbacks run on the game thread.
 */
UCLASS()
class WHEREYOUARE_API UWYAAISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Generate a location-aware side quest and return it via callback.
	 * @param LocationName    Human-readable place name (e.g. "Central Park")
	 * @param Latitude        Player latitude
	 * @param Longitude       Player longitude
	 * @param FactionContext  Short faction description for the prompt
	 * @param OnComplete      Callback — bSuccess=false if ollama is unreachable or times out
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|AI")
	void GenerateQuest(
		const FString& LocationName,
		float Latitude,
		float Longitude,
		const FString& FactionContext,
		const FOnQuestGenerated& OnComplete);

	/**
	 * Generate NPC dialogue and return it via callback.
	 * @param NPCName         NPC identifier
	 * @param NPCRole         Short role description (e.g. "Warden scout, suspicious")
	 * @param PlayerContext   What the player just did or said
	 * @param OnComplete      Callback
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|AI")
	void GenerateDialogue(
		const FString& NPCName,
		const FString& NPCRole,
		const FString& PlayerContext,
		const FOnDialogueGenerated& OnComplete);

	/**
	 * Generate an opponent personality profile and return it via callback.
	 * Call once at spawn — result is written to the AIController's Blackboard.
	 *
	 * @param FactionTag       Faction name, e.g. "Scavenger" or "Iron Compact"
	 * @param LocationContext  Human-readable place name from WYALocationNameResolver
	 * @param OnComplete       Callback — bSuccess=false if ollama unreachable;
	 *                         a fallback personality is always returned regardless
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|AI")
	void GenerateOpponentPersonality(
		const FString& FactionTag,
		const FString& LocationContext,
		const FOnPersonalityGenerated& OnComplete);

	/**
	 * Pop the next ready pre-generated side quest, if any.
	 * Returns false if none are ready yet.
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|AI")
	bool TryPopPregeneratedQuest(FWYAPregeneratedQuest& OutQuest);

	/**
	 * Pop the next ready pre-generated contract, if any.
	 * Returns false if none are ready yet (generation still in flight).
	 * WYAContractSubsystem calls this to refill the contract board.
	 */
	bool TryPopPregeneratedContract(FWYAContract& OutContract);

	/** True if ollama was reachable on last request. */
	UFUNCTION(BlueprintPure, Category = "WYA|AI")
	bool IsAvailable() const { return bOllamaAvailable; }

private:
	void OnLocationResolved(FWYAGeoCoord Coord, bool bSuccess);

	/**
	 * Schedule a single pre-gen quest for the given coord.
	 * Reserves a slot in PregeneratedQuests immediately; populates it async.
	 */
	void ScheduleOnePregen(FWYAGeoCoord Coord);

	/**
	 * Top up the pre-gen buffer to DesiredBufferSize.
	 * Safe to call at any time — no-ops if buffer is already full.
	 */
	void RefillBuffer();

	void SendRequest(const FString& Prompt, TFunction<void(bool, const FString&)> Callback);
	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnected,
	                       TFunction<void(bool, const FString&)> Callback);

public:
	/**
	 * Generate a stage-aware assistant line. Not Blueprint-exposed — called only
	 * from UWYAAssistantSubsystem. Uses TFunction callback to avoid dynamic delegate
	 * binding constraints.
	 */
	void GenerateAssistantLine(
		EWYAAssistantStage Stage,
		const FString& ContextDescription,
		TFunction<void(bool, const FString&)> Callback);

private:
	/** Parse a TITLE:/BODY: response into title and body strings. */
	static void ParseTitleBody(const FString& Raw, FString& OutTitle, FString& OutBody);

	/** Parse a personality key:value response into an FWYAOpponentPersonality. */
	static void ParsePersonality(const FString& Raw, const FString& FactionTag, FWYAOpponentPersonality& Out);

	FString OllamaURL = TEXT("http://127.0.0.1:11434/api/generate");
	FString ModelName = TEXT("phi3.5");
	bool    bOllamaAvailable = false;

	/**
	 * Target number of ready+in-flight pre-gens to maintain.
	 * RefillBuffer() schedules new generations until this count is met.
	 */
	static constexpr int32 DesiredBufferSize = 3;

	/** Last GPS coord from OnLocationResolved — used by RefillBuffer(). */
	FWYAGeoCoord LastResolvedCoord;
	bool         bHasResolvedCoord = false;

	/**
	 * Slots for pre-generated quests.
	 * Entries are added immediately on ScheduleOnePregen (bReady=false),
	 * then flipped to bReady=true when the LLM response arrives.
	 * Num() therefore reflects total pipeline depth (ready + in-flight).
	 */
	TArray<FWYAPregeneratedQuest> PregeneratedQuests;

	// ── Contract pre-generation ───────────────────────────────────────────────

	/** Target number of ready+in-flight contracts to maintain. */
	static constexpr int32 DesiredContractBufferSize = 6;

	TArray<FWYAPregeneratedContract> PregeneratedContracts;

	/**
	 * Schedule one contract pre-gen for Coord.
	 * Seeds the prompt with a random archetype/crime/faction combination
	 * so each generated contract has a distinct premise.
	 */
	void ScheduleOneContractPregen(FWYAGeoCoord Coord, EWYAContractTier Tier);

	/** Top up the contract buffer to DesiredContractBufferSize. */
	void RefillContractBuffer();

	/** Parse a contract key:value response into an FWYAContract. */
	static bool ParseContract(const FString& Raw, FWYAContract& Out);

	UPROPERTY()
	TObjectPtr<UWYALocationNameResolver> LocationNameResolver;
};
