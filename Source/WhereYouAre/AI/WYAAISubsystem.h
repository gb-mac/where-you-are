#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Http.h"
#include "Location/WYAGeoTypes.h"
#include "WYAAISubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnQuestGenerated, bool, bSuccess, const FString&, QuestText);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnDialogueGenerated, bool, bSuccess, const FString&, DialogueText);

/** A pre-generated side quest waiting to be assigned to the player. */
USTRUCT(BlueprintType)
struct FWYAPregeneratedQuest
{
	GENERATED_BODY()

	/** The quest text produced by the local LLM. */
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
	 * Pop the next ready pre-generated side quest, if any.
	 * Returns false if none are ready yet.
	 */
	UFUNCTION(BlueprintCallable, Category = "WYA|AI")
	bool TryPopPregeneratedQuest(FWYAPregeneratedQuest& OutQuest);

	/** True if ollama was reachable on last request. */
	UFUNCTION(BlueprintPure, Category = "WYA|AI")
	bool IsAvailable() const { return bOllamaAvailable; }

private:
	void OnLocationResolved(FWYAGeoCoord Coord, bool bSuccess);
	void SendRequest(const FString& Prompt, TFunction<void(bool, const FString&)> Callback);
	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnected,
	                       TFunction<void(bool, const FString&)> Callback);

	FString OllamaURL = TEXT("http://127.0.0.1:11434/api/generate");
	FString ModelName = TEXT("phi3.5");
	bool    bOllamaAvailable = false;

	/** Ring buffer of pre-generated side quests ready to assign. */
	TArray<FWYAPregeneratedQuest> PregeneratedQuests;
};
