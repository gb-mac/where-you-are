#include "WYAGameMode.h"
#include "WYAGameState.h"
#include "WYACharacter.h"
#include "WYAPlayerController.h"
#include "Location/WYALocationSubsystem.h"
#include "Quest/WYAQuestSubsystem.h"
#include "Engine/World.h"
#include "CesiumGeoreference.h"
#include "EngineUtils.h"

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

    // Set Cesium georeference origin so World Terrain + 3D Tiles centre on the player's location
    for (ACesiumGeoreference* GeoRef : TActorRange<ACesiumGeoreference>(GetWorld()))
    {
        GeoRef->SetOriginLongitudeLatitudeHeight(
            FVector(Coord.Longitude, Coord.Latitude, Coord.Altitude));
        break; // only one georeference expected in the level
    }

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
    RestartPlayer(PC);

    // Park pawn way above terrain immediately so it can't fall through
    // while Cesium collision meshes are still streaming in.
    if (APawn* Pawn = PC->GetPawn())
        Pawn->SetActorLocation(FVector(0.f, 0.f, 500000.f));

    TrySpawnOnTerrain(PC, 30); // polls every 0.5s, up to 15s

    // Quest/story fire once — independent of terrain polling
    if (UWYAQuestSubsystem* QuestSub = GetGameInstance()->GetSubsystem<UWYAQuestSubsystem>())
        QuestSub->AdvanceMainStory(PC);

    TryAssignOpeningSideQuest(PC);
}

void AWYAGameMode::TrySpawnOnTerrain(APlayerController* PC, int32 AttemptsLeft)
{
    if (!IsValid(PC)) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // Trace from 5km above origin straight down — hits when Cesium collision is ready
    const FVector TraceStart(0.f, 0.f, 500000.f);
    const FVector TraceEnd  (0.f, 0.f, -100000.f);
    FHitResult Hit;
    FCollisionQueryParams Params;
    if (APawn* Pawn = PC->GetPawn()) Params.AddIgnoredActor(Pawn);

    if (World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, Params))
    {
        if (APawn* Pawn = PC->GetPawn())
        {
            Pawn->SetActorLocation(Hit.ImpactPoint + FVector(0.f, 0.f, 200.f));
            UE_LOG(LogTemp, Log, TEXT("WYAGameMode: landed on terrain at Z=%.0f after %d attempt(s)"),
                Hit.ImpactPoint.Z, 31 - AttemptsLeft);
        }
        return;
    }

    if (AttemptsLeft <= 0)
    {
        if (APawn* Pawn = PC->GetPawn())
            Pawn->SetActorLocation(FVector(0.f, 0.f, 50000.f));
        UE_LOG(LogTemp, Warning, TEXT("WYAGameMode: terrain never loaded, spawning at fallback height"));
        return;
    }

    World->GetTimerManager().SetTimer(TerrainSpawnRetryHandle,
        FTimerDelegate::CreateUObject(this, &AWYAGameMode::TrySpawnOnTerrain, PC, AttemptsLeft - 1),
        0.5f, false);
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
