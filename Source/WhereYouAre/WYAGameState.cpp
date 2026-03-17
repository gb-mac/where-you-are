#include "WYAGameState.h"
#include "Net/UnrealNetwork.h"

void AWYAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AWYAGameState, WorldOriginGeo);
}
