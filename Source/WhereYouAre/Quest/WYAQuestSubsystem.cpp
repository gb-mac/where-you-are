#include "Quest/WYAQuestSubsystem.h"
#include "AI/WYAAISubsystem.h"
#include "GameFramework/PlayerController.h"

void UWYAQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UWYAAISubsystem>();
	Super::Initialize(Collection);

	AISub = GetGameInstance()->GetSubsystem<UWYAAISubsystem>();
}

bool UWYAQuestSubsystem::TryAssignSideQuest(APlayerController* PC)
{
	if (!AISub) return false;

	FWYAPregeneratedQuest PreGen;
	if (!AISub->TryPopPregeneratedQuest(PreGen)) return false;

	FWYAQuest Quest;
	Quest.ID    = FString::Printf(TEXT("side_%lld"), FDateTime::UtcNow().GetTicks());
	Quest.Type  = EWYAQuestType::SideQuest;
	Quest.Title = PreGen.Title;
	Quest.Body  = PreGen.Text;

	UE_LOG(LogTemp, Log, TEXT("WYAQuestSubsystem: assigned side quest to %s"),
	       *GetNameSafe(PC));

	OnQuestAssigned.Broadcast(PC, Quest);
	return true;
}

void UWYAQuestSubsystem::AdvanceMainStory(APlayerController* PC)
{
	// STUB — narrative agent: populate Content/Quests/DT_MainStory.uasset
	// and replace this body with a DataTable lookup on MainStoryProgress[PC].
	UE_LOG(LogTemp, Warning,
	       TEXT("WYAQuestSubsystem: AdvanceMainStory called but main quest data not yet authored. "
	            "Narrative agent owns Content/Quests/DT_MainStory.uasset."));
}
