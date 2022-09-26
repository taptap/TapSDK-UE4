#include "AAUChinaAndroidImpl.h"

#include "AntiAddictionUE.h"
#include "Engine.h"
#include "TUDebuger.h"
#include "TUJsonHelper.h"
#include "TUMobileBridge.h"
#include "Android/AndroidApplication.h"
#include "Server/AAUHelper.h"

#define TAP_ANTI_CLZ "com.tapsdk.antiaddictionui.wrapper.TDSAntiAddictionUIService"
#define TAP_ANTI_IMPL "com.tapsdk.antiaddictionui.wrapper.TDSAntiAddictionUIServiceImpl"
#define TAP_ANTI_SERVICE "TDSAntiAddictionUIService"

AAUChinaAndroidImpl::~AAUChinaAndroidImpl() {
}

AAUChinaAndroidImpl::AAUChinaAndroidImpl() {
	TUMobileBridge::Register(TEXT(TAP_ANTI_CLZ),TEXT(TAP_ANTI_IMPL));
}


void AAUChinaAndroidImpl::Startup(const FString& UserID) {
	FString JsonOutString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("userID"), UserID);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_ANTI_SERVICE, "startup", JsonOutString);
}

void AAUChinaAndroidImpl::Exit() {
	TUMobileBridge::AsyncPerform(TAP_ANTI_SERVICE, "exit", "");
}

EAAUAgeLimit AAUChinaAndroidImpl::GetAgeRange() {
	FString ResultStr = TUMobileBridge::Perform(TAP_ANTI_SERVICE, "getAgeRange", "");
	ResultStr = ResultStr.Replace(TEXT("\""),TEXT(""));
	const int Num = FCString::Atod(*ResultStr);
	return AAUHelper::MakeAgeLimit(Num);
}

int AAUChinaAndroidImpl::GetRemainingTime() {
	FString ResultStr = TUMobileBridge::Perform(TAP_ANTI_SERVICE, "getRemainingTime", "");
	ResultStr = ResultStr.Replace(TEXT("\""),TEXT(""));
	return FCString::Atod(*ResultStr);
}

void AAUChinaAndroidImpl::InitImpl(const FAAUConfig& _Config) {
	FString JsonOutString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("clientID"), _Config.ClientID);
	Writer->WriteValue(TEXT("useTapLogin"), _Config.UseTapLogin);
	Writer->WriteValue(TEXT("showSwitchAccount"), _Config.ShowSwitchAccount); //安卓不用region, SDK里只有China
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAP_ANTI_SERVICE, "initImpl", JsonOutString, [=](const FString& ResultStr) {
		const TSharedPtr<FJsonObject> ResultObj = TUJsonHelper::GetJsonObject(ResultStr);
		if (ResultObj.IsValid()) {
			const int Code = ResultObj->GetIntegerField(TEXT("code"));
			switch (Code) {
			case AntiAddictionUE::LoginSuccess:
				AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::LoginSuccess, "");
				break;
			case AntiAddictionUE::Exited:
				AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::Exited, "");
				break;
			case AntiAddictionUE::SwitchAccount:
				AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::SwitchAccount, "");
				break;
			case AntiAddictionUE::DurationLimit:
				AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::DurationLimit, "");
				break;
			case AntiAddictionUE::PeriodRestrict:
				AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::PeriodRestrict, "");
				break;
			case AntiAddictionUE::RealNameStop:
				AntiAddictionUE::OnCallBack.ExecuteIfBound(AntiAddictionUE::RealNameStop, "");
				break;
			default:
				break;
			}
		}
	}, false);
}

void AAUChinaAndroidImpl::EnterGame() {
	TUMobileBridge::AsyncPerform(TAP_ANTI_SERVICE, "enterGame", "");
}

void AAUChinaAndroidImpl::LeaveGame() {
	TUMobileBridge::AsyncPerform(TAP_ANTI_SERVICE, "leaveGame", "");
}

void AAUChinaAndroidImpl::CheckPayLimit(int Amount, TFunction<void(bool Status)> CallBack,
                                        TFunction<void(const FString& Msg)> FailureHandler) {
	FString JsonOutString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("amount"), Amount);
	Writer->WriteObjectEnd();
	Writer->Close();

	TUMobileBridge::AsyncPerform(TAP_ANTI_SERVICE, "checkPayLimit", JsonOutString, [=](const FString& ResultStr) {
		if (CallBack == nullptr) {
			return;
		}
		const TSharedPtr<FJsonObject> ResultObj = TUJsonHelper::GetJsonObject(ResultStr);
		if (ResultObj.IsValid()) {
			const int code = ResultObj->GetIntegerField(TEXT("code"));
			const bool IsSuccess = code != -1; //sdk not error -1:抛出错误了
			if (IsSuccess) {
				CallBack(code == 1); //state:0,1
				return;
			}

			FString MsgStr;
			if (ResultObj->TryGetStringField(TEXT("msg"), MsgStr)) {
				if (FailureHandler) {
					FailureHandler(MsgStr);
				}
				return;
			}
		}

		if (FailureHandler) {
			FailureHandler("fail");
		}

	});
}

void AAUChinaAndroidImpl::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
                                          TFunction<void(const FString& Msg)> FailureHandler) {
	FString JsonOutString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("amount"), Amount);
	Writer->WriteObjectEnd();
	Writer->Close();

	TUMobileBridge::AsyncPerform(TAP_ANTI_SERVICE, "submitPayResult", JsonOutString, [=](const FString& ResultStr) {
		if (CallBack == nullptr) {
			return;
		}
		const TSharedPtr<FJsonObject> ResultObj = TUJsonHelper::GetJsonObject(ResultStr);
		if (ResultObj.IsValid()) {
			const bool IsSuccess = ResultObj->GetIntegerField(TEXT("code")) == 1;
			if (IsSuccess) {
				CallBack(true);
				return;
			}
			FString MsgStr;
			if (ResultObj->TryGetStringField(TEXT("msg"), MsgStr)) {
				if (FailureHandler) {
					FailureHandler(MsgStr);
				}
				return;
			}
		}
		if (FailureHandler) {
			FailureHandler("fail");
		}
	});
}

FString AAUChinaAndroidImpl::CurrentToken() {
	FString ResultStr = TUMobileBridge::Perform(TAP_ANTI_SERVICE, "currentToken", "");
	ResultStr = ResultStr.Replace(TEXT("\""),TEXT(""));
	return ResultStr;
}
