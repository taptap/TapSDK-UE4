#include "AAUImpl.h"
#include "AAUNet.h"
#include "AntiAddictionUE.h"
#include "TUDebuger.h"
#include "China/AAUChinaImpl.h"
#include "China/Android/AAUChinaAndroidImpl.h"
#include "Vietnam/AAUVietnamImpl.h"
#if PLATFORM_IOS
#include "China/iOS/AAUChinaIOSImpl.h"
#endif


TSharedPtr<AAUImpl> AAUImpl::Instance = nullptr;
FAAUConfig AAUImpl::Config;
FString AAUImpl::LocalTokenString = "standalone_anti_addiction_token";
int AAUImpl::AdultRemainTime = 9999;

TSharedPtr<AAUImpl>& AAUImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShareable(new AAUImpl);
	}
	return Instance;
}

bool AAUImpl::IsNeedStandAlone(TFunction<void(bool IsNeed)> CallBack) {
	if (AAUNet::RealNameServerIsCrash == false) {
		if (CallBack) {
			CallBack(false);
		}
		return false;
	}
	bool Enable = true;   // 目前比较宽松，尽量放开
	if (StandAloneEnable.IsValid()) {
		if (CallBack) {
			CallBack(*StandAloneEnable.Get());
		}
		return *StandAloneEnable.Get();
	}
	else {
		AAUNet::GetStaticSetting([=](TSharedPtr<FAAUSettingsModel> ModelPtr, const FTUError& Error) {
			if (ModelPtr.IsValid()) {
				StandAloneEnable = MakeShareable(new bool(ModelPtr->stand_alone_mode));
				if (CallBack) {
					CallBack(ModelPtr->stand_alone_mode);
				}
			}
			else {
				TUDebuger::WarningLog(
					FString::Printf(TEXT("Request Static Setting Fail: %s"), *Error.GetErrorReason()));
				if (CallBack) {
					CallBack(Enable);
				}
			}
		});
	}
	return Enable;
}

void AAUImpl::Init(const FAAUConfig& _Config) {
	Config = _Config;
	if (Config.Region == EAAURegion::China) {
#if PLATFORM_ANDROID
		Instance = MakeShareable(new AAUChinaAndroidImpl);
#elif  PLATFORM_IOS
		Instance = MakeShareable(new AAUChinaIOSImpl);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new AAUChinaImpl);
#endif
		Instance->InitImpl(_Config);
	}
	else if (Config.Region == EAAURegion::Vietnam) {
		Instance = MakeShareable(new AAUVietnamImpl);
		Instance->InitImpl(_Config);
	}
}

void AAUImpl::InitImpl(const FAAUConfig& _Config) {
	TUDebuger::ErrorLog("Unsupported");
}

void AAUImpl::Startup(const FString& UserID) {
	TUDebuger::ErrorLog("Unsupported");
}

void AAUImpl::Exit() {
	TUDebuger::ErrorLog("Unsupported");
}

EAAUAgeLimit AAUImpl::GetAgeRange() {
	TUDebuger::ErrorLog("Unsupported");
	return EAAUAgeLimit::Unknown;
}

int AAUImpl::GetRemainingTime() {
	TUDebuger::ErrorLog("Unsupported");
	return 0;
}

void AAUImpl::EnterGame() {
	TUDebuger::ErrorLog("Unsupported");
}

void AAUImpl::LeaveGame() {
	TUDebuger::ErrorLog("Unsupported");
}

void AAUImpl::CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack, TFunction<void(const FString& Msg)> FailureHandler) {
	TUDebuger::ErrorLog("Unsupported");
}

void AAUImpl::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	TUDebuger::ErrorLog("Unsupported");
}

FString AAUImpl::CurrentToken() {
	TUDebuger::ErrorLog("Unsupported");
	return "";
}



