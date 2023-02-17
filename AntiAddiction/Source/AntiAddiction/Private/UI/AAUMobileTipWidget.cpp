#include "AAUMobileTipWidget.h"

#include "TUSettings.h"
#include "Server/AAUHelper.h"
#include "Server/AAUImpl.h"


UAAUMobileTipWidget::UAAUMobileTipWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


UAAUMobileTipWidget* UAAUMobileTipWidget::ShowUI() {
#if PLATFORM_WINDOWS || PLATFORM_MAC
	if (UClass* MyWidgetClass = LoadClass<UAAUMobileTipWidget>(nullptr, TEXT("WidgetBlueprint'/AntiAddiction/AAUTipViewBPPC.AAUTipViewBPPC_C'")))
#else
	if (UClass* MyWidgetClass = LoadClass<UAAUMobileTipWidget>(nullptr, TEXT("WidgetBlueprint'/AntiAddiction/AAUTipViewBP.AAUTipViewBP_C'")))
#endif
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UAAUMobileTipWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->AddToViewport(TUSettings::GetUILevel());
			return widget;
		}
	}
	return nullptr;
}

void UAAUMobileTipWidget::SetContent(const FString& Title, const FString& Content, const FString& ComfirmStr) {
	if (Title.IsEmpty()) {
		TitleLabel->SetVisibility(ESlateVisibility::Collapsed);
	} else {
		TitleLabel->SetVisibility(ESlateVisibility::Visible);
		TitleLabel->SetText(FText::FromString(Title));
	}
	if (Content.IsEmpty()) {
		ContentBox->SetVisibility(ESlateVisibility::Collapsed);
	} else {
		ContentBox->SetVisibility(ESlateVisibility::Visible);
		DetailLabel->SetText(FText::FromString(Content));
	}
	SubButtonLabel->SetText(FText::FromString(ComfirmStr));
}

void UAAUMobileTipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SubmitBtn->OnClicked.AddUniqueDynamic(this, &UAAUMobileTipWidget::OnSubmitBtnClick);

}


void UAAUMobileTipWidget::OnSubmitBtnClick() {
	if (ComformBlock) {
		ComformBlock();
	}
	RemoveFromParent();
}






