#include "TapUEDB.h"

#include "TUDBImpl.h"
#include "TUDebuger.h"

#if PLATFORM_MAC || PLATFORM_WINDOWS
static bool IsInitialized = false;
#define JudgeIsInitialized   \
		if (!IsInitialized) { \
    		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__))); \
    		return; \
    	}
#else
#define JudgeIsInitialized
#endif


void TapUEDB::Init(FTUDBConfig Config) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
	if (IsInitialized) {
		TUDebuger::WarningShow("Has Initialized");
		return;
	}
	IsInitialized = true;
#endif
	TUDBImpl::Get()->Init(Config);
}

void TapUEDB::SetUserWithLoginType(const FString& UserId, const FString& LoginType) {
	JudgeIsInitialized
	TUDBImpl::Get()->SetUserWithLoginType(UserId, LoginType);
}

void TapUEDB::ClearUser() {
	JudgeIsInitialized
	TUDBImpl::Get()->ClearUser();
}

void TapUEDB::SetName(const FString& Name) {
	JudgeIsInitialized
	TUDBImpl::Get()->SetName(Name);
}

void TapUEDB::SetLevel(int Level) {
	JudgeIsInitialized
	TUDBImpl::Get()->SetLevel(Level);
}

void TapUEDB::SetServer(const FString& Server) {
	JudgeIsInitialized
	TUDBImpl::Get()->SetServer(Server);
}

void TapUEDB::OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType,
	const FString& Payment, TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->OnCharge(OrderId, Product, Amount, CurrencyType, Payment, Properties);
}

void TapUEDB::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->TrackEvent(EventName, Properties);
}

void TapUEDB::RegisterStaticProperties(TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->RegisterStaticProperties(Properties);
}

void TapUEDB::UnregisterStaticProperty(const FString& Key) {
	JudgeIsInitialized
	TUDBImpl::Get()->UnregisterStaticProperty(Key);
}

void TapUEDB::ClearStaticProperties() {
	JudgeIsInitialized
	TUDBImpl::Get()->ClearStaticProperties();
}

void TapUEDB::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock) {
	JudgeIsInitialized
	TUDBImpl::Get()->RegisterDynamicProperties(PropertiesBlock);
}

void TapUEDB::DeviceInitialize(TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->DeviceInitialize(Properties);
}

void TapUEDB::DeviceUpdate(TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->DeviceUpdate(Properties);
}

void TapUEDB::DeviceAdd(TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->DeviceAdd(Properties);
}

void TapUEDB::UserInitialize(TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->UserInitialize(Properties);
}

void TapUEDB::UserUpdate(TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->UserUpdate(Properties);
}

void TapUEDB::UserAdd(TSharedPtr<FJsonObject> Properties) {
	JudgeIsInitialized
	TUDBImpl::Get()->UserAdd(Properties);
}

void TapUEDB::AdvertiserIDCollectionEnabled(bool Enable) {
	JudgeIsInitialized
	TUDBImpl::Get()->AdvertiserIDCollectionEnabled(Enable);
}

void TapUEDB::EnableLog(bool Enable) {
	TUDBImpl::Get()->EnableLog(Enable);
}
