#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Slate/SAAURealNameVietnam.h"
#include "AAUVietnamRealNameWidget.generated.h"

/**
 *  
 */
UCLASS()
class ANTIADDICTION_API UAAUVietnamRealNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAAUVietnamRealNameWidget(const FObjectInitializer& ObjectInitializer);

	// static TSharedPtr<class SAAURealNameVietnam> ShowUI(const FOnRealNameClosed& OnClosed, const FOnRealNameSubmit& OnSubmit);
	void Dismiss();

	// 按钮回调，AAURealNameWordTypeVerify, AAURealNameWordTypeVerifyFail这两种状态下Name，CardID可用
	TFunction<void(const FString& Year, const FString& Month, const FString& Day)> SubmitBlock;

	TFunction<void()> CloseBlock;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnCloseBtnClick();

	UFUNCTION()
	void OnSubmitBtnClick();

	UFUNCTION()
	void OnYearBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnMonthBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnDayBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeteilLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SubButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubButtonLabel;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UComboBoxString* YearBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UComboBoxString* MonthBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UComboBoxString* DayBox;

private:

	void UpdateDayBox();
	
};
