#include "Location/WYAIPGeolocator.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UWYAIPGeolocator::RequestLocation()
{
    HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(TEXT("http://ip-api.com/json/"));
    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UWYAIPGeolocator::OnHttpResponse);
    HttpRequest->ProcessRequest();
}

void UWYAIPGeolocator::Cancel()
{
    if (HttpRequest.IsValid())
    {
        HttpRequest->CancelRequest();
        HttpRequest.Reset();
    }
}

void UWYAIPGeolocator::OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!bConnectedSuccessfully || !Response.IsValid() || Response->GetResponseCode() != 200)
    {
        UE_LOG(LogTemp, Warning, TEXT("WYAIPGeolocator: HTTP request failed"));
        BroadcastResult({}, false);
        return;
    }

    TSharedPtr<FJsonObject> JsonObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    if (!FJsonSerializer::Deserialize(Reader, JsonObj) || !JsonObj.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("WYAIPGeolocator: Failed to parse JSON"));
        BroadcastResult({}, false);
        return;
    }

    FString Status;
    JsonObj->TryGetStringField(TEXT("status"), Status);
    if (Status != TEXT("success"))
    {
        UE_LOG(LogTemp, Warning, TEXT("WYAIPGeolocator: API returned status=%s"), *Status);
        BroadcastResult({}, false);
        return;
    }

    FWYAGeoCoord Coord;
    JsonObj->TryGetNumberField(TEXT("lat"), Coord.Latitude);
    JsonObj->TryGetNumberField(TEXT("lon"), Coord.Longitude);
    Coord.Altitude = 0.0;
    Coord.Source = EWYALocationSource::IPGeolocate;

    FString City, Country;
    JsonObj->TryGetStringField(TEXT("city"), City);
    JsonObj->TryGetStringField(TEXT("country"), Country);
    UE_LOG(LogTemp, Log, TEXT("WYAIPGeolocator: Located at %s, %s (%.4f, %.4f)"),
        *City, *Country, Coord.Latitude, Coord.Longitude);

    BroadcastResult(Coord, true);
}
