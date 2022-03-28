// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapFriendsBPLibrary.h"
#include "TapFriends.h"
#include "TapBridge.h"
#include "TapCommon.h"
#include "TapJson.h"
#include "TapResult.h"
#include "TapError.h"
#include "TapCommonBPLibrary.h"
#include "Serialization/JsonWriter.h"
#include "JsonObjectConverter.h"
#include "TapFriendHandleResult.h"
#include "TapFriendResponse.h"
#include "TapFriendshipRequest.h"
#include "TapFriendshipRequestAndInfo.h"


#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#import <TapFriendSDK/TapFriendSDK.h>
#include "IOSHelper.h"
#import "TapFriendsNotificationDelegator.h"
#endif

#define TDS_FRIENDS_SERVICE "TDSFriendsService"
#define TDS_FRIENDS_SERVICE_CLZ "com.tapsdk.friends.wrapper.TDSFriendsService"
#define TDS_FRIENDS_SERVICE_IML "com.tapsdk.friends.wrapper.TDSFriendsServiceImpl"

#define TAP_FRIENDS_STATUS_CHANGED_LISTENER_ID "TAP_FRIENDS_STATUS_CHANGED_LISTENER_ID"
#define TAP_FRIENDS_SELF_ON_LINE_ID "TAP_FRIENDS_SELF_ON_LINE_ID"
#define TAP_FRIENDS_QUERY_FRIENDS_BY_NICKNAME_ID "TAP_FRIENDS_QUERY_FRIENDS_BY_NICKNAME_ID"
#define TAP_FRIENDS_QUERY_FRIEND_BY_SHORTCODE_ID "TAP_FRIENDS_QUERY_FRIEND_BY_SHORTCODE_ID"
#define TAP_FRIENDS_QUERY_FRIEND_BY_OBJECTID_ID "TAP_FRIENDS_QUERY_FRIEND_BY_OBJECTID_ID"
#define TAP_FRIENDS_SAVE_RICH_PRESENCE_ID "TAP_FRIENDS_SAVE_RICH_PRESENCE_ID"
#define TAP_FRIENDS_CLEAR_RICH_PRESENCE_ID "TAP_FRIENDS_CLEAR_RICH_PRESENCE_ID"
#define TAP_FRIENDS_ADD_FRIEND_BY_SHORT_CODE_ID "TAP_FRIENDS_ADD_FRIEND_BY_SHORT_CODE_ID"
#define TAP_FRIENDS_ADD_FRIEND_BY_OBJECT_ID "TAP_FRIENDS_ADD_FRIEND_BY_OBJECT_ID"
#define TAP_FRIENDS_DELETE_FRIEND_BY_OBJECT_ID "TAP_FRIENDS_DELETE_FRIEND_BY_OBJECT_ID"
#define TAP_FRIENDS_QUERY_LIST_ID "TAP_FRIENDS_QUERY_LIST_ID"
#define TAP_FRIENDS_CHECK_SHIP_ID "TAP_FRIENDS_CHECK_SHIP_ID"
#define TAP_FRIENDS_GENERATE_FRIEND_INVITATION_ID "TAP_FRIENDS_GENERATE_FRIEND_INVITATION_ID"
#define TAP_FRIENDS_ACCEPT_FRIEND_REQUEST_ID "TAP_FRIENDS_ACCEPT_FRIEND_REQUEST_ID"
#define TAP_FRIENDS_DECLINE_FRIEND_REQUEST_ID "TAP_FRIENDS_DECLINE_FRIEND_REQUEST_ID"
#define TAP_FRIENDS_QUERY_FRIEND_REQUEST_ID "TAP_FRIENDS_QUERY_FRIEND_REQUEST_ID"
#define TAP_FRIENDS_QUERY_FRIEND_REQUEST_INFO_ID "TAP_FRIENDS_QUERY_FRIEND_REQUEST_INFO_ID"
#define TAP_FRIENDS_DELETE_FRIEND_REQUEST_ID "TAP_FRIENDS_DELETE_FRIEND_REQUEST_ID"

#define TAP_FRIENDS_FOLLOW_TAP_USER_BY_SHORT_CODE_ID "TAP_FRIENDS_FOLLOW_TAP_USER_BY_SHORT_CODE_ID"
#define TAP_FRIENDS_UN_FOLLOW_TAP_USER_BY_SHORT_CODE_ID "TAP_FRIENDS_UN_FOLLOW_TAP_USER_BY_SHORT_CODE_ID"
#define TAP_FRIENDS_UN_BLOCK_TAP_USER_BY_SHORT_CODE_ID "TAP_FRIENDS_UN_BLOCK_TAP_USER_BY_SHORT_CODE_ID"
#define TAP_FRIENDS_BLOCK_TAP_USER_BY_SHORT_CODE_ID "TAP_FRIENDS_BLOCK_TAP_USER_BY_SHORT_CODE_ID"

#define TAP_FRIENDS_FOLLOW_TAP_USER_BY_OBJECT_ID "TAP_FRIENDS_FOLLOW_TAP_USER_BY_OBJECT_ID"
#define TAP_FRIENDS_UN_FOLLOW_TAP_USER_BY_OBJECT_ID "TAP_FRIENDS_UN_FOLLOW_TAP_USER_BY_OBJECT_ID"
#define TAP_FRIENDS_UN_BLOCK_TAP_USER_BY_OBJECT_ID "TAP_FRIENDS_UN_BLOCK_TAP_USER_BY_OBJECT_ID"
#define TAP_FRIENDS_BLOCK_TAP_USER_BY_OBJECT_ID "TAP_FRIENDS_BLOCK_TAP_USER_BY_OBJECT_ID"

#define TAP_FRIENDS_FOLLOW_BY_SHORT_CODE_ID "TAP_FRIENDS_FOLLOW_BY_SHORT_CODE_ID"
#define TAP_FRIENDS_UN_FOLLOW_BY_SHORT_CODE_ID "TAP_FRIENDS_UN_FOLLOW_BY_SHORT_CODE_ID"
#define TAP_FRIENDS_FOLLOW_BY_OBJECT_ID "TAP_FRIENDS_FOLLOW_BY_OBJECT_ID"
#define TAP_FRIENDS_UN_FOLLOW_BY_OBJECT_ID "TAP_FRIENDS_UN_FOLLOW_BY_OBJECT_ID"
#define TAP_FRIENDS_QUERY_FOLLOW_MUTAL_LIST_ID "TAP_FRIENDS_QUERY_FOLLOW_MUTAL_LIST_ID"
#define TAP_FRIENDS_QUERY_FOLLOWEE_LIST_ID "TAP_FRIENDS_QUERY_FOLLOWEE_LIST_ID"
#define TAP_FRIENDS_QUERY_FOLLOWER_LIST_ID "TAP_FRIENDS_QUERY_FOLLOWER_LIST_ID"
#define TAP_FRIENDS_QUERY_BLOCK_LIST_ID "TAP_FRIENDS_QUERY_BLOCK_LIST_ID"

#define TAP_FRIENDS_QUERY_THIRD_Friend_LIST_ID "TAP_FRIENDS_QUERY_THIRD_Friend_LIST_ID"
#define TAP_FRIENDS_QUERY_THIRD_LIST_ID "TAP_FRIENDS_QUERY_THIRD_LIST_ID"
#define TAP_FRIENDS_QUERY_THIRD_FOLLOWER_LIST_ID "TAP_FRIENDS_QUERY_THIRD_FOLLOWER_LIST_ID"
#define TAP_FRIENDS_QUERY_THIRD_FOLLOWEE_LIST_ID "TAP_FRIENDS_QUERY_THIRD_FOLLOWEE_LIST_ID"
#define TAP_FRIENDS_QUERY_THIRD_BLOCK_LIST_ID "TAP_FRIENDS_QUERY_THIRD_BLOCK_LIST_ID"

#define TAP_FRIENDS_FOLLOW_BLOCK_BY_OBJECT_ID "TAP_FRIENDS_FOLLOW_BLOCK_BY_OBJECT_ID"
#define TAP_FRIENDS_FOLLOW_UNBLOCK_BY_OBJECT_ID "TAP_FRIENDS_FOLLOW_UNBLOCK_BY_OBJECT_ID"
#define TAP_FRIENDS_FOLLOW_BLOCK_BY_SHORTCODE_ID "TAP_FRIENDS_FOLLOW_BLOCK_BY_SHORTCODE_ID"
#define TAP_FRIENDS_FOLLOW_UNBLOCK_BY_SHORTCODE_ID "TAP_FRIENDS_FOLLOW_UNBLOCK_BY_SHORTCODE_ID"

#define TAP_FRIENDS_HANDLE_INVITATION_ID "TAP_FRIENDS_HANDLE_INVITATION_ID"
#define TAP_FRIENDS_FOLLOW_HANDLE_INVITATION_ID "TAP_FRIENDS_FOLLOW_HANDLE_INVITATION_ID"
#define TAP_FRIENDS_FOLLOW_PARSE_LINK_INFO_ID  "TAP_FRIENDS_FOLLOW_PARSE_LINK_INFO_ID"

#if PLATFORM_IOS
class TapFriendIOSHelper
{
public:
	static void BroadcastFriendHandleResult(TDSFriendHandleResult *result, NSError *error, NSString *methodName, const FTapFriendsModule::FFriendFollowOperationCallback& callBack) {
		FTapError Error;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-%@: %@", methodName, error);
		}
		NSLog(@"TDSFriendLogInfo-%@: %@", methodName, result);
		callBack.Broadcast(result.code, IOSHelper::convertString(result.message), Error);
	}
	
	static void BroadcastBoolResult(BOOL result, NSError *error, NSString *methodName, const FTapFriendsModule::FFriendOperationCallback& callBack) {
		FTapError Error;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-%@: %@", methodName, error);
		}
		NSLog(@"TDSFriendLogInfo-%@: %d", methodName, result);
		callBack.Broadcast(result, Error);
	}
	
	static void BroadcastThirdPartyFriendResult(TDSThirdPartyFriendResult *result, NSError *error, NSString *methodName, const FTapFriendsModule::FQueryThirdPartyFriendList& callBack) {
		FTapError Error;
		FString Cursor;
		TArray<FTapThirdPartyFriendInfo> Infos;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-%@: %@", methodName, error);
		}
		if (result) {
			Cursor = IOSHelper::convertString(result.cursor);
			for (TDSThirdPartyFriend *info in result.friendList) {
				Infos.Add(FTapThirdPartyFriendInfo((NSObject *)info));
			}
		}
		NSLog(@"TDSFriendLogInfo-%@: %d", methodName, Infos.Num());
		callBack.Broadcast(Cursor, Infos, Error);
	}
	
	static void BroadcastFriendResult(TDSFriendResult *result, NSError *error, NSString *methodName, const FTapFriendsModule::FQueryFriendsInfoList& callBack) {
		FTapError Error;
		FString Cursor;
		TArray<FTapFriendInfo> Infos;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-%@: %@", methodName, error);
		}
		if (result) {
			Cursor = IOSHelper::convertString(result.cursor);
			for (TDSFriendInfo *info in result.friendList) {
				Infos.Add(FTapFriendInfo((NSObject *)info));
			}
		}
		callBack.Broadcast(Cursor, Infos, Error);
	}

	static void BroadcastFriendInfos(NSArray<TDSFriendInfo *> *friendInfos, NSError *error, NSString *methodName, const FTapFriendsModule::FFriendQueryListCallback& callBack) {
		FTapError Error;
		TArray<FTapFriendInfo> Infos;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-%@: %@", methodName, error);
		} else {
			for (TDSFriendInfo *info in friendInfos) {
				Infos.Add(FTapFriendInfo((NSObject *)info));
			}
		}
		callBack.Broadcast(Infos, Error);
	}

	static void BroadcastFriendInfo(TDSFriendInfo *friendInfo, NSError *error, NSString *methodName, const FTapFriendsModule::FFriendQueryInfoCallback& callBack) {
		FTapError Error;
		FTapFriendInfo Info;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-%@: %@", methodName, error);
		} else {
			Info = FTapFriendInfo((NSObject *)friendInfo);
		}
		callBack.Broadcast(Info, Error);
	}
};



#endif

UTapFriendsBPLibrary::UTapFriendsBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("init TapFriends"));
	UTapCommonBPLibrary::Register(TEXT(TDS_FRIENDS_SERVICE_CLZ),TEXT(TDS_FRIENDS_SERVICE_IML));
	FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapFriendsBPLibrary::OnBridgeCallback);
#endif
#if PLATFORM_IOS
	friendDelegator = [[TapFriendsNotificationDelegator alloc] init];
#endif
}

// 注册好友回调
void UTapFriendsBPLibrary::RegisterFriendsStatusChangedListener()
{
#if PLATFORM_ANDROID
	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("registerFriendsStatusChangedListener"),TEXT(""), true,
		TEXT(TAP_FRIENDS_STATUS_CHANGED_LISTENER_ID), false);
	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends registerNotificationDelegate:friendDelegator];
#endif
}

// 注销好友回调
void UTapFriendsBPLibrary::RemoveFriendsStatusChangedListener()
{
#if PLATFORM_ANDROID

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("removeFriendsStatusChangedListener"),TEXT(""), false, TEXT(""), true);
	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends unregisterNotificationDelegate];
#endif
}

// 上线
void UTapFriendsBPLibrary::Online()
{
#if PLATFORM_ANDROID

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("online"),TEXT(""), true,TEXT(TAP_FRIENDS_SELF_ON_LINE_ID), true);
	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends online];
#endif
}

// 下线
void UTapFriendsBPLibrary::Offline()
{
#if PLATFORM_ANDROID

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("offline"),TEXT(""), false,TEXT(""), true);
	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends offline];
#endif
}

// TDSFriends 根据nickName搜索好友信息
void UTapFriendsBPLibrary::QueryFiendsByName(FString nickName)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("nickName", nickName);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryFriendsByNickName"), JsonOutString, true,
		TEXT(TAP_FRIENDS_QUERY_FRIENDS_BY_NICKNAME_ID), true);
	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends searchUserWithNickname:nickName.GetNSString() callback:^(NSArray<TDSFriendInfo *> * _Nullable friendInfos, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendInfos(friendInfos, error, @"searchUserWithNickname", FTapFriendsModule::OnQueryFriendsByNickName);
	}];
#endif
}

// TDSFriends 根据shortCode搜索好友信息
void UTapFriendsBPLibrary::QueryFriendByShortCode(FString shortCode)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("shortCode", shortCode);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryFriendByShortCode"), JsonOutString, true,
		TEXT(TAP_FRIENDS_QUERY_FRIEND_BY_SHORTCODE_ID), true);
	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends searchUserWithShortCode:shortCode.GetNSString() callback:^(TDSFriendInfo * _Nullable friendInfo, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendInfo(friendInfo, error, @"searchUserWithShortCode", FTapFriendsModule::OnQueryFriendByShortCode);
	}];
#endif
}

void UTapFriendsBPLibrary::QueryFriendByObjectId(FString objectId)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("objectId", objectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryFriendByObjectId"), JsonOutString, true,
		TEXT(TAP_FRIENDS_QUERY_FRIEND_BY_OBJECTID_ID), true);
	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends searchUserWithObjectId:objectId.GetNSString() callback:^(TDSFriendInfo * _Nullable friendInfo, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendInfo(friendInfo, error, @"searchUserWithShortCode", FTapFriendsModule::OnQueryFriendByObjectId);
	}];
#endif
}

void UTapFriendsBPLibrary::SaveRichPresence(TMap<FString, FString> richPresence)
{
	
#if PLATFORM_ANDROID

	FString JsonOutString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue("richPresence", TapJson::ToJSON(richPresence));
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("saveRichPresence"), JsonOutString, true,TEXT(TAP_FRIENDS_SAVE_RICH_PRESENCE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends setRichPresencesWithDictionary:IOSHelper::convertMap(richPresence) callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"setRichPresencesWithDictionary", FTapFriendsModule::OnSaveRichPresence);
	}];
#endif
}

void UTapFriendsBPLibrary::ClearRicePresence(TArray<FString> richPresenceKeys)
{
#if PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("richPresenceKeys"), richPresenceKeys);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("clearRichPresence"), JsonOutString, true,
		TEXT(TAP_FRIENDS_CLEAR_RICH_PRESENCE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends clearRichPresencesWithKeys:IOSHelper::convertArray(richPresenceKeys) callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"clearRichPresencesWithKeys", FTapFriendsModule::OnClearRichPresence);
	}];
#endif
}

void UTapFriendsBPLibrary::AddFriendByShortCode(FString shortCode, TMap<FString, FString> attrs)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), shortCode);
	Writer->WriteValue(TEXT("attrs"), TapJson::ToJSON(attrs));
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("addFriendByShortCode"), JsonOutString, true,
		TEXT(TAP_FRIENDS_ADD_FRIEND_BY_SHORT_CODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends addFriendWithShortCode:shortCode.GetNSString() attributes:IOSHelper::convertMap(attrs) callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"addFriendWithShortCode", FTapFriendsModule::OnAddFriendByShortCode);
	}];
#endif
}

void UTapFriendsBPLibrary::AddFriendByObjectId(FString objectId, TMap<FString, FString> attrs)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteValue(TEXT("attrs"), TapJson::ToJSON(attrs));
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("addFriendByObjectId"), JsonOutString, true,
		TEXT(TAP_FRIENDS_ADD_FRIEND_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends addFriendWithUserId:objectId.GetNSString() attributes:IOSHelper::convertMap(attrs) callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"addFriendWithUserId", FTapFriendsModule::OnAddFriendByObjectId);
	}];
#endif
}

void UTapFriendsBPLibrary::DeleteFriendByObjectId(FString objectId)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("deleteFriendByObjectId"), JsonOutString, true,
		TEXT(TAP_FRIENDS_DELETE_FRIEND_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends deleteFriendWithUserId:objectId.GetNSString() callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"deleteFriendWithUserId", FTapFriendsModule::OnFriendDeleteByObjectId);
	}];

#endif
}

void UTapFriendsBPLibrary::QueryFriendRequestList(int64 status, int64 from, int64 limit)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("requestStatus"), status);
	Writer->WriteValue(TEXT("from"), from);
	Writer->WriteValue(TEXT("limit"), limit);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryFriendRequest"), JsonOutString, true,
		TEXT(TAP_FRIENDS_QUERY_FRIEND_REQUEST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	TDSFriendQueryOption *option = [[TDSFriendQueryOption alloc] init];
	option.from = from;
	option.limit = limit;
	TDSUserFriendshipRequestStatus requestStatus = TDSUserFriendshipRequestStatusAny;
	if (status == 0x01) {
		requestStatus = TDSUserFriendshipRequestStatusPending;
	} else if (status == 0x02) {
		requestStatus = TDSUserFriendshipRequestStatusAccepted;
	} else if (status == 0x04) {
		requestStatus = TDSUserFriendshipRequestStatusDeclined;
	}
	[TDSFriends queryFriendRequestWithStatus:requestStatus option:option callback:^(NSArray<LCFriendshipRequest *> * _Nullable requests, NSError * _Nullable error) {
		TArray<FTapFriendshipRequest> Requests;
		FTapError Error;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-queryFriendRequestWithStatus: %@", error);
		}
		if (requests) {
			for (LCFriendshipRequest *request in requests) {
				Requests.Add(FTapFriendshipRequest((NSObject *)request));
			}
		}
		FTapFriendsModule::OnFriendQueryRequest.Broadcast(Requests, Error);
	}];
#endif
}

void UTapFriendsBPLibrary::QueryFriendRequestWithFriendStateList(int64 status, int64 from, int64 limit)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("requestStatus"), status);
	Writer->WriteValue(TEXT("from"), from);
	Writer->WriteValue(TEXT("limit"), limit);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryFriendRequestWithFriendStateList"), JsonOutString, true,
		TEXT(TAP_FRIENDS_QUERY_FRIEND_REQUEST_INFO_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	TDSFriendQueryOption *option = [[TDSFriendQueryOption alloc] init];
	option.from = from;
	option.limit = limit;
	TDSUserFriendshipRequestStatus requestStatus = TDSUserFriendshipRequestStatusAny;
	if (status == 0x01) {
		requestStatus = TDSUserFriendshipRequestStatusPending;
	} else if (status == 0x02) {
		requestStatus = TDSUserFriendshipRequestStatusAccepted;
	} else if (status == 0x04) {
		requestStatus = TDSUserFriendshipRequestStatusDeclined;
	}
    [TDSFriends queryFriendRequestAndStateWithStatus:requestStatus option:option callback:^(NSArray<TDSFriendshipRequest *> * _Nullable requests, NSError * _Nullable error) {
    	TArray<FTapFriendshipRequestAndInfo> Requests;
		FTapError Error;
		if (error) {
			Error = IOSHelper::convertError(error);
			NSLog(@"TDSFriendLogError-queryFriendRequestWithStatus: %@", error);
		}
		if (requests) {
			for (TDSFriendshipRequest *request in requests) {
				Requests.Add(FTapFriendshipRequestAndInfo((NSObject *)request));
			}
		}
		FTapFriendsModule::OnQueryFriendRequestWithFriendStateList.Broadcast(Requests, Error);
	}];
#endif
}

void UTapFriendsBPLibrary::AcceptFriendRequest(FString requestObjectId, TMap<FString, FString> attrs)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("requestObjectId"), requestObjectId);
	Writer->WriteValue(TEXT("attrs"), TapJson::ToJSON(attrs));
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("acceptFriendRequest"), JsonOutString, true,
		TEXT(TAP_FRIENDS_ACCEPT_FRIEND_REQUEST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	LCFriendshipRequest * request = [LCFriendshipRequest objectWithObjectId:requestObjectId.GetNSString()];
	[TDSFriends acceptFriendRequest:request attributes:IOSHelper::convertMap(attrs) callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"acceptFriendRequest", FTapFriendsModule::OnAcceptFriendRequest);
	}];
#endif
}

void UTapFriendsBPLibrary::DeclineFriendRequest(FString requestObjectId)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("requestObjectId"), requestObjectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("declineFriendRequest"), JsonOutString, true,
		TEXT(TAP_FRIENDS_DECLINE_FRIEND_REQUEST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	LCFriendshipRequest * request = [LCFriendshipRequest objectWithObjectId:requestObjectId.GetNSString()];
	[TDSFriends declineFriendRequest:request callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"declineFriendRequest", FTapFriendsModule::OnFriendDeclineRequest);
	}];
#endif
}

void UTapFriendsBPLibrary::DeleteFriendRequest(FString requestObjectId)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("requestObjectId"), requestObjectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("deleteFriendsRequest"), JsonOutString, true,
		TEXT(TAP_FRIENDS_DELETE_FRIEND_REQUEST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	LCFriendshipRequest * request = [LCFriendshipRequest objectWithObjectId:requestObjectId.GetNSString()];
	[TDSFriends deleteFriendRequest:request callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"deleteFriendRequest", FTapFriendsModule::OnFriendDeleteRequest);
	}];
#endif
}

void UTapFriendsBPLibrary::QueryFriendsList(int64 from, int64 limit)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("from"), from);
	Writer->WriteValue(TEXT("limit"), limit);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryFriendsList"), JsonOutString, true,
		TEXT(TAP_FRIENDS_QUERY_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	TDSFriendQueryOption *option = [[TDSFriendQueryOption alloc] init];
	option.from = from;
	option.limit = limit;
	[TDSFriends queryFriendWithOption:option callback:^(NSArray<TDSFriendInfo *> * _Nullable friendInfos, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendInfos(friendInfos, error, @"queryFriendWithOption", FTapFriendsModule::OnQueryFriendsList);
	}];
#endif
}

void UTapFriendsBPLibrary::CheckFriendShip(FString objectId)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("checkFriendShip"), JsonOutString, true,
		TEXT(TAP_FRIENDS_CHECK_SHIP_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends checkFriendshipWithUserId:objectId.GetNSString() callback:^(NSNumber * _Nullable isFriend, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(isFriend.boolValue, error, @"checkFriendshipWithUserId", FTapFriendsModule::OnCheckFriendShip);
	}];
#endif
}

void UTapFriendsBPLibrary::SetShareLink(FString link)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("link"), link);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("setShareLink"), JsonOutString, false,
		TEXT(TAP_FRIENDS_CHECK_SHIP_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFriends setShareLink:link.GetNSString()];
#endif
}

void UTapFriendsBPLibrary::FollowByShortCode(FString shortCode, TMap<FString, FString> attrs)
{
#if PLATFORM_IOS
	[TDSFollows followWithShortCode:shortCode.GetNSString() attributes:IOSHelper::convertMap(attrs) callback:^(TDSFriendHandleResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendHandleResult(result, error, @"followWithShortCode", FTapFriendsModule::OnFollowByShortCode);
	}];
#elif PLATFORM_ANDROID


	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), shortCode);
	Writer->WriteValue(TEXT("attrs"), TapJson::ToJSON(attrs));
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followByShortCode"), JsonOutString, false,
		TEXT(TAP_FRIENDS_FOLLOW_BY_SHORT_CODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::FollowByObjectId(FString objectId, TMap<FString, FString> attrs)
{
#if PLATFORM_IOS
	[TDSFollows followWithUserId:objectId.GetNSString() attributes:IOSHelper::convertMap(attrs) callback:^(TDSFriendHandleResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendHandleResult(result, error, @"followWithUserId", FTapFriendsModule::OnFollowByObjectId);
	}];
#elif PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteValue(TEXT("attrs"), TapJson::ToJSON(attrs));
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followByObjectId"), JsonOutString, false,
		TEXT(TAP_FRIENDS_FOLLOW_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::UnFollowByObjectId(FString objectId)
{
#if PLATFORM_IOS
	[TDSFollows unfollowWithUserId:objectId.GetNSString() callback:^(TDSFriendHandleResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendHandleResult(result, error, @"unfollowWithUserId", FTapFriendsModule::OnUnFollowByObjectId);
	}];
#elif PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("unFollowByObjectId"), JsonOutString, false,
		TEXT(TAP_FRIENDS_UN_FOLLOW_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::UnFollowByShortCode(FString shortCode)
{
#if PLATFORM_IOS
	[TDSFollows unfollowWithShortCode:shortCode.GetNSString() callback:^(TDSFriendHandleResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendHandleResult(result, error, @"unfollowWithShortCode", FTapFriendsModule::OnUnFollowByShortCode);
	}];
#elif PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), shortCode);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("unFollowByShortCode"), JsonOutString, false,
		TEXT(TAP_FRIENDS_UN_FOLLOW_BY_SHORT_CODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}


void UTapFriendsBPLibrary::FollowBlockByObjectId(FString ObjectId)
{
#if PLATFORM_IOS
	[TDSFollows blockWithObjectId:ObjectId.GetNSString() callback:^(TDSFriendHandleResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendHandleResult(result, error, @"blockWithObjectId", FTapFriendsModule::OnBlockByObjectId);
	}];
#elif PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), ObjectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followBlockByObjectId"), JsonOutString, false,
		TEXT(TAP_FRIENDS_FOLLOW_BLOCK_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::FollowUnBlockByObjectId(FString ObjectId)
{
#if PLATFORM_IOS
	[TDSFollows unblockWithObjectId:ObjectId.GetNSString() callback:^(TDSFriendHandleResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendHandleResult(result, error, @"unblockWithObjectId", FTapFriendsModule::OnUnBlockByObjectId);
	}];
#elif PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), ObjectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followUnBlockByObjectId"), JsonOutString, false,
		TEXT(TAP_FRIENDS_FOLLOW_UNBLOCK_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

}

void UTapFriendsBPLibrary::FollowBlockByShortCode(FString ShortCode)
{
#if PLATFORM_IOS
	[TDSFollows blockWithShortCode:ShortCode.GetNSString() callback:^(TDSFriendHandleResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendHandleResult(result, error, @"blockWithShortCode", FTapFriendsModule::OnBlockByShortCode);
	}];
#elif PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), ShortCode);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followBlockByShortCode"), JsonOutString, false,
		TEXT(TAP_FRIENDS_FOLLOW_BLOCK_BY_SHORTCODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

}

void UTapFriendsBPLibrary::FollowUnBlockByShortCode(FString ShortCode)
{
#if PLATFORM_IOS
	[TDSFollows unblockWithShortCode:ShortCode.GetNSString() callback:^(TDSFriendHandleResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendHandleResult(result, error, @"unblockWithShortCode", FTapFriendsModule::OnUnBlockByShortCode);
	}];
#elif PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), ShortCode);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followUnBlockByShortCode"), JsonOutString, false,
		TEXT(TAP_FRIENDS_FOLLOW_UNBLOCK_BY_SHORTCODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

}

void UTapFriendsBPLibrary::FollowQueryMutualList(int64 pageSize, FString cursor, FString sortCondition)
{
#if PLATFORM_IOS
	TDSFriendQueryOption *option = [[TDSFriendQueryOption alloc] init];
	option.from = FCString::Atoi(*cursor);
	option.limit = pageSize;
	option.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[option.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryMutualListWithOption:option callback:^(TDSFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendResult(result, error, @"queryMutualListWithOption", FTapFriendsModule::OnQueryFollowMutalList);
	}];
#elif PLATFORM_ANDROID


	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), pageSize);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followQueryMutualList"), JsonOutString, false,
		TEXT(TAP_FRIENDS_QUERY_FOLLOW_MUTAL_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::FollowQueryFolloweeList(int64 pageSize, FString cursor, FString sortCondition)
{
#if PLATFORM_IOS
	TDSFriendQueryOption *option = [[TDSFriendQueryOption alloc] init];
	option.from = FCString::Atoi(*cursor);
	option.limit = pageSize;
	option.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[option.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryFolloweeWithOption:option callback:^(TDSFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendResult(result, error, @"queryFolloweeWithOption", FTapFriendsModule::OnQueryFolloweeList);
	}];
#elif PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), pageSize);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followQueryFolloweeList"), JsonOutString, false,
		TEXT(TAP_FRIENDS_QUERY_FOLLOWEE_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::FollowQueryFollowerList(int64 pageSize, FString cursor, FString sortCondition)
{
#if PLATFORM_IOS
	TDSFriendQueryOption *option = [[TDSFriendQueryOption alloc] init];
	option.from = FCString::Atoi(*cursor);
	option.limit = pageSize;
	option.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[option.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryFollowerWithOption:option callback:^(TDSFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendResult(result, error, @"queryFollowerWithOption", FTapFriendsModule::OnQueryFollowerList);
	}];
#elif PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), pageSize);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followQueryFollowerList"), JsonOutString, false,
		TEXT(TAP_FRIENDS_QUERY_FOLLOWER_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::FollowQueryBlockList(int64 PageSize, FString cursor, FString sortCondition)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), PageSize);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followQueryBlockList"), JsonOutString, true,
		TEXT(TAP_FRIENDS_QUERY_BLOCK_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	TDSFriendQueryOption *option = [[TDSFriendQueryOption alloc] init];
	option.from = FCString::Atoi(*cursor);
	option.limit = PageSize;
	option.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[option.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryBlockListWithOption:option callback:^(TDSFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastFriendResult(result, error, @"queryBlockListWithOption", FTapFriendsModule::OnFollowQueryBlockList);
	}];
#endif
}

void UTapFriendsBPLibrary::FollowTapUserByShortCode(FString shortCode)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), shortCode);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followTapUserByShortCode"), JsonOutString, false,
		TEXT(TAP_FRIENDS_FOLLOW_TAP_USER_BY_SHORT_CODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFollows followTapUserWithShortCode:shortCode.GetNSString() callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"followTapUserWithShortCode", FTapFriendsModule::OnFollowTapUserByShortCode);
	}];
#endif
}

void UTapFriendsBPLibrary::UnFollowTapUserByShortCode(FString shortCode)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), shortCode);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("unfollowTapUserByShortCode"), JsonOutString, false,
		TEXT(TAP_FRIENDS_UN_FOLLOW_TAP_USER_BY_SHORT_CODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFollows unfollowTapUserWithShortCode:shortCode.GetNSString() callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"unfollowTapUserWithShortCode", FTapFriendsModule::OnUnFollowTapUserByShortCode);
	}];
#endif
}

void UTapFriendsBPLibrary::BlockTapUserByShortCode(FString shortCode)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), shortCode);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("blockTapUserByShortCode"), JsonOutString, false,
		TEXT(TAP_FRIENDS_BLOCK_TAP_USER_BY_SHORT_CODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFollows blockTapUserWithShortCode:shortCode.GetNSString() callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"blockTapUserWithShortCode", FTapFriendsModule::OnBlockTapUserByShortCode);
	}];
#endif
}

void UTapFriendsBPLibrary::UnBlockTapUserByShortCode(FString shortCode)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("shortCode"), shortCode);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("unBlockTapUserByShortCode"), JsonOutString, false,
		TEXT(TAP_FRIENDS_UN_BLOCK_TAP_USER_BY_SHORT_CODE_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFollows unblockTapUserWithShortCode:shortCode.GetNSString() callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"unblockTapUserWithShortCode", FTapFriendsModule::OnUnBlockTapUserByShortCode);
	}];
#endif
}

void UTapFriendsBPLibrary::FollowTapUserByObjectId(FString objectId)
{
#if PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("followTapUserByObjectId"), JsonOutString, false,
		TEXT(TAP_FRIENDS_FOLLOW_TAP_USER_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
#if PLATFORM_IOS
	[TDSFollows followTapUserWithObjectId:objectId.GetNSString() callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"followTapUserWithObjectId", FTapFriendsModule::OnFollowTapUserByObjectId);
	}];
#endif
}

void UTapFriendsBPLibrary::UnFollowTapUserByObjectId(FString objectId)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("unfollowTapUserByObjectId"), JsonOutString, false,
		TEXT(TAP_FRIENDS_UN_FOLLOW_TAP_USER_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFollows unfollowTapUserWithObjectId:objectId.GetNSString() callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"unfollowTapUserWithObjectId", FTapFriendsModule::OnUnFollowTapUserByObjectId);
	}];

#endif
}

void UTapFriendsBPLibrary::BlockTapUserByObjectId(FString objectId)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("blockTapUserByObjectId"), JsonOutString, false,
		TEXT(TAP_FRIENDS_BLOCK_TAP_USER_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFollows blockTapUser:[TDSUser objectWithObjectId:objectId.GetNSString()] callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"blockTapUser", FTapFriendsModule::OnBlockTapUserByObjectId);
	}];
#endif
}

void UTapFriendsBPLibrary::UnBlockTapUserByObjectId(FString objectId)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("objectId"), objectId);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("unBlockTapUserByObjectId"), JsonOutString, false,
		TEXT(TAP_FRIENDS_UN_BLOCK_TAP_USER_BY_OBJECT_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	[TDSFollows unblockTapUser:[TDSUser objectWithObjectId:objectId.GetNSString()] callback:^(BOOL succeeded, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"unblockTapUser", FTapFriendsModule::OnUnBlockTapUserByObjectId);
	}];
#endif
}

// 这里应该接queryThirdPartyFriendListWithOption，但是其实和三方互关走的一样的接口
void UTapFriendsBPLibrary::QueryThirdPartyFriendList(FString platform, int64 pageSize, FString cursor,
														  FString sortCondition, int cachePolicy)
{
#if PLATFORM_IOS
	TDSThirdPartyFriendQueryOption *tpOption = [[TDSThirdPartyFriendQueryOption alloc] init];
	tpOption.cachePolicy = cachePolicy == 1 ? TDSFriendCachePolicyOnlyNetwork : TDSFriendCachePolicyCacheElseNetwork;;
	tpOption.pageSize = pageSize;
	tpOption.cursor = cursor.GetNSString();
	tpOption.platform = platform.GetNSString();
	tpOption.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[tpOption.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryThirdPartyMutualListWithOption:tpOption callback:^(TDSThirdPartyFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastThirdPartyFriendResult(result, error, @"queryThirdPartyMutualListWithOption", FTapFriendsModule::OnQueryThirdPartyFriendList);
	}];
#elif PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), pageSize);
	Writer->WriteValue(TEXT("platform"), platform);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteValue(TEXT("cachePolicy"), cachePolicy);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryThirdPartyMutualList"), JsonOutString, false,
		TEXT(TAP_FRIENDS_QUERY_THIRD_Friend_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::QueryThirdPartyFriendMutalList(FString platform, int64 pageSize, FString cursor,
														  FString sortCondition, int cachePolicy)
{
#if PLATFORM_IOS
	TDSThirdPartyFriendQueryOption *tpOption = [[TDSThirdPartyFriendQueryOption alloc] init];
	tpOption.cachePolicy = cachePolicy == 1 ? TDSFriendCachePolicyOnlyNetwork : TDSFriendCachePolicyCacheElseNetwork;;
	tpOption.pageSize = pageSize;
	tpOption.cursor = cursor.GetNSString();
	tpOption.platform = platform.GetNSString();
	tpOption.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[tpOption.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryThirdPartyMutualListWithOption:tpOption callback:^(TDSThirdPartyFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastThirdPartyFriendResult(result, error, @"queryThirdPartyMutualListWithOption", FTapFriendsModule::OnQueryThirdPartyFriendMutalList);
	}];
#elif PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), pageSize);
	Writer->WriteValue(TEXT("platform"), platform);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteValue(TEXT("cachePolicy"), cachePolicy);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryThirdPartyMutualList"), JsonOutString, false,
		TEXT(TAP_FRIENDS_QUERY_THIRD_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif
}

void UTapFriendsBPLibrary::QueryThirdPartyFriendFolloweeList(FString platform, int64 pageSize, FString cursor,
                                                             FString sortCondition, int cachePolicy)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), pageSize);
	Writer->WriteValue(TEXT("platform"), platform);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteValue(TEXT("cachePolicy"), cachePolicy);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryThirdFolloweeList"), JsonOutString, false,
		TEXT(TAP_FRIENDS_QUERY_THIRD_FOLLOWEE_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	TDSThirdPartyFriendQueryOption *tpOption = [[TDSThirdPartyFriendQueryOption alloc] init];
	tpOption.cachePolicy = cachePolicy == 1 ? TDSFriendCachePolicyOnlyNetwork : TDSFriendCachePolicyCacheElseNetwork;;
	tpOption.pageSize = pageSize;
	tpOption.cursor = cursor.GetNSString();
	tpOption.platform = platform.GetNSString();
	tpOption.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[tpOption.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryThirdPartyFolloweeListWithOption:tpOption callback:^(TDSThirdPartyFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastThirdPartyFriendResult(result, error, @"queryThirdPartyFolloweeListWithOption", FTapFriendsModule::OnQueryThirdPartyFriendFolloweeList);
	}];
#endif
}

void UTapFriendsBPLibrary::QueryThirdPartyFriendFollowerList(FString platform, int64 pageSize, FString cursor,
                                                             FString sortCondition, int cachePolicy)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), pageSize);
	Writer->WriteValue(TEXT("platform"), platform);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteValue(TEXT("cachePolicy"), cachePolicy);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryThirdFollowerList"), JsonOutString, false,
		TEXT(TAP_FRIENDS_QUERY_THIRD_FOLLOWER_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	TDSThirdPartyFriendQueryOption *tpOption = [[TDSThirdPartyFriendQueryOption alloc] init];
	tpOption.cachePolicy = cachePolicy == 1 ? TDSFriendCachePolicyOnlyNetwork : TDSFriendCachePolicyCacheElseNetwork;;
	tpOption.pageSize = pageSize;
	tpOption.cursor = cursor.GetNSString();
	tpOption.platform = platform.GetNSString();
	tpOption.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[tpOption.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryThirdPartyFollowerListWithOption:tpOption callback:^(TDSThirdPartyFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastThirdPartyFriendResult(result, error, @"queryThirdPartyFollowerListWithOption", FTapFriendsModule::OnQueryThirdPartyFriendFollowerList);
	}];
#endif
}

void UTapFriendsBPLibrary::QueryThirdPartyFriendBlockList(FString platform, int64 pageSize, FString cursor,
                                                          FString sortCondition, int cachePolicy)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("pageSize"), pageSize);
	Writer->WriteValue(TEXT("platform"), platform);
	Writer->WriteValue(TEXT("cursor"), cursor);
	Writer->WriteValue(TEXT("sortCondition"), sortCondition);
	Writer->WriteValue(TEXT("cachePolicy"), cachePolicy);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("queryThirdPartyBlockList"), JsonOutString, false,
		TEXT(TAP_FRIENDS_QUERY_THIRD_BLOCK_LIST_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	TDSThirdPartyFriendQueryOption *tpOption = [[TDSThirdPartyFriendQueryOption alloc] init];
	tpOption.cachePolicy = cachePolicy == 1 ? TDSFriendCachePolicyOnlyNetwork : TDSFriendCachePolicyCacheElseNetwork;;
	tpOption.pageSize = pageSize;
	tpOption.cursor = cursor.GetNSString();
	tpOption.platform = platform.GetNSString();
	tpOption.sortCondition = [[TDSFriendQuerySortCondition alloc] init];
	[tpOption.sortCondition append:sortCondition.GetNSString() error:nil];
	[TDSFollows queryThirdPartyBlockListWithOption:tpOption callback:^(TDSThirdPartyFriendResult * _Nullable result, NSError * _Nullable error) {
		TapFriendIOSHelper::BroadcastThirdPartyFriendResult(result, error, @"queryThirdPartyBlockListWithOption", FTapFriendsModule::OnQueryThirdPartyFriendBlockList);
	}];
#endif
}

void UTapFriendsBPLibrary::Toast(FString toast)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("toast"), toast);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("friendsToast"), JsonOutString, false,
		TEXT(""),
		true);

	GetBridge()->CallHandler(Command);
#endif
#if PLATFORM_IOS

#endif
}


void UTapFriendsBPLibrary::GenerateFriendInvitationLink(FString RoleName, TMap<FString, FString> attrs)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("roleName"), RoleName);
	Writer->WriteValue(TEXT("queries"), TapJson::ToJSON(attrs));
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("generateFriendInvitationLink"), JsonOutString, true,
		TEXT(TAP_FRIENDS_GENERATE_FRIEND_INVITATION_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	TDSFriendLinkOption *option = [[TDSFriendLinkOption alloc] init];
	if (RoleName.IsEmpty())
	{
		option.roleName = nil;
	} else
	{
		option.roleName = RoleName.GetNSString();
	}
	option.queries = IOSHelper::convertMap(attrs);
	NSError *error = nil;
	NSString *link = [TDSFriends generateFriendInvitationLinkWithOption:option error:&error];
	FTapError Error;
	if (error) {
		Error = IOSHelper::convertError(error);
		NSLog(@"TDSFriendLogError-%@: %@", @"generateFriendInvitationLinkWithOption", error);
	}
	NSLog(@"TDSFriendLogInfo-%@: %@", @"generateFriendInvitationLinkWithOption", link);
	FTapFriendsModule::OnGenerateFriendInvitation.Broadcast(IOSHelper::convertString(link), Error);
#endif
}

void UTapFriendsBPLibrary::FastGenerateFriendInvitationLink()
{
	TMap<FString, FString> temp;
	GenerateFriendInvitationLink(TEXT(""), temp);
}

void UTapFriendsBPLibrary::ParseFriendInvitationLink(FString Uri)
{
#if PLATFORM_ANDROID
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("uri"), Uri);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("parseFriendInvitationLink"), JsonOutString, true,
		TEXT(TAP_FRIENDS_FOLLOW_PARSE_LINK_INFO_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	NSString *urlStr = Uri.GetNSString();
	if (urlStr) {
		NSURL *url = [NSURL URLWithString:urlStr];
		TDSFriendLinkInfo *info = [TDSFriends parseFriendInvitationLink:url];
		FTapFriendLinkInfo LinkInfo;
		LinkInfo.queries = IOSHelper::getJson(info.queries);
		LinkInfo.roleName = IOSHelper::convertString(info.roleName);
		LinkInfo.identity = IOSHelper::convertString(info.identity);
		FTapFriendsModule::OnParseFriendLinkInfo.Broadcast(LinkInfo);
	}
#endif
}

void UTapFriendsBPLibrary::HandFriendInvitationLink(FString Uri)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("uri"), Uri);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("handFriendInvitationLink"), JsonOutString, true,
		TEXT(TAP_FRIENDS_HANDLE_INVITATION_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	NSString *urlStr = Uri.GetNSString();
	if (urlStr) {
		NSURL *url = [NSURL URLWithString:urlStr];
		[TDSFriends handleFriendInvitationLink:url callback:^(BOOL succeeded, TDSFriendLinkInfo * _Nullable linkInfo, NSError * _Nullable error) {
			TapFriendIOSHelper::BroadcastBoolResult(succeeded, error, @"handleFriendInvitationLink", FTapFriendsModule::OnHandleFriendInvitationLink);
		}];
	}
#endif
}

void UTapFriendsBPLibrary::HandFollowInvitationLink(FString Uri)
{
#if PLATFORM_ANDROID

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("uri"), Uri);
	Writer->WriteObjectEnd();
	Writer->Close();

	const FString Command = TapJson::ConstructorCommand(
		TEXT(TDS_FRIENDS_SERVICE),TEXT("handlerFollowInvitationLink"), JsonOutString, true,
		TEXT(TAP_FRIENDS_FOLLOW_HANDLE_INVITATION_ID),
		true);

	GetBridge()->CallHandler(Command);
#endif

#if PLATFORM_IOS
	NSString *urlStr = Uri.GetNSString();
	if (urlStr) {
		NSURL *url = [NSURL URLWithString:urlStr];
        [TDSFollows handleFriendInvitationLink:url callback:^(TDSFriendHandleResult * _Nullable result, TDSFriendLinkInfo * _Nullable linkInfo, NSError * _Nullable error) {
        	FTapError Error;
			if (error) {
				Error = IOSHelper::convertError(error);
				NSLog(@"TDSFriendLogError-%@: %@", @"handleFriendInvitationLink", error);
			}
        	NSLog(@"TDSFriendLogInfo-%@: %@", @"handleFriendInvitationLink", result);
        	FTapFriendsModule::OnHandleFollowInvitationLink.Broadcast(result.code, IOSHelper::convertString(result.message), Error);
		}];
	}
#endif
}



void UTapFriendsBPLibrary::OnBridgeCallback(const FString& result)
{
	UE_LOG(LogTemp, Warning, TEXT("TapFriends OnBridgeCallback:%s"), *result);

	FTapResult TapResult;
	const bool ParseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FTapResult>(result, &TapResult, 0, 0);
	if (!ParseSuccess)
	{
		return;
	}

	FTapFriendResponse Response;
	const bool ParseResponse = FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendResponse>(
		TapResult.content, &Response, 0, 0);
	if (!ParseResponse)
	{
		return;
	}

	// Handler TapFriends status change callback
	if (TapResult.callbackId.Equals(TAP_FRIENDS_STATUS_CHANGED_LISTENER_ID))
	{
		if (Response.code == 1)
		{
			FTapFriendshipRequestAndInfo requestInfo;
			FJsonObjectConverter::JsonObjectStringToUStruct(Response.content, &requestInfo, 0, 0);
			FTapFriendsModule::OnRequestComming.Broadcast(requestInfo);
		}
		else if (Response.code == 2)
		{
			FTapFriendshipRequestAndInfo requestInfo;
			FJsonObjectConverter::JsonObjectStringToUStruct(Response.content, &requestInfo, 0, 0);
			FTapFriendsModule::OnRequestAccepted.Broadcast(requestInfo);
		}
		else if (Response.code == 3)
		{
			FTapFriendshipRequestAndInfo requestInfo;
			FJsonObjectConverter::JsonObjectStringToUStruct(Response.content, &requestInfo, 0, 0);
			FTapFriendsModule::OnRequestDeclined.Broadcast(requestInfo);
		}
		else if (Response.code == 4)
		{
			FTapFriendsModule::OnFriendOnline.Broadcast(Response.content);
		}
		else if (Response.code == 5)
		{
			FTapFriendsModule::OnFriendOffline.Broadcast(Response.content);
		}
		else if (Response.code == 6)
		{
			const TSharedPtr<FJsonObject> Content = TapJson::Parse(Response.content);
			const FString UserId = Content->GetStringField(TEXT("userId"));
			const TSharedPtr<FJsonObject> RichPresence = Content->GetObjectField(TEXT("richPresence"));
			const FString RichPresenceJson = TapJson::ToJSON(RichPresence);
			FTapFriendsModule::OnFriendRichPresenceChanged.Broadcast(UserId, RichPresenceJson);
		}
		else if (Response.code == 7)
		{
			FTapFriendInfo FriendInfo;
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendInfo>(Response.content, &FriendInfo, 0, 0);
			FTapFriendsModule::OnFriendAdd.Broadcast(FriendInfo);
		}
		else if (Response.code == 8)
		{
			FTapFriendsModule::OnFriendConnected.Broadcast();
		}
		else if (Response.code == 9)
		{
			FTapFriendsModule::OnFriendDisconnected.Broadcast();
		}
		else if (Response.code == 10)
		{
			FTapError Error;
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
			FTapFriendsModule::OnConnectedError.Broadcast(Error);
		}
		// else if (Response.code == 11)
		// {
		// 	const TSharedPtr<FJsonObject> Content = TapJson::Parse(Response.content);
		// 	const FString NickName = Content->GetStringField(TEXT("nickName"));
		// 	const bool Success = Content->GetBoolField(TEXT("isSuccess"));
		// 	TSharedPtr<FJsonObject> ErrorJsonObject = Content->GetObjectField(TEXT("error"));
		// 	struct FTapError Error;
		// 	Error.code = ErrorJsonObject->GetIntegerField("code");
		// 	Error.error_description = ErrorJsonObject->GetStringField("detailMessage");
		// 	FTapFriendsModule::OnSendFriendRequestToSharer.Broadcast(Success, NickName, Error);
		// }
		return;
	}
	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_FRIEND_REQUEST_ID))
	{
		TArray<FTapFriendshipRequest> Requests;
		FTapError Error;
		if (Response.success)
		{
			FJsonObjectConverter::JsonArrayStringToUStruct(Response.content, &Requests, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnFriendQueryRequest.Broadcast(Requests, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_FRIEND_REQUEST_INFO_ID))
	{
		TArray<FTapFriendshipRequestAndInfo> Requests;
		FTapError Error;
		if (Response.success)
		{
			FJsonObjectConverter::JsonArrayStringToUStruct(Response.content, &Requests, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnQueryFriendRequestWithFriendStateList.Broadcast(Requests, Error);
		return;
	}
	if (TapResult.callbackId.Equals(TAP_FRIENDS_ACCEPT_FRIEND_REQUEST_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnAcceptFriendRequest.Broadcast(Response.success, Error);
		return;
	}
	if (TapResult.callbackId.Equals(TAP_FRIENDS_DECLINE_FRIEND_REQUEST_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnFriendDeclineRequest.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_DELETE_FRIEND_REQUEST_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnFriendDeleteRequest.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_SELF_ON_LINE_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnSelfOnline.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_FRIENDS_BY_NICKNAME_ID))
	{
		FTapError Error;
		TArray<FTapFriendInfo> Infos;
		if (Response.success)
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			UE_LOG(LogTemp, Warning, TEXT("TapFriends Parse content before"));
			FJsonSerializer::Deserialize(Reader, Root);
			UE_LOG(LogTemp, Warning, TEXT("TapFriends Parse content after"));
			const TArray<TSharedPtr<FJsonValue>> Members = Root->AsArray();
			if (Members.Num() > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Member Size %d"), Members.Num());

				for (auto JsonValue : Members)
				{
					UE_LOG(LogTemp, Warning, TEXT("Query Result:%s"), *TapJson::ToJSON(JsonValue->AsObject()));

					Infos.Add(ParseFriendInfo(JsonValue->AsObject()));
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Infos Size %d"), Infos.Num());
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnQueryFriendsByNickName.Broadcast(Infos, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_FRIEND_BY_SHORTCODE_ID))
	{
		FTapError Error;
		FTapFriendInfo Info;
		if (Response.success)
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);
			Info = ParseFriendInfo(Root->AsObject());
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnQueryFriendByShortCode.Broadcast(Info, Error);
		return;
	}

		if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_FRIEND_BY_OBJECTID_ID))
		{
			FTapError Error;
			FTapFriendInfo Info;
			if (Response.success)
			{
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
				TSharedPtr<FJsonValue> Root;
				FJsonSerializer::Deserialize(Reader, Root);
				Info = ParseFriendInfo(Root->AsObject());
			}
			else
			{
				FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
			}
			FTapFriendsModule::OnQueryFriendByObjectId.Broadcast(Info, Error);
			return;
		}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_SAVE_RICH_PRESENCE_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnSaveRichPresence.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_CLEAR_RICH_PRESENCE_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnClearRichPresence.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_ADD_FRIEND_BY_SHORT_CODE_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnAddFriendByShortCode.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_ADD_FRIEND_BY_OBJECT_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnAddFriendByObjectId.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_DELETE_FRIEND_BY_OBJECT_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnFriendDeleteByObjectId.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_LIST_ID))
	{
		FTapError Error;
		TArray<FTapFriendInfo> Infos;
		if (Response.success)
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);
			const TArray<TSharedPtr<FJsonValue>> Members = Root->AsArray();
			if (Members.Num() > 0)
			{
				for (auto JsonValue : Members)
				{
					Infos.Add(ParseFriendInfo(JsonValue->AsObject()));
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("Infos Size %d"), Infos.Num());
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnQueryFriendsList.Broadcast(Infos, Error);
		return;
	}
	if (TapResult.callbackId.Equals(TAP_FRIENDS_CHECK_SHIP_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnCheckFriendShip.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_GENERATE_FRIEND_INVITATION_ID))
	{
		FTapError Error;
		FString Link;
		if (Response.success == false)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			Link = Response.content;
		}
		FTapFriendsModule::OnGenerateFriendInvitation.Broadcast(Link, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_BY_OBJECT_ID))
	{
		FTapError Error;
		FTapFriendHandleResult Result;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(Response.content, &Result, 0, 0);
		}
		FTapFriendsModule::OnFollowByObjectId.Broadcast(Result.code, Result.message, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_BY_SHORT_CODE_ID))
	{
		FTapError Error;
		FTapFriendHandleResult Result;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(Response.content, &Result, 0, 0);
		}
		FTapFriendsModule::OnFollowByShortCode.Broadcast(Result.code, Result.message, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_UN_FOLLOW_BY_OBJECT_ID))
	{
		FTapError Error;
		FTapFriendHandleResult Result;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(Response.content, &Result, 0, 0);
		}
		FTapFriendsModule::OnUnFollowByObjectId.Broadcast(Result.code, Result.message, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_UN_FOLLOW_BY_SHORT_CODE_ID))
	{
		FTapError Error;
		FTapFriendHandleResult Result;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(Response.content, &Result, 0, 0);
		}
		FTapFriendsModule::OnUnFollowByShortCode.Broadcast(Result.code, Result.message, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_FOLLOW_MUTAL_LIST_ID))
	{
		FTapError Error;
		TArray<FTapFriendInfo> Infos;
		FString Cursor;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectFriendInfos(Root);
		}
		FTapFriendsModule::OnQueryFollowMutalList.Broadcast(Cursor, Infos, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_TAP_USER_BY_SHORT_CODE_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnFollowTapUserByShortCode.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_UN_FOLLOW_TAP_USER_BY_SHORT_CODE_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnUnFollowTapUserByShortCode.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_UN_BLOCK_TAP_USER_BY_SHORT_CODE_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnUnBlockTapUserByShortCode.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_BLOCK_TAP_USER_BY_SHORT_CODE_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnBlockTapUserByShortCode.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_BLOCK_TAP_USER_BY_OBJECT_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnBlockTapUserByObjectId.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_UN_BLOCK_TAP_USER_BY_OBJECT_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnUnBlockTapUserByObjectId.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_TAP_USER_BY_OBJECT_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnFollowTapUserByObjectId.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_UN_FOLLOW_TAP_USER_BY_OBJECT_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnUnFollowTapUserByObjectId.Broadcast(Response.success, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_FOLLOWEE_LIST_ID))
	{
		FTapError Error;
		TArray<FTapFriendInfo> Infos;
		FString Cursor;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectFriendInfos(Root);
		}
		FTapFriendsModule::OnQueryFolloweeList.Broadcast(Cursor, Infos, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_FOLLOWER_LIST_ID))
	{
		FTapError Error;
		TArray<FTapFriendInfo> Infos;
		FString Cursor;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectFriendInfos(Root);
		}
		FTapFriendsModule::OnQueryFollowerList.Broadcast(Cursor, Infos, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_BLOCK_LIST_ID))
	{
		FTapError Error;
		TArray<FTapFriendInfo> Infos;
		FString Cursor;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectFriendInfos(Root);
		}
		FTapFriendsModule::OnFollowQueryBlockList.Broadcast(Cursor, Infos, Error);
		return;
	}
	
	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_THIRD_Friend_LIST_ID))
	{
		FTapError Error;
		FString Cursor;
		TArray<FTapThirdPartyFriendInfo> Infos;

		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectThirdPartyFriendInfos(Root);
		}
		FTapFriendsModule::OnQueryThirdPartyFriendList.Broadcast(Cursor, Infos, Error);
		return;
	}
	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_THIRD_LIST_ID))
	{
		FTapError Error;
		FString Cursor;
		TArray<FTapThirdPartyFriendInfo> Infos;

		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectThirdPartyFriendInfos(Root);
		}
		FTapFriendsModule::OnQueryThirdPartyFriendMutalList.Broadcast(Cursor, Infos, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_THIRD_FOLLOWEE_LIST_ID))
	{
		FTapError Error;
		FString Cursor;
		TArray<FTapThirdPartyFriendInfo> Infos;

		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectThirdPartyFriendInfos(Root);
		}
		FTapFriendsModule::OnQueryThirdPartyFriendFolloweeList.Broadcast(Cursor, Infos, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_THIRD_FOLLOWER_LIST_ID))
	{
		FTapError Error;
		FString Cursor;
		TArray<FTapThirdPartyFriendInfo> Infos;

		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectThirdPartyFriendInfos(Root);
		}
		FTapFriendsModule::OnQueryThirdPartyFriendFollowerList.Broadcast(Cursor, Infos, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_QUERY_THIRD_BLOCK_LIST_ID))
	{
		FTapError Error;
		FString Cursor;
		TArray<FTapThirdPartyFriendInfo> Infos;

		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response.content);
			TSharedPtr<FJsonValue> Root;
			FJsonSerializer::Deserialize(Reader, Root);

			Root->AsObject()->TryGetStringField(TEXT("cursor"), Cursor);
			Infos = CollectThirdPartyFriendInfos(Root);
		}
		FTapFriendsModule::OnQueryThirdPartyFriendBlockList.Broadcast(Cursor, Infos, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_HANDLE_INVITATION_ID))
	{
		FTapError Error;
		if (Response.success == false && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		FTapFriendsModule::OnHandleFriendInvitationLink.Broadcast(Response.success, Error);
		return;
	}
	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_HANDLE_INVITATION_ID))
	{
		FTapError Error;
		FTapFriendHandleResult HandleResult;
		if (Response.success == false)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(
				Response.content, &HandleResult, 0, 0);
		}
		FTapFriendsModule::OnHandleFollowInvitationLink.Broadcast(HandleResult.code, HandleResult.message, Error);
		return;
	}
	
	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_PARSE_LINK_INFO_ID))
	{
		FTapFriendLinkInfo LinkInfo;

		if (Response.success == true && Response.content.Len() > 0)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendLinkInfo>(Response.content, &LinkInfo, 0, 0);
		}
		FTapFriendsModule::OnParseFriendLinkInfo.Broadcast(LinkInfo);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_BLOCK_BY_OBJECT_ID))
	{
		FTapError Error;
		FTapFriendHandleResult HandleResult;
		if (Response.success == false)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(
				Response.content, &HandleResult, 0, 0);
		}
		FTapFriendsModule::OnBlockByObjectId.Broadcast(HandleResult.code, HandleResult.message, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_UNBLOCK_BY_OBJECT_ID))
	{
		FTapError Error;
		FTapFriendHandleResult HandleResult;
		if (Response.success == false)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(
				Response.content, &HandleResult, 0, 0);
		}
		FTapFriendsModule::OnUnBlockByObjectId.Broadcast(HandleResult.code, HandleResult.message, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_BLOCK_BY_SHORTCODE_ID))
	{
		FTapError Error;
		FTapFriendHandleResult HandleResult;
		if (Response.success == false)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(
				Response.content, &HandleResult, 0, 0);
		}
		FTapFriendsModule::OnBlockByShortCode.Broadcast(HandleResult.code, HandleResult.message, Error);
		return;
	}

	if (TapResult.callbackId.Equals(TAP_FRIENDS_FOLLOW_UNBLOCK_BY_SHORTCODE_ID))
	{
		FTapError Error;
		FTapFriendHandleResult HandleResult;
		if (Response.success == false)
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(Response.content, &Error, 0, 0);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct<FTapFriendHandleResult>(
				Response.content, &HandleResult, 0, 0);
		}
		FTapFriendsModule::OnUnBlockByShortCode.Broadcast(HandleResult.code, HandleResult.message, Error);
		return;
	}
}

TArray<FTapThirdPartyFriendInfo> UTapFriendsBPLibrary::CollectThirdPartyFriendInfos(TSharedPtr<FJsonValue> Root)
{
	TArray<FTapThirdPartyFriendInfo> Infos;
	const TArray<TSharedPtr<FJsonValue>>* FriendsInfoJsonObject;
	if (Root->AsObject()->TryGetArrayField(TEXT("thirdFriendsInfo"), FriendsInfoJsonObject))
	{
		for (int32 i = 0; i < (*FriendsInfoJsonObject).Num(); i++)
		{
			Infos.Add(ParseThirdFriendInfo((*FriendsInfoJsonObject)[i]));
		}
	}
	return Infos;
}

TArray<FTapFriendInfo> UTapFriendsBPLibrary::CollectFriendInfos(TSharedPtr<FJsonValue> Root)
{
	TArray<FTapFriendInfo> Infos;
	const TArray<TSharedPtr<FJsonValue>>* FriendsInfoJsonObject;
	if (Root->AsObject()->TryGetArrayField(TEXT("friendsInfo"), FriendsInfoJsonObject))
	{
		for (int32 i = 0; i < (*FriendsInfoJsonObject).Num(); i++)
		{
			Infos.Add(ParseFriendInfo((*FriendsInfoJsonObject)[i]->AsObject()));
		}
	}
	return Infos;
}

FTapFriendInfo UTapFriendsBPLibrary::ParseFriendInfo(TSharedPtr<FJsonObject> Root)
{
	FTapFriendInfo Info;
	FTapUser User;
	Info.online = Root->GetBoolField(TEXT("online"));
	Info.richPresence = Root->GetStringField(TEXT("richPresence"));
	const TSharedPtr<FJsonObject> UserInfoJsonObject = Root->GetObjectField("tdsUser");
	UserInfoJsonObject->TryGetBoolField("isAnonymous", User.isAnonymous);
	UserInfoJsonObject->TryGetStringField("sessionToken", User.sessionToken);
	UserInfoJsonObject->TryGetStringField("email", User.email);
	UserInfoJsonObject->TryGetStringField("uuid", User.uuid);
	UserInfoJsonObject->TryGetStringField("phoneNumber", User.phoneNumber);
	UserInfoJsonObject->TryGetStringField("userName", User.userName);
	UserInfoJsonObject->TryGetStringField("shortId", User.shortId);
	UserInfoJsonObject->TryGetStringField("avatar", User.avatar);
	UserInfoJsonObject->TryGetStringField("nickName", User.nickName);
	UserInfoJsonObject->TryGetStringField("serverData", User.serverData);
	Info.tdsUser = User;

	return Info;
}

FTapThirdPartyFriendInfo UTapFriendsBPLibrary::ParseThirdFriendInfo(TSharedPtr<FJsonValue> Root)
{
	FTapThirdPartyFriendInfo ThirdPartyFriendInfo;
	Root->AsObject()->TryGetStringField(TEXT("userId"), ThirdPartyFriendInfo.userId);
	Root->AsObject()->TryGetStringField(TEXT("avatar"), ThirdPartyFriendInfo.avatar);
	Root->AsObject()->TryGetStringField(TEXT("userName"), ThirdPartyFriendInfo.userName);
	const TSharedPtr<FJsonObject> UserInfoJsonObject = Root->AsObject()->GetObjectField("userInfo");
	ThirdPartyFriendInfo.userInfo = ParseFriendInfo(UserInfoJsonObject);
	return ThirdPartyFriendInfo;
}
