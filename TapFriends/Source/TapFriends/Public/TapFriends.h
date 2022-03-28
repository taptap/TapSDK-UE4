// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TapError.h"
#include "TapFriendInfo.h"
#include "TapThirdPartyFriendInfo.h"
#include "TapFriendHandleResult.h"
#include "TapFriendLinkInfo.h"
#include "Modules/ModuleManager.h"
#include "TapFriendshipRequest.h"
#include "TapFriendshipRequestAndInfo.h"

class TAPFRIENDS_API FTapFriendsModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


	/** RegisterFriendStatusChanged Callback */
	/** add new Friend (The trigger timing is the same as "The sent friend request is accepted" ) */
	DECLARE_MULTICAST_DELEGATE_OneParam(FFriendAdd, const FTapFriendInfo&);
	UPROPERTY(BlueprintAssignable, Category = "TapFriends")
	static FFriendAdd OnFriendAdd;

	
	DECLARE_MULTICAST_DELEGATE_OneParam(TapFriendInfoCallback, const FTapFriendshipRequestAndInfo&);
	/** add Friend Request */
	UPROPERTY(BlueprintAssignable, Category="TapFriends")
	static TapFriendInfoCallback OnRequestComming;

	// accept friend request
	UPROPERTY(BlueprintAssignable, Category="TapFriends")
	static TapFriendInfoCallback OnRequestAccepted;

	// decline friend request
	UPROPERTY(BlueprintAssignable, Category="TapFriends")
	static TapFriendInfoCallback OnRequestDeclined;

	// friend online
	DECLARE_MULTICAST_DELEGATE_OneParam(FFriendOnline, const FString&)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendOnline OnFriendOnline;

	// friend offline
	DECLARE_MULTICAST_DELEGATE_OneParam(FFriendOffline, const FString&)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendOffline OnFriendOffline;


	// friend rich presence changed
	DECLARE_MULTICAST_DELEGATE_TwoParams(FFriendRichPresenceChanged, const FString&, const FString&)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendRichPresenceChanged OnFriendRichPresenceChanged;

	// friend on connected
	DECLARE_MULTICAST_DELEGATE(FFriendConnectedCallback)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendConnectedCallback OnFriendConnected;

	// friend disconnected
	DECLARE_MULTICAST_DELEGATE(FFriendDisconnected)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendConnectedCallback OnFriendDisconnected;

	// friend disconnected
	DECLARE_MULTICAST_DELEGATE_OneParam(FFriendOnConnectedError, const FTapError&)
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendOnConnectedError OnConnectedError;

	// request friend request list 
	DECLARE_MULTICAST_DELEGATE_TwoParams(FFriendQueryRequest, const TArray<FTapFriendshipRequest>&, const FTapError&)
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendQueryRequest OnFriendQueryRequest;

	// request friend request and info list 
	DECLARE_MULTICAST_DELEGATE_TwoParams(FFriendQueryFriendRequestWithFriendStateList, const TArray<FTapFriendshipRequestAndInfo>&, const FTapError&)
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendQueryFriendRequestWithFriendStateList OnQueryFriendRequestWithFriendStateList;

	// accept friend request
	DECLARE_MULTICAST_DELEGATE_TwoParams(FFriendOperationCallback, const bool, const FTapError&)
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendOperationCallback OnAcceptFriendRequest;

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FFriendFollowOperationCallback, const int64, const FString&,
	                                       const FTapError&)

	// decline friend request
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendOperationCallback OnFriendDeclineRequest;

	// delete friend request
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendOperationCallback OnFriendDeleteRequest;

	// online
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendOperationCallback OnSelfOnline;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FFriendQueryListCallback, const TArray<FTapFriendInfo>&, const FTapError&)

	// query friend by nick name
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendQueryListCallback OnQueryFriendsByNickName;

	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FFriendQueryInfoCallback, const FTapFriendInfo&, const FTapError&)
	// query friend by shortCode
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendQueryInfoCallback OnQueryFriendByShortCode;

	// query friend by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendQueryInfoCallback OnQueryFriendByObjectId;

	// save friend rich presence
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendOperationCallback OnSaveRichPresence;

	// clear friend rich presence
	UPROPERTY(BlueprintAssignable, Category="TapFriendsCore")
	static FFriendOperationCallback OnClearRichPresence;

	// add friend by short code
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendOperationCallback OnAddFriendByShortCode;

	// add friend by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendOperationCallback OnAddFriendByObjectId;

	// delete friend by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendOperationCallback OnFriendDeleteByObjectId;

	// query friend list
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendQueryListCallback OnQueryFriendsList;

	// check friend ship
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendOperationCallback OnCheckFriendShip;

	// generateFriendInvitation
	DECLARE_MULTICAST_DELEGATE_TwoParams(FFriendGenerateFriendInvitation, const FString&, const FTapError&)
	UPROPERTY(BlueprintAssignable, Category="TapFriends pattern")
	static FFriendGenerateFriendInvitation OnGenerateFriendInvitation;

	// follow by shortCode
	UPROPERTY(BlueprintAssignable, Category="TapFollows pattern")
	static FFriendFollowOperationCallback OnFollowByShortCode;

	// follow by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFollows pattern")
	static FFriendFollowOperationCallback OnFollowByObjectId;

	// unfollow by shortCode
	UPROPERTY(BlueprintAssignable, Category="TapFollows pattern")
	static FFriendFollowOperationCallback OnUnFollowByShortCode;

	// unfollow by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFollows pattern")
	static FFriendFollowOperationCallback OnUnFollowByObjectId;

	// query follow mutal list
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FQueryFriendsInfoList, const FString&, const TArray<FTapFriendInfo>&,
	                                       const FTapError&)
	UPROPERTY(BlueprintAssignable, Category="TapFollows pattern")
	static FQueryFriendsInfoList OnQueryFollowMutalList;

	// query followee list
	UPROPERTY(BlueprintAssignable, Category="TapFollows pattern")
	static FQueryFriendsInfoList OnQueryFolloweeList;

	// query follower list
	UPROPERTY(BlueprintAssignable, Category="TapFollows pattern")
	static FQueryFriendsInfoList OnQueryFollowerList;

	UPROPERTY(BlueprintAssignable, Category = "TapFollows Query block list")
	static FQueryFriendsInfoList OnFollowQueryBlockList;

	// follow TapUser by shortCode
	UPROPERTY(BlueprintAssignable, Category="TapFollows TapUser pattern")
	static FFriendOperationCallback OnFollowTapUserByShortCode;

	// unfollow TapUser by shortCode
	UPROPERTY(BlueprintAssignable, Category="TapFollows TapUser pattern")
	static FFriendOperationCallback OnUnFollowTapUserByShortCode;

	// block TapUser by shortCode
	UPROPERTY(BlueprintAssignable, Category="TapFollows TapUser pattern")
	static FFriendOperationCallback OnBlockTapUserByShortCode;

	// block TapUser by shortCode
	UPROPERTY(BlueprintAssignable, Category="TapFollows TapUser pattern")
	static FFriendOperationCallback OnUnBlockTapUserByShortCode;

	// follow TapUser by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFollows TapUser pattern")
	static FFriendOperationCallback OnFollowTapUserByObjectId;

	// unfollow TapUser by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFollows TapUser pattern")
	static FFriendOperationCallback OnUnFollowTapUserByObjectId;

	// block TapUser by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFollows TapUser pattern")
	static FFriendOperationCallback OnBlockTapUserByObjectId;

	// block TapUser by objectId
	UPROPERTY(BlueprintAssignable, Category="TapFollows TapUser pattern")
	static FFriendOperationCallback OnUnBlockTapUserByObjectId;

	// query third party mutal list
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FQueryThirdPartyFriendList, const FString&,
										   const TArray<FTapThirdPartyFriendInfo>&,
										   const FTapError&)
	
	// query third party friend list
	UPROPERTY(BlueprintAssignable, Category="TapFriends ThirdParty pattern")
	static FQueryThirdPartyFriendList OnQueryThirdPartyFriendList;
	
	// query third party mutal list
	UPROPERTY(BlueprintAssignable, Category="TapFriends ThirdParty pattern")
	static FQueryThirdPartyFriendList OnQueryThirdPartyFriendMutalList;

	// query third party follower list
	UPROPERTY(BlueprintAssignable, Category="TapFriends ThirdParty pattern")
	static FQueryThirdPartyFriendList OnQueryThirdPartyFriendFollowerList;

	// query third party follower list
	UPROPERTY(BlueprintAssignable, Category="TapFriends ThirdParty pattern")
	static FQueryThirdPartyFriendList OnQueryThirdPartyFriendFolloweeList;

	// query third party block list
	UPROPERTY(BlueprintAssignable, Category="TapFriends ThirdParty pattern")
	static FQueryThirdPartyFriendList OnQueryThirdPartyFriendBlockList;

	UPROPERTY(BlueprintAssignable, Category="TapFriends HandleFriends Invitation link")
	static FFriendOperationCallback OnHandleFriendInvitationLink;

	UPROPERTY(BlueprintAssignable, Category="TapFollows HandFriends Invitation link")
	static FFriendFollowOperationCallback OnHandleFollowInvitationLink;

	UPROPERTY(BlueprintAssignable, Category="TapFollows Block by ObjectId")
	static FFriendFollowOperationCallback OnBlockByObjectId;

	UPROPERTY(BlueprintAssignable, Category="TapFollows unBlock by ObjectId")
	static FFriendFollowOperationCallback OnUnBlockByObjectId;

	UPROPERTY(BlueprintAssignable, Category="TapFollows Block by code")
	static FFriendFollowOperationCallback OnBlockByShortCode;

	UPROPERTY(BlueprintAssignable, Category="TapFollows unBlock by code")
	static FFriendFollowOperationCallback OnUnBlockByShortCode;

	DECLARE_MULTICAST_DELEGATE_OneParam(FParseFriendLinkInfo, const FTapFriendLinkInfo&);
	UPROPERTY(BlueprintAssignable,Category="TapFollows Parse link FriendInfo")
	static FParseFriendLinkInfo OnParseFriendLinkInfo;
	
};
