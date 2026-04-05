#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWYAExfilPoint.generated.h"

class USphereComponent;

/**
 * Extraction point — spawns after the contract target is downed.
 * Player enters the trigger to complete the run and collect bonuses.
 *
 * WYAContractSubsystem spawns this 100–200m from the target death location.
 * The actor destroys itself once used or after ExfilWindowSeconds expires.
 *
 * Blueprint subclass BP_ExfilPoint sets the mesh/VFX and implements
 * BP_OnExfilCountdown(SecondsRemaining) for the HUD timer.
 */
UCLASS()
class WHEREYOUARE_API AWYAExfilPoint : public AActor
{
    GENERATED_BODY()

public:
    AWYAExfilPoint();

    /** Which contract this exfil belongs to. Set by WYAContractSubsystem on spawn. */
    UPROPERTY(BlueprintReadWrite, Category = "WYA|Contract")
    FString ContractID;

    /** Seconds the player has to reach this point before it expires. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WYA|Contract")
    float ExfilWindowSeconds = 90.f;

    /** Seconds remaining — exposed for HUD countdown. */
    UPROPERTY(BlueprintReadOnly, Category = "WYA|Contract")
    float SecondsRemaining = 0.f;

    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Contract")
    void BP_OnExfilWindowOpen(float WindowSeconds);

    /** Fired each second with remaining time. Drive HUD countdown from this. */
    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Contract")
    void BP_OnExfilCountdown(float Remaining);

    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Contract")
    void BP_OnExfilUsed();

    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|Contract")
    void BP_OnExfilExpired();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<USphereComponent> TriggerVolume;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);

    void OnCountdownTick();
    void OnWindowExpired();

    FTimerHandle CountdownHandle;
    FTimerHandle ExpireHandle;
    bool bUsed = false;
};
