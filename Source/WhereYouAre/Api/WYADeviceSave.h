#pragma once

#include "GameFramework/SaveGame.h"
#include "WYADeviceSave.generated.h"

/** Persists device-local identity across sessions. */
UCLASS()
class UWYADeviceSave : public USaveGame
{
	GENERATED_BODY()
public:
	/** Stable device UUID used as API credentials. */
	UPROPERTY() FString DeviceUUID;

	/** JWT from the most recent successful auth. */
	UPROPERTY() FString JWT;

	/** Account ID returned by register/login. */
	UPROPERTY() FString AccountId;
};
