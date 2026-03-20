#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WYAInteractionWidget.generated.h"

/**
 * HUD widget shown when the player is near an interactable world item.
 * Create WBP_Interaction in Content/UI/ as a Blueprint subclass of this class.
 *
 * Bind the two text fields in Blueprint using the GetItemTypeText / GetPromptText
 * functions, or override UpdateDisplay for full control.
 */
UCLASS(Abstract)
class WHEREYOUARE_API UWYAInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Call from C++ to push new display state. */
	UFUNCTION(BlueprintCallable, Category = "WYA|UI")
	void UpdateDisplay(const FText& ItemType, const FText& Prompt);

	/** Called after UpdateDisplay so Blueprint can animate / react. */
	UFUNCTION(BlueprintImplementableEvent, Category = "WYA|UI")
	void OnDisplayUpdated(const FText& ItemType, const FText& Prompt);

	/** Show feedback after a claim attempt (success or fail). */
	UFUNCTION(BlueprintCallable, Category = "WYA|UI")
	void ShowClaimResult(bool bSuccess, const FText& Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "WYA|UI")
	void OnClaimResult(bool bSuccess, const FText& Message);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WYA|UI")
	FText CurrentItemType;

	UPROPERTY(BlueprintReadOnly, Category = "WYA|UI")
	FText CurrentPrompt;
};
