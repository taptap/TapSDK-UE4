#pragma once

#include "CoreMinimal.h"
#include "AAUBackViewWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "AAUTipWidget.generated.h"

/**
 *  
 */
UCLASS()
class ANTIADDICTION_API UAAUTipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAAUTipWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowTip(const FString& Content, const FString& BtnStr, TFunction<void()> ClickBlock);
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSubmitBtnClick();


private:
	UPROPERTY(meta = (BindWidget))
	UAAUBackViewWidget* BackView;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ContentLabel;

	FString Content;

	FString BtnStr;

	TFunction<void()> ClickBlock;
	

	
	

	
};
