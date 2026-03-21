#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "WYAAssistantCharacter.generated.h"

UENUM(BlueprintType)
enum class EWYAEyeState : uint8
{
    Red    UMETA(DisplayName = "Red"),    // default — threat/unknown
    Green  UMETA(DisplayName = "Green"),  // resolved, coherent
};

/**
 * The Assistant character — Robot companion with emissive eye state machine.
 * Eyes: left=red, right=green by default. SetEyeState drives both via M_AssistantEye MID.
 *
 * Material parameters on M_AssistantEye:
 *   EyeColor_Linear    (Vector3)
 *   EmissiveMultiplier (Scalar)
 *   RingOpacity        (Scalar)
 */
UCLASS()
class WHEREYOUARE_API AWYAAssistantCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AWYAAssistantCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /**
     * Transition the eye to a new state.
     * bInstant=true: sets parameters directly (no timeline) — used for the last-second red flip.
     * bInstant=false: 0.4s ease-in-out cross-fade through amber midpoint.
     */
    UFUNCTION(BlueprintCallable, Category = "Assistant|Eyes")
    void SetEyeState(EWYAEyeState NewState, bool bInstant = false);

    UFUNCTION(BlueprintPure, Category = "Assistant|Eyes")
    EWYAEyeState GetEyeState() const { return CurrentEyeState; }

protected:
    /** Iris mesh — separate from the head, assigned M_AssistantEye MID at BeginPlay */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assistant|Eyes")
    TObjectPtr<UStaticMeshComponent> IrisMesh;

    /** Eye material — set this to M_AssistantEye in the Blueprint subclass */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assistant|Eyes")
    TObjectPtr<UMaterialInterface> EyeMaterial;

    /** Transition curve asset — float track 0→1 over 0.4s, ease-in-out S-curve */
    UPROPERTY(EditDefaultsOnly, Category = "Assistant|Eyes")
    TObjectPtr<UCurveFloat> EyeTransitionCurve;

    // ── Color constants (from shader spec) ────────────────────────────────
    // Red: linear (0.527, 0.044, 0.026) × 4.0
    // Green: linear (0.021, 0.439, 0.128) × 3.5
    UPROPERTY(EditDefaultsOnly, Category = "Assistant|Eyes")
    FLinearColor EyeColor_Red   = FLinearColor(0.527f, 0.044f, 0.026f, 1.f);

    UPROPERTY(EditDefaultsOnly, Category = "Assistant|Eyes")
    FLinearColor EyeColor_Green = FLinearColor(0.021f, 0.439f, 0.128f, 1.f);

    UPROPERTY(EditDefaultsOnly, Category = "Assistant|Eyes")
    float EmissiveMultiplier_Red   = 4.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Assistant|Eyes")
    float EmissiveMultiplier_Green = 3.5f;

private:
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> MID_Eye;

    UPROPERTY()
    UTimelineComponent* EyeTimeline;

    EWYAEyeState CurrentEyeState = EWYAEyeState::Red;

    // Lerp sources — captured when a transition starts
    FLinearColor EyeColor_From;
    FLinearColor EyeColor_To;
    float        Multiplier_From = 4.0f;
    float        Multiplier_To   = 4.0f;

    UFUNCTION()
    void OnEyeTimelineUpdate(float Alpha);

    UFUNCTION()
    void OnEyeTimelineFinished();

    void SetEyeParametersDirect(const FLinearColor& Color, float Multiplier);

    /** LOD changed callback — adjusts RingOpacity to prevent FSR emissive bleed */
    virtual void OnLODChanged(int32 NewLODLevel);
};
