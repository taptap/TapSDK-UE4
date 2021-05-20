// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapMomentBPLibrary.h"
#include "TapMoment.h"
#include "TapBridge.h"
#include "TapCommon.h"
#include "TapJson.h"
#include "TapResult.h"
#include "TapError.h"
#include "TapCommonBPLibrary.h"
#include "Containers/UnrealString.h"
#include "Serialization/JsonWriter.h"

#define TAP_MOMENT_CLZ "com.tapsdk.moment.wrapper.TapMomentService"
#define TAP_MOMENT_IMPL "com.tapsdk.moment.wrapper.TapMomentServiceImpl"
#define TAP_MOMENT_SERVICE "TapMomentService"
#define TAP_MOMENT_CALLBACK_ID "TAP_MOMENT_CALLBACK_ID"

UTapMomentBPLibrary::UTapMomentBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(TEXT(TAP_MOMENT_CLZ),TEXT(TAP_MOMENT_IMPL));
    FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapMomentBPLibrary::OnBridgeCallback);
#endif
}

void UTapMomentBPLibrary::SetCallback(){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("setCallback"), TEXT(""), true, TEXT(TAP_MOMENT_CALLBACK_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::FetchNotification(){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("fetchNotification"), TEXT(""), false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::Open(int config){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString args;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&args);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("config"), config);
    Writer->WriteObjectEnd();
    Writer->Close();
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("open"), args , false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::OpenWithImagePath(int config,TArray<FString> imagePath,FString content){
#if PLATFORM_ANDROID || PLATFORM_IOS

	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("imagePaths"), imagePath);
    Writer->WriteValue(TEXT("config"), config);
    Writer->WriteValue(TEXT("content"), content);
    Writer->WriteObjectEnd();
    Writer->Close();

	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("publish"), JsonOutString , false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::OpenWithMedia(int config,TArray<FString> imagePath,TArray<FString> videoPath,FString title,FString content){
#if PLATFORM_ANDROID || PLATFORM_IOS

	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("imagePaths"), imagePath);
    Writer->WriteValue(TEXT("videoPaths"), videoPath);
    Writer->WriteValue(TEXT("config"), config);
    Writer->WriteValue(TEXT("title"), title);
    Writer->WriteValue(TEXT("desc"), content);
    Writer->WriteObjectEnd();
    Writer->Close();

	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("publishVideoImage"), JsonOutString , false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::OpenWithVidePath(int config,TArray<FString> videoPath,FString title,FString content){
#if PLATFORM_ANDROID || PLATFORM_IOS

	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("videoPaths"), videoPath);
    Writer->WriteValue(TEXT("config"), config);
    Writer->WriteValue(TEXT("title"), title);
    Writer->WriteValue(TEXT("desc"), content);
    Writer->WriteObjectEnd();
    Writer->Close();

	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("publishVideo"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::DirectlyOpen(int config,FString page,FString extras){
#if PLATFORM_ANDROID || PLATFORM_IOS

	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("config"), config);
    Writer->WriteValue(TEXT("page"), page);
    Writer->WriteValue(TEXT("extras"), extras);
    Writer->WriteObjectEnd();
    Writer->Close();

	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("directlyOpen"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::Close(){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("close"), TEXT("") , false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::CloseWithParams(FString title,FString content){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("title"), title);
    Writer->WriteValue(TEXT("content"), content);
    Writer->WriteObjectEnd();
    Writer->Close();
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_MOMENT_SERVICE), TEXT("closeWithConfirmWindow"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapMomentBPLibrary::OnBridgeCallback(const FString &result){
#if PLATFORM_ANDROID || PLATFORM_IOS
    UE_LOG(LogTemp,Warning,TEXT("onBridgeCallback:%s"),*result);
    TSharedPtr<FJsonObject> JsonObject = TapJson::Parse(result);
    FString callbackId = JsonObject->GetStringField("callbackId");
    if(callbackId.Equals(TAP_MOMENT_CALLBACK_ID))
    {
        if (JsonObject != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Moment Callback JSON parse success"));
            TSharedPtr<FJsonObject> ArgsObject = TapJson::Parse(JsonObject->GetStringField(TEXT("content")));
            if (ArgsObject != nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("Moment Callback JSON Args parse success"));
                int code = ArgsObject->GetIntegerField(TEXT("code"));
                FString message = ArgsObject->GetStringField(TEXT("message"));
                FTapMomentModule::OnMomentCallback.Broadcast(code, message);
            }
        }
    }
#endif
}

