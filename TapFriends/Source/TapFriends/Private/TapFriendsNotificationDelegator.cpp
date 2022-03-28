
#include "TapFriendsNotificationDelegator.h"
#if PLATFORM_IOS
#include "TapFriends.h"
#include "IOSHelper.h"

TapFriendsNotificationDelegator *friendDelegator = nil;

@implementation TapFriendsNotificationDelegator

- (void)onNewRequestComing:(TDSFriendshipRequest *)request {
	const FTapFriendshipRequestAndInfo requestInfo = FTapFriendshipRequestAndInfo((NSObject *)request);
	FTapFriendsModule::OnRequestComming.Broadcast(requestInfo);
}

- (void)onRequestAccepted:(TDSFriendshipRequest *)request {
	const FTapFriendshipRequestAndInfo requestInfo = FTapFriendshipRequestAndInfo((NSObject *)request);
	FTapFriendsModule::OnRequestAccepted.Broadcast(requestInfo);
}

- (void)onRequestDeclined:(TDSFriendshipRequest *)request {
	const FTapFriendshipRequestAndInfo requestInfo = FTapFriendshipRequestAndInfo((NSObject *)request);
	FTapFriendsModule::OnRequestDeclined.Broadcast(requestInfo);
}

- (void)onFriendAdded:(TDSFriendInfo *)friendInfo {
	FTapFriendInfo FriendInfo = FTapFriendInfo((NSObject *)friendInfo);
	FTapFriendsModule::OnFriendAdd.Broadcast(FriendInfo);
}

- (void)onFriendOnline:(NSString *)userId {
	FTapFriendsModule::OnFriendOnline.Broadcast(IOSHelper::convertString(userId));
}

- (void)onFriendOffline:(NSString *)userId {
	FTapFriendsModule::OnFriendOffline.Broadcast(IOSHelper::convertString(userId));
}

- (void)onRichPresenceChanged:(NSString *)userId dictionary:(NSDictionary * _Nullable)dictionary {
	const FString UserId = IOSHelper::convertString(userId);
	const FString RichPresenceJson = IOSHelper::getJson(dictionary);
	FTapFriendsModule::OnFriendRichPresenceChanged.Broadcast(UserId, RichPresenceJson);
}

- (void)onConnected {
	FTapFriendsModule::OnFriendConnected.Broadcast();
}

- (void)onDisconnectedWithError:(NSError * _Nullable)error {
	FTapFriendsModule::OnFriendDisconnected.Broadcast();
}

- (void)onConnectionError:(NSError *)error {
	FTapFriendsModule::OnConnectedError.Broadcast(IOSHelper::convertError(error));
}
// 少一个回调
// FTapFriendsModule::OnSendFriendRequestToSharer.Broadcast(Success, NickName, Error);

@end

#endif