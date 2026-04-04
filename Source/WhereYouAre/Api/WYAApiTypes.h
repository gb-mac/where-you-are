#pragma once

#include "CoreMinimal.h"
#include "WYAApiTypes.generated.h"

/**
 * Joinable factions + territory-only states.
 * Fractured and Contested are zone assignment states, not player factions.
 */
UENUM(BlueprintType)
enum class EWYAFaction : uint8
{
	None        UMETA(DisplayName = "None"),

	// ── Joinable factions ────────────────────────────────────────────────────
	Holdouts    UMETA(DisplayName = "Holdouts"),       // residential, neighborhoods
	IronCompact UMETA(DisplayName = "Iron Compact"),   // government, military, civic
	Wardens     UMETA(DisplayName = "Wardens"),        // heritage, community, farmland
	Signal      UMETA(DisplayName = "Signal"),         // research, education, tech
	Covenant    UMETA(DisplayName = "Covenant"),       // former tech campuses, server infra
	Hollow      UMETA(DisplayName = "Hollow"),         // industrial, abandoned, transit
	Reckoners   UMETA(DisplayName = "Reckoners"),      // edge territory, rural-urban fringe
	Coherent    UMETA(DisplayName = "Coherent"),       // infrastructure: power, water, comms

	// ── Territory-only states (non-joinable) ─────────────────────────────────
	Fractured   UMETA(DisplayName = "Fractured"),      // dead zones — no faction hold
	Contested   UMETA(DisplayName = "Contested"),      // two factions scoring closely
};

UENUM(BlueprintType)
enum class EWYAItemType : uint8
{
	SupplyCache     UMETA(DisplayName = "Supply Cache"),
	DeadDrop        UMETA(DisplayName = "Dead Drop"),
	WardenMarker    UMETA(DisplayName = "Warden Marker"),
	FactionCache    UMETA(DisplayName = "Faction Cache"),
};

/**
 * Result of a ClaimItem attempt.
 * InsufficientFunds is 402 — player can't afford the Dead Drop price.
 * Failed is any other non-success (distance, already claimed, network).
 */
UENUM(BlueprintType)
enum class EWYAClaimResult : uint8
{
	Success           UMETA(DisplayName = "Success"),
	InsufficientFunds UMETA(DisplayName = "Insufficient Funds"),
	Failed            UMETA(DisplayName = "Failed"),
};

/** Mirror of WorldItemDTO from the backend (types.ts). */
USTRUCT(BlueprintType)
struct FWYAItemData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FString       Id;
	UPROPERTY(BlueprintReadOnly) EWYAItemType  Type    = EWYAItemType::SupplyCache;
	UPROPERTY(BlueprintReadOnly) double        Lat     = 0.0;
	UPROPERTY(BlueprintReadOnly) double        Lon     = 0.0;
	UPROPERTY(BlueprintReadOnly) double        Alt     = 0.0;
	UPROPERTY(BlueprintReadOnly) EWYAFaction   Faction = EWYAFaction::None;
	UPROPERTY(BlueprintReadOnly) FString       OwnerId;
	UPROPERTY(BlueprintReadOnly) FString       PlacedAt;
	UPROPERTY(BlueprintReadOnly) FString       ExpiresAt;
	UPROPERTY(BlueprintReadOnly) FString       ClaimedBy;   // empty = available
	UPROPERTY(BlueprintReadOnly) FString       ClaimedAt;

	/** 0 = free. Currency is "Gold", "Silver", or "" (free). */
	UPROPERTY(BlueprintReadOnly) int32         PriceAmount   = 0;
	UPROPERTY(BlueprintReadOnly) FString       PriceCurrency;

	bool IsAvailable()  const { return ClaimedBy.IsEmpty(); }
	bool IsPriced()     const { return PriceAmount > 0 && !PriceCurrency.IsEmpty(); }
};

/** Mirror of AccountDTO. */
USTRUCT(BlueprintType)
struct FWYAAccountData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FString     Id;
	UPROPERTY(BlueprintReadOnly) FString     Username;
	UPROPERTY(BlueprintReadOnly) EWYAFaction Faction      = EWYAFaction::None;
	UPROPERTY(BlueprintReadOnly) int32       GoldBalance   = 0;
	UPROPERTY(BlueprintReadOnly) int32       SilverBalance = 0;
};

// ── Helpers ───────────────────────────────────────────────────────────────────

inline FString WYAItemTypeToString(EWYAItemType Type)
{
	switch (Type)
	{
		case EWYAItemType::SupplyCache:   return TEXT("SupplyCache");
		case EWYAItemType::DeadDrop:      return TEXT("DeadDrop");
		case EWYAItemType::WardenMarker:  return TEXT("WardenMarker");
		case EWYAItemType::FactionCache:  return TEXT("FactionCache");
		default:                          return TEXT("SupplyCache");
	}
}

inline EWYAItemType WYAItemTypeFromString(const FString& S)
{
	if (S == TEXT("DeadDrop"))     return EWYAItemType::DeadDrop;
	if (S == TEXT("WardenMarker")) return EWYAItemType::WardenMarker;
	if (S == TEXT("FactionCache")) return EWYAItemType::FactionCache;
	return EWYAItemType::SupplyCache;
}

inline FString WYAFactionToString(EWYAFaction F)
{
	switch (F)
	{
		case EWYAFaction::Holdouts:    return TEXT("Holdouts");
		case EWYAFaction::IronCompact: return TEXT("IronCompact");
		case EWYAFaction::Wardens:     return TEXT("Wardens");
		case EWYAFaction::Signal:      return TEXT("Signal");
		case EWYAFaction::Covenant:    return TEXT("Covenant");
		case EWYAFaction::Hollow:      return TEXT("Hollow");
		case EWYAFaction::Reckoners:   return TEXT("Reckoners");
		case EWYAFaction::Coherent:    return TEXT("Coherent");
		case EWYAFaction::Fractured:   return TEXT("Fractured");
		case EWYAFaction::Contested:   return TEXT("Contested");
		default:                       return TEXT("None");
	}
}

inline EWYAFaction WYAFactionFromString(const FString& S)
{
	if (S == TEXT("Holdouts"))    return EWYAFaction::Holdouts;
	if (S == TEXT("IronCompact")) return EWYAFaction::IronCompact;
	if (S == TEXT("Wardens"))     return EWYAFaction::Wardens;
	if (S == TEXT("Signal"))      return EWYAFaction::Signal;
	if (S == TEXT("Covenant"))    return EWYAFaction::Covenant;
	if (S == TEXT("Hollow"))      return EWYAFaction::Hollow;
	if (S == TEXT("Reckoners"))   return EWYAFaction::Reckoners;
	if (S == TEXT("Coherent"))    return EWYAFaction::Coherent;
	if (S == TEXT("Fractured"))   return EWYAFaction::Fractured;
	if (S == TEXT("Contested"))   return EWYAFaction::Contested;
	return EWYAFaction::None;
}

/** Parse a WorldItemDTO JSON object into FWYAItemData. Returns false on error. */
inline bool WYAParseItemData(const TSharedPtr<class FJsonObject>& Obj, FWYAItemData& Out)
{
	if (!Obj.IsValid()) return false;
	Obj->TryGetStringField(TEXT("id"),        Out.Id);
	Obj->TryGetStringField(TEXT("ownerId"),   Out.OwnerId);
	Obj->TryGetStringField(TEXT("placedAt"),  Out.PlacedAt);
	Obj->TryGetStringField(TEXT("expiresAt"), Out.ExpiresAt);
	Obj->TryGetStringField(TEXT("claimedBy"), Out.ClaimedBy);
	Obj->TryGetStringField(TEXT("claimedAt"), Out.ClaimedAt);
	Obj->TryGetNumberField(TEXT("lat"),       Out.Lat);
	Obj->TryGetNumberField(TEXT("lon"),       Out.Lon);
	Obj->TryGetNumberField(TEXT("alt"),       Out.Alt);
	FString TypeStr, FactionStr;
	Obj->TryGetStringField(TEXT("type"),          TypeStr);
	Obj->TryGetStringField(TEXT("faction"),        FactionStr);
	Obj->TryGetStringField(TEXT("priceCurrency"),  Out.PriceCurrency);
	Out.Type    = WYAItemTypeFromString(TypeStr);
	Out.Faction = WYAFactionFromString(FactionStr);

	// priceAmount may come as a number or be absent (free item)
	double PriceNum = 0.0;
	if (Obj->TryGetNumberField(TEXT("priceAmount"), PriceNum))
		Out.PriceAmount = FMath::RoundToInt(PriceNum);

	return !Out.Id.IsEmpty();
}
