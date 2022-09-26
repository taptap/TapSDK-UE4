#include "TUCommonImpl.h"

#include "TUDebuger.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TUCommonPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile//TUCommonMobileImpl.h"
#endif

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<TUCommonImpl> TUCommonImpl::Instance = nullptr;

TSharedPtr<TUCommonImpl>& TUCommonImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TUCommonPCImpl);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TUCommonMobileImpl);
#else
		Instance = MakeShareable(new TUCommonImpl);
#endif
	}
	return Instance;
}

void TUCommonImpl::SetLanguage(ELanguageType LanguageType) {
	UnsupportedPlatformsLog
}
