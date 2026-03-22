#include "Vehicles/WYAVehicleSpawner.h"
#include "Engine/World.h"

AWYAVehicleSpawner::AWYAVehicleSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AWYAVehicleSpawner::SpawnVehiclesAt(const FVector& Origin, int32 Count)
{
    if (VehicleClasses.IsEmpty()) return;

    UWorld* World = GetWorld();
    if (!World) return;

    const float AngleStep = 360.f / Count;

    for (int32 i = 0; i < Count; ++i)
    {
        // Even ring around origin
        const float Angle = FMath::DegreesToRadians(AngleStep * i);
        const FVector RingPos = Origin
            + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * SpawnRadius;

        // Trace down to terrain surface
        const FVector TraceStart = RingPos + FVector(0.f, 0.f, TraceStartHeight);
        const FVector TraceEnd   = RingPos - FVector(0.f, 0.f, TraceStartHeight);

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

        FVector SpawnPos = RingPos + FVector(0.f, 0.f, 50.f); // fallback if no hit
        if (World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, Params))
        {
            SpawnPos = Hit.ImpactPoint + FVector(0.f, 0.f, 50.f);
        }

        // Random vehicle class from the assigned array
        const int32 ClassIdx = FMath::RandRange(0, VehicleClasses.Num() - 1);
        TSubclassOf<AWYAVehicleBase> ChosenClass = VehicleClasses[ClassIdx];
        if (!ChosenClass) continue;

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // Face a random direction
        const FRotator SpawnRot(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

        World->SpawnActor<AWYAVehicleBase>(ChosenClass, SpawnPos, SpawnRot, SpawnParams);

        UE_LOG(LogTemp, Log, TEXT("WYAVehicleSpawner: spawned vehicle %d at (%.0f, %.0f, %.0f)"),
            i, SpawnPos.X, SpawnPos.Y, SpawnPos.Z);
    }
}
