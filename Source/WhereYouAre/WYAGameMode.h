#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Location/WYAGeoTypes.h"
#include "WYAGameMode.generated.h"

class UWYALocationSubsystem;
class UWYAQuestSubsystem;

UCLASS()
class WHEREYOUARE_API AWYAGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AWYAGameMode();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
    /** Called when the location subsystem resolves a coordinate. */
    void OnLocationResolved(FWYAGeoCoord Coord, bool bSuccess);

private:
    /** Players who joined before location resolved, waiting to be spawned. */
    TArray<APlayerController*> PendingPlayers;

    bool bLocationResolved = false;

    void SpawnPlayer(APlayerController* PC);

    /** Try to assign an opening side quest; retries once after a short delay if AI isn't ready. */
    void TryAssignOpeningSideQuest(APlayerController* PC);

    FTimerHandle SideQuestRetryHandle;
};
