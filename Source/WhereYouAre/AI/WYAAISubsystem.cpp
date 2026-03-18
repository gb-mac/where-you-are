#include "AI/WYAAISubsystem.h"
#include "Location/WYALocationSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

void UWYAAISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// Ensure location subsystem is up before we bind to it
	Collection.InitializeDependency<UWYALocationSubsystem>();
	Super::Initialize(Collection);

	bOllamaAvailable = true;

	// Auto-wire: pre-generate an opening side quest as soon as GPS origin is known
	if (UWYALocationSubsystem* LocSys = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>())
	{
		if (LocSys->IsResolved())
		{
			// Location already resolved (e.g. from cache) — fire immediately
			OnLocationResolved(LocSys->GetOrigin(), true);
		}
		else
		{
			LocSys->OnLocationResolved.AddUObject(this, &UWYAAISubsystem::OnLocationResolved);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("WYAAISubsystem: initialized, target=%s model=%s"),
	       *OllamaURL, *ModelName);
}

// ---------------------------------------------------------------------------
// Pre-generation
// ---------------------------------------------------------------------------

void UWYAAISubsystem::OnLocationResolved(FWYAGeoCoord Coord, bool bSuccess)
{
	if (!bSuccess) return;

	// Queue one opening side quest for the player's spawn location.
	// The game mode calls TryPopPregeneratedQuest() when it needs one.
	FWYAPregeneratedQuest Pending;
	Pending.Origin = Coord;
	Pending.bReady = false;
	const int32 SlotIndex = PregeneratedQuests.Add(Pending);

	const FString Prompt = FString::Printf(
		TEXT("You are a quest generator for an open-world game. The player just spawned at their real-world GPS location.\n\n"
		     "Coordinates: %.4f, %.4f\n"
		     "Faction context: Wardens (urban preservationists fighting Scavenger encroachment)\n\n"
		     "Generate a short opening side quest (3-5 sentences) tied to this real location. "
		     "Include a clear objective and a reward hint. Do not mention coordinates directly. Quest:"),
		Coord.Latitude, Coord.Longitude);

	SendRequest(Prompt, [this, SlotIndex](bool bOk, const FString& Text)
	{
		if (PregeneratedQuests.IsValidIndex(SlotIndex))
		{
			PregeneratedQuests[SlotIndex].Text  = Text;
			PregeneratedQuests[SlotIndex].bReady = bOk;
		}
		UE_LOG(LogTemp, Log, TEXT("WYAAISubsystem: pre-gen quest ready (slot %d, success=%d)"),
		       SlotIndex, bOk);
	});
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

bool UWYAAISubsystem::TryPopPregeneratedQuest(FWYAPregeneratedQuest& OutQuest)
{
	for (int32 i = 0; i < PregeneratedQuests.Num(); ++i)
	{
		if (PregeneratedQuests[i].bReady)
		{
			OutQuest = PregeneratedQuests[i];
			PregeneratedQuests.RemoveAt(i);
			return true;
		}
	}
	return false;
}

void UWYAAISubsystem::GenerateQuest(
	const FString& LocationName,
	float Latitude,
	float Longitude,
	const FString& FactionContext,
	const FOnQuestGenerated& OnComplete)
{
	const FString Prompt = FString::Printf(
		TEXT("You are a quest generator for an open-world game. The player is at a real-world location.\n\n"
		     "Location: %s (%.4f, %.4f)\n"
		     "Faction context: %s\n\n"
		     "Generate a short side quest (3-5 sentences) tied to the real location, matching the faction theme, "
		     "with a clear objective and reward hint. Quest:"),
		*LocationName, Latitude, Longitude, *FactionContext);

	SendRequest(Prompt, [OnComplete](bool bSuccess, const FString& Text)
	{
		OnComplete.ExecuteIfBound(bSuccess, Text);
	});
}

void UWYAAISubsystem::GenerateDialogue(
	const FString& NPCName,
	const FString& NPCRole,
	const FString& PlayerContext,
	const FOnDialogueGenerated& OnComplete)
{
	const FString Prompt = FString::Printf(
		TEXT("You are writing dialogue for an NPC in an open-world game.\n\n"
		     "NPC: %s (%s)\n"
		     "Situation: %s\n\n"
		     "Write 2-3 sentences of in-character dialogue. No stage directions, no quotes. Dialogue:"),
		*NPCName, *NPCRole, *PlayerContext);

	SendRequest(Prompt, [OnComplete](bool bSuccess, const FString& Text)
	{
		OnComplete.ExecuteIfBound(bSuccess, Text);
	});
}

// ---------------------------------------------------------------------------
// Internal
// ---------------------------------------------------------------------------

void UWYAAISubsystem::SendRequest(const FString& Prompt, TFunction<void(bool, const FString&)> Callback)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(OllamaURL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("model"), ModelName);
	Body->SetStringField(TEXT("prompt"), Prompt);
	Body->SetBoolField(TEXT("stream"), false);

	FString BodyString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
	FJsonSerializer::Serialize(Body.ToSharedRef(), Writer);
	Request->SetContentAsString(BodyString);

	Request->OnProcessRequestComplete().BindUObject(
		this, &UWYAAISubsystem::OnRequestComplete, MoveTemp(Callback));

	Request->ProcessRequest();
}

void UWYAAISubsystem::OnRequestComplete(
	FHttpRequestPtr Request,
	FHttpResponsePtr Response,
	bool bConnected,
	TFunction<void(bool, const FString&)> Callback)
{
	if (!bConnected || !Response.IsValid())
	{
		bOllamaAvailable = false;
		UE_LOG(LogTemp, Warning, TEXT("WYAAISubsystem: ollama unreachable at %s"), *OllamaURL);
		Callback(false, FString());
		return;
	}

	bOllamaAvailable = true;

	TSharedPtr<FJsonObject> JsonObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (!FJsonSerializer::Deserialize(Reader, JsonObj) || !JsonObj.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("WYAAISubsystem: failed to parse ollama response"));
		Callback(false, FString());
		return;
	}

	FString ResponseText;
	JsonObj->TryGetStringField(TEXT("response"), ResponseText);
	ResponseText.TrimStartAndEndInline();

	Callback(true, ResponseText);
}
