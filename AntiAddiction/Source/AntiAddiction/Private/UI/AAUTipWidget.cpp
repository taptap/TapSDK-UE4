#include "AAUTipWidget.h"

#include "TUSettings.h"


UAAUTipWidget::UAAUTipWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UAAUTipWidget::ShowTip(const FString& Content, const FString& BtnStr, TFunction<void()> ClickBlock) {
	if (UClass* MyWidgetClass = LoadClass<UAAUTipWidget>(nullptr, TEXT("WidgetBlueprint'/AntiAddiction/AAUTipUI.AAUTipUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UAAUTipWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->Content = Content;
			widget->BtnStr = BtnStr;
			widget->ClickBlock = ClickBlock;
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UAAUTipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BackView->HeadOverlay->SetVisibility(ESlateVisibility::Hidden);
	// BackView->SubButtonLabel->SetText(FText::FromString(TEXT("提交")));
	BackView->SubButton->OnClicked.AddUniqueDynamic(this, &UAAUTipWidget::OnSubmitBtnClick);
	ContentLabel->SetText(FText::FromString(Content));
	BackView->SubButtonLabel->SetText(FText::FromString(BtnStr));
}


void UAAUTipWidget::OnSubmitBtnClick() {
	if (ClickBlock) {
		ClickBlock();
	}
	RemoveFromParent();
}






