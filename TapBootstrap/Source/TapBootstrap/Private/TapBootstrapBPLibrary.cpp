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
    FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapBootstrapBPLibrary::OnBridgeCallback);
#endif
}

void UTapBootstrapBPLibrary::Login(TArray<FString> permissions){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("permissions"), permissions);
    Writer->WriteValue(TEXT("login"),0);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE), TEXT("login"), JsonOutString, false, TEXT(""),false);
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
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_BOOTSTRAP_SERVICE),TEXT("getTestQualification"),TEXT(""),true,TEXT(TAP_BOOTSTRAP_GET_TEST_QUALIFICATION_ID),true);
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
    if(tapResult.callbackId.Equals(TAP_BOOTSTRAP_REGISTER_LOGIN_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapError error = {80080,"TapSDK RegisterLoginResultListener Error!"};
            FTapBootstrapModule::OnLoginError.Broadcast(error);
            return;
        }

        FTapLoginWrapper loginWrapper;
        FJsonObjectConverter::JsonObjectStringToUStruct<FTapLoginWrapper>(tapResult.content,&loginWrapper,0,0);

        if(loginWrapper.loginCallbackCode == 0){
            FTapAccessToken accessToken;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapAccessToken>(loginWrapper.wrapper,&accessToken,0,0);
            FTapBootstrapModule::OnLoginSuccess.Broadcast(accessToken);
            return;
        }

        if(loginWrapper.loginCallbackCode == 1){
            FTapBootstrapModule::OnLoginCancel.Broadcast();
            return;
        }

        if(loginWrapper.loginCallbackCode == 2){
            FTapError loginError;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(loginWrapper.wrapper,&loginError,0,0);
            FTapBootstrapModule::OnLoginError.Broadcast(loginError);
            return;
        }
        return;
    }

    /** Handler Logout Callback */    
    if(tapResult.callbackId.Equals(TAP_BOOTSTRAP_REGISTER_STATUS_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            return;
        }
        FTapUserStatusWrapper tapUserStatusWrapper;
        FTapError userStatusError;
        FJsonObjectConverter::JsonObjectStringToUStruct<FTapUserStatusWrapper>(tapResult.content,&tapUserStatusWrapper,0,0);

        if(tapUserStatusWrapper.userStatusCallbackCode == 1){
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(tapUserStatusWrapper.wrapper,&userStatusError,0,0);
            FTapBootstrapModule::OnLogout.Broadcast(userStatusError);
            return;
        }
    }

    /** Handler GetAccessToken Callback */
    if(tapResult.callbackId.Equals(TAP_BOOTSTRAP_GET_ACCESSTOKEN_ID)){
        FTapAccessToken accessToken;
        FTapError accessTokenError;
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            accessTokenError = {80080,"TapSDK get AccessToken error!"};
            return;
        }
        FJsonObjectConverter::JsonObjectStringToUStruct<FTapAccessToken>(tapResult.content,&accessToken,0,0);
        FTapBootstrapModule::OnGetAccessToken.Broadcast(accessToken);
        return;
    }

    /** Handler GetUser Callback */
    if(tapResult.callbackId.Equals(TAP_BOOTSTRAP_GET_USER_ID)){
        FTapUser user;
        FTapError userError;
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            userError = {80080,"TapSDK get User error!"};
            FTapBootstrapModule::OnGetUserError.Broadcast(userError);
            return;
        }

        FTapUserInfoWrapper userWrapper;
        FJsonObjectConverter::JsonObjectStringToUStruct<FTapUserInfoWrapper>(tapResult.content,&userWrapper,0,0);

        if(userWrapper.getUserInfoCode == 0){
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapUser>(userWrapper.wrapper,&user,0,0);
            FTapBootstrapModule::OnGetUserSuccess.Broadcast(user);
            return;
        }

        FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(userWrapper.wrapper,&userError,0,0);
        FTapBootstrapModule::OnGetUserError.Broadcast(userError);
        return;
    }

    /** Handler GetUser Detail */
    if(tapResult.callbackId.Equals(TAP_BOOTSTRAP_GET_DETAIL_USER_ID)){
        FTapUserDetail detailUser;
        FTapError detailError;
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            detailError = {80080,"TapSDK get Detail error!"};
            FTapBootstrapModule::OnGetUserDetailError.Broadcast(detailError);
            return;
        }
        FTapUserDetailInfoWrapper userDetailWrapper;
        FJsonObjectConverter::JsonObjectStringToUStruct<FTapUserDetailInfoWrapper>(tapResult.content,&userDetailWrapper,0,0);

        if(userDetailWrapper.getUserDetailInfoCode != 0){
            if(!FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(userDetailWrapper.wrapper,&detailError,0,0)){
                detailError = {80000,"GetDetailUserInfo Undefine Error"};
            }
            FTapBootstrapModule::OnGetUserDetailError.Broadcast(detailError);
            return;
        }

        TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(userDetailWrapper.wrapper);
        TSharedPtr<FJsonObject> root;
        FTapUserCenterEntry userCenterEntry;
        bool getUserDetailPaseEnable = FJsonSerializer::Deserialize(reader,root);
        if(getUserDetailPaseEnable){

            TSharedPtr<FJsonObject> entryJsonObject = root->GetObjectField(TEXT("entry"));
            userCenterEntry.isMomentEnable = entryJsonObject->GetBoolField(TEXT("isMomentEnable"));

            detailUser.entry = userCenterEntry;
            detailUser.user_id = root->GetStringField(TEXT("user_id"));
            detailUser.avatar = root->GetStringField(TEXT("avatar"));      
            detailUser.name = root->GetStringField(TEXT("name"));      
            detailUser.taptap_user_id = root->GetStringField(TEXT("taptap_user_id"));      
            detailUser.isGuest = root->GetBoolField(TEXT("isGuest"));      
            detailUser.gender = root->GetIntegerField(TEXT("gender"));

            FTapBootstrapModule::OnGetUserDetailSuccess.Broadcast(detailUser);
            return;
        }
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
                if(testQualification){
                    TSharedPtr<FJsonObject> testQualificationInner = testQualificationRoot->GetObjectField(TEXT("error"));
                    error = {testQualificationInner->GetIntegerField(TEXT("code")),testQualificationInner->GetStringField(TEXT("error_description"))};
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


