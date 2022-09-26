#pragma once

#include "CoreMinimal.h"
#include "AAUBackViewWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/SizeBox.h"
#include "AAUMobileTipWidget.generated.h"

/**
 *  
 */
UCLASS()
class ANTIADDICTION_API UAAUMobileTipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAAUMobileTipWidget(const FObjectInitializer& ObjectInitializer);

	static UAAUMobileTipWidget* ShowUI();

	void SetContent(const FString& Title, const FString& Content, const FString& ComfirmStr);
	
	TFunction<void()> ComformBlock;
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSubmitBtnClick();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DetailLabel;

	UPROPERTY(meta = (BindWidget))
	USizeBox* ContentBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* SubmitBtn;

private:

	
};
