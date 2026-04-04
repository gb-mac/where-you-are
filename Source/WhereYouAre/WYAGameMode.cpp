#include "WYAGameMode.h"
#include "WYAGameState.h"
#include "WYACharacter.h"
#include "WYAPlayerController.h"
#include "Characters/WYAOpponentCharacter.h"
#include "Loot/AWYALootActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Location/WYALocationSubsystem.h"
#include "Quest/WYAQuestSubsystem.h"
#include "Save/WYASaveSubsystem.h"
#include "Save/WYASaveGame.h"
#include "Inventory/WYAInventoryComponent.h"
#include "Survival/WYASurvivalComponent.h"
#include "Economy/WYACurrencySubsystem.h"
#include "Engine/World.h"
#include "CesiumGeoreference.h"
#include "EngineUtils.h"
#include "WorldPartition/HLOD/HLODActor.h"

AWYAGameMode::AWYAGameMode()
{
    DefaultPawnClass      = AWYACharacter::StaticClass();
    PlayerControllerClass = AWYAPlayerController::StaticClass();
    GameStateClass        = AWYAGameState::StaticClass();
}

void AWYAGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    // Hide World Partition HLOD proxy actors — they're baked from the old Landscape
    // and show as white vertical planes over the Cesium terrain.
    for (AWorldPartitionHLOD* HLOD : TActorRange<AWorldPartitionHLOD>(GetWorld()))
    {
        HLOD->SetActorHiddenInGame(true);
    }

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

    // Restore saved inventory onto the new pawn
    if (UWYASaveSubsystem* SaveSub = GetGameInstance()->GetSubsystem<UWYASaveSubsystem>())
    {
        if (SaveSub->HasSaveData())
        {
            if (AWYACharacter* Char = PC ? Cast<AWYACharacter>(PC->GetPawn()) : nullptr)
            {
                const UWYASaveGame* Save = SaveSub->GetSaveGame();

                if (Char->Inventory)
                {
                    for (const FWYAInventoryItem& SavedItem : Save->SavedInventory)
                    {
                        Char->Inventory->AddItem(SavedItem.Type, SavedItem.Quantity);
                    }
                }

                if (Char->Survival)
                {
                    Char->Survival->ApplySaveData(Save->SavedWater, Save->SavedFood);
                }
            }

            // Currency — subsystem-level, not pawn-level
            if (UWYACurrencySubsystem* Currency = GetGameInstance()->GetSubsystem<UWYACurrencySubsystem>())
            {
                const UWYASaveGame* Save = SaveSub->GetSaveGame();
                Currency->ApplySaveData(Save->SavedGold, Save->SavedSilver);
            }
        }
    }

    // Freeze movement so the pawn can't fall through terrain while Cesium
    // collision meshes are still streaming in.
    if (ACharacter* Char = PC ? Cast<ACharacter>(PC->GetPawn()) : nullptr)
    {
        Char->GetCharacterMovement()->SetMovementMode(MOVE_None);
        Char->SetActorLocation(FVector(0.f, 0.f, 500000.f), false, nullptr, ETeleportType::TeleportPhysics);
    }

    TrySpawnOnTerrain(PC, 180); // polls every 0.5s, up to 90s — Cesium tiles can take 40+ seconds

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

    auto UnfreezeAndPlace = [](APlayerController* InPC, FVector Pos)
    {
        if (ACharacter* Char = InPC ? Cast<ACharacter>(InPC->GetPawn()) : nullptr)
        {
            Char->SetActorLocation(Pos, false, nullptr, ETeleportType::TeleportPhysics);
            Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        }
    };

    if (World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, Params))
    {
        const FVector LandPos = Hit.ImpactPoint + FVector(0.f, 0.f, 200.f);
        UnfreezeAndPlace(PC, LandPos);
        UE_LOG(LogTemp, Log, TEXT("WYAGameMode: landed on terrain at Z=%.0f after %d attempt(s)"),
            Hit.ImpactPoint.Z, 181 - AttemptsLeft);

        // Terrain is now confirmed loaded — spawn opponents and debug loot on-surface
        SpawnOpponents(Hit.ImpactPoint.Z);
        SpawnDebugLoot(Hit.ImpactPoint.Z);
        return;
    }

    if (AttemptsLeft <= 0)
    {
        UnfreezeAndPlace(PC, FVector(0.f, 0.f, 50000.f));
        UE_LOG(LogTemp, Warning, TEXT("WYAGameMode: terrain never loaded, spawning at fallback height"));
        return;
    }

    World->GetTimerManager().SetTimer(TerrainSpawnRetryHandle,
        FTimerDelegate::CreateUObject(this, &AWYAGameMode::TrySpawnOnTerrain, PC, AttemptsLeft - 1),
        0.5f, false);
}

void AWYAGameMode::SpawnOpponents(float TerrainZ)
{
    UWorld* World = GetWorld();
    if (!World) return;

    // 3 scavengers scattered 8–20m from the player spawn (world origin)
    const int32 Count   = 1;
    const float MinDist = 800.f;
    const float MaxDist = 2000.f;

    for (int32 i = 0; i < Count; ++i)
    {
        const float Angle = FMath::RandRange(0.f, 360.f);
        const float Dist  = FMath::RandRange(MinDist, MaxDist);

        FVector SpawnPos(
            FMath::Cos(FMath::DegreesToRadians(Angle)) * Dist,
            FMath::Sin(FMath::DegreesToRadians(Angle)) * Dist,
            TerrainZ + 200.f); // place just above confirmed terrain height

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        World->SpawnActor<AWYAOpponentCharacter>(
            AWYAOpponentCharacter::StaticClass(), SpawnPos, FRotator::ZeroRotator, Params);

        UE_LOG(LogTemp, Log, TEXT("WYAGameMode: spawned opponent %d at (%.0f, %.0f, %.0f)"),
            i + 1, SpawnPos.X, SpawnPos.Y, SpawnPos.Z);
    }
}

void AWYAGameMode::SpawnDebugLoot(float TerrainZ)
{
    UWorld* World = GetWorld();
    if (!World) return;

    struct FLootEntry
    {
        EWYACarriedItemType Type;
        int32               Quantity;
    };

    const FLootEntry LootTable[] = {
        { EWYACarriedItemType::FixHim_MobilityParts, 1 },
        { EWYACarriedItemType::Food,                 3 },
        { EWYACarriedItemType::Scrap,                2 },
    };

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (const FLootEntry& Entry : LootTable)
    {
        const float Angle = FMath::RandRange(0.f, 360.f);
        const float Dist  = FMath::RandRange(500.f, 1500.f);

        FVector SpawnPos(
            FMath::Cos(FMath::DegreesToRadians(Angle)) * Dist,
            FMath::Sin(FMath::DegreesToRadians(Angle)) * Dist,
            TerrainZ + 100.f);

        AWYALootActor* Loot = World->SpawnActor<AWYALootActor>(
            AWYALootActor::StaticClass(), SpawnPos, FRotator::ZeroRotator, Params);

        if (Loot)
        {
            Loot->ItemType = Entry.Type;
            Loot->Quantity = Entry.Quantity;

            UE_LOG(LogTemp, Log, TEXT("WYAGameMode: spawned debug loot '%s' x%d at (%.0f, %.0f, %.0f)"),
                *AWYALootActor::GetTypeDisplayName(Entry.Type).ToString(),
                Entry.Quantity, SpawnPos.X, SpawnPos.Y, SpawnPos.Z);
        }
    }
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
