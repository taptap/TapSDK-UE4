#include "AAURealNameSuccessWidget.h"

#include "TUSettings.h"


UAAURealNameSuccessWidget::UAAURealNameSuccessWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UAAURealNameSuccessWidget::ShowTip(TFunction<void()> CompleteBlock) {
	if (UClass* MyWidgetClass = LoadClass<UAAURealNameSuccessWidget>(nullptr, TEXT("WidgetBlueprint'/AntiAddiction/AAURealNameSuccessUI.AAURealNameSuccessUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UAAURealNameSuccessWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->CompleteBlock = CompleteBlock;
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}


void UAAURealNameSuccessWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// BackView->CloseButton->SetVisibility(ESlateVisibility::Hidden);
	BackView->TitleLabel->SetText(FText::FromString(TEXT("游戏实名认证")));
	BackView->BackButton->SetVisibility(ESlateVisibility::Hidden);
	BackView->SubButtonLabel->SetText(FText::FromString(TEXT("返回游戏")));
	BackView->SubButton->OnClicked.AddUniqueDynamic(this, &UAAURealNameSuccessWidget::OnSubmitBtnClick);
	BackView->CloseButton->OnClicked.AddUniqueDynamic(this, &UAAURealNameSuccessWidget::OnCloseBtnClick);

	CountDownSec = 5;

	UpdateCountDownLabel();
	GetWorld()->GetTimerManager().SetTimer(CountDownTimer, [=]() {
		CountDownSec--;
		UpdateCountDownLabel();
		if (CountDownSec == 0) {
			CloseAndPerformCompleteBlock();
		}
	}, 1, true);
	
}

void UAAURealNameSuccessWidget::NativeDestruct() {
	Super::NativeDestruct();
	if (CountDownTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(CountDownTimer);
		CountDownTimer.Invalidate();
	}
}


void UAAURealNameSuccessWidget::OnSubmitBtnClick() {
	CloseAndPerformCompleteBlock();
}

void UAAURealNameSuccessWidget::OnCloseBtnClick() {
	CloseAndPerformCompleteBlock();
}

void UAAURealNameSuccessWidget::UpdateCountDownLabel() {
	FString Tip = FString::Printf(TEXT("即将关闭当前页面（%ds）"), CountDownSec);
	TipLabel->SetText(FText::FromString(Tip));
}

void UAAURealNameSuccessWidget::CloseAndPerformCompleteBlock() {
	if (CompleteBlock) {
		CompleteBlock();
	}
	RemoveFromParent();
}






