// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapBootstrapBPLibrary.h"
#include "TapBootstrap.h"
#include "TapBridge.h"
#include "TapCommon.h"
#include "TapUser.h"
#include "TapJson.h"
#include "TapResult.h"
#include "TapError.h"
#include "TapCommonBPLibrary.h"
#include "TapBridgeWrapper.h"
#include "Containers/UnrealString.h"
#include "Serialization/JsonWriter.h"

/** Bootstrap ServiceName and Android Platform packageName */

#define TAP_BOOTSTRAP_CLZ "com.tapsdk.bootstrap.wrapper.TapBootstrapService"
#define TAP_BOOTSTRAP_IMPL "com.tapsdk.bootstrap.wrapper.TapBootstrapServiceImpl"
#define TAP_BOOTSTRAP_SERVICE "TapBootstrapService"

/** Bootstrap Method Id ID used to distinguish the method */

#define TAP_BOOTSTRAP_REGISTER_STATUS_ID "TAP_BOOTSTRAP_REGISTER_STATUS_ID"
#define TAP_BOOTSTRAP_REGISTER_LOGIN_ID "TAP_BOOTSTRAP_REGISTER_LOGIN_ID"
#define TAP_BOOTSTRAP_GET_USER_ID  "TAP_BOOTSTRAP_GET_USER_ID"
#define TAP_BOOTSTRAP_GET_DETAIL_USER_ID  "TAP_BOOTSTRAP_GET_DETAIL_USER_ID"
#define TAP_BOOTSTRAP_GET_ACCESSTOKEN_ID  "TAP_BOOTSTRAP_GET_ACCESSTOKEN_ID"
#define TAP_BOOTSTRAP_GET_TEST_QUALIFICATION_ID  "TAP_BOOTSTRAP_GET_TEST_QUALIFICATION_ID"

UTapBootstrapBPLibrary::UTapBootstrapBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(TEXT(TAP_BOOTSTRAP_CLZ),TEXT(TAP_BOOTSTRAP_IMPL));
	GetBridge()->Register(TEXT("com.taptap.sdk.wrapper.TDSLoginService"),TEXT("com.taptap.sdk.wrapper.TDSLoginServiceImpl"));
    FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapBootstrapBPLibrary::OnBridgeCallback);
#endif
}

void UTapBootstrapBPLibrary::Login(TArray<FString> permissions){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("permissions"), permissions);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE), TEXT("login"), JsonOutString, true, TEXT(TAP_BOOTSTRAP_REGISTER_LOGIN_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::AnonymouslyLogin(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE), TEXT("anonymouslyLogin"), TEXT(""), true, TEXT(TAP_BOOTSTRAP_REGISTER_LOGIN_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::RegisterUserStatusChangedListener(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE), TEXT("registerUserStatusChangedListener"), TEXT(""), false, TEXT(TAP_BOOTSTRAP_REGISTER_STATUS_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::RegisterLoginResultListener(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE), TEXT("registerLoginResultListener"), TEXT(""), false, TEXT(TAP_BOOTSTRAP_REGISTER_LOGIN_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::GetTestQualification(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSLoginService"),TEXT("getTestQualification"),TEXT(""),true,TEXT(TAP_BOOTSTRAP_GET_TEST_QUALIFICATION_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::GetUser(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE),TEXT("getUser"),TEXT(""),true,TEXT(TAP_BOOTSTRAP_GET_USER_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::GetDetailUser(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE),TEXT("getUserDetails"),TEXT(""),true,TEXT(TAP_BOOTSTRAP_GET_DETAIL_USER_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::GetAccessToken(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE),TEXT("getCurrentToken"),TEXT(""),true,TEXT(TAP_BOOTSTRAP_GET_ACCESSTOKEN_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::OpenUserCenter(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE),TEXT("openUserCenter"),TEXT(""),false,TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::Logout(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE),TEXT("logout"),TEXT(""),false,TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::SetPreferLanguage(FTapLanguage language)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    TSharedPtr<FJsonObject> args = MakeShareable(new FJsonObject);
    args->SetNumberField(TEXT("preferredLanguage"),(uint8)language);
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE),TEXT("setPreferredLanguage"),TapJson::ToJSON(args),false,TEXT(""),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapBootstrapBPLibrary::OnBridgeCallback(const FString &result){
#if PLATFORM_ANDROID || PLATFORM_IOS
    
    UE_LOG(LogTemp,Warning,TEXT("TapBootstrap OnBridgeCallback:%s"),*result);
    
    FTapResult tapResult;
    bool parseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FTapResult>(result,&tapResult,0,0);
    if(!parseSuccess){
        return;
    }

    /** Handler RegisterLogin Callback */
    if(tapResult.callbackId.Equals(TAP_BOOTSTRAP_REGISTER_LOGIN_ID)) {

        FTapLoginWrapper loginWrapper;
        FJsonObjectConverter::JsonObjectStringToUStruct<FTapLoginWrapper>(tapResult.content,&loginWrapper,0,0);

        if(loginWrapper.loginCallbackCode == 0){
            FTapUser user;
            UE_LOG(LogTemp,Warning,TEXT("TapBootstrap OnLoginSuccess:%s"),*loginWrapper.wrapper);
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapUser>(loginWrapper.wrapper,&user,0,0);
            FTapBootstrapModule::OnLoginSuccess.Broadcast(user);
        } else {
            FTapError loginError;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(loginWrapper.wrapper,&loginError,0,0);
            FTapBootstrapModule::OnLoginError.Broadcast(loginError);
        }
        return;
    }

    /** Handler GetUser Callback */
    if(tapResult.callbackId.Equals(TAP_BOOTSTRAP_GET_USER_ID)){
        FTapUserInfoWrapper userWrapper;
        FJsonObjectConverter::JsonObjectStringToUStruct<FTapUserInfoWrapper>(tapResult.content,&userWrapper,0,0);

        if(userWrapper.getUserInfoCode == 0){
            FTapUser user;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapUser>(userWrapper.wrapper,&user,0,0);
            FTapBootstrapModule::OnGetUserSuccess.Broadcast(user);
        } else {
            FTapError userError;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(userWrapper.wrapper,&userError,0,0);
            FTapBootstrapModule::OnGetUserError.Broadcast(userError);
        }
        return;
    }

    if(tapResult.callbackId.Equals(TAP_BOOTSTRAP_GET_TEST_QUALIFICATION_ID)){
            FTapError error;
            if(!UTapCommonBPLibrary::CheckResult(tapResult)){
                error = {80080,"TapSDK get TestQualification error!"};
                FTapBootstrapModule::OnGetTestQualificationError.Broadcast(error);
                return;
            }

            TSharedRef<TJsonReader<TCHAR>> testQualificationReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
            TSharedPtr<FJsonObject> testQualificationRoot;
            bool getTestQualificationParseEnable = FJsonSerializer::Deserialize(testQualificationReader,testQualificationRoot);
            if(getTestQualificationParseEnable){
                bool testQualification = testQualificationRoot->GetIntegerField(TEXT("userTestQualification")) == 1;
                if(!testQualification){
                    FString testInnerStr = testQualificationRoot->GetStringField(TEXT("error"));
                    FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(testInnerStr,&error,0,0);
                    FTapBootstrapModule::OnGetTestQualificationError.Broadcast(error);
                }
                FTapBootstrapModule::OnGetTestQualification.Broadcast(testQualification);
            }else{
                error = {80080,"TapSDK get TestQualification error!"};
                FTapBootstrapModule::OnGetTestQualificationError.Broadcast(error);
            }
        }

#endif
}


