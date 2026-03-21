#include "AI/WYAAssistantSubsystem.h"
#include "AI/WYAAISubsystem.h"

static constexpr int32 NumStages = 5;

void UWYAAssistantSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UWYAAISubsystem>();
	Super::Initialize(Collection);

	AISub = GetGameInstance()->GetSubsystem<UWYAAISubsystem>();

	UE_LOG(LogTemp, Log, TEXT("WYAAssistantSubsystem: initialized at Stage 1"));
}

// ---------------------------------------------------------------------------
// Stage
// ---------------------------------------------------------------------------

void UWYAAssistantSubsystem::SetStage(EWYAAssistantStage NewStage)
{
	if (NewStage != CurrentStage)
	{
		CurrentStage = NewStage;
		UE_LOG(LogTemp, Log, TEXT("WYAAssistantSubsystem: stage → %d"),
		       static_cast<int32>(NewStage));
	}
}

// ---------------------------------------------------------------------------
// Override registry
// ---------------------------------------------------------------------------

void UWYAAssistantSubsystem::RegisterOverride(
	const FString& TriggerTag,
	EWYAAssistantStage Stage,
	const FString& ScriptedLine)
{
	TArray<FString>& Lines = Overrides.FindOrAdd(TriggerTag);
	if (Lines.Num() < NumStages)
	{
		Lines.SetNum(NumStages);
	}
	Lines[static_cast<int32>(Stage)] = ScriptedLine;
}

bool UWYAAssistantSubsystem::TryGetOverride(const FString& TriggerTag, FString& OutLine) const
{
	const TArray<FString>* Lines = Overrides.Find(TriggerTag);
	if (!Lines) return false;

	const int32 StageIdx = static_cast<int32>(CurrentStage);
	if (!Lines->IsValidIndex(StageIdx)) return false;

	const FString& Line = (*Lines)[StageIdx];
	if (Line.IsEmpty()) return false;

	OutLine = Line;
	return true;
}

// ---------------------------------------------------------------------------
// Line request
// ---------------------------------------------------------------------------

void UWYAAssistantSubsystem::RequestLine(
	const FString& TriggerTag,
	const FString& ContextDescription,
	const FOnAssistantLine& OnComplete)
{
	// Scripted override takes absolute priority
	FString ScriptedLine;
	if (TryGetOverride(TriggerTag, ScriptedLine))
	{
		UE_LOG(LogTemp, Log, TEXT("WYAAssistantSubsystem: scripted '%s' stage %d"),
		       *TriggerTag, static_cast<int32>(CurrentStage));
		OnComplete.ExecuteIfBound(true, ScriptedLine);
		return;
	}

	// Fall through to LLM
	if (!AISub || !AISub->IsAvailable())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("WYAAssistantSubsystem: no override for '%s' and ollama unavailable"),
		       *TriggerTag);
		OnComplete.ExecuteIfBound(false, FString());
		return;
	}

	AISub->GenerateAssistantLine(CurrentStage, ContextDescription,
		[OnComplete](bool bSuccess, const FString& Line)
		{
			OnComplete.ExecuteIfBound(bSuccess, Line);
		});
}
