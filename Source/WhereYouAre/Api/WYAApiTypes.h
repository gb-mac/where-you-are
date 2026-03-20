#pragma once

#include "CoreMinimal.h"
#include "WYAApiTypes.generated.h"

UENUM(BlueprintType)
enum class EWYAFaction : uint8
{
	None            UMETA(DisplayName = "None"),
	Machines        UMETA(DisplayName = "Machines"),
	Humans          UMETA(DisplayName = "Humans"),
	Wardens         UMETA(DisplayName = "Wardens"),
	Excommunicado   UMETA(DisplayName = "Excommunicado"),
};

UENUM(BlueprintType)
enum class EWYAItemType : uint8
{
	SupplyCache     UMETA(DisplayName = "Supply Cache"),
	DeadDrop        UMETA(DisplayName = "Dead Drop"),
	WardenMarker    UMETA(DisplayName = "Warden Marker"),
	FactionCache    UMETA(DisplayName = "Faction Cache"),
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

	bool IsAvailable() const { return ClaimedBy.IsEmpty(); }
};

/** Mirror of AccountDTO. */
USTRUCT(BlueprintType)
struct FWYAAccountData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FString     Id;
	UPROPERTY(BlueprintReadOnly) FString     Username;
	UPROPERTY(BlueprintReadOnly) EWYAFaction Faction = EWYAFaction::None;
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
		case EWYAFaction::Machines:      return TEXT("Machines");
		case EWYAFaction::Humans:        return TEXT("Humans");
		case EWYAFaction::Wardens:       return TEXT("Wardens");
		case EWYAFaction::Excommunicado: return TEXT("Excommunicado");
		default:                         return TEXT("None");
	}
}

inline EWYAFaction WYAFactionFromString(const FString& S)
{
	if (S == TEXT("Machines"))      return EWYAFaction::Machines;
	if (S == TEXT("Humans"))        return EWYAFaction::Humans;
	if (S == TEXT("Wardens"))       return EWYAFaction::Wardens;
	if (S == TEXT("Excommunicado")) return EWYAFaction::Excommunicado;
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
	Obj->TryGetStringField(TEXT("type"),    TypeStr);
	Obj->TryGetStringField(TEXT("faction"), FactionStr);
	Out.Type    = WYAItemTypeFromString(TypeStr);
	Out.Faction = WYAFactionFromString(FactionStr);
	return !Out.Id.IsEmpty();
}
