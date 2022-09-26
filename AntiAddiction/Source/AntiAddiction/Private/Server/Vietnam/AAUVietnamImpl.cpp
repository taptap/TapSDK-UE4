#include "AAUVietnamImpl.h"

#include "AAUVietnamRealName.h"
#include "AntiAddictionUE.h"
#include "TapCommon.h"
#include "TUDebuger.h"
#include "TUJsonHelper.h"
#include "TUSettings.h"
#include "Server/AAUHelper.h"
#include "Server/AAUNet.h"
#include "Server/AAUStorage.h"
#include "UI/AAUMobileTipWidget.h"
#include "UI/Vietnam/AAUVietnamRealNameWidget.h"

AAUVietnamImpl::AAUVietnamImpl() {
	Server = MakeShareable(new AAUVietnamServer);
	Server->Delegate = this;
}

AAUVietnamImpl::~AAUVietnamImpl() {
}

void AAUVietnamImpl::InitImpl(const FAAUConfig& _Config) {
	AAUNet::GetSDKConfig([](TSharedPtr<FAAUVietnamConfigModel> ModelPtr, const FTUError& Error) {
		if (ModelPtr.IsValid()) {
			TUDebuger::DisplayLog(TUJsonHelper::GetJsonString(ModelPtr));
			FAAUVietnamConfigModel::SaveToLocal(ModelPtr);
		} else {
			TUDebuger::WarningLog(FString::Printf(TEXT("Request AA Config Fail: %s"), *Error.GetErrorReason()));
		}
	});
}

void AAUVietnamImpl::EnterGame() {
	Server->EnterGame();
}

void AAUVietnamImpl::LeaveGame() {
	Server->LeaveGame();
}

void AAUVietnamImpl::Startup(const FString& UserID) {
	CurrentUserID = UserID;
	IsFirstRealNameSuccess = false;
	TSharedPtr<FAAUUser> LoginUser = TUDataStorage<FAAUStorage>::LoadStruct<FAAUUser>(FAAUStorage::VienamHasLoginedUser + UserID);

	FTapCommonModule::TapThrobberShowWait();
	AAUVietnamRealName::CheckRealNameState(UserID, [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error) {
		FTapCommonModule::TapThrobberDismiss();
		if (ModelPtr.IsValid()) {
			switch (ModelPtr->GetAuthState()) {
			case FAAURealNameResultModel::Success:
				Login(ModelPtr->anti_addiction_token, AAUHelper::MakeAgeLimit(ModelPtr->age_limit));
				break;
			default:
				ShowRealNameUI();
				break;
			}
		} else {
			if (LoginUser.IsValid() && !LoginUser->AccessToken.IsEmpty()) {
				Login(LoginUser->AccessToken, LoginUser->AgeLimit);
			} else {
				TryAgainLogin("");
			}
		}
	});
}

void AAUVietnamImpl::Exit() {
	AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::Exited, "");
	Server->Logout();
}

EAAUAgeLimit AAUVietnamImpl::GetAgeRange() {
	return Server->GetAgeLimit();
}

int AAUVietnamImpl::GetRemainingTime() {
	return Server->GetRemainTime();
}

void AAUVietnamImpl::CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	Server->CheckPayLimit(Amount, [=](bool Status, const FString& Title, const FString& Description) {
		if (CallBack) {
			CallBack(Status);
		}
		if (Status == false) {
			auto Widget = UAAUMobileTipWidget::ShowUI();
			auto ContentUI = FAAUVietnamConfigModel::GetLocalModel()->ui_config.pay_reminder;
			Widget->SetContent(Title, Description, ContentUI.button_confirm);
		}
	}, FailureHandler);
}

void AAUVietnamImpl::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	Server->SubmitPayResult(Amount, CallBack, FailureHandler);
}


void AAUVietnamImpl::KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) {
	if (Boundary == AAUTimeBoundaryTimeLimit || Boundary == AAUTimeBoundaryCurfew) {
		ShowTimeLimitUI([=]() {
#if WITH_EDITOR
			GEngine->Exec(nullptr, TEXT("QUIT_EDITOR"), *GLog);
#else
			FGenericPlatformMisc::RequestExit(true);
#endif
		}, true);
	}
	else {
		Exit();
	}
	
}

FString AAUVietnamImpl::CurrentToken() {
	return Server->CurrentToken();
}

void AAUVietnamImpl::Login(const FString& AccessToken, EAAUAgeLimit AgeLimit) {
	FAAUUser User;
	User.AccessToken = AccessToken;
	User.AgeLimit = AgeLimit;
	User.UserID = CurrentUserID;
	TUDataStorage<FAAUStorage>::SaveStruct(FAAUStorage::VienamHasLoginedUser + CurrentUserID, User);

	FTapCommonModule::TapThrobberShowWait();
	Server->Login(User, [=](const AAULoginResult& Result) {
		FTapCommonModule::TapThrobberDismiss();
		if (Result.LoginState == AAULoginResult::SuccessWithLimit || Result.LoginState ==
			AAULoginResult::SuccessWithNoLimit) {
			if (IsFirstRealNameSuccess) {
				IsFirstRealNameSuccess = false;
				auto Info = FAAUVietnamConfigModel::GetLocalModel()->ui_config.input_realname_info;
				FTapCommonModule::TapThrobberShowToast(Info.submit_success_message);
			}
			AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::LoginSuccess, "Success");
		}
		else {
			if (Result.LoginState == AAULoginResult::FailWithTimeout) {
				ShowTimeLimitUI([=]() {
					AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::SwitchAccount, "");
				}, false);
			}
			else {
				TryAgainLogin("");
				// UTUHUD::ShowToast(TEXT("Prevent addiction into failure"));
				// PerformLoginCallBack(false, "Time Limit");
			}
		}
	});
	
}

void AAUVietnamImpl::ShowRealNameUI()
{
	auto CloseBlock = [=]()
	{
		AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::RealNameStop, "");
	};
	auto SubmitBlock = [=](const FString& Year, const FString& Month, const FString& Day)
	{
		FTapCommonModule::TapThrobberShowWait();
		auto Callback = [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)
		{
			FTapCommonModule::TapThrobberDismiss();
			if (ModelPtr.IsValid())
			{
				switch (ModelPtr->GetAuthState())
				{
				case FAAURealNameResultModel::Success:
					{
						IsFirstRealNameSuccess = true;
						Login(ModelPtr->anti_addiction_token, AAUHelper::MakeAgeLimit(ModelPtr->age_limit));
						if (GEngine && GEngine->GameViewport && RealName)
						{
							GEngine->GameViewport->RemoveViewportWidgetContent(RealName.ToSharedRef());
						}
					}
					break;
				default:
					FTapCommonModule::TapThrobberShowToast("Vietnam GetAuthState Error");
					break;
				}
			}
			else
			{
				FTapCommonModule::TapThrobberShowToast("Network error, please try again");
			}
		};
		AAUVietnamRealName::ManualVerify(CurrentUserID, Year, Month, Day, Callback);
	};
	
	if (GEngine && GEngine->GameViewport)
	{
		auto Info = FAAUVietnamConfigModel::GetLocalModel()->ui_config.input_realname_info;

		RealName = SNew(SAAURealNameVietnam)
		.OnClosed(FOnRealNameClosed::CreateLambda(CloseBlock))
		.OnSubmit(FOnRealNameSubmit::CreateLambda(SubmitBlock))
		.Title(FText::FromString(Info.title))
		.Content(FText::FromString(Info.description))
		.SubmitText(FText::FromString(Info.button));
		
		GEngine->GameViewport->AddViewportWidgetContent(RealName.ToSharedRef(), TUSettings::GetUILevel());
	}
}

void AAUVietnamImpl::ShowTimeLimitUI(TFunction<void()> ComformBlock, bool IsExit) {
	AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::DurationLimit, "");
	auto Widget = UAAUMobileTipWidget::ShowUI();
	auto ContentUI = FAAUVietnamConfigModel::GetLocalModel()->ui_config.health_reminder;
	FString Title, Description, Button;
	Title = ContentUI.title;
	Description = ContentUI.description;
	if (IsExit) {
		Button = ContentUI.button_exit;
	} else {
		Button = ContentUI.button_switch;
	}
	Widget->SetContent(Title, Description, Button);
	Widget->ComformBlock = ComformBlock;
}

void AAUVietnamImpl::TryAgainLogin(const FString& ErrMsg) {
	FString Msg = TEXT("Network error, please try again");
	if (!ErrMsg.IsEmpty()) {
		Msg = ErrMsg;
	}
	auto Widget = UAAUMobileTipWidget::ShowUI();
	Widget->SetContent(Msg, "", "Retry");
	Widget->ComformBlock = [=]() {
		Startup(CurrentUserID);
	};
}
