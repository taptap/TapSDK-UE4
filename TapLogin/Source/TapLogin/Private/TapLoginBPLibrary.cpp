// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapLoginBPLibrary.h"

#include "FLoginWrapper.h"
#include "JsonObjectConverter.h"
#include "TapBridge.h"
#include "TapCommon.h"
#include "TapCommonBPLibrary.h"
#include "TapLogin.h"
#include "TapResult.h"
#include "TapJson.h"
#include "FriendWrapper.h"

#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#import <TapLoginSDK/TapLoginSDK.h>
#include "IOSHelper.h"
#endif

/** TapLogin ServiceName and Android Platform packageName */

#define TAP_LOGIN_CLZ "com.taptap.sdk.wrapper.TDSLoginService"
#define TAP_LOGIN_IMPL "com.taptap.sdk.wrapper.TDSLoginServiceImpl"
#define TAP_LOGIN_SERVICE "TDSLoginService"

#define TAP_FRIEND_CLZ "com.taptap.sdk.wrapper.TapFriendsService"
#define TAP_FRIEND_IMPL "com.taptap.sdk.wrapper.TapFriendsServiceImpl"
#define TAP_FRIEND_SERVICE "TapFriendsService"

/** TapLogin Method Id ID used to distinguish the method */

#define TAP_LOGIN_REGISTER_LOGIN_ID "TAP_LOGIN_REGISTER_LOGIN_ID"
#define TAP_LOGIN_GET_ACCESSTOKEN_ID  "TAP_LOGIN_GET_ACCESSTOKEN_ID"
#define TAP_LOGIN_GET_PROFILE_ID  "TAP_LOGIN_GET_PROFILE_ID"
#define TAP_LOGIN_GET_TEST_QUALIFICATION_ID  "TAP_LOGIN_GET_TEST_QUALIFICATION_ID"
#define TAP_FRIEND_GET_QUERY_MUTUALLIST_LIST_ID  "TAP_FRIEND_GET_QUERY_MUTUALLIST_LIST_ID"
UTapLoginBPLibrary::UTapLoginBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(TEXT(TAP_LOGIN_CLZ),TEXT(TAP_LOGIN_IMPL));
	GetBridge()->Register(TEXT(TAP_FRIEND_CLZ),TEXT(TAP_FRIEND_IMPL));
	FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapLoginBPLibrary::OnBridgeCallback);
#endif
}


void UTapLoginBPLibrary::RegisterLoginCallback()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE), TEXT("registerLoginCallback"), TEXT(""), true, TEXT(TAP_LOGIN_REGISTER_LOGIN_ID),true);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::UnRegisterLoginCallback()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE), TEXT("unregisterLoginCallback"), TEXT(""), true, TEXT(""),true);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::Init(FString clientID)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("clientID",clientID);
	Writer->WriteObjectEnd();
	Writer->Close();

	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE), TEXT("init"), JsonOutString, true, TEXT(""),false);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::InitWithParams(FString clientID, bool bIsCN, bool bRoundCorner)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("clientID",clientID);
	Writer->WriteValue("regionType",bIsCN);
	Writer->WriteValue("roundCorner",bRoundCorner);
	Writer->WriteObjectEnd();
	Writer->Close();

	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE), TEXT("initWithClientID"), JsonOutString, true, TEXT(""),false);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::FetchProfile()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE),TEXT("fetchProfileForCurrentAccessToken"),TEXT(""),true,TEXT(TAP_LOGIN_GET_PROFILE_ID),true);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::GetProfile()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE),TEXT("currentProfile"),TEXT(""),true,TEXT(TAP_LOGIN_GET_PROFILE_ID),true);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::GetAccessToken()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE),TEXT("getCurrentToken"),TEXT(""),true,TEXT(TAP_LOGIN_GET_ACCESSTOKEN_ID),true);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::Login(TArray<FString> permissions)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	RegisterLoginCallback();
	
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("permissions"), permissions);
	Writer->WriteObjectEnd();
	Writer->Close();
	
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE), TEXT("startTapLogin"), JsonOutString, true, TEXT(""),false);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::Logout()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_LOGIN_SERVICE), TEXT("logout"), TEXT(""), true, TEXT(""),false);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::QueryMutualList(FString cursor, int size)
{
#if PLATFORM_IOS
	TapFriendQueryOption *option = [[TapFriendQueryOption alloc] init];
	option.size = size;
	option.cursor = cursor.GetNSString();
	[TapFriends queryMutualListWithOption:option callback:^(TapFriendResult * _Nullable result, NSError * _Nullable error) {
		FTapError Error;
		FTapFriendResult Result;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-%@: %@", @"queryMutualListWithOption", error);
		} else {
			Result.cursor = IOSHelper::convertString(result.cursor);
			for (TapFriendInfo *info in result.data) {
				FLoginTapFriendInfo Info;
				Info.name = IOSHelper::convertString(info.name);
				Info.avatar = IOSHelper::convertString(info.avatar);
				Info.openid = IOSHelper::convertString(info.openid);
				Result.data.Add(Info);
			}
		}
		FTapLoginModule::OnQueryMutualList.Broadcast(Result, Error);
	}];
#elif PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("cursor", cursor);
	Writer->WriteValue("size", size);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TAP_FRIEND_SERVICE),TEXT("queryMutualList"), JsonOutString, true,
		TEXT(TAP_FRIEND_GET_QUERY_MUTUALLIST_LIST_ID), true);
	GetBridge()->CallHandler(Command);
#endif
}

void UTapLoginBPLibrary::GetTestQualification()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString commandJson = TapJson::ConstructorCommand(TEXT("TDSLoginService"),TEXT("getTestQualification"),TEXT(""),true,TEXT(TAP_LOGIN_GET_TEST_QUALIFICATION_ID),true);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapLoginBPLibrary::OnBridgeCallback(const FString& result)
{
	UE_LOG(LogTemp, Warning, TEXT("TapLogin OnBridgeCallback:%s"), *result);

	FTapResult tapResult;
	bool parseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FTapResult>(result, &tapResult, 0, 0);
	if (!parseSuccess)
	{
		return;
	}

	/** Handler RegisterLogin Callback */
	if (tapResult.callbackId.Equals(TAP_LOGIN_REGISTER_LOGIN_ID))
	{
		UnRegisterLoginCallback();
		FLoginWrapper loginWrapper;
		FJsonObjectConverter::JsonObjectStringToUStruct<FLoginWrapper>(tapResult.content, &loginWrapper, 0, 0);

		if (loginWrapper.loginCallbackCode == 0)
		{
			FAccessToken accessToken;
			UE_LOG(LogTemp, Warning, TEXT("TapLogin OnLoginSuccess:%s"), *loginWrapper.wrapper);
			FJsonObjectConverter::JsonObjectStringToUStruct<FAccessToken>(loginWrapper.wrapper, &accessToken, 0, 0);
			FTapLoginModule::OnLoginSuccess.Broadcast(accessToken);
			return;
		}
		if (loginWrapper.loginCallbackCode == 1)
		{
			FTapLoginModule::OnLoginCancel.Broadcast();
			return;
		}
		FTapError loginError;
		FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(loginWrapper.wrapper, &loginError, 0, 0);
		FTapLoginModule::OnLoginError.Broadcast(loginError);
		return;
	}

	if (tapResult.callbackId.Equals(TAP_LOGIN_GET_ACCESSTOKEN_ID))
	{
		if (!UTapCommonBPLibrary::CheckResult(tapResult))
		{
			FTapError Error;
			Error = {80080, "TapLogin get AccessToken error!"};
			FTapLoginModule::OnGetTestQualificationError.Broadcast(Error);
			return;
		}
		FAccessToken AccessToken;
		UE_LOG(LogTemp, Warning, TEXT("TapLogin OnLoginSuccess:%s"), *tapResult.content);
		FJsonObjectConverter::JsonObjectStringToUStruct<FAccessToken>(tapResult.content, &AccessToken, 0, 0);
		FTapLoginModule::OnGetAccessTokenSuccess.Broadcast(AccessToken);
		return;
	}

	if (tapResult.callbackId.Equals(TAP_LOGIN_GET_PROFILE_ID))
	{
		if (!UTapCommonBPLibrary::CheckResult(tapResult))
		{
			FTapError Error;
			Error = {80080, "TapLogin get profile error"};
			FTapLoginModule::OnGetProfileError.Broadcast(Error);
			return;
		}
		FLoginWrapper Wrapper;
		FJsonObjectConverter::JsonObjectStringToUStruct<FLoginWrapper>(tapResult.content, &Wrapper, 0, 0);
		if (Wrapper.loginCallbackCode == 0)
		{
			FProfile profile;
			FJsonObjectConverter::JsonObjectStringToUStruct<FProfile>(Wrapper.wrapper, &profile, 0, 0);
			FTapLoginModule::OnGetProfileSuccess.Broadcast(profile);
			return;
		}
		FTapError Error;
		Error = {80080, Wrapper.wrapper};
		FTapLoginModule::OnGetProfileError.Broadcast(Error);
		return;
	}

	if (tapResult.callbackId.Equals(TAP_FRIEND_GET_QUERY_MUTUALLIST_LIST_ID))
	{
		FFriendWrapper Response;
		FJsonObjectConverter::JsonObjectStringToUStruct(tapResult.content, &Response, 0, 0);

		FTapError Error;
		FTapFriendResult Result;
		FString Link;
		if (Response.success == false)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct(Response.content, &Result, 0, 0);
		}
		FTapLoginModule::OnQueryMutualList.Broadcast(Result, Error);
		return;
	}

	if (tapResult.callbackId.Equals(TAP_LOGIN_GET_TEST_QUALIFICATION_ID))
	{
		FTapError error;
		if (!UTapCommonBPLibrary::CheckResult(tapResult))
		{
			error = {80080, "TapSDK get TestQualification error!"};
			FTapLoginModule::OnGetTestQualificationError.Broadcast(error);
			return;
		}

		TSharedRef<TJsonReader<TCHAR>> testQualificationReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
		TSharedPtr<FJsonObject> testQualificationRoot;
		bool getTestQualificationParseEnable = FJsonSerializer::Deserialize(
			testQualificationReader, testQualificationRoot);
		if (getTestQualificationParseEnable)
		{
			bool testQualification = testQualificationRoot->GetIntegerField(TEXT("userTestQualification")) == 1;
			if (!testQualification)
			{
				FString testInnerStr = testQualificationRoot->GetStringField(TEXT("error"));
				FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(testInnerStr, &error, 0, 0);
				FTapLoginModule::OnGetTestQualificationError.Broadcast(error);
			}
			FTapLoginModule::OnGetTestQualification.Broadcast(testQualification);
		}
		else
		{
			error = {80080, "TapSDK get TestQualification error!"};
			FTapLoginModule::OnGetTestQualificationError.Broadcast(error);
		}
	}
}
