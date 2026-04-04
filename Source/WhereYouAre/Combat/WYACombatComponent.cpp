#include "Combat/WYACombatComponent.h"

UWYACombatComponent::UWYACombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWYACombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

float UWYACombatComponent::ApplyDamage(float Amount, AActor* Attacker)
{
	if (!IsAlive() || Amount <= 0.f) return 0.f;

	const float Dealt = FMath::Min(Amount, CurrentHealth);
	CurrentHealth -= Dealt;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0.f;
		SetWoundState(EWYAWoundState::Down);
		OnDowned.Broadcast(Attacker);
	}
	else if (WoundState == EWYAWoundState::Healthy && CurrentHealth < MaxHealth * 0.6f)
	{
		// Took a hit — not patched, performance starts to show it
		SetWoundState(EWYAWoundState::FieldStable);
	}
	else if (WoundState == EWYAWoundState::FieldStable && CurrentHealth < MaxHealth * 0.3f)
	{
		// Bad shape — needs real treatment
		SetWoundState(EWYAWoundState::NeedsTreatment);
	}

	return Dealt;
}

float UWYACombatComponent::Heal(float Amount)
{
	if (IsDown() || Amount <= 0.f) return 0.f;

	const float Actual = FMath::Min(Amount, MaxHealth - CurrentHealth);
	CurrentHealth += Actual;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	// Recalculate wound state based on new health level
	if (CurrentHealth >= MaxHealth * 0.6f)
		SetWoundState(EWYAWoundState::Healthy);
	else if (CurrentHealth >= MaxHealth * 0.3f)
		SetWoundState(EWYAWoundState::FieldStable);

	return Actual;
}

void UWYACombatComponent::Revive(float StartingHealth)
{
	if (!IsDown()) return;

	// Bypass SetWoundState guard — force out of Down state first
	WoundState = EWYAWoundState::Down; // already Down, setter guard would skip
	CurrentHealth = FMath::Clamp(StartingHealth, 1.f, MaxHealth);

	// Manually transition to NeedsTreatment
	WoundState = EWYAWoundState::NeedsTreatment;
	OnWoundStateChanged.Broadcast(EWYAWoundState::NeedsTreatment);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UWYACombatComponent::SetWoundState(EWYAWoundState New)
{
	if (WoundState == New) return;
	WoundState = New;
	OnWoundStateChanged.Broadcast(New);
}
