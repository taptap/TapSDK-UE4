#include "AAUVietnamServer.h"
#include "TUDebuger.h"
#include "TUSettings.h"
#include "Server/AAUHelper.h"
#include "Server/AAUImpl.h"
#include "Server/AAUNet.h"

static int64 UploadSpan = 120;


void AAUVietnamServer::Login(const FAAUUser& User, TFunction<void(const AAULoginResult& Result)> CallBack) {
	if (CallBack == nullptr) {
		TUDebuger::ErrorLog("AAUVietnamServer::Login CallBack can not be nullptr");
		return;
	}
	ResetValues();
	
	AAUHelper::GetTimeSpanWithSeverTime([=](int64 _TimeSpan) {
		this->TimeSpan = _TimeSpan;

		AAUNet::CheckPlayable(User.UserID, User.AccessToken, {}, {},
		[=](TSharedPtr<FAAUPlayableModel> ModelPtr, const FTUError& Error) {
			if (ModelPtr.IsValid()) {
				AAULoginResult Result;
				Result.Title = ModelPtr->title;
				Result.Description = ModelPtr->description;
				Result.RemainTime = ModelPtr->remain_time;
				this->RemainTime = ModelPtr->remain_time;
				this->CostTime = ModelPtr->cost_time;

				if (User.AgeLimit == EAAUAgeLimit::Adult) {
					Result.LoginState = AAULoginResult::SuccessWithNoLimit;
					LoginSuccess(User);
					CallBack(Result);
				}
				else {
					if (ModelPtr->remain_time > 0) {
						Result.LoginState = AAULoginResult::SuccessWithLimit;
						LoginSuccess(User);
						CallBack(Result);
					}
					else {
						// 不在判断restrict_type类型，remain_time == 0 就可玩时长耗尽
						Result.LoginState = AAULoginResult::FailWithTimeout;
						CallBack(Result);
					}
				}
			}
			else {
				// 如果不是被踢出，且是成年人，网络请求错误，也是登录成功
				if (Error.code != 16 && User.AgeLimit == EAAUAgeLimit::Adult) {
					RemainTime = AAUImpl::AdultRemainTime;
					AAULoginResult Result;
					Result.LoginState = AAULoginResult::SuccessWithNoLimit;
					LoginSuccess(User);
					CallBack(Result);
				}
				else {
					AAULoginResult Result;
					Result.LoginState = AAULoginResult::FailWithOther;
					CallBack(Result);
				}
			}
		}, true);
	});
}

void AAUVietnamServer::EnterGame() {
	if (!CurrentUser.IsValid()) {
		return;
	}
	if (IsInGame == true) {
		return;
	}
	IsInGame = true;
	TimeRecordBegin = GetCurrentTime();
	TimeRecordEnd = TimeRecordBegin;
	StartTimer();
}

void AAUVietnamServer::LeaveGame() {
	if (!CurrentUser.IsValid()) {
		return;
	}
	if (IsInGame == false) {
		return;
	}
	IsInGame = false;
	UpdateCostAndRemainTime();// 离开游戏时，更新下时长
	UploadTime(true);// 给服务端更新下时长
	StopTimer();
}

void AAUVietnamServer::Logout() {
	if (!CurrentUser.IsValid()) {
		return;
	}
	LeaveGame();
	CurrentUser = nullptr;
}

int AAUVietnamServer::GetRemainTime() {
	if (!CurrentUser.IsValid()) {
		return 0;
	}
	if (CurrentUser->AgeLimit == EAAUAgeLimit::Adult) {
		return AAUImpl::AdultRemainTime;
	}
	if (IsInGame) {
		UpdateCostAndRemainTime();
	}
	return RemainTime;
}

EAAUAgeLimit AAUVietnamServer::GetAgeLimit() {
	if (!CurrentUser.IsValid()) {
		return EAAUAgeLimit::Unknown;
	}
	return CurrentUser->AgeLimit;
}

FString AAUVietnamServer::CurrentToken() {
	if (!CurrentUser.IsValid()) {
		return "";
	}
	return CurrentUser->AccessToken;
}

void AAUVietnamServer::CheckPayLimit(int Amount,
	TFunction<void(bool Status, const FString& Title, const FString& Description)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	if (!CurrentUser.IsValid()) {
		if (FailureHandler) {
			FailureHandler(TEXT("Not logged in yet. Please log in first"));
		}
		return;
	}
	AAUNet::CheckPayment(Amount, *CurrentUser.Get(), [=](TSharedPtr<FAAUPayableModel> ModelPtr, const FTUError& Error) {
		if (ModelPtr.IsValid()) {
			if (CallBack) {
				CallBack(ModelPtr->status, ModelPtr->title, ModelPtr->description);
			}
		}
		else {
			if (FailureHandler) {
				FailureHandler(Error.error_description);
			}
		}
	});
}

void AAUVietnamServer::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	if (!CurrentUser.IsValid()) {
		if (FailureHandler) {
			FailureHandler(TEXT("Not logged in yet. Please log in first"));
		}
		return;
	}
	AAUNet::SetPayment(Amount, *CurrentUser.Get(), [=](TSharedPtr<FAAUPaymentModel> ModelPtr, const FTUError& Error) {
		if (ModelPtr.IsValid()) {
			if (CallBack) {
				CallBack(true);
			}
		}
		else {
			if (FailureHandler) {
				FailureHandler(Error.error_description);
			}
		}
	});
}

void AAUVietnamServer::ResetValues() {
	IsInGame = false;
	PlayableServerCrashCount = 0;
	CostTime = 0;
	RemainTime = 0;
	TimeRecordBegin = 0;
	TimeRecordEnd = 0;
	StopTimer();
	CurrentUser = nullptr;
}

void AAUVietnamServer::StartUploadTimer() {
	if (!FAAUVietnamConfigModel::GetLocalModel()->upload_user_action) {
		return;
	}
	TUSettings::GetGameInstance()->GetTimerManager().SetTimer(UploadTimer, [=]() {
		UpdateCostAndRemainTime();
		UploadTime(false);
	}, UploadSpan, true);
}

void AAUVietnamServer::StartTimeLimitTimer() {
	TUSettings::GetGameInstance()->GetTimerManager().SetTimer(TimeLimitTimer, [=]() {
		if (CurrentUser->AgeLimit == EAAUAgeLimit::Adult) {
			return;
		}
		UpdateCostAndRemainTime();
		if (PlayableServerCrashCount >= 3 || !FAAUVietnamConfigModel::GetLocalModel()->upload_user_action) {
			if (RemainTime <= 0) {
				auto Tip = FAAUVietnamConfigModel::GetLocalModel()->ui_config.health_reminder;
				KickOut(AAUTimeBoundaryTimeLimit, Tip.title, Tip.description);
			}
			else {
				StartTimeLimitTimer();
			}
		}
		else {
			UploadTime(false);
		}
	}, FMath::Max<int>(1, RemainTime), false);
}

FDateTime AAUVietnamServer::GetCurrentTime() {
	return AAUHelper::GetVietnamCurrentTime() + FTimespan(TimeSpan * ETimespan::TicksPerSecond);
}

void AAUVietnamServer::LoginSuccess(const FAAUUser& User) {
	auto LoginTime = GetCurrentTime();
	CurrentUser = MakeShareable(new FAAUUser(User));
	EnterGame();
}

void AAUVietnamServer::UpdateCostAndRemainTime() {
	auto Now = GetCurrentTime();
	auto Span = (Now - TimeRecordEnd).GetTotalSeconds();
	TimeRecordEnd = Now;
	RemainTime -= Span;
	CostTime += Span;
}

void AAUVietnamServer::UploadTime(bool IsPure) {
	// 如果是成年人不用上报及判断
	if (CurrentUser->AgeLimit == EAAUAgeLimit::Adult) {
		return;
	}
	static bool IsUploading = false;
	if (IsUploading) {
		return;
	}
	IsUploading = true;
	
	AAUNet::CheckPlayable(CurrentUser->UserID, CurrentUser->AccessToken, TArray<TArray<int>>(), TArray<TArray<int>>(), [=](TSharedPtr<FAAUPlayableModel> ModelPtr,  const FTUError& Error) {
		IsUploading = false;
		if (ModelPtr.IsValid()) {
			PlayableServerCrashCount = 0;
	
			this->RemainTime = ModelPtr->remain_time;
			this->CostTime = ModelPtr->cost_time;
			if (IsPure) {
				return;
			}

			// 踢出
			if (RemainTime <= 0) {
				if (ModelPtr->restrict_type == 1) {
					KickOut(AAUTimeBoundaryCurfew, ModelPtr->title, ModelPtr->description);
				}
				else {
					KickOut(AAUTimeBoundaryTimeLimit, ModelPtr->title, ModelPtr->description);
				}
			}
			else if (this->RemainTime <= UploadSpan) {
				StartTimeLimitTimer();
			}
		}
		else {
			PlayableServerCrashCount++;
			if (IsPure) {
				return;
			}
			if (Error.code == 16) {
				// 服务强制踢出
				KickOut(AAUTimeBoundaryNoLimit, TEXT("Health Tip"), Error.GetErrorReason());
				return;
			}
			StartTimeLimitTimer();
		}
	}, false);
}

void AAUVietnamServer::KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) {
	IsInGame = false;
	StopTimer();
	CurrentUser = nullptr;
	if (Delegate) {
		Delegate->KickOut(Boundary, Title, Content);
	}
}

void AAUVietnamServer::StopTimer() {
	if (UploadTimer.IsValid()) {
		TUSettings::GetGameInstance()->GetTimerManager().ClearTimer(UploadTimer);
	}
	UploadTimer.Invalidate();

	if (TimeLimitTimer.IsValid()) {
		TUSettings::GetGameInstance()->GetTimerManager().ClearTimer(TimeLimitTimer);
	}
	TimeLimitTimer.Invalidate();
}

void AAUVietnamServer::StartTimer() {
	// 如果是成年人不用上报及判断
	if (!CurrentUser.IsValid() || CurrentUser->AgeLimit == EAAUAgeLimit::Adult) {
		return;
	}
	StartUploadTimer();
	if (RemainTime <= UploadSpan) {
		StartTimeLimitTimer();
	}
}



