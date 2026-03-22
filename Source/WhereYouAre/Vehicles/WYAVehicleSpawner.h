#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vehicles/WYAVehicleBase.h"
#include "WYAVehicleSpawner.generated.h"

/**
 * Spawns driveable vehicles in a ring around the player's spawn point.
 * Called once from WYAGameMode after terrain landing is confirmed.
 *
 * Blueprint subclass: assign VehicleClasses array with your BP_Vehicle_* assets.
 */
UCLASS()
class WHEREYOUARE_API AWYAVehicleSpawner : public AActor
{
    GENERATED_BODY()

public:
    AWYAVehicleSpawner();

    /**
     * Spawn Count vehicles in a ring at SpawnRadius around Origin.
     * Each vehicle is line-traced to the terrain surface before placement.
     */
    void SpawnVehiclesAt(const FVector& Origin, int32 Count = 4);

protected:
    /** Blueprint vehicle subclasses to pick from randomly. Set in BP_VehicleSpawner. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicles")
    TArray<TSubclassOf<AWYAVehicleBase>> VehicleClasses;

    /** Ring radius around origin where vehicles appear (UU — 2000 = 20m). */
    UPROPERTY(EditDefaultsOnly, Category = "Vehicles")
    float SpawnRadius = 2000.f;

    /** How far above the trace start point to begin the downward line trace. */
    UPROPERTY(EditDefaultsOnly, Category = "Vehicles")
    float TraceStartHeight = 5000.f;
};
