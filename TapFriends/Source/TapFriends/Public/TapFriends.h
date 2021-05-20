// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "TapError.h"
#include "TapUserRelationShip.h"

class FTapFriendsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** 添加 Friends Callback */
    DECLARE_MULTICAST_DELEGATE_OneParam(FAddFriend, const bool);

    DECLARE_MULTICAST_DELEGATE_OneParam(FAddFriendError, const FTapError);

	UPROPERTY(BlueprintAssignable, Category = "TapFriends")
    static FAddFriend OnAddFriendCallback;

	UPROPERTY(BlueprintAssignable, Category = "TapFriends")
    static FAddFriendError OnAddFriendError;

	/** 删除好友 Callback */
    DECLARE_MULTICAST_DELEGATE_OneParam(FDeleteFriend, const bool);

    DECLARE_MULTICAST_DELEGATE_OneParam(FDeleteFriendError, const FTapError);

	UPROPERTY(BlueprintAssignable, Category = "TapFriends")
    static FDeleteFriend OnDeleteFriendCallback;

	UPROPERTY(BlueprintAssignable, Category = "TapFriends")
    static FDeleteFriendError OnDeleteFriendError;

	/** 获取关注列表 Callback */

	DECLARE_MULTICAST_DELEGATE_OneParam(FGetFollowingList, const TArray<FTapUserRelationShip>);

    DECLARE_MULTICAST_DELEGATE_OneParam(FGetFollowingListError, const FTapError);

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FGetFollowingList OnGetFollowingListSuccess;

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FGetFollowingListError OnGetFollowingListError;

	/** 获取互相关注 Callback */

	DECLARE_MULTICAST_DELEGATE_OneParam(FGetFollowerList, const TArray<FTapUserRelationShip>);

    DECLARE_MULTICAST_DELEGATE_OneParam(FGetFollowerListError, const FTapError);

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FGetFollowerList OnGetFollowerListSuccess;

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FGetFollowerListError OnGetFollowerListError;

	/** 拉黑 Callback */

	DECLARE_MULTICAST_DELEGATE_OneParam(FBlockUser, const bool);

    DECLARE_MULTICAST_DELEGATE_OneParam(FBlockUserError, const FTapError);

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FBlockUser OnBlockUserCallback;

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FBlockUserError OnBlockUserError;

	/** 解除拉黑 Callback */

	DECLARE_MULTICAST_DELEGATE_OneParam(FUnBlockUser, const bool);

    DECLARE_MULTICAST_DELEGATE_OneParam(FUnBlockUserError, const FTapError);

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FUnBlockUser OnUnBlockUserCallback;

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FUnBlockUserError OnUnBlockUserError;

	/** 获取拉黑列表 Callback */

	DECLARE_MULTICAST_DELEGATE_OneParam(FGetBlockUserList, const TArray<FTapUserRelationShip>);

    DECLARE_MULTICAST_DELEGATE_OneParam(FGetBlockUserListError, const FTapError);

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FGetBlockUserList OnBlockUserListCallback;

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FGetBlockUserListError OnGetBlockUserListError;

	/** 搜索好友 */
	DECLARE_MULTICAST_DELEGATE_OneParam(FSearchUser, const FTapUserRelationShip);

    DECLARE_MULTICAST_DELEGATE_OneParam(FSearchUserError, const FTapError);

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FSearchUser OnSearchCallback;

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FSearchUserError OnSearchError;

    DECLARE_MULTICAST_DELEGATE_OneParam(FGenerateFriendInvitation, const FString &);

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FGenerateFriendInvitation OnGenerateFriendInvitationCallback;

	DECLARE_MULTICAST_DELEGATE_OneParam(FSendFriendInvitation, const FString &);

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FSendFriendInvitation OnSendFriendInvitationCallback;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FRegisterMessageListener,const int,const FString &)

	UPROPERTY(BlueprintAssignable,Category = "TapFriends")
	static FRegisterMessageListener OnMessage;

};
