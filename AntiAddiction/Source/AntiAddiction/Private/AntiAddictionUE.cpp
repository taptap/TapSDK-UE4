#include "AntiAddictionUE.h"

#include "TUDebuger.h"
#include "Model/Vietnam/AAUVietnamConfigModel.h"
#include "Server/AAUImpl.h"
#include "UI/AAUHealthTipWidget.h"

#if !UE_BUILD_SHIPPING
#include "TUSettings.h"
#include "UI/Vietnam/AAUVietnamRealNameWidget.h"
#endif

AntiAddictionUE::FCallBack AntiAddictionUE::OnCallBack;


void AntiAddictionUE::Init(const FAAUConfig& Config) {
	AAUImpl::Get()->Init(Config);
}

void AntiAddictionUE::Startup(const FString& UserID) {
	if (UserID.IsEmpty()) {
		TUDebuger::ErrorLog("AntiAddiction UserID is Empty");
		return;
	}
	AAUImpl::Get()->Startup(UserID);
}

void AntiAddictionUE::Exit() {
	AAUImpl::Get()->Exit();
}

EAAUAgeLimit AntiAddictionUE::GetAgeRange() {
	return AAUImpl::Get()->GetAgeRange();
}

int AntiAddictionUE::GetRemainingTimeInMinutes() {
	double Remaintime = GetRemainingTime();
	return FMath::CeilToDouble(Remaintime / 60);
}

int AntiAddictionUE::GetRemainingTime() {
	return AAUImpl::Get()->GetRemainingTime();
}

void AntiAddictionUE::EnterGame() {
	AAUImpl::Get()->EnterGame();
}

void AntiAddictionUE::LeaveGame() {
	AAUImpl::Get()->LeaveGame();
}

void AntiAddictionUE::CheckPayLimit(int Amount,TFunction<void(bool Status)> CallBack,TFunction<void(const FString& Msg)> FailureHandler) {
	AAUImpl::Get()->CheckPayLimit(Amount, CallBack, FailureHandler);
}

void AntiAddictionUE::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	AAUImpl::Get()->SubmitPayResult(Amount, CallBack, FailureHandler);
}

FString AntiAddictionUE::CurrentToken() {
	return AAUImpl::Get()->CurrentToken();
}
void AntiAddictionUE::Test()
{
#if !UE_BUILD_SHIPPING
	if (GEngine && GEngine->GameViewport)
	{
		auto Info = FAAUVietnamConfigModel::GetLocalModel()->ui_config.input_realname_info;

		TSharedRef<SAAURealNameVietnam> RealName = SNew(SAAURealNameVietnam)
		.Title(FText::FromString(Info.title))
		.Content(FText::FromString(Info.description))
		.SubmitText(FText::FromString(Info.button));
		
		GEngine->GameViewport->AddViewportWidgetContent(RealName, TUSettings::GetUILevel());
	}
#endif
}
