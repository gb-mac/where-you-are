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
	RefillContractBuffer();
}

void UWYAAISubsystem::RefillBuffer()
{
	if (!bHasResolvedCoord) return;

	// Schedule at most one request at a time — ollama is sequential on this hardware
	// and concurrent requests time each other out. Each callback chains back here.
	if (PregeneratedQuests.Num() < DesiredBufferSize)
	{
		ScheduleOnePregen(LastResolvedCoord);
	}
}

void UWYAAISubsystem::ScheduleOnePregen(FWYAGeoCoord Coord)
{
	// Reserve a slot immediately so Num() reflects the in-flight request.
	FWYAPregeneratedQuest Pending;
	Pending.Origin = Coord;
	Pending.bReady = false;
	PregeneratedQuests.Add(Pending);

	// Resolve a human-readable place name first, then build the prompt.
	// Nominatim result is cached — subsequent calls for the same coord are instant.
	auto BuildAndSend = [this](const FString& LocationDisplay)
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

		SendRequest(Prompt, [this](bool bOk, const FString& Raw)
		{
			// With serialised requests there is exactly one non-ready slot — find it.
			const int32 SlotIndex = PregeneratedQuests.IndexOfByPredicate(
				[](const FWYAPregeneratedQuest& Q){ return !Q.bReady; });

			if (SlotIndex != INDEX_NONE)
			{
				if (bOk)
				{
					FString Title, Body;
					ParseTitleBody(Raw, Title, Body);
					PregeneratedQuests[SlotIndex].Title = Title;
					PregeneratedQuests[SlotIndex].Text  = Body;
					PregeneratedQuests[SlotIndex].bReady = true;
				}
				else
				{
					// Remove the failed slot so RefillBuffer() will replace it.
					PregeneratedQuests.RemoveAt(SlotIndex);
				}
			}

			UE_LOG(LogTemp, Log, TEXT("WYAAISubsystem: pre-gen %s (buffer=%d ready, %d total)"),
			       bOk ? TEXT("ready") : TEXT("failed — retrying"),
			       PregeneratedQuests.FilterByPredicate([](const FWYAPregeneratedQuest& Q){ return Q.bReady; }).Num(),
			       PregeneratedQuests.Num());

			// Chain: schedule the next pre-gen if the buffer still needs filling.
			RefillBuffer();
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
// Contract pre-generation
// ---------------------------------------------------------------------------

void UWYAAISubsystem::RefillContractBuffer()
{
	if (!bHasResolvedCoord) return;

	// One in-flight request at a time — same serialised pattern as quest pre-gen.
	// Distribute tiers: roughly 60% Standard, 30% Priority, 10% HighTable.
	while (PregeneratedContracts.Num() < DesiredContractBufferSize)
	{
		const float Roll = FMath::FRand();
		EWYAContractTier Tier = EWYAContractTier::Standard;
		if      (Roll > 0.90f) Tier = EWYAContractTier::HighTable;
		else if (Roll > 0.60f) Tier = EWYAContractTier::Priority;

		ScheduleOneContractPregen(LastResolvedCoord, Tier);
		break; // only one at a time — callback chains back here
	}
}

void UWYAAISubsystem::ScheduleOneContractPregen(FWYAGeoCoord Coord, EWYAContractTier Tier)
{
	// Reserve a slot immediately
	FWYAPregeneratedContract Pending;
	Pending.Contract.GeneratedForCoord = Coord;
	Pending.Contract.Tier = Tier;
	Pending.bReady = false;
	PregeneratedContracts.Add(Pending);

	// ── Variety seeds ─────────────────────────────────────────────────────────
	// 6 archetypes × 6 crimes × 4 factions = 144 base combos before LLM variance.

	static const TCHAR* Archetypes[] = {
		TEXT("Enforcer"),
		TEXT("Information broker"),
		TEXT("Fixer"),
		TEXT("Courier"),
		TEXT("Black market supplier"),
		TEXT("Faction traitor"),
	};
	static const TCHAR* Crimes[] = {
		TEXT("defaulted on a substantial debt to a faction"),
		TEXT("violated territorial boundaries and cost lives"),
		TEXT("sold information to the wrong side"),
		TEXT("broke a sanctioned contract"),
		TEXT("is actively extorting civilians under faction protection"),
		TEXT("stole material from a protected supply line"),
	};
	static const TCHAR* Clients[] = {
		TEXT("the Wardens"),
		TEXT("the Covenant"),
		TEXT("the Iron Compact"),
		TEXT("a Riven cell"),
	};
	static const TCHAR* TierLabels[] = { TEXT("Standard"), TEXT("Priority"), TEXT("HighTable") };

	const int32 ArchIdx   = FMath::RandRange(0, 5);
	const int32 CrimeIdx  = FMath::RandRange(0, 5);
	const int32 ClientIdx = FMath::RandRange(0, 3);
	const int32 TierIdx   = static_cast<int32>(Tier);

	// Gold reward range per tier
	int32 GoldMin = 10, GoldMax = 20;
	if      (Tier == EWYAContractTier::Priority)  { GoldMin = 20; GoldMax = 35; }
	else if (Tier == EWYAContractTier::HighTable)  { GoldMin = 35; GoldMax = 50; }
	const int32 GoldReward = FMath::RandRange(GoldMin, GoldMax);

	auto BuildAndSend = [this, Tier, TierIdx, GoldReward, ArchIdx, CrimeIdx, ClientIdx]
	                    (const FString& LocationDisplay)
	{
		static const TCHAR* Archetypes2[] = {
			TEXT("Enforcer"), TEXT("Information broker"), TEXT("Fixer"),
			TEXT("Courier"), TEXT("Black market supplier"), TEXT("Faction traitor"),
		};
		static const TCHAR* Crimes2[] = {
			TEXT("defaulted on a substantial debt to a faction"),
			TEXT("violated territorial boundaries and cost lives"),
			TEXT("sold information to the wrong side"),
			TEXT("broke a sanctioned contract"),
			TEXT("is actively extorting civilians under faction protection"),
			TEXT("stole material from a protected supply line"),
		};
		static const TCHAR* Clients2[] = {
			TEXT("the Wardens"), TEXT("the Covenant"),
			TEXT("the Iron Compact"), TEXT("a Riven cell"),
		};
		static const TCHAR* TierLabels2[] = { TEXT("Standard"), TEXT("Priority"), TEXT("HighTable") };

		const FString Prompt = FString::Printf(
			TEXT("You are generating a hit contract for a post-collapse urban faction game.\n\n"
			     "Location: %s\n"
			     "Contract tier: %s\n"
			     "Target archetype: %s\n"
			     "Reason for contract: This person %s. The contract is posted by %s.\n"
			     "Gold reward: %d\n\n"
			     "Respond with exactly these lines and nothing else:\n"
			     "TARGET_NAME: <a believable full name or alias, no titles>\n"
			     "TARGET_ROLE: <one line — faction, location, role>\n"
			     "LAST_SEEN: <specific location description near %s>\n"
			     "INTEL: <2-3 sentences — backstory, current threat, urgency. No fluff.>\n"
			     "REWARD: %d\n"
			     "TIER: %s\n\n"
			     "TARGET_NAME:"),
			*LocationDisplay,
			TierLabels2[TierIdx],
			Archetypes2[ArchIdx],
			Crimes2[CrimeIdx],
			Clients2[ClientIdx],
			GoldReward,
			*LocationDisplay,
			GoldReward,
			TierLabels2[TierIdx]);

		SendRequest(Prompt, [this, GoldReward, Tier](bool bOk, const FString& Raw)
		{
			const int32 SlotIndex = PregeneratedContracts.IndexOfByPredicate(
				[](const FWYAPregeneratedContract& C){ return !C.bReady; });

			if (SlotIndex != INDEX_NONE)
			{
				if (bOk)
				{
					FWYAContract Parsed;
					if (ParseContract(Raw, Parsed))
					{
						Parsed.ID = FString::Printf(TEXT("contract_%lld"), FDateTime::UtcNow().GetTicks());
						Parsed.Tier = PregeneratedContracts[SlotIndex].Contract.Tier;
						Parsed.GeneratedForCoord = PregeneratedContracts[SlotIndex].Contract.GeneratedForCoord;
						// Use the LLM-parsed reward if in tier range, else keep seeded value
						if (Parsed.GoldReward <= 0) Parsed.GoldReward = GoldReward;
						PregeneratedContracts[SlotIndex].Contract = Parsed;
						PregeneratedContracts[SlotIndex].bReady = true;
					}
					else
					{
						PregeneratedContracts.RemoveAt(SlotIndex);
					}
				}
				else
				{
					PregeneratedContracts.RemoveAt(SlotIndex);
				}
			}

			const int32 ReadyCount = PregeneratedContracts.FilterByPredicate(
				[](const FWYAPregeneratedContract& C){ return C.bReady; }).Num();
			UE_LOG(LogTemp, Log, TEXT("WYAAISubsystem: contract pre-gen %s (buffer=%d ready, %d total)"),
				bOk ? TEXT("ready") : TEXT("failed — retrying"),
				ReadyCount, PregeneratedContracts.Num());

			RefillContractBuffer();
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

bool UWYAAISubsystem::TryPopPregeneratedContract(FWYAContract& OutContract)
{
	for (int32 i = 0; i < PregeneratedContracts.Num(); ++i)
	{
		if (PregeneratedContracts[i].bReady)
		{
			OutContract = PregeneratedContracts[i].Contract;
			PregeneratedContracts.RemoveAt(i);
			RefillContractBuffer();
			return true;
		}
	}
	return false;
}

bool UWYAAISubsystem::ParseContract(const FString& Raw, FWYAContract& Out)
{
	// Expected format (model continues from trailing "TARGET_NAME:"):
	//   <name>\nTARGET_ROLE: <role>\nLAST_SEEN: <loc>\nINTEL: <text>\nREWARD: <n>\nTIER: <tier>

	TArray<FString> Lines;
	Raw.ParseIntoArrayLines(Lines, /*bCullEmpty=*/true);

	// The first line is the target name (model continues from the prompt's trailing "TARGET_NAME:")
	// or it echoes "TARGET_NAME: <name>" — handle both.
	bool bGotName = false;

	for (FString& Line : Lines)
	{
		Line.TrimStartAndEndInline();
		if (Line.IsEmpty()) continue;

		auto Extract = [&Line](const TCHAR* Prefix) -> FString
		{
			if (Line.StartsWith(Prefix, ESearchCase::IgnoreCase))
				return Line.Mid(FCString::Strlen(Prefix)).TrimStart();
			return FString();
		};

		if (!bGotName)
		{
			// Could be "TARGET_NAME: Foo" or just "Foo" as continuation
			if (FString V = Extract(TEXT("TARGET_NAME:")); !V.IsEmpty())
				Out.TargetName = V;
			else
				Out.TargetName = Line;
			bGotName = true;
		}
		else if (FString V = Extract(TEXT("TARGET_ROLE:")); !V.IsEmpty())
		{
			Out.TargetRole = V;
		}
		else if (FString V = Extract(TEXT("LAST_SEEN:")); !V.IsEmpty())
		{
			Out.LastSeen = V;
		}
		else if (FString V = Extract(TEXT("INTEL:")); !V.IsEmpty())
		{
			Out.Intel = V;
		}
		else if (Out.Intel.Len() > 0 && !Line.Contains(TEXT(":")))
		{
			// Continuation line for INTEL (multi-line body)
			Out.Intel += TEXT(" ") + Line;
		}
		else if (FString V = Extract(TEXT("REWARD:")); !V.IsEmpty())
		{
			Out.GoldReward = FMath::Clamp(FCString::Atoi(*V), 5, 60);
		}
		else if (FString V = Extract(TEXT("TIER:")); !V.IsEmpty())
		{
			V.TrimStartAndEndInline();
			if (V.Contains(TEXT("High"), ESearchCase::IgnoreCase))
				Out.Tier = EWYAContractTier::HighTable;
			else if (V.Contains(TEXT("Prior"), ESearchCase::IgnoreCase))
				Out.Tier = EWYAContractTier::Priority;
			else
				Out.Tier = EWYAContractTier::Standard;
		}
	}

	// Derive phase weight from tier — can be overridden by a future PHASE_WEIGHT: field
	Out.PhaseWeight = FWYAContract::DerivePhaseWeight(Out.Tier);

	// Valid if we got at minimum a name and some intel
	return !Out.TargetName.IsEmpty() && !Out.Intel.IsEmpty();
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
