#include "TUSupportImpl.h"
#include "TUDebuger.h"
#if PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile/TUSupportMobileImpl.h"
#elif PLATFORM_MAC || PLATFORM_WINDOWS
#include "PC/TUSupportPCImpl.h"
#endif


#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("TUSupportImpl Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<TUSupportImpl> TUSupportImpl::Instance;


TSharedPtr<TUSupportImpl>& TUSupportImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TUSupportMobileImpl);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TUSupportPCImpl);
#else
		Instance = MakeShareable(new TUSupportImpl);
#endif
	}
	return Instance;
}

void TUSupportImpl::Init(FTapSupportConfig _Config) {
	UnsupportedPlatformsLog
}

void TUSupportImpl::SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) {
	UnsupportedPlatformsLog
}

void TUSupportImpl::UpdateDefaultField(const FString& Key, const TSharedPtr<FJsonValue>& NewValue)
{
	UnsupportedPlatformsLog
}

void TUSupportImpl::LoginAnonymously(const FString& UserID) {
	UnsupportedPlatformsLog
}

void TUSupportImpl::Logout() {
	UnsupportedPlatformsLog
}

FString TUSupportImpl::GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> FieldsData) {
	UnsupportedPlatformsLog
	return "";
}

void TUSupportImpl::OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> FieldsData) {
	UnsupportedPlatformsLog
}

void TUSupportImpl::CloseSupportView() {
	UnsupportedPlatformsLog
}

void TUSupportImpl::Resume() {
	UnsupportedPlatformsLog
}

void TUSupportImpl::Pause() {
	UnsupportedPlatformsLog
}

void TUSupportImpl::FetchUnReadStatus() {
	UnsupportedPlatformsLog
}
