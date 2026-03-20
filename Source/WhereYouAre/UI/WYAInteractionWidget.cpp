#include "UI/WYAInteractionWidget.h"

void UWYAInteractionWidget::UpdateDisplay(const FText& ItemType, const FText& Prompt)
{
	CurrentItemType = ItemType;
	CurrentPrompt   = Prompt;
	OnDisplayUpdated(ItemType, Prompt);
}

void UWYAInteractionWidget::ShowClaimResult(bool bSuccess, const FText& Message)
{
	OnClaimResult(bSuccess, Message);
}
