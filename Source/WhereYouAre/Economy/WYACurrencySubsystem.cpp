#include "Economy/WYACurrencySubsystem.h"

// ── Gold ──────────────────────────────────────────────────────────────────────

int32 UWYACurrencySubsystem::AwardGold(int32 Amount)
{
	if (Amount <= 0) return Gold;
	Gold += Amount;
	OnGoldChanged.Broadcast(Gold);
	UE_LOG(LogTemp, Log, TEXT("WYACurrencySubsystem: +%d Gold (balance: %d)"), Amount, Gold);
	return Gold;
}

bool UWYACurrencySubsystem::SpendGold(int32 Amount)
{
	if (Amount <= 0 || Gold < Amount) return false;
	Gold -= Amount;
	OnGoldChanged.Broadcast(Gold);
	UE_LOG(LogTemp, Log, TEXT("WYACurrencySubsystem: -%d Gold (balance: %d)"), Amount, Gold);
	return true;
}

// ── Silver ────────────────────────────────────────────────────────────────────

int32 UWYACurrencySubsystem::AwardSilver(int32 Amount)
{
	if (Amount <= 0) return Silver;
	Silver += Amount;
	OnSilverChanged.Broadcast(Silver);
	UE_LOG(LogTemp, Log, TEXT("WYACurrencySubsystem: +%d Silver (balance: %d)"), Amount, Silver);
	return Silver;
}

bool UWYACurrencySubsystem::SpendSilver(int32 Amount)
{
	if (Amount <= 0 || Silver < Amount) return false;
	Silver -= Amount;
	OnSilverChanged.Broadcast(Silver);
	UE_LOG(LogTemp, Log, TEXT("WYACurrencySubsystem: -%d Silver (balance: %d)"), Amount, Silver);
	return true;
}

// ── Save/load ─────────────────────────────────────────────────────────────────

void UWYACurrencySubsystem::GetSaveData(int32& OutGold, int32& OutSilver) const
{
	OutGold   = Gold;
	OutSilver = Silver;
}

void UWYACurrencySubsystem::ApplySaveData(int32 SavedGold, int32 SavedSilver)
{
	Gold   = FMath::Max(SavedGold,   0);
	Silver = FMath::Max(SavedSilver, 0);
	OnGoldChanged.Broadcast(Gold);
	OnSilverChanged.Broadcast(Silver);
}
