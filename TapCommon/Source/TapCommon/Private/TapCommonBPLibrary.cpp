// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapCommonBPLibrary.h"
#include "TapCommon.h"
#include "TapBridge.h"
#include "TapJson.h"
#include "TUError.h"


#define TAPCOMMON_REGION_CODE_ID "TAPCOMMON_REGION_CODE_ID"
#define TAPCOMMON_IS_TAPTAP_INSTALLED_ID "TAPCOMMON_IS_TAPTAP_INSTALLED_ID"
#define TAPCOMMON_IS_TAPGLOBAL_INSTALLED_ID "TAPCOMMON_IS_TAPGLOBAL_INSTALLED_ID"
#define TAPCOMMON_UPDATE_GAME_TAPTAP_ID "TAPCOMMON_UPDATE_GAME_TAPTAP_ID"
#define TAPCOMMON_UPDATE_GAME_TAPGLOBAL_ID "TAPCOMMON_UPDATE_GAME_TAPGLOBAL_ID"
#define TAPCOMMON_OPEN_GAME_TAPGLOBAL_ID "TAPCOMMON_OPEN_GAME_TAPGLOBAL_ID"
#define TAPCOMMON_OPEN_GAME_TAPTAP_ID "TAPCOMMON_OPEN_GAME_TAPTAP_ID"

UTapCommonBPLibrary::UTapCommonBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    UE_LOG(LogTemp, Warning, TEXT("init TapCommon"));
    Register(TEXT("com.tds.common.wrapper.TDSCommonService"),TEXT("com.tds.common.wrapper.TDSCommonServiceImpl"));
	FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapCommonBPLibrary::OnBridgeCallback);
#endif
}

void UTapCommonBPLibrary::Register(FString serviceClz, FString serviceImpl)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(serviceClz, serviceImpl);
#endif 
}

void UTapCommonBPLibrary::CallHandler(FString command)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    UE_LOG(LogTemp, Warning, TEXT("CallHandler: %s"), *command);
	GetBridge()->CallHandler(command);
#endif
}

// void UTapCommonBPLibrary::GetRegionCode()
// {
// #if PLATFORM_ANDROID || PLATFORM_IOS
//     FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"), TEXT("getRegionCode"), TEXT(""), true, TEXT(TAPCOMMON_REGION_CODE_ID),true);
// 	GetBridge()->CallHandler(commandJson);
// #endif
// }


// void UTapCommonBPLibrary::IsTapTapInstalled(){
// #if PLATFORM_ANDROID
//     FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("isTapTapInstalled"),TEXT(""),true,TEXT(TAPCOMMON_IS_TAPTAP_INSTALLED_ID),true);
//     GetBridge()->CallHandler(commandJson);
// #endif
// }

// void UTapCommonBPLibrary::IsTapGlobalInstalled(){
// #if PLATFORM_ANDROID
//     FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("isTapGlobalInstalled"),TEXT(""),true,TEXT(TAPCOMMON_IS_TAPGLOBAL_INSTALLED_ID),true);
//     GetBridge()->CallHandler(commandJson);
// #endif
// }

// void UTapCommonBPLibrary::UpdateGameInTapTap(FString appId){
// #if PLATFORM_ANDROID
//     FString JsonOutString;
//     TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
//     Writer->WriteObjectStart();
//     Writer->WriteValue(TEXT("appId"), appId);
//     Writer->WriteObjectEnd();
//     Writer->Close();
//     FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("updateGameInTapTap"),JsonOutString,true,TEXT(TAPCOMMON_UPDATE_GAME_TAPTAP_ID),true);
//     GetBridge()->CallHandler(commandJson);
// #endif
// }

// void UTapCommonBPLibrary::UpdateGameInTapGlobal(FString appId){
// #if PLATFORM_ANDROID
//     FString JsonOutString;
//     TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
//     Writer->WriteObjectStart();
//     Writer->WriteValue(TEXT("appId"), appId);
//     Writer->WriteObjectEnd();
//     Writer->Close();
//     FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("updateGameInTapGlobal"),JsonOutString,true,TEXT(TAPCOMMON_UPDATE_GAME_TAPGLOBAL_ID),true);
//     GetBridge()->CallHandler(commandJson);
// #endif
// }

// void UTapCommonBPLibrary::OpenReviewInTapTap(FString appId){
// #if PLATFORM_ANDROID
//     FString JsonOutString;
//     TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
//     Writer->WriteObjectStart();
//     Writer->WriteValue(TEXT("appId"), appId);
//     Writer->WriteObjectEnd();
//     Writer->Close();
//     FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("openReviewInTapTap"),JsonOutString,true,TEXT(TAPCOMMON_OPEN_GAME_TAPTAP_ID),true);
//     GetBridge()->CallHandler(commandJson);
// #endif
// }

// void UTapCommonBPLibrary::OpenReviewInTapGlobal(FString appId){
// #if PLATFORM_ANDROID
//     FString JsonOutString;
//     TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
//     Writer->WriteObjectStart();
//     Writer->WriteValue(TEXT("appId"), appId);
//     Writer->WriteObjectEnd();
//     Writer->Close();
//     FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("openReviewInTapGlobal"),JsonOutString,true,TEXT(TAPCOMMON_OPEN_GAME_TAPGLOBAL_ID),true);
//     GetBridge()->CallHandler(commandJson);
// #endif
// }

TMap<FString, FString> UTapCommonBPLibrary::RemoveEmptyKey(TMap<FString, FString> stringMap)
{
    // FTUError Error;
    // Error.code = 10;
    // Error.error_description = TEXT("哈哈__123'1；231_。。。。");
    // FString ErrorJson;
    // FJsonObjectConverter::UStructToJsonObjectString(FTUError::StaticStruct(), &Error, ErrorJson, 0, 0);
    // UE_LOG(LogTemp, Display, TEXT("hyf__%s"), *ErrorJson);
    stringMap.Remove("");
    return stringMap;
}

TArray<FString> UTapCommonBPLibrary::RemoveEmptyString(TArray<FString> stringArray)
{
    stringArray.Remove("");
    return stringArray;
}

void UTapCommonBPLibrary::OnBridgeCallback(const FString &result)
{
#if PLATFORM_ANDROID || PLATFORM_IOS

    UE_LOG(LogTemp,Warning,TEXT("TapCommon OnBridgeCallback:%s"),*result);
    
    FTapResult tapResult;
    bool parseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FTapResult>(result,&tapResult,0,0);
    if(!parseSuccess){
        return;
    }

	if(tapResult.callbackId.Equals(TAPCOMMON_REGION_CODE_ID))
	{
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            return;
        }
		FTapCommonRegionCode RegionCodeBean;
		bool JsonParseEnable = FJsonObjectConverter::JsonObjectStringToUStruct<FTapCommonRegionCode>(tapResult.content,&RegionCodeBean,0,0);
        if(!JsonParseEnable)
        {
            return;
        }
		FTapCommonModule::OnRegionCode.Broadcast(RegionCodeBean.isMainland);
        return;
	}

    if(tapResult.callbackId.Equals(TAPCOMMON_IS_TAPTAP_INSTALLED_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnTapTapInstalled.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> taptapInstallReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> taptapInstallRoot;
        bool bSuccessful = FJsonSerializer::Deserialize(taptapInstallReader, taptapInstallRoot);
        if(bSuccessful){
            FTapCommonModule::OnTapTapInstalled.Broadcast(taptapInstallRoot->GetBoolField("isTapTapInstalled"));
            return;
        }
        FTapCommonModule::OnTapTapInstalled.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_IS_TAPGLOBAL_INSTALLED_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnTapGlobalInstalled.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> tapIOInstallReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> tapIOInstallRoot;
        bool bSuccessful = FJsonSerializer::Deserialize(tapIOInstallReader, tapIOInstallRoot);
        if(bSuccessful){
            FTapCommonModule::OnTapGlobalInstalled.Broadcast(tapIOInstallRoot->GetBoolField("isTapGlobalInstalled"));
            return;
        }
        FTapCommonModule::OnTapGlobalInstalled.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_UPDATE_GAME_TAPTAP_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnUpdateGameInTapTap.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> updateTapReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> updateTapRoot;
        bool bSuccessful = FJsonSerializer::Deserialize(updateTapReader, updateTapRoot);
        if(bSuccessful){
            FTapCommonModule::OnUpdateGameInTapTap.Broadcast(updateTapRoot->GetBoolField("updateGameInTapTap"));
            return;
        }
        FTapCommonModule::OnUpdateGameInTapTap.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_UPDATE_GAME_TAPGLOBAL_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnUpdateGameInTapGlobal.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> updateTapIOReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> updateTapIORoot;
        bool bSuccessful = FJsonSerializer::Deserialize(updateTapIOReader, updateTapIORoot);
        if(bSuccessful){
            FTapCommonModule::OnUpdateGameInTapGlobal.Broadcast(updateTapIORoot->GetBoolField("updateGameInTapGlobal"));
            return;
        }
        FTapCommonModule::OnUpdateGameInTapGlobal.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_OPEN_GAME_TAPTAP_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnOpenReviewInTapTap.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> openTapReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> openTapRoot;
        bool bSuccessful = FJsonSerializer::Deserialize(openTapReader, openTapRoot);
        if(bSuccessful){
            FTapCommonModule::OnOpenReviewInTapTap.Broadcast(openTapRoot->GetBoolField("openReviewInTapTap"));
            return;
        }
        FTapCommonModule::OnOpenReviewInTapTap.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_OPEN_GAME_TAPGLOBAL_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnOpenReviewInTapGlobal.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> openTapIOReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> openTapIORoot;
        bool bSuccessful = FJsonSerializer::Deserialize(openTapIOReader, openTapIORoot);
        if(bSuccessful){
            FTapCommonModule::OnOpenReviewInTapGlobal.Broadcast(openTapIORoot->GetBoolField("openReviewInTapGlobal"));
            return;
        }
        FTapCommonModule::OnOpenReviewInTapGlobal.Broadcast(false);
        return;
    }
#endif 
}

bool UTapCommonBPLibrary::CheckResult(const FTapResult result)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    if(result.code !=0){
        return false;
    }
    return result.content.Len();
#endif
    return false;
}
