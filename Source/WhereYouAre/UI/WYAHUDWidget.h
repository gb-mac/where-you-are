#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Combat/WYACombatComponent.h"
#include "Survival/WYASurvivalComponent.h"
#include "WYAHUDWidget.generated.h"

/**
 * Persistent HUD — health, wound state, survival bars, currency.
 *
 * Create WBP_WYAHud in Content/UI/ as a Blueprint subclass of this class.
 * Set HUDWidgetClass on BP_WYAPlayerController to WBP_WYAHud.
 *
 * AWYAPlayerController polls all subsystems every HUDUpdateInterval seconds
 * and calls the three Update* methods below. Implement each as a BP event
 * to drive your progress bars and text blocks.
 *
 * Wound state display:
 *   Healthy        — no indicator
 *   FieldStable    — yellow indicator, movement speed penalty visible
 *   NeedsTreatment — orange indicator, find a medic
 *   Down           — red overlay, auto-recovery timer
 *
 * Survival state display (per resource):
 *   Normal   — no indicator
 *   Warned   — yellow bar pulse (below 20%)
 *   Debuffed — orange, grace period ticking
 *   Severe   — red, wound state penalty active
 */
UCLASS(Abstract)
class WHEREYOUARE_API UWYAHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /**
     * Push updated health state to Blueprint.
     * HealthPercent: 0.0–1.0.
     * WoundState: drives the condition indicator color/text.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|HUD")
    void UpdateHealthDisplay(float HealthPercent, EWYAWoundState WoundState);

    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|HUD")
    void OnHealthUpdated(float HealthPercent, EWYAWoundState WoundState);

    /**
     * Push updated survival state to Blueprint.
     * *Percent: 0.0–1.0. *State: drives bar color and alert logic.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|HUD")
    void UpdateSurvivalDisplay(float WaterPercent, EWYADepletionState WaterState,
                               float FoodPercent,  EWYADepletionState FoodState);

    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|HUD")
    void OnSurvivalUpdated(float WaterPercent, EWYADepletionState WaterState,
                           float FoodPercent,  EWYADepletionState FoodState);

    /**
     * Push updated currency balances to Blueprint.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|HUD")
    void UpdateCurrencyDisplay(int32 Gold, int32 Silver);

    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|HUD")
    void OnCurrencyUpdated(int32 Gold, int32 Silver);

    /**
     * Push the active quest to Blueprint.
     * Called when a quest is assigned or completed.
     * bHasQuest=false means the quest slot is empty — hide the tracker.
     * Title and Body are empty strings when bHasQuest=false.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|HUD")
    void UpdateQuestDisplay(bool bHasQuest, const FString& Title, const FString& Body);

    UFUNCTION(BlueprintImplementableEvent, Category = "WYA|HUD")
    void OnQuestUpdated(bool bHasQuest, const FString& Title, const FString& Body);

protected:
    // Last-pushed values — read from Blueprint if needed
    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    float LastHealthPercent = 1.f;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    EWYAWoundState LastWoundState = EWYAWoundState::Healthy;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    float LastWaterPercent = 1.f;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    EWYADepletionState LastWaterState = EWYADepletionState::Normal;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    float LastFoodPercent = 1.f;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    EWYADepletionState LastFoodState = EWYADepletionState::Normal;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    int32 LastGold = 0;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    int32 LastSilver = 0;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    bool bLastHasQuest = false;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    FString LastQuestTitle;

    UPROPERTY(BlueprintReadOnly, Category = "WYA|HUD")
    FString LastQuestBody;
};
