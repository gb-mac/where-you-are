#include "Characters/WYAAssistantCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AWYAAssistantCharacter::AWYAAssistantCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    IrisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IrisMesh"));
    IrisMesh->SetupAttachment(GetMesh());
    IrisMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    EyeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EyeTimeline"));
}

void AWYAAssistantCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Create the MID and apply it to the iris mesh
    if (EyeMaterial)
    {
        MID_Eye = UMaterialInstanceDynamic::Create(EyeMaterial, this);
        IrisMesh->SetMaterial(0, MID_Eye);
    }

    // Wire the timeline curve
    if (EyeTransitionCurve && EyeTimeline)
    {
        FOnTimelineFloat UpdateCallback;
        UpdateCallback.BindUFunction(this, FName("OnEyeTimelineUpdate"));
        EyeTimeline->AddInterpFloat(EyeTransitionCurve, UpdateCallback);

        FOnTimelineEvent FinishedCallback;
        FinishedCallback.BindUFunction(this, FName("OnEyeTimelineFinished"));
        EyeTimeline->SetTimelineFinishedFunc(FinishedCallback);

        EyeTimeline->SetLooping(false);
        EyeTimeline->SetTimelineLength(0.4f);
    }

    // Start in red state
    SetEyeParametersDirect(EyeColor_Red, EmissiveMultiplier_Red);
    if (MID_Eye) MID_Eye->SetScalarParameterValue(TEXT("RingOpacity"), 1.0f);
}

void AWYAAssistantCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// ── Public ────────────────────────────────────────────────────────────────────

void AWYAAssistantCharacter::SetEyeState(EWYAEyeState NewState, bool bInstant)
{
    CurrentEyeState = NewState;

    const FLinearColor& TargetColor = (NewState == EWYAEyeState::Green) ? EyeColor_Green : EyeColor_Red;
    const float TargetMultiplier    = (NewState == EWYAEyeState::Green) ? EmissiveMultiplier_Green : EmissiveMultiplier_Red;

    if (bInstant)
    {
        // Last-second red flip — no timeline, set directly
        if (EyeTimeline) EyeTimeline->Stop();
        SetEyeParametersDirect(TargetColor, TargetMultiplier);
        return;
    }

    // Cross-fade — capture current MID values as lerp source
    if (MID_Eye)
    {
        MID_Eye->GetVectorParameterValue(FMaterialParameterInfo(TEXT("EyeColor_Linear")),
            *(FLinearColor*)&EyeColor_From);
        float CurrentMult = 4.0f;
        MID_Eye->GetScalarParameterValue(FMaterialParameterInfo(TEXT("EmissiveMultiplier")), CurrentMult);
        Multiplier_From = CurrentMult;
    }

    EyeColor_To    = TargetColor;
    Multiplier_To  = TargetMultiplier;

    if (EyeTimeline)
    {
        EyeTimeline->Stop();
        EyeTimeline->PlayFromStart();
    }
}

// ── Timeline callbacks ────────────────────────────────────────────────────────

void AWYAAssistantCharacter::OnEyeTimelineUpdate(float Alpha)
{
    if (!MID_Eye) return;

    const FLinearColor BlendedColor = FLinearColor::LerpUsingHSV(EyeColor_From, EyeColor_To, Alpha);
    const float BlendedMultiplier   = FMath::Lerp(Multiplier_From, Multiplier_To, Alpha);

    SetEyeParametersDirect(BlendedColor, BlendedMultiplier);
}

void AWYAAssistantCharacter::OnEyeTimelineFinished()
{
    // Snap to exact target to prevent float drift
    SetEyeParametersDirect(EyeColor_To, Multiplier_To);
}

// ── Helpers ───────────────────────────────────────────────────────────────────

void AWYAAssistantCharacter::SetEyeParametersDirect(const FLinearColor& Color, float Multiplier)
{
    if (!MID_Eye) return;
    MID_Eye->SetVectorParameterValue(TEXT("EyeColor_Linear"), Color);
    MID_Eye->SetScalarParameterValue(TEXT("EmissiveMultiplier"), Multiplier);
}

void AWYAAssistantCharacter::OnLODChanged(int32 NewLODLevel)
{
    if (!MID_Eye) return;
    // Ring must never disappear — FSR anchor. Soften at LOD3+ but never zero.
    const float RingOpacity = (NewLODLevel >= 3) ? 0.5f : 1.0f;
    MID_Eye->SetScalarParameterValue(TEXT("RingOpacity"), RingOpacity);
}
