#include "Quest/WYAFixHimQuestSubsystem.h"
#include "AI/WYAAssistantSubsystem.h"

void UWYAFixHimQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UWYAAssistantSubsystem>();
	Super::Initialize(Collection);

	AssistantSub = GetGameInstance()->GetSubsystem<UWYAAssistantSubsystem>();

	// Explicit Stage 1 init on new game. LoadRepairState() will override this for
	// saved games before the player spawns.
	if (AssistantSub)
	{
		AssistantSub->SetStage(EWYAAssistantStage::Stage1_Yoda);
	}

	UE_LOG(LogTemp, Log, TEXT("WYAFixHimQuestSubsystem: initialized — assistant at Stage 1"));
}

// ---------------------------------------------------------------------------
// Questline activation
// ---------------------------------------------------------------------------

void UWYAFixHimQuestSubsystem::TriggerQuestline()
{
	if (bQuestlineActive)
	{
		return; // already triggered
	}

	if (bMobilityRepaired)
	{
		// Stage 1 repair already done — questline is implicitly active
		return;
	}

	bQuestlineActive = true;

	// The "Fix." conversation fires here. The assistant delivers:
	//   "Fix." — pause — "Parts." — pause — "Get."
	// Routed through the scripted override at tag "fixhim_quest_trigger" when the calling
	// system requests a line. Register the override in DT_AssistantOverrides at Stage 1.
	// See docs/narrative/dialogue/assistant-register-overrides.md.

	UE_LOG(LogTemp, Log, TEXT("WYAFixHimQuestSubsystem: fix-him questline activated"));
}

// ---------------------------------------------------------------------------
// Repair completions
// ---------------------------------------------------------------------------

void UWYAFixHimQuestSubsystem::OnMobilityRepairCompleted()
{
	if (bMobilityRepaired) return;
	bMobilityRepaired  = true;
	bQuestlineActive   = true; // questline is live once any repair is done

	AdvanceStage(EWYAAssistantStage::Stage2_Short, TEXT("Mobility"));
}

void UWYAFixHimQuestSubsystem::OnProcessingRepairCompleted()
{
	if (bProcessingRepaired) return;
	if (!bMobilityRepaired)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("WYAFixHimQuestSubsystem: Processing repair completed before Mobility — "
			     "advancing anyway but check quest ordering"));
	}
	bProcessingRepaired = true;

	AdvanceStage(EWYAAssistantStage::Stage3_Voice, TEXT("Processing"));
}

void UWYAFixHimQuestSubsystem::OnPowerRepairCompleted()
{
	if (bPowerRepaired) return;
	if (!bProcessingRepaired)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("WYAFixHimQuestSubsystem: Power repair completed before Processing — "
			     "advancing anyway but check quest ordering"));
	}
	bPowerRepaired = true;

	AdvanceStage(EWYAAssistantStage::Stage4_Operational, TEXT("Power"));
}

void UWYAFixHimQuestSubsystem::OnCommunicationRepairCompleted()
{
	if (bCommsRepaired) return;
	if (!bPowerRepaired)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("WYAFixHimQuestSubsystem: Communication repair completed before Power — "
			     "advancing anyway but check quest ordering"));
	}
	bCommsRepaired = true;

	AdvanceStage(EWYAAssistantStage::Stage5_Restored, TEXT("Communication"));

	// Stage 5 fires the Vael warning. Routed through scripted override "stage5_warning".
	// The audio layer stops completely for this moment — no playlist underneath.
	// See docs/narrative/assistant-stage-triggers.md and dt-assistant-overrides.md.
	UE_LOG(LogTemp, Log,
		TEXT("WYAFixHimQuestSubsystem: Stage 5 entered — Vael warning delivery. "
		     "Audio layer should stop. Override tag: 'stage5_warning'"));
}

// ---------------------------------------------------------------------------
// Save/load
// ---------------------------------------------------------------------------

void UWYAFixHimQuestSubsystem::LoadRepairState(
	bool bMobility, bool bProcessing, bool bPower, bool bComms)
{
	bMobilityRepaired   = bMobility;
	bProcessingRepaired = bProcessing;
	bPowerRepaired      = bPower;
	bCommsRepaired      = bComms;

	if (bMobility || bProcessing || bPower || bComms)
	{
		bQuestlineActive = true;
	}

	// Derive correct stage from repair state and set it silently (no dialogue).
	EWYAAssistantStage LoadedStage = EWYAAssistantStage::Stage1_Yoda;
	if      (bComms)      LoadedStage = EWYAAssistantStage::Stage5_Restored;
	else if (bPower)      LoadedStage = EWYAAssistantStage::Stage4_Operational;
	else if (bProcessing) LoadedStage = EWYAAssistantStage::Stage3_Voice;
	else if (bMobility)   LoadedStage = EWYAAssistantStage::Stage2_Short;

	if (AssistantSub)
	{
		AssistantSub->SetStage(LoadedStage);
	}

	UE_LOG(LogTemp, Log,
		TEXT("WYAFixHimQuestSubsystem: loaded repair state — mobility=%d processing=%d power=%d comms=%d → Stage %d"),
		bMobility, bProcessing, bPower, bComms, static_cast<int32>(LoadedStage));
}

// ---------------------------------------------------------------------------
// Internal
// ---------------------------------------------------------------------------

void UWYAFixHimQuestSubsystem::AdvanceStage(EWYAAssistantStage NewStage, const TCHAR* RepairName)
{
	if (!AssistantSub)
	{
		UE_LOG(LogTemp, Error,
			TEXT("WYAFixHimQuestSubsystem: AssistantSubsystem not found — cannot advance stage"));
		return;
	}

	AssistantSub->SetStage(NewStage);
	OnRepairStageChanged.Broadcast(NewStage);

	UE_LOG(LogTemp, Log,
		TEXT("WYAFixHimQuestSubsystem: %s repair complete → Stage %d"),
		RepairName, static_cast<int32>(NewStage));
}
