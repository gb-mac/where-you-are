#include "Actors/AWYAArraySensor.h"
#include "Quest/WYAFixHimQuestSubsystem.h"
#include "Components/SphereComponent.h"
#include "WYACharacter.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AWYAArraySensor::AWYAArraySensor()
{
    PrimaryActorTick.bCanEverTick = false;

    ProximityVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityVolume"));
    ProximityVolume->InitSphereRadius(ProximityRadius);
    ProximityVolume->SetCollisionProfileName(TEXT("Trigger"));
    SetRootComponent(ProximityVolume);
}

void AWYAArraySensor::BeginPlay()
{
    Super::BeginPlay();

    // Update sphere radius in case the designer changed it in the editor
    ProximityVolume->SetSphereRadius(ProximityRadius);

    ProximityVolume->OnComponentBeginOverlap.AddDynamic(this, &AWYAArraySensor::OnOverlapBegin);
}

void AWYAArraySensor::OnOverlapBegin(UPrimitiveComponent*, AActor* OtherActor,
                                      UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    AWYACharacter* PlayerChar = Cast<AWYACharacter>(OtherActor);
    if (!PlayerChar) return;

    APlayerController* PC = Cast<APlayerController>(PlayerChar->GetController());
    if (!PC) return;

    // Only fires once the fix-him comms are repaired (Stage 5 — assistant is fully online)
    UGameInstance* GI = GetGameInstance();
    if (!GI) return;

    UWYAFixHimQuestSubsystem* FixHim = GI->GetSubsystem<UWYAFixHimQuestSubsystem>();
    if (!FixHim || !FixHim->IsCommsRepaired()) return;

    UE_LOG(LogTemp, Log, TEXT("AWYAArraySensor: Stage 4 player entered array proximity"));

    BP_OnPlayerEnterProximity(PC);
}
