#pragma once

#include "GameFramework/SaveGame.h"
#include "WYALocationSave.generated.h"

UCLASS()
class UWYALocationSave : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY() double Latitude  = 0.0;
    UPROPERTY() double Longitude = 0.0;
    UPROPERTY() double Altitude  = 0.0;
    UPROPERTY() FDateTime Timestamp;
};
