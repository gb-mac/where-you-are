#include "WYAGameMode.h"
#include "WYAGameState.h"
#include "WYACharacter.h"
#include "WYAPlayerController.h"
#include "Location/WYALocationSubsystem.h"
#include "Quest/WYAQuestSubsystem.h"
#include "Engine/World.h"

AWYAGameMode::AWYAGameMode()
{
    DefaultPawnClass      = AWYACharacter::StaticClass();
    PlayerControllerClass = AWYAPlayerController::StaticClass();
    GameStateClass        = AWYAGameState::StaticClass();
}

void AWYAGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    UWYALocationSubsystem* LocationSub = GetGameInstance()->GetSubsystem<UWYALocationSubsystem>();
    if (!LocationSub) return;

    LocationSub->OnLocationResolved.AddUObject(this, &AWYAGameMode::OnLocationResolved);
    LocationSub->RequestLocation();
}

void AWYAGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (bLocationResolved)
    {
        SpawnPlayer(NewPlayer);
    }
    else
    {
        PendingPlayers.Add(NewPlayer);
    }
}

void AWYAGameMode::OnLocationResolved(FWYAGeoCoord Coord, bool bSuccess)
{
    bLocationResolved = true;

    // Store origin in GameState for clients to access
    if (AWYAGameState* GS = GetGameState<AWYAGameState>())
    {
        GS->WorldOriginGeo = Coord;
    }

    UE_LOG(LogTemp, Log, TEXT("WYAGameMode: World origin set to (%.4f, %.4f) via %s"),
        Coord.Latitude, Coord.Longitude,
        bSuccess ? TEXT("provider") : TEXT("fallback"));

    // Spawn any players who connected before location resolved
    for (APlayerController* PC : PendingPlayers)
    {
        if (IsValid(PC))
        {
            SpawnPlayer(PC);
        }
    }
    PendingPlayers.Empty();
}

void AWYAGameMode::SpawnPlayer(APlayerController* PC)
{
    // World origin = FVector::ZeroVector, which maps to the player's real-world start location
    RestartPlayer(PC);

    // Kick off the main story (no-ops until narrative agent authors DT_MainStory)
    if (UWYAQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UWYAQuestSubsystem>())
    {
        QuestSub->AdvanceMainStory(PC);
    }

    TryAssignOpeningSideQuest(PC);
}

void AWYAGameMode::TryAssignOpeningSideQuest(APlayerController* PC)
{
    UWYAQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UWYAQuestSubsystem>();
    if (!QuestSub) return;

    if (QuestSub->TryAssignSideQuest(PC)) return;

    // AI still generating — retry once after 5s (11.6 t/s on Beelink, ~25s total)
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(SideQuestRetryHandle,
            FTimerDelegate::CreateUObject(this, &AWYAGameMode::TryAssignOpeningSideQuest, PC),
            5.0f, /*bLoop=*/false);
    }
}
