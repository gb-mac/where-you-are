#include "AI/WYACodecSubsystem.h"
#include "GameFramework/PlayerController.h"

void UWYACodecSubsystem::RegisterCall(const FWYACodecCall& Call)
{
	if (Call.CallTag.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("WYACodecSubsystem: RegisterCall called with empty CallTag — ignored"));
		return;
	}
	Calls.Add(Call.CallTag, Call);
}

void UWYACodecSubsystem::TriggerCodec(const FString& EventTag, APlayerController* PC)
{
	if (bCallActive)
	{
		UE_LOG(LogTemp, Log, TEXT("WYACodecSubsystem: TriggerCodec('%s') ignored — call already active"),
		       *EventTag);
		return;
	}

	const FWYACodecCall* Call = Calls.Find(EventTag);
	if (!Call)
	{
		UE_LOG(LogTemp, Log, TEXT("WYACodecSubsystem: no call registered for '%s'"), *EventTag);
		return;
	}

	if (!Call->bRepeatable && PlayedCalls.Contains(EventTag))
	{
		UE_LOG(LogTemp, Log, TEXT("WYACodecSubsystem: '%s' already played and not repeatable"), *EventTag);
		return;
	}

	BeginCall(*Call);
}

void UWYACodecSubsystem::AdvanceLine()
{
	if (!bCallActive) return;

	++ActiveLineIdx;

	if (ActiveLineIdx < ActiveCall.Lines.Num())
	{
		// More lines — UI will display ActiveCall.Lines[ActiveLineIdx]
		// (UI polls or subscribes; we don't push individual lines here to keep
		// the subsystem decoupled from the display implementation)
		return;
	}

	// All lines delivered — present branches or end the call
	if (ActiveCall.Branches.IsEmpty())
	{
		EndCall();
	}
	// else: UI reads ActiveCall.Branches and calls SelectBranch()
}

void UWYACodecSubsystem::SelectBranch(int32 BranchIndex)
{
	if (!bCallActive) return;
	if (!ActiveCall.Branches.IsValidIndex(BranchIndex)) return;

	const FWYACodecBranch& Branch = ActiveCall.Branches[BranchIndex];

	if (Branch.NextCallTag.IsEmpty())
	{
		EndCall();
		return;
	}

	// Chain into the next call
	const FWYACodecCall* Next = Calls.Find(Branch.NextCallTag);
	if (!Next)
	{
		UE_LOG(LogTemp, Warning, TEXT("WYACodecSubsystem: branch NextCallTag '%s' not found"),
		       *Branch.NextCallTag);
		EndCall();
		return;
	}

	EndCall();
	BeginCall(*Next);
}

void UWYACodecSubsystem::BeginCall(const FWYACodecCall& Call)
{
	ActiveCall    = Call;
	ActiveLineIdx = 0;
	bCallActive   = true;

	PlayedCalls.Add(Call.CallTag);

	UE_LOG(LogTemp, Log, TEXT("WYACodecSubsystem: beginning call '%s' (%d lines)"),
	       *Call.CallTag, Call.Lines.Num());

	OnCodecCallStarted.Broadcast(Call.CallTag, Call);
}

void UWYACodecSubsystem::EndCall()
{
	const FString Tag = ActiveCall.CallTag;
	bCallActive   = false;
	ActiveLineIdx = 0;
	ActiveCall    = FWYACodecCall{};

	UE_LOG(LogTemp, Log, TEXT("WYACodecSubsystem: call '%s' ended"), *Tag);
	OnCodecCallEnded.Broadcast(Tag);
}
