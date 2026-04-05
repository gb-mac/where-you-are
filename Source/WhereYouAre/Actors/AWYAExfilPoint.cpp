#include "Actors/AWYAExfilPoint.h"
#include "Contracts/WYAContractSubsystem.h"
#include "WYACharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AWYAExfilPoint::AWYAExfilPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerVolume"));
    TriggerVolume->InitSphereRadius(300.f); // 3m — generous, player just needs to be close
    TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
    SetRootComponent(TriggerVolume);
}

void AWYAExfilPoint::BeginPlay()
{
    Super::BeginPlay();

    SecondsRemaining = ExfilWindowSeconds;

    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AWYAExfilPoint::OnOverlapBegin);

    // Tick countdown every second
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(CountdownHandle,
            this, &AWYAExfilPoint::OnCountdownTick,
            1.f, /*bLooping=*/true);

        World->GetTimerManager().SetTimer(ExpireHandle,
            this, &AWYAExfilPoint::OnWindowExpired,
            ExfilWindowSeconds, false);
    }

    BP_OnExfilWindowOpen(ExfilWindowSeconds);
}

void AWYAExfilPoint::OnOverlapBegin(UPrimitiveComponent*, AActor* OtherActor,
                                     UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    if (bUsed) return;

    AWYACharacter* PlayerChar = Cast<AWYACharacter>(OtherActor);
    if (!PlayerChar) return;

    APlayerController* PC = Cast<APlayerController>(PlayerChar->GetController());
    if (!PC) return;

    bUsed = true;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(CountdownHandle);
        World->GetTimerManager().ClearTimer(ExpireHandle);
    }

    // Notify ContractSubsystem — marks exfil reached, triggers bonus calculation
    if (UWYAContractSubsystem* ContractSub = GetGameInstance()->GetSubsystem<UWYAContractSubsystem>())
    {
        ContractSub->ReachExfil(PC, ContractID);
    }

    BP_OnExfilUsed();
    SetLifeSpan(2.f); // brief pause then destroy
}

void AWYAExfilPoint::OnCountdownTick()
{
    SecondsRemaining = FMath::Max(0.f, SecondsRemaining - 1.f);
    BP_OnExfilCountdown(SecondsRemaining);
}

void AWYAExfilPoint::OnWindowExpired()
{
    if (bUsed) return;

    if (UWorld* World = GetWorld())
        World->GetTimerManager().ClearTimer(CountdownHandle);

    BP_OnExfilExpired();

    UE_LOG(LogTemp, Log, TEXT("AWYAExfilPoint: exfil window expired for contract %s"), *ContractID);
    Destroy();
}
