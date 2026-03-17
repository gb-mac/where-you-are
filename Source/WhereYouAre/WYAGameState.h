#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Location/WYAGeoTypes.h"
#include "WYAGameState.generated.h"

UCLASS()
class WHEREYOUARE_API AWYAGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    /** GPS coordinate of UE5 world origin. Set once by GameMode after location resolves. */
    UPROPERTY(BlueprintReadOnly, Replicated)
    FWYAGeoCoord WorldOriginGeo;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
