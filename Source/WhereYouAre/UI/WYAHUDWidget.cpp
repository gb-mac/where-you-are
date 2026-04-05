#include "UI/WYAHUDWidget.h"

void UWYAHUDWidget::UpdateHealthDisplay(float HealthPercent, EWYAWoundState WoundState)
{
    LastHealthPercent = HealthPercent;
    LastWoundState    = WoundState;
    OnHealthUpdated(HealthPercent, WoundState);
}

void UWYAHUDWidget::UpdateSurvivalDisplay(
    float WaterPercent, EWYADepletionState WaterState,
    float FoodPercent,  EWYADepletionState FoodState)
{
    LastWaterPercent = WaterPercent;
    LastWaterState   = WaterState;
    LastFoodPercent  = FoodPercent;
    LastFoodState    = FoodState;
    OnSurvivalUpdated(WaterPercent, WaterState, FoodPercent, FoodState);
}

void UWYAHUDWidget::UpdateCurrencyDisplay(int32 Gold, int32 Silver)
{
    LastGold   = Gold;
    LastSilver = Silver;
    OnCurrencyUpdated(Gold, Silver);
}

void UWYAHUDWidget::UpdateQuestDisplay(bool bHasQuest, const FString& Title, const FString& Body)
{
    LastQuestTitle  = Title;
    LastQuestBody   = Body;
    bLastHasQuest   = bHasQuest;
    OnQuestUpdated(bHasQuest, Title, Body);
}
