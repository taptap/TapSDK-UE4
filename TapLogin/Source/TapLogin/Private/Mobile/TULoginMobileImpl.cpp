#include "TULoginMobileImpl.h"

#include "FLoginWrapper.h"
#include "FriendWrapper.h"
#include "TUJsonHelper.h"
#include "TUMobileBridge.h"

#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#import <TapLoginSDK/TapLoginSDK.h>
#include "IOSHelper.h"
#endif

#define TAP_LOGIN_CLZ "com.taptap.sdk.wrapper.TDSLoginService"
#define TAP_LOGIN_IMPL "com.taptap.sdk.wrapper.TDSLoginServiceImpl"
#define TAP_LOGIN_SERVICE "TDSLoginService"

#define TAP_FRIEND_CLZ "com.taptap.sdk.wrapper.TapFriendsService"
#define TAP_FRIEND_IMPL "com.taptap.sdk.wrapper.TapFriendsServiceImpl"
#define TAP_FRIEND_SERVICE "TapFriendsService"


TULoginMobileImpl::TULoginMobileImpl() {
	TUMobileBridge::Register(TEXT(TAP_LOGIN_CLZ),TEXT(TAP_LOGIN_IMPL));
	TUMobileBridge::Register(TEXT(TAP_FRIEND_CLZ),TEXT(TAP_FRIEND_IMPL));
}

void TULoginMobileImpl::Init(FTULoginConfig _Config) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("clientID",_Config.ClientID);
	Writer->WriteValue("regionType",_Config.RegionType == ERegionType::CN);
	Writer->WriteValue("roundCorner", _Config.bRoundCorner);
	Writer->WriteObjectEnd();
	Writer->Close();

	TUMobileBridge::AsyncPerform(TAP_LOGIN_SERVICE, "initWithClientID", JsonOutString);
}

TSharedPtr<FTULoginProfileModel> TULoginMobileImpl::GetProfile() {
	FString ResultStr = TUMobileBridge::Perform(TAP_LOGIN_SERVICE, "currentProfile", "");
	return TUJsonHelper::GetUStruct<FTULoginProfileModel>(ResultStr);
}

void TULoginMobileImpl::FetchProfile(
	TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {
	TUMobileBridge::AsyncPerform(TAP_LOGIN_SERVICE, "fetchProfileForCurrentAccessToken", "", [=](const FString& ResultStr) {
		
		if (CallBack == nullptr) {
			return;
		}
		auto WapperPtr = TUJsonHelper::GetUStruct<FLoginWrapper>(ResultStr);
		if (!WapperPtr.IsValid()) {
			CallBack(nullptr, FTUError(FTUError::BRIDGE_EXECUTE, "TapLogin get profile error"));
			return;
		}
		if (WapperPtr->loginCallbackCode != 0) {
			CallBack(nullptr, FTUError(WapperPtr->loginCallbackCode, WapperPtr->wrapper));
			return;
		}
		auto ModelPtr = TUJsonHelper::GetUStruct<FTULoginProfileModel>(WapperPtr->wrapper);
		if (ModelPtr.IsValid()) {
			CallBack(ModelPtr, FTUError());
		} else {
			CallBack(nullptr, FTUError(FTUError::BRIDGE_EXECUTE, "TapLogin get profile error"));
		}
	});
}

TSharedPtr<FTUAccessToken> TULoginMobileImpl::GetAccessToken() {
	FString ResultStr = TUMobileBridge::Perform(TAP_LOGIN_SERVICE, "currentAccessToken", "");
	return TUJsonHelper::GetUStruct<FTUAccessToken>(ResultStr);
}

void TULoginMobileImpl::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	TUMobileBridge::AsyncPerform(TAP_LOGIN_SERVICE, "registerLoginCallback", "", [=](const FString& ResultStr) {
		//去掉注册
		TUMobileBridge::AsyncPerform(TAP_LOGIN_SERVICE, "unregisterLoginCallback", "");

		if (CallBack == nullptr) {
			return;
		}
		auto WapperPtr = TUJsonHelper::GetUStruct<FLoginWrapper>(ResultStr);
		if (WapperPtr.IsValid()) {
			if (WapperPtr->loginCallbackCode == 0) {
				auto TokenPtr = TUJsonHelper::GetUStruct<FTUAccessToken>(WapperPtr->wrapper);
				if (TokenPtr.IsValid()) {
					CallBack(TUAuthResult::SuccessInit(TokenPtr));
					return;
				}
			}
			else if (WapperPtr->loginCallbackCode == 1) {
				CallBack(TUAuthResult::CancelInit());
				return;
			} else {
				auto ErrorPtr = TUJsonHelper::GetUStruct<FTUError>(WapperPtr->wrapper);
				if (ErrorPtr.IsValid()) {
					CallBack(TUAuthResult::FailInit(*ErrorPtr.Get()));
					return;
				}
			}
		}

		TUAuthResult Result = TUAuthResult::FailInit(FTUError(FTUError::BRIDGE_EXECUTE, "TapLogin Login Bridge Error"));
		CallBack(Result);
	});

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("permissions"), Permissions);
	Writer->WriteObjectEnd();
	Writer->Close();

	TUMobileBridge::AsyncPerform(TAP_LOGIN_SERVICE, "startTapLogin", JsonOutString);
}

void TULoginMobileImpl::Logout() {
	TUMobileBridge::AsyncPerform(TAP_LOGIN_SERVICE, "logout", "");
}

void TULoginMobileImpl::GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) {
	TUMobileBridge::AsyncPerform(TAP_LOGIN_SERVICE, "getTestQualification", "", [=](const FString& ResultStr) {
		if (CallBack == nullptr) {
			return;
		}
		TSharedPtr<FJsonObject> TestQualificationRoot = TUJsonHelper::GetJsonObject(ResultStr);
		if (TestQualificationRoot.IsValid()) {
			bool TestQualification = TestQualificationRoot->GetIntegerField(TEXT("userTestQualification")) == 1;
			if (TestQualification) {
				CallBack(true, FTUError());
				return;
			} 
			FString TestInnerStr;
			if (TestQualificationRoot->TryGetStringField(TEXT("error"), TestInnerStr)) {
				auto ErrorPtr = TUJsonHelper::GetUStruct<FTUError>(TestInnerStr);
				if (ErrorPtr.IsValid()) {
					CallBack(false, *ErrorPtr.Get());
					return;
				}
			}
		}

		CallBack(false, FTUError());
	});
}

void TULoginMobileImpl::QueryMutualList(FString Cursor, int Size,
	TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack) {
	// 需要在bootstrap里init 的server url，不然会奔溃。所以这个库放在这里格格不入。
#if PLATFORM_IOS
	TapFriendQueryOption *option = [[TapFriendQueryOption alloc] init];
	option.size = Size;
	option.cursor = Cursor.GetNSString();
	[TapFriends queryMutualListWithOption:option callback:^(TapFriendResult * _Nullable result, NSError * _Nullable error) {
		if (CallBack == nullptr) {
			return;
		}
		FTUError Error;
		TSharedPtr<FTULoginFriendResult> ModelPtr = nullptr;
		if (error) {
			Error = IOSHelper::Convert(error);
			NSLog(@"TDSFriendLogError-%@: %@", @"queryMutualListWithOption", error);
		} else {
			ModelPtr = MakeShareable(new FTULoginFriendResult);
			ModelPtr->cursor = IOSHelper::Convert(result.cursor);
			for (TapFriendInfo *info in result.data) {
				FTULoginFriendInfo Info;
				Info.name = IOSHelper::Convert(info.name);
				Info.avatar = IOSHelper::Convert(info.avatar);
				Info.openid = IOSHelper::Convert(info.openid);
				ModelPtr->data.Add(Info);
			}
		}
		AsyncTask(ENamedThreads::GameThread, [=]() {
			CallBack(ModelPtr, Error);
		});
	}];
#else
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("cursor", Cursor);
	Writer->WriteValue("size", Size);
	Writer->WriteObjectEnd();
	Writer->Close();

	TUMobileBridge::AsyncPerform(TAP_FRIEND_SERVICE, "queryMutualList", JsonOutString, [=](const FString& ResultStr) {
		if (CallBack == nullptr) {
			return;
		}
		auto WapperPtr = TUJsonHelper::GetUStruct<FFriendWrapper>(ResultStr);
		if (WapperPtr.IsValid()) {
			if (WapperPtr->success) {
				TSharedPtr<FTULoginFriendResult> ResultPtr = TUJsonHelper::GetUStruct<FTULoginFriendResult>(WapperPtr->content);
				if (ResultPtr.IsValid()) {
					CallBack(ResultPtr, FTUError());
					return;
				}
			} else {
				TSharedPtr<FTUError> ErrorPtr = TUJsonHelper::GetUStruct<FTUError>(WapperPtr->content);
				if (ErrorPtr.IsValid()) {
					CallBack(nullptr, *ErrorPtr.Get());
					return;
				}
			}
		}
		CallBack(nullptr, FTUError(FTUError::BRIDGE_EXECUTE, "TapLogin QueryMutualList Bridge Error"));
	});
#endif
	
}
