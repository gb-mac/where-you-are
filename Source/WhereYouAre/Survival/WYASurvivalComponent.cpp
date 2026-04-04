#include "Survival/WYASurvivalComponent.h"

UWYASurvivalComponent::UWYASurvivalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWYASurvivalComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentWater = MaxWater;
	CurrentFood  = MaxFood;
}

void UWYASurvivalComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DrainWater(DeltaTime);
	DrainFood(DeltaTime);
}

// ── State setters ─────────────────────────────────────────────────────────────

void UWYASurvivalComponent::SetExerting(bool bInExerting) { bExerting = bInExerting; }
void UWYASurvivalComponent::SetMode(EWYASurvivalMode M) { Mode = M; }
void UWYASurvivalComponent::SetWaterEnvModifier(float M)   { WaterEnvMod   = FMath::Max(M, 0.f); }
void UWYASurvivalComponent::SetFoodEnvModifier(float M)    { FoodEnvMod    = FMath::Max(M, 0.f); }
void UWYASurvivalComponent::SetWaterWoundModifier(float M) { WaterWoundMod = FMath::Max(M, 0.f); }
void UWYASurvivalComponent::SetFoodWoundModifier(float M)  { FoodWoundMod  = FMath::Max(M, 0.f); }

// ── Consumption and restoration ───────────────────────────────────────────────

float UWYASurvivalComponent::DrinkWater(float Amount)
{
	if (Amount <= 0.f) return 0.f;
	const float Actual = FMath::Min(Amount, MaxWater - CurrentWater);
	CurrentWater += Actual;
	OnWaterChanged.Broadcast(CurrentWater, MaxWater);

	// Re-evaluate state after drinking
	if (CurrentWater > MaxWater * 0.2f && WaterState != EWYADepletionState::Normal)
	{
		WaterGraceClock = 0.f;
		SetWaterState(EWYADepletionState::Normal);
	}
	return Actual;
}

float UWYASurvivalComponent::EatFood(float Amount)
{
	if (Amount <= 0.f) return 0.f;
	const float Actual = FMath::Min(Amount, MaxFood - CurrentFood);
	CurrentFood += Actual;
	OnFoodChanged.Broadcast(CurrentFood, MaxFood);

	if (CurrentFood > MaxFood * 0.2f && FoodState != EWYADepletionState::Normal)
	{
		FoodGraceClock = 0.f;
		SetFoodState(EWYADepletionState::Normal);
	}
	return Actual;
}

// ── Save/load ─────────────────────────────────────────────────────────────────

void UWYASurvivalComponent::GetSaveData(float& OutWater, float& OutFood) const
{
	OutWater = CurrentWater;
	OutFood  = CurrentFood;
}

void UWYASurvivalComponent::ApplySaveData(float SavedWater, float SavedFood)
{
	CurrentWater = FMath::Clamp(SavedWater, 0.f, MaxWater);
	CurrentFood  = FMath::Clamp(SavedFood,  0.f, MaxFood);
	OnWaterChanged.Broadcast(CurrentWater, MaxWater);
	OnFoodChanged.Broadcast(CurrentFood, MaxFood);
}

// ── Internal drain ────────────────────────────────────────────────────────────

float UWYASurvivalComponent::ModeWaterScale() const
{
	switch (Mode)
	{
		case EWYASurvivalMode::Contractor: return 0.5f;
		case EWYASurvivalMode::Survivor:   return 1.5f;
		default:                           return 1.0f;
	}
}

float UWYASurvivalComponent::ModeFoodScale() const
{
	switch (Mode)
	{
		case EWYASurvivalMode::Contractor: return 0.5f;
		case EWYASurvivalMode::Survivor:   return 1.3f;
		default:                           return 1.0f;
	}
}

void UWYASurvivalComponent::DrainWater(float DeltaTime)
{
	const float BaseRate    = bExerting ? WaterDrainActive : WaterDrainIdle;
	const float DrainPerSec = BaseRate * ModeWaterScale() * WaterEnvMod * WaterWoundMod;
	CurrentWater = FMath::Max(CurrentWater - DrainPerSec * DeltaTime, 0.f);
	OnWaterChanged.Broadcast(CurrentWater, MaxWater);

	if (CurrentWater <= 0.f)
	{
		if (WaterState == EWYADepletionState::Normal || WaterState == EWYADepletionState::Warned)
			SetWaterState(EWYADepletionState::Debuffed);

		if (WaterState == EWYADepletionState::Debuffed)
		{
			WaterGraceClock += DeltaTime;
			if (WaterGraceClock >= WaterGraceDuration)
			{
				WaterGraceClock = 0.f;
				SetWaterState(EWYADepletionState::Severe);
			}
		}
	}
	else if (CurrentWater < MaxWater * 0.2f && WaterState == EWYADepletionState::Normal)
	{
		SetWaterState(EWYADepletionState::Warned);
	}
}

void UWYASurvivalComponent::DrainFood(float DeltaTime)
{
	const float BaseRate    = bExerting ? FoodDrainActive : FoodDrainIdle;
	const float DrainPerSec = BaseRate * ModeFoodScale() * FoodEnvMod * FoodWoundMod;
	CurrentFood = FMath::Max(CurrentFood - DrainPerSec * DeltaTime, 0.f);
	OnFoodChanged.Broadcast(CurrentFood, MaxFood);

	if (CurrentFood <= 0.f)
	{
		if (FoodState == EWYADepletionState::Normal || FoodState == EWYADepletionState::Warned)
			SetFoodState(EWYADepletionState::Debuffed);

		if (FoodState == EWYADepletionState::Debuffed)
		{
			FoodGraceClock += DeltaTime;
			if (FoodGraceClock >= FoodGraceDuration)
			{
				FoodGraceClock = 0.f;
				SetFoodState(EWYADepletionState::Severe);
			}
		}
	}
	else if (CurrentFood < MaxFood * 0.2f && FoodState == EWYADepletionState::Normal)
	{
		SetFoodState(EWYADepletionState::Warned);
	}
}

void UWYASurvivalComponent::SetWaterState(EWYADepletionState New)
{
	if (WaterState == New) return;
	WaterState = New;
	OnWaterDepletionStateChanged.Broadcast(New);
}

void UWYASurvivalComponent::SetFoodState(EWYADepletionState New)
{
	if (FoodState == New) return;
	FoodState = New;
	OnFoodDepletionStateChanged.Broadcast(New);
}
