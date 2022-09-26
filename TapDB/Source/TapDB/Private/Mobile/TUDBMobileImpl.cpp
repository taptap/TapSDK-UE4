#include "TUDBMobileImpl.h"

#include "TUJsonHelper.h"
#include "TUMobileBridge.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"
#endif

#if PLATFORM_IOS
#import <TapDB/TapDB.h>
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"

#include "Core.h"
#include "IOSAppDelegate.h"

#endif

#define TAPDB_SERVICE "TDSTapDBService"
#define TAPDB_SERVICE_CLZ "com.tds.tapdb.wrapper.TapDBService"
#define TAPDB_SERVICE_IMPL "com.tds.tapdb.wrapper.TapDBServiceImpl"

static TFunction<TSharedPtr<FJsonObject>()> _PropertiesBlock = nullptr;

TUDBMobileImpl::TUDBMobileImpl() {
	TUMobileBridge::Register(TAPDB_SERVICE_CLZ, TAPDB_SERVICE_IMPL);
}

void TUDBMobileImpl::Init(FTUDBConfig InitConfig) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("clientId"), InitConfig.ClientId);
	Writer->WriteValue(TEXT("channel"), InitConfig.Channel);
	Writer->WriteValue(TEXT("isCN"), InitConfig.RegionType == ERegionType::CN);
	Writer->WriteValue(TEXT("gameVersion"), InitConfig.GameVersion);
	Writer->WriteObjectEnd();
	Writer->Close();

	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "init", JsonOutString);
}

void TUDBMobileImpl::SetUserWithLoginType(const FString& UserId, const FString& LoginType) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("userId"), UserId);
	if (LoginType.Len() > 0) {
		Writer->WriteValue(TEXT("loginType"), LoginType);
	}
	Writer->WriteObjectEnd();
	Writer->Close();

	FString MethodStr = LoginType.Len() > 0 ? "setUserWithParams" : "setUser";
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, MethodStr, JsonOutString);
}

void TUDBMobileImpl::ClearUser() {
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "clearUser", "");
}

void TUDBMobileImpl::SetName(const FString& Name) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("name"), Name);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "setName", JsonOutString);
}

void TUDBMobileImpl::SetLevel(int Level) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("level"), Level);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "setLevel", JsonOutString);
}

void TUDBMobileImpl::SetServer(const FString& Server) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("server"), Server);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "setServer", JsonOutString);
}

void TUDBMobileImpl::OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType,
	const FString& Payment, TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("orderId"), OrderId);
	Writer->WriteValue(TEXT("product"), Product);
	Writer->WriteValue(TEXT("amount"), Amount);
	Writer->WriteValue(TEXT("currencyType"), CurrencyType);
	Writer->WriteValue(TEXT("payment"), Payment);
	Writer->WriteValue(TEXT("properties"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "onChargeWithProperties", JsonOutString);
}

void TUDBMobileImpl::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("eventName"), EventName);
	Writer->WriteValue(TEXT("properties"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "trackEvent", JsonOutString);
}

void TUDBMobileImpl::RegisterStaticProperties(TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("registerStaticProperties"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "registerStaticProperties", JsonOutString);
}

void TUDBMobileImpl::UnregisterStaticProperty(const FString& Key) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("unregisterStaticProperty"), Key);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "unregisterStaticProperty", JsonOutString);
}

void TUDBMobileImpl::ClearStaticProperties() {
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "clearStaticProperties", "");
}

void TUDBMobileImpl::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock) {
#if PLATFORM_IOS

	[TapDB registerDynamicProperties:^NSDictionary * _Nonnull {
		FString ueJson = "";
		if (PropertiesBlock) {
			TSharedPtr<FJsonObject> Properties = PropertiesBlock();
			ueJson = TUJsonHelper::GetJsonString(Properties);
		}
		NSString* dynamicProperties = ueJson.GetNSString();
		NSData*jsonData = [dynamicProperties dataUsingEncoding:NSUTF8StringEncoding];
		NSError*err;
		NSDictionary*dic = [NSJSONSerialization JSONObjectWithData:jsonData
												   options:NSJSONReadingMutableContainers
													 error:&err];
		return dic;
	}];
#else
	_PropertiesBlock = PropertiesBlock;
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "registerDynamicPropertiesUE", "");
#endif
}

void TUDBMobileImpl::DeviceInitialize(TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("deviceInitialize"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "deviceInitialize", JsonOutString);
}

void TUDBMobileImpl::DeviceUpdate(TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("deviceUpdate"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "deviceUpdate", JsonOutString);
}

void TUDBMobileImpl::DeviceAdd(TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("deviceAdd"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "deviceAdd", JsonOutString);
}

void TUDBMobileImpl::UserInitialize(TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("userInitialize"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "userInitialize", JsonOutString);
}

void TUDBMobileImpl::UserUpdate(TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("userUpdate"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "userUpdate", JsonOutString);
}

void TUDBMobileImpl::UserAdd(TSharedPtr<FJsonObject> Properties) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("userAdd"), TUJsonHelper::GetJsonString(Properties));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "userAdd", JsonOutString);
}

void TUDBMobileImpl::AdvertiserIDCollectionEnabled(bool Enable) {
#if PLATFORM_IOS
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("advertiserIDCollectionEnabled"), Enable);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "advertiserIDCollectionEnabled", JsonOutString);
#else
	TUDBImpl::AdvertiserIDCollectionEnabled(Enable);
#endif
	
}

void TUDBMobileImpl::EnableLog(bool Enable) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("enableLog"), Enable);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPDB_SERVICE, "enableLog", JsonOutString);
}

#if PLATFORM_ANDROID

#ifdef __cplusplus
extern "C"
{
#endif

	__attribute__((visibility("default"))) jstring Java_com_tds_tapdb_wrapper_TapDBServiceImpl_GetDynamicProperties(JNIEnv *env,jclass thiz){
		FString dynamicProperties = "";
		if (_PropertiesBlock) {
			TSharedPtr<FJsonObject> Properties = _PropertiesBlock();
			dynamicProperties = TUJsonHelper::GetJsonString(Properties);
		}
		jstring result = env->NewStringUTF(TCHAR_TO_UTF8(*dynamicProperties));
		return result;
	}

#ifdef __cplusplus
}
#endif

#endif
