// Fill out your copyright notice in the Description page of Project Settings.
#include "TAULoginWidget.h"
#include "TULoginImpl.h"
#include "Desktop/TULoginLanguage.h"
#include "Desktop/Server/TULoginNet.h"
#include "TUHelper.h"
#include "TUDebuger.h"
#include "TUSettings.h"
#include "Desktop/TUWebAuthHelper.h"


UTAULoginWidget::UTAULoginWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UTAULoginWidget::ShowLoginUI(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> Completed) {
	if (UClass* MyWidgetClass = LoadClass<UTAULoginWidget>(nullptr, TEXT("WidgetBlueprint'/TapLogin/BPTapLoginUI.BPTapLoginUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UTAULoginWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->Permissions = Permissions;
			widget->Completed = Completed;
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UTAULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddUniqueDynamic(this, &UTAULoginWidget::OnCloseBtnClick);
	RefreshButton->OnClicked.AddUniqueDynamic(this, &UTAULoginWidget::OnRefreshBtnClick);
	JumpWebButton->OnClicked.AddUniqueDynamic(this, &UTAULoginWidget::OnJumpWebBtnClick);

	TitleUseLabel->SetText(FText::FromString(TULoginLanguage::GetCurrentLang()->TitleUse()));
	TitleLoginLabel->SetText(FText::FromString(TULoginLanguage::GetCurrentLang()->TitleLogin()));
	QrTitleLoginLabel->SetText(FText::FromString(TULoginLanguage::GetCurrentLang()->QrTitleLogin()));
	QrRefreshLabel->SetText(FText::FromString(TULoginLanguage::GetCurrentLang()->QrRefresh()));
	QrNoticeScanToLoginRichLabel->SetText(FText::FromString(TULoginLanguage::GetCurrentLang()->QrNoticeScanToLogin()));
	WebLoginLabel->SetText(FText::FromString(TULoginLanguage::GetCurrentLang()->WebLogin()));
	WebNoticeLabel->SetText(FText::FromString(TULoginLanguage::GetCurrentLang()->WebNotice()));
	WebButtonJumpToWebLabel->SetText(FText::FromString(TULoginLanguage::GetCurrentLang()->WebButtonJumpToWeb()));
	
	HiddenRefreshButton();

	GetQrCode();

	IsRequestingAccessToken = false;
	IsWaitRequestAccessToken = false;
}

void UTAULoginWidget::NativeDestruct() {
	Super::NativeDestruct();
	InvalidCheckScanTimer();
	if (WebAuthHelper.IsValid()) {
		WebAuthHelper->StopProcess();
		WebAuthHelper = nullptr;
	}

	if (TipTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(TipTimerHandle);
		TipTimerHandle.Invalidate();
	}
}

void UTAULoginWidget::OnCloseBtnClick()
{
	Close(TUAuthResult::CancelInit());
}

void UTAULoginWidget::OnRefreshBtnClick()
{
	HiddenRefreshButton();
	GetQrCode();
}

void UTAULoginWidget::OnJumpWebBtnClick()
{
	if (!WebAuthHelper.IsValid()) {
		WebAuthHelper = MakeShareable(new TUWebAuthHelper);
	}
	// static TauWebAuthHelper Helper(Permissions);
	WebAuthHelper->ProcessWebAuth(Permissions, [=](FString WebCode) {
		GetTokenFromWebCode(WebCode);
	});
	TUDebuger::DisplayShow("OnJumpWebBtnClick");
}

void UTAULoginWidget::ShowRefreshButton()
{
	RefreshButton->SetVisibility(ESlateVisibility::Visible);
	QRCoverView->SetVisibility(ESlateVisibility::Visible);
}

void UTAULoginWidget::HiddenRefreshButton()
{
	RefreshButton->SetVisibility(ESlateVisibility::Hidden);
	QRCoverView->SetVisibility(ESlateVisibility::Hidden);
}

void UTAULoginWidget::InvalidCheckScanTimer() {
	if (CheckScanTimer.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(CheckScanTimer);
	}
	CheckScanTimer.Invalidate();
}

void UTAULoginWidget::CheckScanRequest(int64 ExpireAt) {
	int64 Now = FDateTime::UtcNow().ToUnixTimestamp();
	if (Now > ExpireAt) {
		ShowRefreshButton();
		InvalidCheckScanTimer();
		return;
	}
	if (IsRequestingAccessToken) {
		IsWaitRequestAccessToken = true;
	}
	else {
		const TWeakObjectPtr<UTAULoginWidget> WeakSelf(this);
		IsRequestingAccessToken = true;
		IsWaitRequestAccessToken = false;
		TULoginNet::RequestAccessToken(QrCodeModel->device_code, [=](TSharedPtr<FTUAccessToken> Model, FTULoginError Error) {
			if (!IsValid(WeakSelf.Get()) || !IsInViewport()) {
				return;
			}
			if (Model.IsValid()) {
				GetProfile(Model);
				InvalidCheckScanTimer();
			} else {
				if (Error.error == "authorization_pending") {

				}
				else if (Error.error == "authorization_waiting") {
					ShowTip(TULoginLanguage::GetCurrentLang()->QrnNoticeSuccess(),
							TULoginLanguage::GetCurrentLang()->QrnNoticeSuccess2());
				}
				else if (Error.error == "access_denied") {
					ShowTip(TULoginLanguage::GetCurrentLang()->QrNoticeCancel(),
							TULoginLanguage::GetCurrentLang()->QrNoticeCancel2());
					GetQrCode();
					InvalidCheckScanTimer();
				}
				else if (Error.error == "invalid_grant") {
					ShowRefreshButton();
					InvalidCheckScanTimer();
				}
				else if (Error.error == "slow_down") {

				}
				else {
					ShowRefreshButton();
					InvalidCheckScanTimer();
				}
			}
			IsRequestingAccessToken = false;
			if (IsWaitRequestAccessToken && CheckScanTimer.IsValid()) {
				CheckScanRequest(ExpireAt);
			}
		});
	}
}

void UTAULoginWidget::ResetQrCode(const FString& Content)
{
	HiddenRefreshButton();
	auto texture = TUHelper::GenerateQrCode(Content);
	QRImage->SetBrushFromTexture(texture);
}

void UTAULoginWidget::ShowTip(const FString& Tip, const FString& SubTip)
{
	TipLabel->SetText(FText::FromString(Tip));
	SubTipLabel->SetText(FText::FromString(SubTip));
	GetWorld()->GetTimerManager().ClearTimer(TipTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TipTimerHandle,
		[=]()
		{
			TipLabel->SetText(FText::FromString(""));
			SubTipLabel->SetText(FText::FromString(""));
		}, 3.f, false);
}

void UTAULoginWidget::StartCheck() {
	if (!QrCodeModel.IsValid()) {
		ShowRefreshButton();
		return;
	}
	int64 ExpireAt = FDateTime::UtcNow().ToUnixTimestamp() + QrCodeModel->expires_in;
	InvalidCheckScanTimer();
	GetWorld()->GetTimerManager().SetTimer(CheckScanTimer, [=]() {
		CheckScanRequest(ExpireAt);
	}, QrCodeModel->interval, true);
	
}


void UTAULoginWidget::GetProfile(const TSharedPtr<FTUAccessToken>& AccessToken)
{
	const TWeakObjectPtr<UTAULoginWidget> WeakSelf(this);
	TULoginNet::RequestProfile(*AccessToken.Get(), [=](TSharedPtr<FTULoginProfileModel> Model, FTULoginError Error)
	{
		if (!WeakSelf.IsValid()) {
			return;
		}
		if (Model.IsValid())
		{
			AccessToken->SaveToLocal();
			Model->SaveToLocal();
			Close(TUAuthResult::SuccessInit(AccessToken));
		} else
		{
			FTUError TapError;
			TapError.code = Error.code;
			TapError.error_description = Error.error_description + ".\t" + "Get profile error";
			Close(TUAuthResult::FailInit(TapError));
		}
	});
}

void UTAULoginWidget::Close(const TUAuthResult& Result)
{
	RemoveFromParent();
	if (Completed)
	{
		Completed(Result);
	}
}

void UTAULoginWidget::GetQrCode()
{
	const TWeakObjectPtr<UTAULoginWidget> WeakSelf(this);
	TULoginNet::RequestLoginQrCode(Permissions,
	[=](TSharedPtr<FTUQrCodeModel> Model, FTULoginError Error)
	{
		if (!WeakSelf.IsValid()) {
			return;
		}
		if (Model.IsValid())
		{
			QrCodeModel = Model;
			ResetQrCode(Model->qrcode_url);
			StartCheck();
			TUDebuger::DisplayLog("QRCODE Get");
		} else
		{
			ShowRefreshButton();
			TUDebuger::WarningLog("QRCODE Get Fail");
		}
	});
}

void UTAULoginWidget::GetTokenFromWebCode(const FString& WebCode) {
	if (!WebAuthHelper.IsValid()) {
		TUDebuger::ErrorLog("WebAuthHelper is invalid, please check code");
		return;
	}
	TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
	Paras->SetStringField("client_id", TULoginImpl::Get()->Config.ClientID);
	Paras->SetStringField("grant_type", "authorization_code");
	Paras->SetStringField("secret_type", "hmac-sha-1");
	Paras->SetStringField("code", WebCode);
	Paras->SetStringField("redirect_uri", WebAuthHelper->RedirectUri);
	Paras->SetStringField("code_verifier", WebAuthHelper->GetCodeVerifier());

	ShowTip(TULoginLanguage::GetCurrentLang()->WebNoticeLogin(),"");
	const TWeakObjectPtr<UTAULoginWidget> WeakSelf(this);
	TULoginNet::RequestAccessTokenFromWeb(Paras, [=](TSharedPtr<FTUAccessToken> Model, FTULoginError Error) {
		if (!WeakSelf.IsValid()) {
			return;
		}
		if (Model.IsValid()) {
			GetProfile(Model);
		} else {
			TUDebuger::WarningLog("web login fail");
			ShowTip(TULoginLanguage::GetCurrentLang()->WebNoticeFail(), TULoginLanguage::GetCurrentLang()->WebNoticeFail2());
		}
	});
}



