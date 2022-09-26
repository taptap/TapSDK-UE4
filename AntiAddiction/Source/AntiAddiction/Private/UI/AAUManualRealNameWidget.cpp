#include "AAUManualRealNameWidget.h"

#include "TUSettings.h"
#include "Server/AAUHelper.h"


UAAUManualRealNameWidget::UAAUManualRealNameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

UAAUManualRealNameWidget * UAAUManualRealNameWidget::ShowUI(AAURealNameWordType Type) {
	if (UClass* MyWidgetClass = LoadClass<UAAUManualRealNameWidget>(nullptr, TEXT("WidgetBlueprint'/AntiAddiction/AAUManualRealNameUI.AAUManualRealNameUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UAAUManualRealNameWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->Type = Type;
			widget->AddToViewport(TUSettings::GetUILevel());
			return widget;
		}
	}
	return nullptr;
}

void UAAUManualRealNameWidget::Dismiss() {
	RemoveFromParent();
}

void UAAUManualRealNameWidget::ShowError(const FString& ErrorMsg) {
	ErrorTipView->SetVisibility(ESlateVisibility::Visible);
	ErrorTipLabel->SetText(FText::FromString(ErrorMsg));
}

void UAAUManualRealNameWidget::ChangeType(AAURealNameWordType _Type) {
	Type = _Type;
	FAAUChinaAuthIdentifyWord CurrentWord;
	for (auto Auth_Identify_Word : FAAUChinaConfigModel::GetLocalModel()->ui_config.auth_identify_words) {
		if (FCString::Atoi(*Auth_Identify_Word.type) == Type) {
			CurrentWord = Auth_Identify_Word;
			break;
		}
	}
	BackView->TitleLabel->SetText(FText::FromString(CurrentWord.title));
	ContentRichLabel->SetText(FText::FromString(AAUHelper::ReplaceHtmlTag(CurrentWord.description)));
	BackView->SubButtonLabel->SetText(FText::FromString(CurrentWord.positive_button));

	if (Type != AAURealNameWordTypeVerify && Type != AAURealNameWordTypeVerifyFail) {
		InputView->SetVisibility(ESlateVisibility::Collapsed);
	} else {
		InputView->SetVisibility(ESlateVisibility::Visible);
	}

	ErrorTipView->SetVisibility(ESlateVisibility::Hidden);
}

void UAAUManualRealNameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// BackView->TitleLabel->SetText(FText::FromString("hahahaha"));
	
	BackView->BackButton->OnClicked.AddUniqueDynamic(this, &UAAUManualRealNameWidget::OnBackBtnClick);
	BackView->CloseButton->OnClicked.AddUniqueDynamic(this, &UAAUManualRealNameWidget::OnCloseBtnClick);
	BackView->SubButton->OnClicked.AddUniqueDynamic(this, &UAAUManualRealNameWidget::OnSubmitBtnClick);

	ChangeType(Type);
}

void UAAUManualRealNameWidget::OnBackBtnClick() {
	if (BackBlock) {
		BackBlock();
	}
}

void UAAUManualRealNameWidget::OnCloseBtnClick() {
	if (CloseBlock) {
		CloseBlock();
	}
	RemoveFromParent();
}

void UAAUManualRealNameWidget::OnSubmitBtnClick() {
	ErrorTipView->SetVisibility(ESlateVisibility::Hidden);
	if (SubmitBlock) {
		SubmitBlock(NameTF->GetText().ToString(), CardIDTF->GetText().ToString());
	}
}






