#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Location/WYAGeoTypes.h"
#include "AWYAArraySensor.generated.h"

class USphereComponent;

/**
 * The pre-Cascade sensor array — placed at its real-world GPS location.
 *
 * When a player enters the proximity sphere, fires BP_OnPlayerEnterProximity
 * if their fix-him comms stage is complete (Stage 5 / Comms repaired).
 * Before that, the array is silent.
 *
 * In the main game this actor is placed near the Antarctic sensor array site.
 * In the PvP side mode it may be discovered by accident — that's intended.
 *
 * Blueprint subclass BP_ArraySensor implements:
 *   BP_OnPlayerEnterProximity — assistant recognition lines, ambient audio
 */
UCLASS()
class WHEREYOUARE_API AWYAArraySensor : public AActor
{
    GENERATED_BODY()

public:
    AWYAArraySensor();

    /**
     * GPS coordinate of this sensor array — informational only.
     * Position the actor in the editor; this is for documentation/tooling.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WYA|Array")
    FWYAGeoCoord SensorArrayCoord;

    /** Proximity radius in UU. Default 5000 = 50m. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WYA|Array")
    float ProximityRadius = 5000.f;

    /**
     * Fires when a player with Stage 4+ (Comms repaired) enters proximity.
     * Drive assistant recognition lines and ambient audio from Blueprint.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Array")
    void BP_OnPlayerEnterProximity(APlayerController* PC);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WYA|Array")
    TObjectPtr<USphereComponent> ProximityVolume;

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);
};
