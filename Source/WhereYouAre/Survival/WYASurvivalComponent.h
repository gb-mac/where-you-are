#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WYASurvivalComponent.generated.h"

/**
 * Difficulty mode — scales survival drain rates.
 * Contractor: 50% of baseline (casual).
 * Operative: baseline.
 * Survivor: 150% water / 130% food.
 */
UENUM(BlueprintType)
enum class EWYASurvivalMode : uint8
{
    Contractor UMETA(DisplayName = "Contractor"),
    Operative  UMETA(DisplayName = "Operative"),
    Survivor   UMETA(DisplayName = "Survivor"),
};

/**
 * Current survival deficit state. Cascades from Warned → Debuffed → Severe.
 * Same enum for both water and food — severity applies independently.
 */
UENUM(BlueprintType)
enum class EWYADepletionState : uint8
{
    Normal   UMETA(DisplayName = "Normal"),
    Warned   UMETA(DisplayName = "Warned"),    // below 20% — UI cue
    Debuffed UMETA(DisplayName = "Debuffed"),  // hit 0, grace clock ticking
    Severe   UMETA(DisplayName = "Severe"),    // grace expired — wound state applied
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaterChanged, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFoodChanged,  float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnWaterDepletionState, EWYADepletionState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnFoodDepletionState,  EWYADepletionState, NewState);

/**
 * Tracks water and food levels for the player character.
 *
 * Drain is activity-based with a real-time floor (see economy-core.md):
 *   Water: 90 min active / 4 hours idle at Operative baseline.
 *   Food:  3 hours active / 12 hours idle at Operative baseline.
 *
 * Attach to AWYACharacter only — opponents don't need survival pressure.
 *
 * Environmental and wound modifiers are set externally by the relevant systems
 * (biome subsystem, combat component) via SetEnvironmentalModifier / SetWoundModifier.
 */
UCLASS(ClassGroup=(WYA), meta=(BlueprintSpawnableComponent))
class WHEREYOUARE_API UWYASurvivalComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWYASurvivalComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    // ── State setters called by external systems ─────────────────────────────

    /** True = combat or sprint active. Raises water × 2, food × 1.5. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Survival")
    void SetExerting(bool bInExerting);

    /** Difficulty mode. Scales all drain rates. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Survival")
    void SetMode(EWYASurvivalMode NewMode);

    /**
     * Environmental water modifier (multiplicative, stacks with wound modifier).
     * Heat zone = 1.5, Vael proximity = 1.3, normal = 1.0.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Survival")
    void SetWaterEnvModifier(float Multiplier);

    /**
     * Environmental food modifier.
     * Cold zone = 1.2, normal = 1.0.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Survival")
    void SetFoodEnvModifier(float Multiplier);

    /**
     * Wound-driven water modifier. Active bleed = 1.2.
     * Called by WYACombatComponent on wound state change.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Survival")
    void SetWaterWoundModifier(float Multiplier);

    /**
     * Wound-driven food modifier. Toxin exposure = 1.3.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Survival")
    void SetFoodWoundModifier(float Multiplier);

    // ── Consumption and restoration ──────────────────────────────────────────

    /** Drink water. Returns actual amount restored. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Survival")
    float DrinkWater(float Amount);

    /** Eat food. Returns actual amount restored. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Survival")
    float EatFood(float Amount);

    // ── Accessors ────────────────────────────────────────────────────────────

    UFUNCTION(BlueprintPure, Category = "WYA|Survival")
    float GetWater() const { return CurrentWater; }

    UFUNCTION(BlueprintPure, Category = "WYA|Survival")
    float GetFood() const { return CurrentFood; }

    UFUNCTION(BlueprintPure, Category = "WYA|Survival")
    float GetWaterPercent() const { return CurrentWater / MaxWater; }

    UFUNCTION(BlueprintPure, Category = "WYA|Survival")
    float GetFoodPercent() const { return CurrentFood / MaxFood; }

    UFUNCTION(BlueprintPure, Category = "WYA|Survival")
    EWYADepletionState GetWaterState() const { return WaterState; }

    UFUNCTION(BlueprintPure, Category = "WYA|Survival")
    EWYADepletionState GetFoodState() const { return FoodState; }

    // ── Delegates ────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "WYA|Survival")
    FOnWaterChanged OnWaterChanged;

    UPROPERTY(BlueprintAssignable, Category = "WYA|Survival")
    FOnFoodChanged OnFoodChanged;

    /** Broadcast when depletion tier changes (Normal → Warned → Debuffed → Severe). */
    UPROPERTY(BlueprintAssignable, Category = "WYA|Survival")
    FOnWaterDepletionState OnWaterDepletionStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "WYA|Survival")
    FOnFoodDepletionState OnFoodDepletionStateChanged;

    // ── Configuration ────────────────────────────────────────────────────────

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WYA|Survival")
    float MaxWater = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WYA|Survival")
    float MaxFood = 100.f;

    /** Mode set at game start from player preferences. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WYA|Survival")
    EWYASurvivalMode Mode = EWYASurvivalMode::Operative;

    // ── Save/load support ────────────────────────────────────────────────────

    /** Called by WYASaveSubsystem on save. */
    void GetSaveData(float& OutWater, float& OutFood) const;

    /** Called by WYASaveSubsystem on load — restores levels without triggering drains. */
    void ApplySaveData(float SavedWater, float SavedFood);

private:
    // Water drain rates per second at Operative baseline
    static constexpr float WaterDrainActive = 100.f / 5400.f;   // 90 min
    static constexpr float WaterDrainIdle   = 100.f / 14400.f;  // 4 hours

    // Food drain rates per second at Operative baseline
    static constexpr float FoodDrainActive  = 100.f / 10800.f;  // 3 hours
    static constexpr float FoodDrainIdle    = 100.f / 43200.f;  // 12 hours

    // Grace window durations (seconds)
    static constexpr float WaterGraceDuration = 600.f;   // 10 minutes
    static constexpr float FoodGraceDuration  = 1800.f;  // 30 minutes

    float CurrentWater = 100.f;
    float CurrentFood  = 100.f;

    bool               bExerting       = false;
    float              WaterEnvMod     = 1.f;
    float              FoodEnvMod      = 1.f;
    float              WaterWoundMod   = 1.f;
    float              FoodWoundMod    = 1.f;

    EWYADepletionState WaterState = EWYADepletionState::Normal;
    EWYADepletionState FoodState  = EWYADepletionState::Normal;

    float WaterGraceClock = 0.f;  // counts up from 0 while Debuffed
    float FoodGraceClock  = 0.f;

    float ModeWaterScale() const;
    float ModeFoodScale()  const;

    void DrainWater(float DeltaTime);
    void DrainFood(float DeltaTime);

    void SetWaterState(EWYADepletionState New);
    void SetFoodState(EWYADepletionState New);
};
