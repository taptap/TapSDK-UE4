// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapDBBPLibrary.h"
#include "TapDB.h"
#include "TapBridge.h"
#include "TapCommon.h"
#include "TapJson.h"
#include "TapCommonBPLibrary.h"
#include "Containers/UnrealString.h"
#include "Serialization/JsonWriter.h"

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

UTapDBBPLibrary::UTapDBBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(TEXT(TAPDB_SERVICE_CLZ),TEXT(TAPDB_SERVICE_IMPL));
#endif
}

void UTapDBBPLibrary::Init(FString clientID,FString channel,FString gameVersion,bool isCN){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("clientId"), clientID);
    Writer->WriteValue(TEXT("channel"), channel);
    Writer->WriteValue(TEXT("isCN"), isCN);
    Writer->WriteValue(TEXT("gameVersion"), gameVersion);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("init"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapDBBPLibrary::SetUser(FString userId){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("userId"), userId);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("setUser"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapDBBPLibrary::SetUserWithLoginType(FString userId,FString loginType){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("userId"), userId);
    Writer->WriteValue(TEXT("loginType"), loginType);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("setUserWithParams"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::SetName(FString name){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("name"), name);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("setName"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::SetLevel(int level){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("level"), level);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("setLevel"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::SetServer(FString server){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("server"), server);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("setServer"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::OnCharge(FString orderId,FString product,int32 amount,FString currencyType,FString payment){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("orderId"), orderId);
    Writer->WriteValue(TEXT("product"), product);
    Writer->WriteValue(TEXT("amount"), amount);
    Writer->WriteValue(TEXT("currencyType"), currencyType);
    Writer->WriteValue(TEXT("payment"), payment);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("onCharge"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::TrackEvent(FString eventName,FString properties){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("eventName"), eventName);
    Writer->WriteValue(TEXT("properties"), properties);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("trackEvent"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapDBBPLibrary::RegisterDynamicProperties(TapDBDynamicProperties* properties){
#if PLATFORM_ANDROID
    *dbDynamicProperties = *properties;

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("registerDynamicPropertiesUE"), TEXT(""), false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#elif PLATFORM_IOS

        [TapDB registerDynamicProperties:^NSDictionary * _Nonnull{
        FString ueJson = properties->GetDynamicProperties();
        NSString* dynamicProperties = ueJson.GetNSString();
        NSData*jsonData = [dynamicProperties dataUsingEncoding:NSUTF8StringEncoding];
        NSError*err;
        NSDictionary*dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                       options:NSJSONReadingMutableContainers
                                                         error:&err];
        return dic;
    }];
#endif
}

void UTapDBBPLibrary::RegisterStaticProperties(FString properties){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("registerStaticProperties"), properties);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("registerStaticProperties"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::UnregisterStaticProperty(FString key){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("unregisterStaticProperty"), key);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("unregisterStaticProperty"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::ClearStaticProperties(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("clearStaticProperties"), TEXT(""), false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif	
}

void UTapDBBPLibrary::DeviceInitialize(FString properties){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("deviceInitialize"), properties);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("deviceInitialize"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::DeviceUpdate(FString properties){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("deviceUpdate"), properties);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("deviceUpdate"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::DeviceAdd(FString properties){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("deviceAdd"), properties);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("deviceAdd"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapDBBPLibrary::UserInitialize(FString properties){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("userInitialize"), properties);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("userInitialize"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}
	

void UTapDBBPLibrary::UserUpdate(FString properties){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("userUpdate"), properties);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("userUpdate"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}
	

 void UTapDBBPLibrary::UserAdd(FString properties){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("userAdd"), properties);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("userAdd"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
 }


 void UTapDBBPLibrary::ClearUser(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("clearUser"), TEXT(""), false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif	
 }
	

void UTapDBBPLibrary::AdvertiserIDCollectionEnabled(bool enable){
#if PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("advertiserIDCollectionEnabled"), enable);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAPDB_SERVICE), TEXT("advertiserIDCollectionEnabled"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

#if PLATFORM_ANDROID

#ifdef __cplusplus
extern "C"
{
#endif

    jstring Java_com_tds_tapdb_wrapper_TapDBServiceImpl_GetDynamicProperties(JNIEnv *env,jclass thiz){
        TapDBDynamicProperties* properties = nullptr; 
        * properties = * UTapDBBPLibrary::dbDynamicProperties;
        FString dynamicProperties = properties->GetDynamicProperties();
        jstring result = env->NewStringUTF(TCHAR_TO_UTF8(*dynamicProperties));
        return result;
    }

#ifdef __cplusplus
}
#endif

#endif