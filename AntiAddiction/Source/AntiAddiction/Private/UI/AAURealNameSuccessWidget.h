#pragma once

#include "CoreMinimal.h"
#include "AAUBackViewWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "AAURealNameSuccessWidget.generated.h"

/**
 *  
 */
UCLASS()
class ANTIADDICTION_API UAAURealNameSuccessWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAAURealNameSuccessWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowTip(TFunction<void()> CompleteBlock);
protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void OnSubmitBtnClick();

	UFUNCTION()
	void OnCloseBtnClick();

private:
	UPROPERTY(meta = (BindWidget))
	UAAUBackViewWidget* BackView;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TipLabel;

	TFunction<void()> CompleteBlock;
	
	int CountDownSec;

	FTimerHandle CountDownTimer;

	void UpdateCountDownLabel();

	void CloseAndPerformCompleteBlock();
};
