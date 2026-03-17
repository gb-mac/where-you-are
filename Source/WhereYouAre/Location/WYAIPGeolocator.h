#pragma once

#include "CoreMinimal.h"
#include "Location/WYALocationProvider.h"
#include "Interfaces/IHttpRequest.h"
#include "WYAIPGeolocator.generated.h"

/**
 * Desktop location provider: fetches city-level GPS coordinates from ip-api.com.
 * No API key required. Accuracy ~1-5km — sufficient to anchor the world origin.
 */
UCLASS()
class WHEREYOUARE_API UWYAIPGeolocator : public UWYALocationProvider
{
    GENERATED_BODY()

public:
    virtual void RequestLocation() override;
    virtual void Cancel() override;

private:
    TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> HttpRequest;

    void OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};
