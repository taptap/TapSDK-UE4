#include "LeanCloudImpl.h"

#include "Tools/LCDebuger.h"

#if PLATFORM_IOS

#include "iOS/LeanCloud_iOS.h"
typedef FLeanCloud_iOS LeanCloud;

#elif PLATFORM_ANDROID

#include "Android//LeanCloud_Android.h"
typedef FLeanCloud_Android LeanCloud;

#else
#include "PC/LeanCloud_PC.h"
typedef FLeanCloud_PC LeanCloud;
#endif

#define UnsupportedPlatformsLog LCDebuger::ErrorLog(FString::Printf(TEXT("LeanCloud Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<FLeanCloudImpl> FLeanCloudImpl::Instance;

TSharedPtr<FLeanCloudImpl> FLeanCloudImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShared<LeanCloud>();
	}
	return Instance;
}

void FLeanCloudImpl::Init(const FLCConfig& Config) {
	UnsupportedPlatformsLog
}
