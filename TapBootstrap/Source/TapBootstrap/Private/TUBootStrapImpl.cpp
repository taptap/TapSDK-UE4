#include "TUBootStrapImpl.h"
#include "TUDebuger.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TUBootStrapPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile/TUBootStrapMobileImpl.h"
#endif

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<TUBootStrapImpl> TUBootStrapImpl::Instance = nullptr;

TSharedPtr<TUBootStrapImpl>& TUBootStrapImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TUBootStrapPCImpl);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TUBootStrapMobileImpl);
#else
		Instance = MakeShareable(new TUBootStrapImpl);
#endif
	}
	return Instance;
}

void TUBootStrapImpl::Init(const FTUConfig& Config) {
	UnsupportedPlatformsLog
}

void TUBootStrapImpl::Login(TArray<FString> Permissions, TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	UnsupportedPlatformsLog
	// if (FailBlock) {
	// 	FailBlock(FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	// }
}

void TUBootStrapImpl::AnonymouslyLogin(TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	UnsupportedPlatformsLog
	// if (FailBlock) {
	// 	FailBlock(FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	// }
}

void TUBootStrapImpl::Logout() {
	UnsupportedPlatformsLog
}

TSharedPtr<FTDSUser> TUBootStrapImpl::GetUser() {
	UnsupportedPlatformsLog
	return nullptr;
}

void TUBootStrapImpl::SetPreferLanguage(ELanguageType LangType) {
	UnsupportedPlatformsLog
}
