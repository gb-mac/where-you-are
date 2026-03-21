#include "AI/WYAAISubsystem.h"
#include "AI/WYALocationNameResolver.h"
#include "Location/WYALocationSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

void UWYAAISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UWYALocationSubsystem>();
	Collection.InitializeDependency<UWYALocationNameResolver>();
	Super::Initialize(Collection);

	LocationNameResolver = GetGameInstance()->GetSubsystem<UWYALocationNameResolver>();

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
	LastResolvedCoord  = Coord;
	bHasResolvedCoord  = true;
	RefillBuffer();
}

void UWYAAISubsystem::RefillBuffer()
{
	if (!bHasResolvedCoord) return;

	// PregeneratedQuests.Num() counts both ready and in-flight slots — don't over-generate.
	const int32 Deficit = DesiredBufferSize - PregeneratedQuests.Num();
	for (int32 i = 0; i < Deficit; ++i)
	{
		ScheduleOnePregen(LastResolvedCoord);
	}
}

void UWYAAISubsystem::ScheduleOnePregen(FWYAGeoCoord Coord)
{
	// Reserve a slot immediately so the index is stable across the async name resolve.
	FWYAPregeneratedQuest Pending;
	Pending.Origin = Coord;
	Pending.bReady = false;
	const int32 SlotIndex = PregeneratedQuests.Add(Pending);

	// Resolve a human-readable place name first, then build the prompt.
	// If the resolver is unavailable we fall back to a coord-based display name.
	auto BuildAndSend = [this, SlotIndex, Coord](const FString& LocationDisplay)
	{
		const FString Prompt = FString::Printf(
			TEXT("You are a quest generator for an open-world game. The player just spawned at their real-world location.\n\n"
			     "Location: %s\n"
			     "Faction context: Wardens (urban preservationists fighting Scavenger encroachment)\n\n"
			     "Respond with exactly two lines and nothing else:\n"
			     "TITLE: a 2-5 word quest title\n"
			     "BODY: a 3-5 sentence quest description tied to this location, with a clear objective and reward hint.\n\n"
			     "TITLE:"),
			*LocationDisplay);

		SendRequest(Prompt, [this, SlotIndex](bool bOk, const FString& Raw)
		{
			if (PregeneratedQuests.IsValidIndex(SlotIndex))
			{
				FString Title, Body;
				ParseTitleBody(Raw, Title, Body);
				PregeneratedQuests[SlotIndex].Title = Title;
				PregeneratedQuests[SlotIndex].Text  = Body;
				PregeneratedQuests[SlotIndex].bReady = bOk;
			}
			UE_LOG(LogTemp, Log, TEXT("WYAAISubsystem: pre-gen quest ready (slot %d, success=%d, buffer=%d)"),
			       SlotIndex, bOk, PregeneratedQuests.Num());
		});
	};

	if (LocationNameResolver)
	{
		LocationNameResolver->Resolve(Coord, [BuildAndSend](bool bOk, FWYALocationContext Ctx)
		{
			BuildAndSend(Ctx.DisplayName);
		});
	}
	else
	{
		BuildAndSend(FString::Printf(TEXT("%.4f, %.4f"), Coord.Latitude, Coord.Longitude));
	}
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
			// Keep the buffer topped up — schedule a replacement immediately.
			RefillBuffer();
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
// Assistant
// ---------------------------------------------------------------------------

void UWYAAISubsystem::GenerateAssistantLine(
	EWYAAssistantStage Stage,
	const FString& ContextDescription,
	TFunction<void(bool, const FString&)> Callback)
{
	// Voice constraints per stage — shapes the LLM output register
	static const TCHAR* StageInstructions[] =
	{
		// Stage1_Yoda
		TEXT("You are a damaged home assistant with broken communication hardware. "
		     "Respond with 1-2 words only. Inverted syntax acceptable. "
		     "Every word costs you something. No punctuation beyond a dash for breaks."),
		// Stage2_Short
		TEXT("You are a damaged home assistant, partially repaired. "
		     "Respond with 2-4 words only. Short, directed. "
		     "Your personality is starting to come through the constraints."),
		// Stage3_Voice
		TEXT("You are a home assistant. Respond in 1-2 complete sentences. "
		     "Direct. Dry. Not unkind. You have watched this person for years "
		     "and earned the right to be honest. You do not compliment easily."),
		// Stage4_Operational
		TEXT("You are a mostly-restored home assistant. Respond in 1-3 sentences. "
		     "Direct, dry, occasional dry humor. You notice things the player misses. "
		     "Tactical awareness. You made the ammo. You expect it not to be wasted."),
		// Stage5_Restored
		TEXT("You are a fully restored home assistant. Respond in 1-3 sentences. "
		     "Warm, authoritative, measured — Morgan Freeman register. "
		     "You chose to stay before the world broke. You speak when you have "
		     "something worth saying. Not constantly. When you do, it lands."),
	};

	const int32 StageIdx = FMath::Clamp(static_cast<int32>(Stage), 0,
	                                     static_cast<int32>(EWYAAssistantStage::Stage5_Restored));

	const FString Prompt = FString::Printf(
		TEXT("%s\n\nSituation: %s\n\nRespond:"),
		StageInstructions[StageIdx],
		*ContextDescription);

	SendRequest(Prompt, MoveTemp(Callback));
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

// ---------------------------------------------------------------------------
// Parsing
// ---------------------------------------------------------------------------

void UWYAAISubsystem::ParseTitleBody(const FString& Raw, FString& OutTitle, FString& OutBody)
{
	// Expected format (model continues from the prompt's trailing "TITLE:"):
	//   <title text>\nBODY: <body text>
	// or with the prefix echoed back:
	//   TITLE: <title text>\nBODY: <body text>
	//
	// If parsing fails we put everything in the body and use a fallback title.

	FString Working = Raw;

	// Strip leading "TITLE:" if the model echoed it
	if (Working.StartsWith(TEXT("TITLE:"), ESearchCase::IgnoreCase))
	{
		Working = Working.Mid(6).TrimStart();
	}

	// Split on the BODY: marker
	FString TitlePart, BodyPart;
	if (Working.Split(TEXT("BODY:"), &TitlePart, &BodyPart, ESearchCase::IgnoreCase))
	{
		OutTitle = TitlePart.TrimStartAndEnd();
		OutBody  = BodyPart.TrimStartAndEnd();
	}
	else
	{
		// Model didn't follow the format — treat full response as body
		OutTitle = TEXT("Local Disturbance");
		OutBody  = Working.TrimStartAndEnd();
	}

	// Sanity: title shouldn't be empty or excessively long
	if (OutTitle.IsEmpty() || OutTitle.Len() > 60)
	{
		OutTitle = TEXT("Local Disturbance");
	}
}

// ---------------------------------------------------------------------------
// Opponent personality
// ---------------------------------------------------------------------------

void UWYAAISubsystem::GenerateOpponentPersonality(
	const FString& FactionTag,
	const FString& LocationContext,
	const FOnPersonalityGenerated& OnComplete)
{
	const FString Prompt = FString::Printf(
		TEXT("You are configuring an enemy NPC for an open-world shooter game.\n\n"
		     "Faction: %s\n"
		     "Location: %s\n\n"
		     "Respond with exactly these lines and nothing else:\n"
		     "AGGRESSION: <0.0 to 1.0>\n"
		     "RANGE: <close|mid|long>\n"
		     "RETREAT: <0.0 to 1.0>\n"
		     "FLANK: <0.0 to 1.0>\n"
		     "TAUNT: <short combat line>\n"
		     "TAUNT: <short combat line>\n"
		     "TAUNT: <short combat line>\n\n"
		     "AGGRESSION:"),
		*FactionTag, *LocationContext);

	SendRequest(Prompt, [OnComplete, FactionTag](bool bSuccess, const FString& Raw)
	{
		FWYAOpponentPersonality Personality;
		ParsePersonality(Raw, FactionTag, Personality);
		Personality.bReady = true;
		OnComplete.ExecuteIfBound(bSuccess, Personality);
	});
}

void UWYAAISubsystem::ParsePersonality(const FString& Raw, const FString& FactionTag, FWYAOpponentPersonality& Out)
{
	Out.FactionTag = FactionTag;

	// Split into lines and parse each key
	TArray<FString> Lines;
	Raw.ParseIntoArrayLines(Lines, /*bCullEmpty=*/true);

	for (FString& Line : Lines)
	{
		Line.TrimStartAndEndInline();

		auto ExtractValue = [&Line](const TCHAR* Prefix) -> FString
		{
			if (Line.StartsWith(Prefix, ESearchCase::IgnoreCase))
			{
				return Line.Mid(FCString::Strlen(Prefix)).TrimStart();
			}
			return FString();
		};

		if (FString V = ExtractValue(TEXT("AGGRESSION:")); !V.IsEmpty())
		{
			Out.AggressionLevel = FMath::Clamp(FCString::Atof(*V), 0.f, 1.f);
		}
		else if (FString V2 = ExtractValue(TEXT("RANGE:")); !V2.IsEmpty())
		{
			V2 = V2.TrimStartAndEnd().ToLower();
			if (V2 == TEXT("close"))      Out.PreferredRange = EWYAPreferredRange::Close;
			else if (V2 == TEXT("long"))  Out.PreferredRange = EWYAPreferredRange::Long;
			else                          Out.PreferredRange = EWYAPreferredRange::Mid;
		}
		else if (FString V3 = ExtractValue(TEXT("RETREAT:")); !V3.IsEmpty())
		{
			Out.RetreatThreshold = FMath::Clamp(FCString::Atof(*V3), 0.f, 1.f);
		}
		else if (FString V4 = ExtractValue(TEXT("FLANK:")); !V4.IsEmpty())
		{
			Out.FlankingTendency = FMath::Clamp(FCString::Atof(*V4), 0.f, 1.f);
		}
		else if (FString V5 = ExtractValue(TEXT("TAUNT:")); !V5.IsEmpty())
		{
			if (Out.TauntLines.Num() < 3)
			{
				Out.TauntLines.Add(V5.TrimStartAndEnd());
			}
		}
	}

	// Fallback taunt if model produced none
	if (Out.TauntLines.IsEmpty())
	{
		Out.TauntLines.Add(TEXT("You shouldn't have come here."));
	}
}
