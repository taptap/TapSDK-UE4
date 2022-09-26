#pragma once

#include "CoreMinimal.h"
#include "AAUBackViewWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "AAUHealthTipWidget.generated.h"

/**
 *  
 */
UCLASS()
class ANTIADDICTION_API UAAUHealthTipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAAUHealthTipWidget(const FObjectInitializer& ObjectInitializer);

	static UAAUHealthTipWidget* ShowUI(const FString& Title, const FString& Content, const FString& ComfirmStr);

	void HiddenSwitchAccountBtn();

	TFunction<void()> ComformBlock;
	TFunction<void()> SwitchAccountBlock;
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSubmitBtnClick();

	UFUNCTION()
	void OnSwitchAccountClick();


private:
	UPROPERTY(meta = (BindWidget))
	UAAUBackViewWidget* BackView;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* ContentRichLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* SwitchAccountBtn;

	FString Title;
	FString Content;
	FString ComfirmStr;

	
};
