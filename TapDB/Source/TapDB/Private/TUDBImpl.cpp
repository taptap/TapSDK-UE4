#include "TUDBImpl.h"

#include "TUDebuger.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TUDBPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile//TUDBMobileImpl.h"
#endif

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("TUDBImpl Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<TUDBImpl> TUDBImpl::Instance = nullptr;

TSharedPtr<TUDBImpl>& TUDBImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TUDBPCImpl);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TUDBMobileImpl);
#else
		Instance = MakeShareable(new TUDBImpl);
#endif
	}
	return Instance;
}

void TUDBImpl::Init(FTUDBConfig InitConfig) {
	UnsupportedPlatformsLog
}

void TUDBImpl::SetUserWithLoginType(const FString& UserId, const FString& LoginType) {
	UnsupportedPlatformsLog
}

void TUDBImpl::ClearUser() {
	UnsupportedPlatformsLog
}

void TUDBImpl::SetName(const FString& Name) {
	UnsupportedPlatformsLog
}

void TUDBImpl::SetLevel(int Level) {
	UnsupportedPlatformsLog
}

void TUDBImpl::SetServer(const FString& Server) {
	UnsupportedPlatformsLog
}

void TUDBImpl::OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType,
	const FString& Payment, TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::RegisterStaticProperties(TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::UnregisterStaticProperty(const FString& Key) {
	UnsupportedPlatformsLog
}

void TUDBImpl::ClearStaticProperties() {
	UnsupportedPlatformsLog
}

void TUDBImpl::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock) {
	UnsupportedPlatformsLog
}

void TUDBImpl::DeviceInitialize(TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::DeviceUpdate(TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::DeviceAdd(TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::UserInitialize(TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::UserUpdate(TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::UserAdd(TSharedPtr<FJsonObject> Properties) {
	UnsupportedPlatformsLog
}

void TUDBImpl::AdvertiserIDCollectionEnabled(bool Enable) {
	UnsupportedPlatformsLog
}

void TUDBImpl::EnableLog(bool Enable) {
	UnsupportedPlatformsLog
}
