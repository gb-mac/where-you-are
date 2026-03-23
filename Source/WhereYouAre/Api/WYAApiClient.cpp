#include "Api/WYAApiClient.h"
#include "Api/WYADeviceSave.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Guid.h"

const FString UWYAApiClient::SaveSlot = TEXT("WYADevice");
const FString UWYAApiClient::BaseURL  = TEXT("https://where-you-are-api.fly.dev/v1");

// ── Lifecycle ─────────────────────────────────────────────────────────────────

void UWYAApiClient::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load or create stable device identity
	if (UWYADeviceSave* Save = Cast<UWYADeviceSave>(
		UGameplayStatics::LoadGameFromSlot(SaveSlot, 0)))
	{
		DeviceUUID = Save->DeviceUUID;
		JWT        = Save->JWT;         // attempt to reuse cached JWT
		Account.Id = Save->AccountId;

		UE_LOG(LogTemp, Log, TEXT("WYAApiClient: loaded device UUID %s"), *DeviceUUID);
	}
	else
	{
		DeviceUUID = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
		UE_LOG(LogTemp, Log, TEXT("WYAApiClient: new device UUID %s"), *DeviceUUID);
	}

	// Validate cached JWT with a lightweight whoami — if the JWT is stale or
	// missing, EnsureAuth() registers/logs in afresh.
	if (JWT.IsEmpty())
	{
		EnsureAuth();
	}
	else
	{
		// Fire a quick nearby items request to verify the token; on 401, re-auth.
		// For simplicity in first-playable-loop we just trust the cached JWT
		// and let the first real request discover if it's stale.
		UE_LOG(LogTemp, Log, TEXT("WYAApiClient: using cached JWT"));
		OnAuthReady.Broadcast(true);
	}
}

// ── Auth ──────────────────────────────────────────────────────────────────────

void UWYAApiClient::EnsureAuth()
{
	if (bAuthInProgress || IsAuthenticated()) return;
	bAuthInProgress = true;
	TryRegister();
}

void UWYAApiClient::TryRegister()
{
	// Username: "d_" + first 8 hex chars of UUID (no hyphens) → 10 chars
	const FString UUIDNoHyphens = DeviceUUID.Replace(TEXT("-"), TEXT(""));
	const FString Username      = TEXT("d_") + UUIDNoHyphens.Left(8);
	const FString Email         = TEXT("d_") + DeviceUUID + TEXT("@device.wya.gg");
	const FString Password      = DeviceUUID;

	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("username"), Username);
	Body->SetStringField(TEXT("email"),    Email);
	Body->SetStringField(TEXT("password"), Password);

	FString BodyStr;
	TSharedRef<TJsonWriter<>> W = TJsonWriterFactory<>::Create(&BodyStr);
	FJsonSerializer::Serialize(Body.ToSharedRef(), W);

	// Bypass SendRequest() — we don't have a JWT yet
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Req = FHttpModule::Get().CreateRequest();
	Req->SetURL(BaseURL + TEXT("/auth/register"));
	Req->SetVerb(TEXT("POST"));
	Req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Req->SetContentAsString(BodyStr);

	Req->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr, FHttpResponsePtr Resp, bool bOk)
	{
		if (!bOk || !Resp.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("WYAApiClient: register request failed"));
			bAuthInProgress = false;
			bAuthFailed     = true;
			OnAuthReady.Broadcast(false);
			return;
		}

		const int32 Code = Resp->GetResponseCode();
		if (Code == 201)
		{
			// Registered successfully
			TSharedPtr<FJsonObject> Json;
			TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(Resp->GetContentAsString());
			if (FJsonSerializer::Deserialize(R, Json) && Json.IsValid())
			{
				FString NewJWT;
				Json->TryGetStringField(TEXT("token"), NewJWT);
				FWYAAccountData Acc;
				if (const TSharedPtr<FJsonObject>* AccObj = nullptr;
					Json->TryGetObjectField(TEXT("account"), AccObj))
				{
					(*AccObj)->TryGetStringField(TEXT("id"),       Acc.Id);
					(*AccObj)->TryGetStringField(TEXT("username"), Acc.Username);
					FString FactionStr;
					(*AccObj)->TryGetStringField(TEXT("faction"),  FactionStr);
					Acc.Faction = WYAFactionFromString(FactionStr);
				}
				OnAuthSuccess(NewJWT, Acc);
			}
		}
		else if (Code == 409)
		{
			// Already registered — try login
			TryLogin();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WYAApiClient: register returned %d"), Code);
			bAuthInProgress = false;
			bAuthFailed     = true;
			OnAuthReady.Broadcast(false);
		}
	});

	Req->ProcessRequest();
}

void UWYAApiClient::TryLogin()
{
	const FString Email    = TEXT("d_") + DeviceUUID + TEXT("@device.wya.gg");
	const FString Password = DeviceUUID;

	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("email"),    Email);
	Body->SetStringField(TEXT("password"), Password);

	FString BodyStr;
	TSharedRef<TJsonWriter<>> W = TJsonWriterFactory<>::Create(&BodyStr);
	FJsonSerializer::Serialize(Body.ToSharedRef(), W);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Req = FHttpModule::Get().CreateRequest();
	Req->SetURL(BaseURL + TEXT("/auth/login"));
	Req->SetVerb(TEXT("POST"));
	Req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Req->SetContentAsString(BodyStr);

	Req->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr, FHttpResponsePtr Resp, bool bOk)
	{
		if (!bOk || !Resp.IsValid() || Resp->GetResponseCode() != 200)
		{
			UE_LOG(LogTemp, Warning, TEXT("WYAApiClient: login failed (code=%d)"),
			       Resp.IsValid() ? Resp->GetResponseCode() : 0);
			bAuthInProgress = false;
			bAuthFailed     = true;
			OnAuthReady.Broadcast(false);
			return;
		}

		TSharedPtr<FJsonObject> Json;
		TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(Resp->GetContentAsString());
		if (!FJsonSerializer::Deserialize(R, Json) || !Json.IsValid())
		{
			bAuthInProgress = false;
			bAuthFailed     = true;
			OnAuthReady.Broadcast(false);
			return;
		}

		FString NewJWT;
		Json->TryGetStringField(TEXT("token"), NewJWT);
		FWYAAccountData Acc;
		if (const TSharedPtr<FJsonObject>* AccObj = nullptr;
			Json->TryGetObjectField(TEXT("account"), AccObj))
		{
			(*AccObj)->TryGetStringField(TEXT("id"),       Acc.Id);
			(*AccObj)->TryGetStringField(TEXT("username"), Acc.Username);
			FString FactionStr;
			(*AccObj)->TryGetStringField(TEXT("faction"),  FactionStr);
			Acc.Faction = WYAFactionFromString(FactionStr);
		}
		OnAuthSuccess(NewJWT, Acc);
	});

	Req->ProcessRequest();
}

void UWYAApiClient::OnAuthSuccess(const FString& InJWT, const FWYAAccountData& InAccount)
{
	JWT     = InJWT;
	Account = InAccount;
	bAuthInProgress = false;

	// Persist
	UWYADeviceSave* Save = Cast<UWYADeviceSave>(
		UGameplayStatics::CreateSaveGameObject(UWYADeviceSave::StaticClass()));
	Save->DeviceUUID = DeviceUUID;
	Save->JWT        = JWT;
	Save->AccountId  = Account.Id;
	UGameplayStatics::SaveGameToSlot(Save, SaveSlot, 0);

	UE_LOG(LogTemp, Log, TEXT("WYAApiClient: authenticated as %s (%s)"),
	       *Account.Username, *Account.Id);

	OnAuthReady.Broadcast(true);
	FlushPendingCalls();
}

void UWYAApiClient::FlushPendingCalls()
{
	TArray<TFunction<void()>> Calls = MoveTemp(PendingCalls);
	for (auto& Call : Calls) Call();
}

// ── Item API ──────────────────────────────────────────────────────────────────

void UWYAApiClient::GetNearbyItems(
	double Lat, double Lon, float RadiusMetres,
	TFunction<void(bool, TArray<FWYAItemData>)> Callback)
{
	if (!IsAuthenticated())
	{
		PendingCalls.Add([this, Lat, Lon, RadiusMetres, CB = MoveTemp(Callback)]() mutable
		{
			GetNearbyItems(Lat, Lon, RadiusMetres, MoveTemp(CB));
		});
		EnsureAuth();
		return;
	}

	const FString Path = FString::Printf(
		TEXT("/items/nearby?lat=%f&lon=%f&radius=%d"),
		Lat, Lon, FMath::RoundToInt(RadiusMetres));

	SendRequest(TEXT("GET"), Path, TEXT(""),
		[CB = MoveTemp(Callback)](bool bOk, TSharedPtr<FJsonObject> Json)
	{
		TArray<FWYAItemData> Items;
		if (bOk && Json.IsValid())
		{
			const TArray<TSharedPtr<FJsonValue>>* Arr;
			if (Json->TryGetArrayField(TEXT("items"), Arr))
			{
				for (const TSharedPtr<FJsonValue>& Val : *Arr)
				{
					FWYAItemData Item;
					if (WYAParseItemData(Val->AsObject(), Item))
						Items.Add(Item);
				}
			}
		}
		CB(bOk, MoveTemp(Items));
	});
}

void UWYAApiClient::ClaimItem(
	const FString& ItemId, double ClaimerLat, double ClaimerLon,
	TFunction<void(bool, FWYAItemData)> Callback)
{
	if (!IsAuthenticated())
	{
		PendingCalls.Add([this, ItemId, ClaimerLat, ClaimerLon, CB = MoveTemp(Callback)]() mutable
		{
			ClaimItem(ItemId, ClaimerLat, ClaimerLon, MoveTemp(CB));
		});
		EnsureAuth();
		return;
	}

	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetNumberField(TEXT("claimerLat"), ClaimerLat);
	Body->SetNumberField(TEXT("claimerLon"), ClaimerLon);
	FString BodyStr;
	TSharedRef<TJsonWriter<>> W = TJsonWriterFactory<>::Create(&BodyStr);
	FJsonSerializer::Serialize(Body.ToSharedRef(), W);

	const FString Path = TEXT("/items/") + ItemId + TEXT("/claim");

	SendRequest(TEXT("PATCH"), Path, BodyStr,
		[CB = MoveTemp(Callback)](bool bOk, TSharedPtr<FJsonObject> Json)
	{
		FWYAItemData Item;
		if (bOk && Json.IsValid())
		{
			const TSharedPtr<FJsonObject>* ItemObj;
			if (Json->TryGetObjectField(TEXT("item"), ItemObj))
				WYAParseItemData(*ItemObj, Item);
		}
		CB(bOk, Item);
	});
}

void UWYAApiClient::PlaceItem(
	EWYAItemType Type, double Lat, double Lon, double Alt,
	TFunction<void(bool, FWYAItemData)> Callback)
{
	if (!IsAuthenticated())
	{
		PendingCalls.Add([this, Type, Lat, Lon, Alt, CB = MoveTemp(Callback)]() mutable
		{
			PlaceItem(Type, Lat, Lon, Alt, MoveTemp(CB));
		});
		EnsureAuth();
		return;
	}

	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("type"), WYAItemTypeToString(Type));
	Body->SetNumberField(TEXT("lat"),  Lat);
	Body->SetNumberField(TEXT("lon"),  Lon);
	Body->SetNumberField(TEXT("alt"),  Alt);
	FString BodyStr;
	TSharedRef<TJsonWriter<>> W = TJsonWriterFactory<>::Create(&BodyStr);
	FJsonSerializer::Serialize(Body.ToSharedRef(), W);

	SendRequest(TEXT("POST"), TEXT("/items"), BodyStr,
		[CB = MoveTemp(Callback)](bool bOk, TSharedPtr<FJsonObject> Json)
	{
		FWYAItemData Item;
		if (bOk && Json.IsValid())
		{
			const TSharedPtr<FJsonObject>* ItemObj;
			if (Json->TryGetObjectField(TEXT("item"), ItemObj))
				WYAParseItemData(*ItemObj, Item);
		}
		CB(bOk, Item);
	});
}

// ── Territory API ─────────────────────────────────────────────────────────────

void UWYAApiClient::GetTerritoryRegion(
	double Lat, double Lon,
	TFunction<void(bool, TSharedPtr<FJsonObject>)> Callback)
{
	if (!IsAuthenticated())
	{
		PendingCalls.Add([this, Lat, Lon, CB = MoveTemp(Callback)]() mutable
		{
			GetTerritoryRegion(Lat, Lon, MoveTemp(CB));
		});
		EnsureAuth();
		return;
	}

	const FString Path = FString::Printf(TEXT("/territory?lat=%f&lon=%f"), Lat, Lon);
	SendRequest(TEXT("GET"), Path, TEXT(""), MoveTemp(Callback));
}

// ── Internal HTTP ─────────────────────────────────────────────────────────────

void UWYAApiClient::SendRequest(
	const FString& Verb, const FString& Path,
	const FString& Body,
	TFunction<void(bool, TSharedPtr<FJsonObject>)> Callback)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Req = FHttpModule::Get().CreateRequest();
	Req->SetURL(BaseURL + Path);
	Req->SetVerb(Verb);
	Req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Req->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + JWT);
	if (!Body.IsEmpty()) Req->SetContentAsString(Body);

	Req->OnProcessRequestComplete().BindUObject(
		this, &UWYAApiClient::OnRequestComplete, MoveTemp(Callback));
	Req->ProcessRequest();
}

void UWYAApiClient::OnRequestComplete(
	FHttpRequestPtr, FHttpResponsePtr Response, bool bConnected,
	TFunction<void(bool, TSharedPtr<FJsonObject>)> Callback)
{
	if (!bConnected || !Response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("WYAApiClient: request failed (no connection)"));
		Callback(false, nullptr);
		return;
	}

	const int32 Code = Response->GetResponseCode();

	if (Code == 401)
	{
		// JWT expired — clear and re-auth
		JWT.Empty();
		EnsureAuth();
		Callback(false, nullptr);
		return;
	}

	if (Code < 200 || Code >= 300)
	{
		UE_LOG(LogTemp, Warning, TEXT("WYAApiClient: HTTP %d for request"), Code);
		Callback(false, nullptr);
		return;
	}

	TSharedPtr<FJsonObject> Json;
	TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (!FJsonSerializer::Deserialize(R, Json))
	{
		Callback(true, nullptr);
		return;
	}

	Callback(true, Json);
}
