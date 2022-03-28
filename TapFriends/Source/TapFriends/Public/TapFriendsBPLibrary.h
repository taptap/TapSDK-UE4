// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TapError.h"
#include "TapFriendInfo.h"
#include "TapThirdPartyFriendInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TapFriendsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class TAPFRIENDS_API UTapFriendsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	// 注册通知
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "register friends status changed"))
	static void RegisterFriendsStatusChangedListener();

	// 注销通知
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "remove friends status changed"))
	static void RemoveFriendsStatusChangedListener();

	// 上线
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core", meta = (DisplayName = "TapFrineds OnLine"))
	static void Online();

	// 下线
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core", meta = (DisplayName = "TapFriends Offline"))
	static void Offline();

	// 通过nick name查询好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "TapFriends Query Friends By NickName"))
	static void QueryFiendsByName(FString nickName);

	// 通过short code查询好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "TapFriends Query Friend By ShortCode"))
	static void QueryFriendByShortCode(FString shortCode);

	// 通过object id查询好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "TapFriends Query Friend By ObjectId"))
	static void QueryFriendByObjectId(FString objectId);

	// 设置富信息
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core", meta = (DisplayName = "TapFriends Save RichPresence"))
	static void SaveRichPresence(TMap<FString, FString> richPresence);

	// 清除富信息
	UFUNCTION(BlueprintCallable, Category = "TapFriends Core",
		meta = (DisplayName = "TapFirends clear RichPresence"))
	static void ClearRicePresence(TArray<FString> richPresenceKeys);

	UFUNCTION(BlueprintCallable, Category = "TapFriends Core", meta =(DisplayName = "TapFriends set share link"))
	static void SetShareLink(FString link);

	UFUNCTION(BlueprintCallable, Category="TapFriends Core", meta=(DisplayName ="TapFriends GenerateFriendInvitationLink"))
	static void GenerateFriendInvitationLink(FString RoleName, TMap<FString, FString> attrs);

	UFUNCTION(BlueprintCallable, Category="TapFriends Core",
		meta=(DisplayName ="TapFriends fast GenerateFriendInvitationLink"))
	static void FastGenerateFriendInvitationLink();

	UFUNCTION(BlueprintCallable, Category="TapFriends Core", meta=(DisplayName ="TapFriends ParseFriendInvitationLink"))
	static void ParseFriendInvitationLink(FString Uri);

	// 通过 short code 添加好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends add Friends by shortCode"))
	static void AddFriendByShortCode(FString shortCode, TMap<FString, FString> attrs);

	// 通过 object id 添加好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends add Friends by objectId"))
	static void AddFriendByObjectId(FString objectId, TMap<FString, FString> attrs);

	// 通过 object id 删除好友
	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends delete Friends by objectId"))
	static void DeleteFriendByObjectId(FString objectId);
	
	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends query Friend request"))
	static void QueryFriendRequestList(int64 status, int64 from, int64 limit);

	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends query Friend request state list"))
	static void QueryFriendRequestWithFriendStateList(int64 status, int64 from, int64 limit);

	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFirends accept Friend request"))
	static void AcceptFriendRequest(FString requestObjectId, TMap<FString, FString> attrs);

	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFriends decline Friend request"))
	static void DeclineFriendRequest(FString requestObjectId);

	UFUNCTION(BlueprintCallable, Category = "TapFriends pattern",
		meta = (DisplayName = "TapFriends delete Friend request"))
	static void DeleteFriendRequest(FString requestObjectId);

	UFUNCTION(BlueprintCallable, Category="TapFriends pattern", meta=(DisplayName= "TapFriends query Friends list"))
	static void QueryFriendsList(int64 from, int64 limit);

	UFUNCTION(BlueprintCallable, Category="TapFrineds pattern", meta=(DisplayName="TapFriends check friendship"))
	static void CheckFriendShip(FString objectId);

	UFUNCTION(BlueprintCallable, Category="TapFriends Pattern",
		meta=(DisplayName ="TapFriends handFriendInvitationLink"))
	static void HandFriendInvitationLink(FString Uri);

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName = "TapFollows follow user by code"))
	static void FollowByShortCode(FString shortCode, TMap<FString, FString> attrs);

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern",
		meta=(DisplayName = "TapFollows follow user by objectId"))
	static void FollowByObjectId(FString objectId, TMap<FString, FString> attrs);

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern",
		meta=(DisplayName = "TapFollows unfollow by objectId"))
	static void UnFollowByObjectId(FString objectId);

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern",
		meta=(DisplayName = "TapFollows unfollow by shortCode"))
	static void UnFollowByShortCode(FString shortCode);

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName="TapFollows block by objectId"))
	static void FollowBlockByObjectId(FString ObjectId);

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName="TapFollows unblock by objectId"))
	static void FollowUnBlockByObjectId(FString ObjectId);

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName="TapFollows block by shortCode"))
	static void FollowBlockByShortCode(FString ShortCode);

	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName="TapFollows unblock by shortCode"))
	static void FollowUnBlockByShortCode(FString ShortCode);

	// 查询互关列表
	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName = "TapFollows query mutual list"))
	static void FollowQueryMutualList(int64 pageSize, FString cursor, FString sortCondition);

	// 查询被关注列表
	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName = "TapFollows query followee list"))
	static void FollowQueryFolloweeList(int64 pageSize, FString cursor, FString sortCondition);

	// 查询关注者列表
	UFUNCTION(BlueprintCallable, Category="TapFollows pattern", meta=(DisplayName = "TapFollows query follower list"))
	static void FollowQueryFollowerList(int64 pageSize, FString cursor, FString sortCondition);

	// 查询黑名单列表
	UFUNCTION(BlueprintCallable, Category="TapFollows Pattern", meta=(DisplayName="TapFollows query block list"))
	static void FollowQueryBlockList(int64 PageSize, FString cursor, FString sortCondition);

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows follow TapUser by code"))
	static void FollowTapUserByShortCode(FString shortCode);

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows unfollow Tapuser by code"))
	static void UnFollowTapUserByShortCode(FString shortCode);

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows follow TapUser by objectId"))
	static void FollowTapUserByObjectId(FString objectId);

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows unfollow Tapuser by objectId"))
	static void UnFollowTapUserByObjectId(FString objectId);

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
	meta=(DisplayName = "TapFollows block TapUser by code"))
	static void BlockTapUserByShortCode(FString shortCode);

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows unblock TapUser by code"))
	static void UnBlockTapUserByShortCode(FString shortCode);

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows block TapUser by objectId"))
	static void BlockTapUserByObjectId(FString objectId);

	UFUNCTION(BlueprintCallable, Category="TapFollows Tap pattern",
		meta=(DisplayName = "TapFollows unblock TapUser by objectId"))
	static void UnBlockTapUserByObjectId(FString objectId);

	// 查询三方好友列表
	UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query friend list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);

	UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query mutal list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendMutalList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);

	UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query followee list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendFolloweeList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);

	UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query follower list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendFollowerList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);

	UFUNCTION(BlueprintCallable, Category="TapFriends ThirdParty Platform",
		meta=(DisplayName ="TapFriends ThirdParty query block list"))
	// cachePolicy: default 0
	// 0 - Query from cache first, if not found, then query from server.
	// 1 - Query from server.
	static void QueryThirdPartyFriendBlockList(FString platform, int64 pageSize, FString cursor, FString sortCondition, int cachePolicy);

	UFUNCTION(BlueprintCallable, Category="TapFollows Pattern",
		meta=(DisplayName ="TapFollows handFriendInvitationLink"))
	static void HandFollowInvitationLink(FString Uri);

	UFUNCTION(BlueprintCallable, Category="TapFriends", meta=(DisplayName ="TapFriends Toast"))
	static void Toast(FString toast);

	void OnBridgeCallback(const FString& result);

	static TArray<FTapThirdPartyFriendInfo> CollectThirdPartyFriendInfos(TSharedPtr<FJsonValue> Root);

	static TArray<FTapFriendInfo> CollectFriendInfos(TSharedPtr<FJsonValue> Root);

	static FTapFriendInfo ParseFriendInfo(TSharedPtr<FJsonObject> Root);

	static FTapThirdPartyFriendInfo ParseThirdFriendInfo(TSharedPtr<FJsonValue> Root);
};
