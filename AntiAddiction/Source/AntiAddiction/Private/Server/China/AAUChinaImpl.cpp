#include "AAUChinaImpl.h"
#include "AAUChinaRealName.h"
#include "AntiAddictionUE.h"
#include "TapCommon.h"
#include "TUDebuger.h"
#include "TUHUD.h"
#include "Model/AAUUser.h"
#include "Server/AAUHelper.h"
#include "Server/AAUNet.h"
#include "Server/AAUStorage.h"
#include "UI/AAUHealthTipWidget.h"
#include "UI/AAURealNameSuccessWidget.h"
#include "UI/AAUTipWidget.h"

AAUChinaImpl::AAUChinaImpl() {
	Server = MakeShareable(new AAUChinaSever);
	Server->Delegate = this;
}

AAUChinaImpl::~AAUChinaImpl() {
}

void AAUChinaImpl::InitImpl(const FAAUConfig& _Config) {
	AAUNet::GetSDKConfig([](TSharedPtr<FAAUChinaConfigModel> ModelPtr, const FTUError& Error) {
		if (ModelPtr.IsValid()) {
			FAAUChinaConfigModel::SaveToLocal(ModelPtr);
		} else {
			TUDebuger::WarningLog(FString::Printf(TEXT("Request AA Config Fail: %s"), *Error.GetErrorReason()));
		}
	});
}

void AAUChinaImpl::Startup(const FString& UserID) {
	CurrentUserID = UserID;
	TSharedPtr<FAAUUser> LoginUser = TUDataStorage<FAAUStorage>::LoadStruct<FAAUUser>(FAAUStorage::HasLoginedUser + UserID);

	FTapCommonModule::TapThrobberShowWait();
	auto ResultBlock = [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error) {
		FTapCommonModule::TapThrobberDismiss();
		if (ModelPtr.IsValid()) {
			switch (ModelPtr->GetAuthState()) {
			case FAAURealNameResultModel::Success: {
				Login(ModelPtr->anti_addiction_token, AAUHelper::MakeAgeLimit(ModelPtr->age_limit));
			}
				break;
			case FAAURealNameResultModel::Verifying:
				ShowRealNameUI(nullptr, AAURealNameWordTypeVerifying);
				break;
			case FAAURealNameResultModel::Fail:
				if (ModelPtr->has_auth_record) {
					ShowRealNameUI(nullptr, AAURealNameWordTypeVerifyFail);
				} else {
					ShowRealNameUI(nullptr, AAURealNameWordTypeVerify);
				}
				break;
			}
		} else {
			IsNeedStandAlone([=](bool IsNeed) {
				if (LoginUser.IsValid() && !LoginUser->AccessToken.IsEmpty()) {
					if (LoginUser->AccessToken == LocalTokenString && !IsNeed) {
						// TUDebuger::DisplayLog(FString::Printf(TEXT("haha:%s - %d"), *LoginUser->AccessToken, IsNeed));
						TryAgainLogin(Error.GetErrorReason());
						return;
					}
					Login(LoginUser->AccessToken, LoginUser->AgeLimit);
				}
				else {
					if (IsNeed) {
						ShowRealNameUI(nullptr, AAURealNameWordTypeVerify);
						return;
					}
					TryAgainLogin(Error.GetErrorReason());
				}
			});
		}
	};
	
	if (LoginUser.IsValid() && LoginUser->AccessToken == LocalTokenString) {
		FString SubmitCardName = TUDataStorage<FAAUStorage>::LoadString(FAAUStorage::SubmitCardName + UserID);
		FString SubmitCardID = TUDataStorage<FAAUStorage>::LoadString(FAAUStorage::SubmitCardID + UserID);
		if (!SubmitCardName.IsEmpty() && !SubmitCardID.IsEmpty()) {
			AAUChinaRealName::ManualVerify(UserID, SubmitCardName, SubmitCardID, ResultBlock);
			return;
		}
	}
	AAUChinaRealName::CheckRealNameState(UserID, ResultBlock);
}

void AAUChinaImpl::Exit() {
	AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::Exited, "");
	Server->Logout();
}

EAAUAgeLimit AAUChinaImpl::GetAgeRange() {
	return Server->GetAgeRange();
}

int AAUChinaImpl::GetRemainingTime() {
	return Server->GetRemainingTime();
}

void AAUChinaImpl::KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) {
	if (Boundary == AAUTimeBoundaryCurfew) {
		ShowHealthTipUI(Title, Content, 0, AAUHealthTipTypeInCurfew);
	} else if (Boundary == AAUTimeBoundaryTimeLimit) {
		ShowHealthTipUI(Title, Content, 0, AAUHealthTipTypeTimeout);
	} else {
		Exit();
	}
}

void AAUChinaImpl::EnterGame() {
	Server->EnterGame();
}

void AAUChinaImpl::LeaveGame() {
	Server->LeaveGame();
}

void AAUChinaImpl::CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	Server->CheckPayLimit(Amount, [=](bool Status, const FString& Title, const FString& Description) {
		if (CallBack) {
			CallBack(Status);
		}
		if (Status == false) {
			auto Widget = UAAUHealthTipWidget::ShowUI(Title, Description, TEXT("返回游戏"));
			Widget->HiddenSwitchAccountBtn();
		}
	}, FailureHandler);
}

void AAUChinaImpl::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	Server->SubmitPayResult(Amount, CallBack, FailureHandler);
}

FString AAUChinaImpl::CurrentToken() {
	return Server->CurrentToken();
}

void AAUChinaImpl::TryAgainLogin(const FString& ErrMsg) {
	FString Msg = TEXT("网络错误，请重试");
	if (!ErrMsg.IsEmpty()) {
		Msg = ErrMsg;
	}
	UAAUTipWidget::ShowTip(Msg, TEXT("重试"), [=]() {
		Startup(CurrentUserID);
	});
}

void AAUChinaImpl::Login(const FString& AccessToken, EAAUAgeLimit AgeLimit, bool IsFirst) {
	FAAUUser User;
	User.AccessToken = AccessToken;
	User.AgeLimit = AgeLimit;
	User.UserID = CurrentUserID;
	TUDataStorage<FAAUStorage>::SaveStruct(FAAUStorage::HasLoginedUser + CurrentUserID, User);
	auto LoginBlock = [=]() {
		FTapCommonModule::TapThrobberShowWait();
		Server->Login(User, [=](const AAULoginResult& Result) {
			FTapCommonModule::TapThrobberDismiss();
			if (Result.LoginState == AAULoginResult::SuccessWithNoLimit) {
				AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::LoginSuccess, "Success");
			} else if(Result.LoginState == AAULoginResult::SuccessWithLimit) {
				ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeFirstLogin);
			}
			else {
				if (Result.LoginState == AAULoginResult::FailWithCurfew) {
					ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeInCurfew);
				}
				else if (Result.LoginState == AAULoginResult::FailWithTimeout) {
					ShowHealthTipUI(Result.Title, Result.Description, Result.RemainTime, AAUHealthTipTypeTimeout);
				}
				else if (Result.LoginState == AAULoginResult::FailWithOther) {
					FTapCommonModule::TapThrobberShowToast(TEXT("防沉迷进入失败"));
					Exit();
				}
				else {
					FTapCommonModule::TapThrobberShowToast(TEXT("防沉迷进入失败"));
				}
			}
		});
	};
	if (IsFirst) {
		UAAURealNameSuccessWidget::ShowTip(LoginBlock);
	} else {
		LoginBlock();
	}
	
}

void AAUChinaImpl::ShowRealNameUI(UAAUManualRealNameWidget *Widget, AAURealNameWordType Type) {
	if (Widget == nullptr) {
		Widget = UAAUManualRealNameWidget::ShowUI(Type);
	} else {
		Widget->ChangeType(Type);
	}
	Widget->CloseBlock = [=]() {
		AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::RealNameStop, "");
	};
	if (Type == AAURealNameWordTypeVerifying) {
		Widget->SubmitBlock = [=](const FString& Name, const FString& CardID) {
			// PerformLoginCallBack(false, "RealName Verifying");
			Widget->Dismiss();
		};
	} else if(Type == AAURealNameWordTypeVerify || Type == AAURealNameWordTypeVerifyFail) {
		Widget->SubmitBlock = [=](const FString& Name, const FString& CardID) {
			if (Name.IsEmpty()) {
				Widget->ShowError(TEXT("姓名不能为空"));
				return;
			}
			if (CardID.IsEmpty()) {
				Widget->ShowError(TEXT("身份信息不能为空"));
				return;
			}
			FTapCommonModule::TapThrobberShowWait();
			AAUChinaRealName::ManualVerify(CurrentUserID, Name, CardID, [=](TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error) {
				FTapCommonModule::TapThrobberDismiss();
				if (ModelPtr.IsValid()) {
					switch (ModelPtr->GetAuthState()) {
					case FAAURealNameResultModel::Success: {
						// 判断是否是单机校验的身份证，如果是，缓存身份证给下次登录校验
						if (ModelPtr->anti_addiction_token == LocalTokenString) {
							TUDataStorage<FAAUStorage>::SaveString(FAAUStorage::SubmitCardName + CurrentUserID, Name);
							TUDataStorage<FAAUStorage>::SaveString(FAAUStorage::SubmitCardID + CurrentUserID, CardID);
						}
						Login(ModelPtr->anti_addiction_token, AAUHelper::MakeAgeLimit(ModelPtr->age_limit), true);
						Widget->Dismiss();
					}
					break;
					case FAAURealNameResultModel::Verifying:
						ShowRealNameUI(Widget, AAURealNameWordTypeVerifying);
						break;
					case FAAURealNameResultModel::Fail:
						Widget->ShowError(TEXT("认证未通过，请提交真实信息"));
						break;
					}
				}
				else {
					Widget->ShowError(Error.GetErrorReason());
				}
			});
		};
	}
}

void AAUChinaImpl::ShowHealthTipUI(const FString& Title, const FString& Content, int RemainTime, AAUHealthTipType Type) {
	if (Type == AAUHealthTipTypeTimeout || Type == AAUHealthTipTypeInCurfew) {
		if (Type == AAUHealthTipTypeTimeout) {
			AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::DurationLimit, "");
		} else {
			AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::PeriodRestrict, "");
		}
		auto Widget = UAAUHealthTipWidget::ShowUI(Title, Content, TEXT("退出游戏"));
		Widget->SwitchAccountBlock = [=]() {
			AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::SwitchAccount, "");
		};
		Widget->ComformBlock = [=]() {
#if WITH_EDITOR
			GEngine->Exec(nullptr, TEXT("QUIT_EDITOR"), *GLog);
#else
			FGenericPlatformMisc::RequestExit(true);
#endif
		};
	} else if (Type == AAUHealthTipTypeFirstLogin) {
		FString Temp = Content.Replace(TEXT("# ${remaining} #"), *FString::Printf(TEXT("%i"), RemainTime/60));
		auto Widget = UAAUHealthTipWidget::ShowUI(Title, Temp, TEXT("进入游戏"));
		Widget->HiddenSwitchAccountBtn();
		Widget->ComformBlock = []() {
			AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::LoginSuccess, "Success");
		};
	}

}
