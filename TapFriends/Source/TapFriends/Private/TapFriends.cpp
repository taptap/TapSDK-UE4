// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapFriends.h"

#define LOCTEXT_NAMESPACE "FTapFriendsModule"

void FTapFriendsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FTapFriendsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTapFriendsModule, TapFriends)

FTapFriendsModule::FAddFriend FTapFriendsModule::OnAddFriendCallback;
FTapFriendsModule::FAddFriendError FTapFriendsModule::OnAddFriendError;

FTapFriendsModule::FDeleteFriend FTapFriendsModule::OnDeleteFriendCallback;
FTapFriendsModule::FDeleteFriendError FTapFriendsModule::OnDeleteFriendError;

FTapFriendsModule::FGetFollowingList FTapFriendsModule::OnGetFollowingListSuccess;
FTapFriendsModule::FGetFollowingListError FTapFriendsModule::OnGetFollowingListError;

FTapFriendsModule::FGetFollowerList FTapFriendsModule::OnGetFollowerListSuccess;
FTapFriendsModule::FGetFollowerListError FTapFriendsModule::OnGetFollowerListError;

FTapFriendsModule::FBlockUser FTapFriendsModule::OnBlockUserCallback;
FTapFriendsModule::FBlockUserError FTapFriendsModule::OnBlockUserError;

FTapFriendsModule::FUnBlockUser FTapFriendsModule::OnUnBlockUserCallback;
FTapFriendsModule::FUnBlockUserError FTapFriendsModule::OnUnBlockUserError;

FTapFriendsModule::FGetBlockUserList FTapFriendsModule::OnBlockUserListCallback;
FTapFriendsModule::FGetBlockUserListError FTapFriendsModule::OnGetBlockUserListError;

FTapFriendsModule::FSearchUser FTapFriendsModule::OnSearchCallback;
FTapFriendsModule::FSearchUserError FTapFriendsModule::OnSearchError;

FTapFriendsModule::FGenerateFriendInvitation FTapFriendsModule::OnGenerateFriendInvitationCallback;
FTapFriendsModule::FRegisterMessageListener FTapFriendsModule::OnMessage;
FTapFriendsModule::FSendFriendInvitation FTapFriendsModule::OnSendFriendInvitationCallback;



