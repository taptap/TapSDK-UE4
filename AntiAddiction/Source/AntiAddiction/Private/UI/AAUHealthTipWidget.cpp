#include "AAUHealthTipWidget.h"

#include "TUSettings.h"
#include "Server/AAUHelper.h"
#include "Server/AAUImpl.h"


UAAUHealthTipWidget::UAAUHealthTipWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


UAAUHealthTipWidget* UAAUHealthTipWidget::ShowUI(const FString& Title, const FString& Content,
	const FString& ComfirmStr) {
	if (UClass* MyWidgetClass = LoadClass<UAAUHealthTipWidget>(nullptr, TEXT("WidgetBlueprint'/AntiAddiction/AAUHealthTipUI.AAUHealthTipUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UAAUHealthTipWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->Content = Content;
			widget->Title = Title;
			widget->ComfirmStr = ComfirmStr;
			widget->AddToViewport(TUSettings::GetUILevel());
			return widget;
		}
	}
	return nullptr;
}

void UAAUHealthTipWidget::HiddenSwitchAccountBtn() {
	SwitchAccountBtn->SetVisibility(ESlateVisibility::Hidden);
}

void UAAUHealthTipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BackView->SubButton->OnClicked.AddUniqueDynamic(this, &UAAUHealthTipWidget::OnSubmitBtnClick);
	SwitchAccountBtn->OnClicked.AddUniqueDynamic(this, &UAAUHealthTipWidget::OnSwitchAccountClick);
	BackView->CloseButton->SetVisibility(ESlateVisibility::Hidden);
	BackView->BackButton->SetVisibility(ESlateVisibility::Hidden);

	
	BackView->TitleLabel->SetText(FText::FromString(Title));
	ContentRichLabel->SetText(FText::FromString(AAUHelper::ReplaceHtmlTag(Content)));
	BackView->SubButtonLabel->SetText(FText::FromString(ComfirmStr));
	//
	// if (Type == AAUHealthTipTypeFirstLogin) {
	// 	BackView->SubButtonLabel->SetText(FText::FromString(TEXT("进入游戏")));
	// } else {
	// 	BackView->SubButtonLabel->SetText(FText::FromString(TEXT("退出游戏")));
	// }

	if (AAUImpl::Get()->Config.ShowSwitchAccount) {
		SwitchAccountBtn->SetVisibility(ESlateVisibility::Visible);
	} else {
		SwitchAccountBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UAAUHealthTipWidget::OnSubmitBtnClick() {
	if (ComformBlock) {
		ComformBlock();
	}
	RemoveFromParent();
}

void UAAUHealthTipWidget::OnSwitchAccountClick() {
	if (SwitchAccountBlock) {
		SwitchAccountBlock();
	}
	RemoveFromParent();
}






