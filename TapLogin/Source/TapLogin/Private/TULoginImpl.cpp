#include "TULoginImpl.h"
#include "TUDebuger.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TULoginPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile/TULoginMobileImpl.h"
#endif

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<TULoginImpl> TULoginImpl::Instance = nullptr;

TSharedPtr<TULoginImpl>& TULoginImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TULoginPCImpl);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TULoginMobileImpl);
#else
		Instance = MakeShareable(new TULoginImpl);
#endif
	}
	return Instance;
}

void TULoginImpl::Init(FTULoginConfig _Config) {
	UnsupportedPlatformsLog
}

TSharedPtr<FTULoginProfileModel> TULoginImpl::GetProfile() {
	UnsupportedPlatformsLog
	return nullptr;
}

void TULoginImpl::FetchProfile(TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {
	UnsupportedPlatformsLog
	// if (CallBack) {
	// 	CallBack(nullptr, FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	// }
}

TSharedPtr<FTUAccessToken> TULoginImpl::GetAccessToken() {
	UnsupportedPlatformsLog
	return nullptr;
}

void TULoginImpl::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	UnsupportedPlatformsLog
	// if (CallBack) {
	// 	TUAuthResult Result = TUAuthResult::FailInit(FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	// 	CallBack(Result);
	// }
}

void TULoginImpl::Logout() {
	UnsupportedPlatformsLog
}

void TULoginImpl::GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) {
	UnsupportedPlatformsLog
	// if (CallBack) {
	// 	CallBack(false, FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	// }
}

void TULoginImpl::QueryMutualList(FString Cursor, int Size,
	TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack) {
	UnsupportedPlatformsLog
	// if (CallBack) {
	// 	CallBack(nullptr, FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	// }
}
