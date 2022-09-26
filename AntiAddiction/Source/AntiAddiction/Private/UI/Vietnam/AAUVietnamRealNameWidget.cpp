#include "AAUVietnamRealNameWidget.h"

#include "TUDebuger.h"
#include "TUSettings.h"
#include "Components/Border.h"
#include "Model/Vietnam/AAUVietnamConfigModel.h"
#include "Server/AAUHelper.h"
#include "Slate/SAAURealNameVietnam.h"

UAAUVietnamRealNameWidget::UAAUVietnamRealNameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

// TSharedPtr<SAAURealNameVietnam> UAAUVietnamRealNameWidget::ShowUI(const FOnRealNameClosed& OnClosed, const FOnRealNameSubmit& OnSubmit) {
// 	// if (UClass* MyWidgetClass = LoadClass<UAAUVietnamRealNameWidget>(nullptr, TEXT("WidgetBlueprint'/AntiAddiction/AAUVietnamRealNameBP.AAUVietnamRealNameBP_C'")))
// 	// {
// 	// 	if (TUSettings::GetGameInstance().IsValid()) {
// 	// 		auto widget = CreateWidget<UAAUVietnamRealNameWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
// 	// 		widget->AddToViewport(TUSettings::GetUILevel());
// 	// 		return widget;
// 	// 	}
// 	// }
// 	// return nullptr;
// }

void UAAUVietnamRealNameWidget::Dismiss() {
	RemoveFromParent();
}

void UAAUVietnamRealNameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto Info = FAAUVietnamConfigModel::GetLocalModel()->ui_config.input_realname_info;
	TitleLabel->SetText(FText::FromString(Info.title));
	DeteilLabel->SetText(FText::FromString(Info.description));
	SubButtonLabel->SetText(FText::FromString(Info.button));

	FDateTime CurrentTime = AAUHelper::GetVietnamCurrentTime();
	YearBox->ClearOptions();
	int32 MaxYear = FMath::Max(2022, CurrentTime.GetYear());
	for (int i = MaxYear-100; i <= MaxYear; i++) {
		YearBox->AddOption(FString::Printf(TEXT("%04d"), i));
	}
	YearBox->SetSelectedIndex(100 - 22);
	
	MonthBox->ClearOptions();
	for (int i = 1; i <= 12; i++) {
		MonthBox->AddOption(FString::Printf(TEXT("%02d"), i));
	}
	MonthBox->SetSelectedOption("06");
	
	UpdateDayBox();
	DayBox->SetSelectedOption("15");

	SubButton->OnClicked.AddDynamic(this, &UAAUVietnamRealNameWidget::OnSubmitBtnClick);
	CloseButton->OnClicked.AddDynamic(this, &UAAUVietnamRealNameWidget::OnCloseBtnClick);

	YearBox->OnSelectionChanged.AddDynamic(this, &UAAUVietnamRealNameWidget::OnYearBoxSelectionChanged);
	MonthBox->OnSelectionChanged.AddDynamic(this, &UAAUVietnamRealNameWidget::OnMonthBoxSelectionChanged);
	DayBox->OnSelectionChanged.AddDynamic(this, &UAAUVietnamRealNameWidget::OnDayBoxSelectionChanged);
}

void UAAUVietnamRealNameWidget::OnCloseBtnClick() {
	if (CloseBlock) {
		CloseBlock();
	}
	RemoveFromParent();
}

void UAAUVietnamRealNameWidget::OnSubmitBtnClick() {
	if (SubmitBlock) {
		SubmitBlock(YearBox->GetSelectedOption(), MonthBox->GetSelectedOption(), DayBox->GetSelectedOption());
	}
}

void UAAUVietnamRealNameWidget::OnYearBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {
	TUDebuger::DisplayLog(SelectedItem);
	UpdateDayBox();
}

void UAAUVietnamRealNameWidget::OnMonthBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {
	TUDebuger::DisplayLog(SelectedItem);
	UpdateDayBox();
}

void UAAUVietnamRealNameWidget::OnDayBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {
	TUDebuger::DisplayLog(SelectedItem);
}

void UAAUVietnamRealNameWidget::UpdateDayBox() {
	int32 Year = FCString::Atoi(*YearBox->GetSelectedOption());
	int32 Month = MonthBox->GetSelectedIndex() + 1;
	if (!(Month >= 1 && Month <= 12))
	{
		ensure(false);
		return;
	}
	int32 AllDays = FDateTime::DaysInMonth(Year, Month);
	int32 lastSelectIndex = DayBox->GetSelectedIndex();
	DayBox->ClearOptions();
	for (int i = 1; i <= AllDays; i++) {
		DayBox->AddOption(FString::Printf(TEXT("%02d"), i));
	}
	if (lastSelectIndex < 0) {
		DayBox->SetSelectedIndex(14);
	} else if (lastSelectIndex >= AllDays) {
		DayBox->SetSelectedIndex(AllDays-1);
	} else {
		DayBox->SetSelectedIndex(lastSelectIndex);
	}
}






