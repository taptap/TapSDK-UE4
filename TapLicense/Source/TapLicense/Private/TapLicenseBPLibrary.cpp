// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapLicenseBPLibrary.h"
#include "TapLicense.h"
#include "TapBridge.h"
#include "TapCommon.h"
#include "TapJson.h"
#include "TapResult.h"
#include "TapError.h"
#include "TapCommonBPLibrary.h"
#include "Containers/UnrealString.h"
#include "Serialization/JsonWriter.h"

#define TAP_LICENSE_CLZ "com.taptap.pay.sdk.library.wrapper.TapLicenseService"
#define TAP_LICENSE_IMPL "com.taptap.pay.sdk.library.wrapper.TapLicenseServiceImpl"
#define TAP_LICENSE_SERVICE "TapLicenseService"

#define TAP_LICENSE_CALLBACK_ID "TAP_LICENSE_CALLBACK_ID"
#define TAP_DLC_CALLBACK_ID "TAP_DLC_CALLBACK_ID"

UTapLicenseBPLibrary::UTapLicenseBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(TEXT(TAP_LICENSE_CLZ),TEXT(TAP_LICENSE_IMPL));
    FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapLicenseBPLibrary::OnBridgeCallback);

	UTapLicenseBPLibrary::SetLicencesCallback();
	UTapLicenseBPLibrary::SetDLCCallback();
#endif
}

void UTapLicenseBPLibrary::SetLicencesCallback(){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LICENSE_SERVICE), TEXT("setLicenseCallback"), TEXT(""), true, TEXT(TAP_LICENSE_CALLBACK_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLicenseBPLibrary::SetDLCCallback(){
	UTapLicenseBPLibrary::SetDLCCallbackWithParams(false,TEXT(""));
}	

void UTapLicenseBPLibrary::SetDLCCallbackWithParams(bool checkOnce,FString publicKey){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("checkOnce"), checkOnce);
    Writer->WriteValue(TEXT("publicKey"),publicKey);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LICENSE_SERVICE), TEXT("setDLCCallbackWithParams"), JsonOutString, true, TEXT(TAP_DLC_CALLBACK_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLicenseBPLibrary::Check(){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LICENSE_SERVICE), TEXT("check"), TEXT(""), false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapLicenseBPLibrary::QueryDLC(TArray<FString> skus){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("dlcList"), skus);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LICENSE_SERVICE), TEXT("queryDLC"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapLicenseBPLibrary::PurchaseDLC(FString sku){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("dlc"), sku);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LICENSE_SERVICE), TEXT("purchaseDLC"), JsonOutString, false, TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLicenseBPLibrary::OnBridgeCallback(const FString &result){
#if PLATFORM_ANDROID || PLATFORM_IOS

	UE_LOG(LogTemp,Warning,TEXT("TapLicense OnBridgeCallback:%s"),*result);
    
    FTapResult tapResult;
    bool parseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FTapResult>(result,&tapResult,0,0);
    if(!parseSuccess){
        return;
    }

	if(tapResult.callbackId.Equals(TAP_LICENSE_CALLBACK_ID)){

		if(!UTapCommonBPLibrary::CheckResult(tapResult)){
			return;
		}

		TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> root;
		bool parseEnable = FJsonSerializer::Deserialize(reader,root);
		if(parseEnable){
			FString success = root->GetStringField("login");
			if(success.Equals(TEXT("success"))){
				FTapLicenseModule::OnLicenseSuccess.Broadcast();
			}
		}
		return;
	}

	if(tapResult.callbackId.Equals(TAP_DLC_CALLBACK_ID)){
		if(!UTapCommonBPLibrary::CheckResult(tapResult)){
			return;
		}
		TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> root;
		bool parseEnable = FJsonSerializer::Deserialize(reader,root);
		if(parseEnable){
			FString orderDlc = root->GetStringField("orderDLC");
			if(orderDlc.Len()>0){
				int32 statusCode = root->GetIntegerField("orderStatus");
				FTapLicenseModule::OnOrderCallback.Broadcast(orderDlc,statusCode);
				return;
			}		

			int32 code = root->GetIntegerField("queryCode");
			FString queryJson = root->GetStringField("queryRoot");
			FTapLicenseModule::OnQueryCallback.Broadcast(code,queryJson);
			return;
		}
	}

#endif
}