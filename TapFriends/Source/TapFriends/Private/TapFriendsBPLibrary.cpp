// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapFriendsBPLibrary.h"
#include "TapFriends.h"
#include "TapBridge.h"
#include "TapCommon.h"
#include "TapJson.h"
#include "TapResult.h"
#include "TapError.h"
#include "TapCommonBPLibrary.h"
#include "Containers/UnrealString.h"
#include "Serialization/JsonWriter.h"

#define TAP_FRIENDS_SERVICE "TapFriendService"
#define TAP_FRIENDS_SERVICE_CLZ "com.tapsdk.friends.wrapper.TapFriendsService"
#define TAP_FRIENDS_SERVICE_IML "com.tapsdk.friends.wrapper.TapFriendsServiceImpl"

#define ADD_FRIENDS_ID "ADD_FRIENDS_ID"
#define DELETE_FRIENDS_ID "DELETE_FRIENDS_ID"
#define GET_FOLLOWING_LIST_ID "GET_FOLLOWING_LIST_ID"
#define GET_FOLLOWER_LIST_ID "GET_FOLLOWER_LIST_ID"
#define BLOCK_USER_ID "BLOCK_USER_ID"
#define UN_BLOCK_USER_ID "UN_BLOCK_USER_ID"
#define GET_BLOCK_USER_LIST_ID "GET_BLOCK_USER_LIST_ID"
#define SEARCH_USER_ID "SEARCH_USER_ID"
#define REGISTER_MESSAGE_LISTENER_ID "REGISTER_MESSAGE_LISTENER_ID"
#define GenerateFriendInvitation_ID "GenerateFriendInvitation_ID"
#define SendFriendInvitation_ID "SendFriendInvitation_ID"


UTapFriendsBPLibrary::UTapFriendsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(TEXT(TAP_FRIENDS_SERVICE_CLZ),TEXT(TAP_FRIENDS_SERVICE_IML));
    FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapFriendsBPLibrary::OnBridgeCallback);

    UTapFriendsBPLibrary::RegisterMessageListener();
#endif
}

void UTapFriendsBPLibrary::AddFriends(FString userId){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("addFriend"), userId);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("addFriend"), JsonOutString, true, TEXT(ADD_FRIENDS_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapFriendsBPLibrary::DeleteFriends(FString userId){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("deleteFriend"), userId);
    Writer->WriteObjectEnd();
    Writer->Close();
	
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("deleteFriend"), JsonOutString, true, TEXT(DELETE_FRIENDS_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapFriendsBPLibrary::GetFollowingList(int from,bool mutualAttentation,int limit){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("getFollowingList"), from);
    Writer->WriteValue(TEXT("mutualAttention"), mutualAttentation);
    Writer->WriteValue(TEXT("limit"), limit);
    Writer->WriteObjectEnd();
    Writer->Close();

    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("getFollowingList"), JsonOutString, true, TEXT(GET_FOLLOWING_LIST_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapFriendsBPLibrary::GetFollowerList(int from,int limit){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("getFollowerList"), from);
    Writer->WriteValue(TEXT("limit"), limit);
    Writer->WriteObjectEnd();
    Writer->Close();
		
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("getFollowerList"), JsonOutString, true, TEXT(GET_FOLLOWER_LIST_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}


void UTapFriendsBPLibrary::BlockUser(FString userId){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("blockUser"), userId);
    Writer->WriteObjectEnd();
    Writer->Close();
		
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("blockUser"), JsonOutString, true, TEXT(BLOCK_USER_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapFriendsBPLibrary::UnblockUser(FString userId){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("unblockUser"), userId);
    Writer->WriteObjectEnd();
    Writer->Close();
		
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("unblockUser"), JsonOutString, true, TEXT(UN_BLOCK_USER_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapFriendsBPLibrary::GetBlockUserList(int from,int limit){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("getBlockList"), from);
    Writer->WriteValue(TEXT("limit"), limit);
    Writer->WriteObjectEnd();
    Writer->Close();
		
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("getBlockList"), JsonOutString, true, TEXT(GET_BLOCK_USER_LIST_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapFriendsBPLibrary::SearchUser(FString userId){
#if PLATFORM_ANDROID || PLATFORM_IOS
	FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("searchUser"), userId);
    Writer->WriteObjectEnd();
    Writer->Close();
		
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("searchUser"), JsonOutString, true, TEXT(SEARCH_USER_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapFriendsBPLibrary::RegisterMessageListener(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("registerMessageListener"), TEXT(""), true, TEXT(REGISTER_MESSAGE_LISTENER_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapFriendsBPLibrary::GenerateFriendInvitation(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("generateFriendInvitation"), TEXT(""), true, TEXT(GenerateFriendInvitation_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapFriendsBPLibrary::SendFriendInvitation(){
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT(TAP_FRIENDS_SERVICE), TEXT("sendFriendInvitation"), TEXT(""), true, TEXT(SendFriendInvitation_ID),false);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapFriendsBPLibrary::OnBridgeCallback(const FString &result){
#if PLATFORM_ANDROID || PLATFORM_IOS
    
    UE_LOG(LogTemp,Warning,TEXT("TapBootstrap OnBridgeCallback:%s"),*result);
    
    FTapResult tapResult;
    bool parseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FTapResult>(result,&tapResult,0,0);
    if(!parseSuccess){
        return;
    }

    /** Handler Add Friends */
    if(tapResult.callbackId.Equals(ADD_FRIENDS_ID)){
        FTapError error = {80080,"TapSDK Add Friends Error!"};
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnAddFriendError.Broadcast(error);
            return;
        }

        TSharedRef<TJsonReader<TCHAR>> addFriendReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> addFriendRoot;
        bool addFriendsParseEnable = FJsonSerializer::Deserialize(addFriendReader,addFriendRoot);
        if(addFriendsParseEnable){
            int addFriendCode = addFriendRoot->GetIntegerField(TEXT("addFriend"));
            FString addFriendWrapper = addFriendRoot->GetStringField(TEXT("wrapper"));
            if(addFriendCode == 0){
                FTapFriendsModule::OnAddFriendCallback.Broadcast(true);
                return;
            }
            FTapError addFriendError;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(addFriendWrapper,&addFriendError,0,0);
            FTapFriendsModule::OnAddFriendError.Broadcast(addFriendError);
            return;
        }
        FTapFriendsModule::OnAddFriendError.Broadcast(error);
        return;
    }

    if(tapResult.callbackId.Equals(DELETE_FRIENDS_ID)){
        FTapError error = {80080,"TapSDk Delete Friends Error"};
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnDeleteFriendError.Broadcast(error);
            return;
        }

        TSharedRef<TJsonReader<TCHAR>> deleteFriendsReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> deleteFriendsRoot;
        bool deleteParseEnable = FJsonSerializer::Deserialize(deleteFriendsReader,deleteFriendsRoot);

        if(deleteParseEnable){
            int deleteFriendCode = deleteFriendsRoot->GetIntegerField(TEXT("deleteFriend"));
            FString deleteFriendWrapper = deleteFriendsRoot->GetStringField(TEXT("wrapper"));
            if(deleteFriendCode == 0){
                FTapFriendsModule::OnDeleteFriendCallback.Broadcast(true);
                return;
            }
            FTapError deleteFriendError;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(deleteFriendWrapper,&deleteFriendError,0,0);
            FTapFriendsModule::OnDeleteFriendError.Broadcast(deleteFriendError);
            return;
        }
        FTapFriendsModule::OnDeleteFriendError.Broadcast(error);
        return;
    }

    if(tapResult.callbackId.Equals(BLOCK_USER_ID)){
        FTapError error = {80080,"TapSDk block Friend Error"};
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnBlockUserError.Broadcast(error);
            return;
        }

        TSharedRef<TJsonReader<TCHAR>> blockReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> blockRoot;
        bool blockParseEnable = FJsonSerializer::Deserialize(blockReader,blockRoot);

        if(blockParseEnable){
            int blockCode = blockRoot->GetIntegerField(TEXT("blockFriend"));
            FString blockWrapper = blockRoot->GetStringField(TEXT("wrapper"));
            if(blockCode == 0){
                FTapFriendsModule::OnBlockUserCallback.Broadcast(true);
                return;
            }
            FTapError blockError;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(blockWrapper,&blockError,0,0);
            FTapFriendsModule::OnBlockUserError.Broadcast(blockError);
            return;
        }
        FTapFriendsModule::OnBlockUserError.Broadcast(error);
        return;
    }

    if(tapResult.callbackId.Equals(UN_BLOCK_USER_ID)){
        FTapError error = {80080,"TapSDk block Friend Error"};
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnUnBlockUserError.Broadcast(error);
            return;
        }

        TSharedRef<TJsonReader<TCHAR>> unblockReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> unblockRoot;
        bool unblockParseEnable = FJsonSerializer::Deserialize(unblockReader,unblockRoot);

        if(unblockParseEnable){
            int unblockCode = unblockRoot->GetIntegerField(TEXT("unblockFriend"));
            FString unblockWrapper = unblockRoot->GetStringField(TEXT("wrapper"));
            if(unblockCode == 0){
                FTapFriendsModule::OnUnBlockUserCallback.Broadcast(true);
                return;
            }
            FTapError unblockError;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(unblockWrapper,&unblockError,0,0);
            FTapFriendsModule::OnUnBlockUserError.Broadcast(unblockError);
            return;
        }
        FTapFriendsModule::OnUnBlockUserError.Broadcast(error);
        return;
    }  

    if(tapResult.callbackId.Equals(GET_FOLLOWING_LIST_ID)){
        FTapError error = {80080,"TapSDk getFollowing List Error"};
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnGetFollowingListError.Broadcast(error);
            return;
        }

        TSharedRef<TJsonReader<TCHAR>> followingReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> followingRoot;
        bool followingParseEnable = FJsonSerializer::Deserialize(followingReader,followingRoot);
        if(followingParseEnable){
            int followingCode = followingRoot->GetIntegerField(TEXT("getFollowingList"));
            if(followingCode == 0){
                TArray<FTapUserRelationShip> followingList;
                const TArray<TSharedPtr<FJsonValue>>* members;
                if(followingRoot->TryGetArrayField(TEXT("wrapper"),members)){

                    for(int32 i = 0;i<(*members).Num();i++){
                        TSharedPtr<FJsonObject> jsonMember = (*members)[i]->AsObject();
                        struct FTapUserRelationShip relationShip;
                        relationShip.userId = jsonMember->GetStringField(TEXT("userId"));
                        relationShip.avatar = jsonMember->GetStringField(TEXT("avatar"));
                        relationShip.gender = jsonMember->GetIntegerField(TEXT("gender"));
                        relationShip.relationShip = jsonMember->GetStringField(TEXT("relationShip"));
                        relationShip.name = jsonMember->GetStringField(TEXT("name"));
                        relationShip.mutualAttention = jsonMember->GetBoolField(TEXT("mutualAttention"));
                        followingList.Add(relationShip);
                    }
                    FTapFriendsModule::OnGetFollowingListSuccess.Broadcast(followingList);
                }
            }else{
                FTapError getFollowingListError;
                FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(followingRoot->GetStringField(TEXT("wrapper")),&getFollowingListError,0,0);
                FTapFriendsModule::OnGetFollowingListError.Broadcast(getFollowingListError);
            }
            return;
        }
        FTapFriendsModule::OnGetFollowingListError.Broadcast(error);
        return;
    }

    if(tapResult.callbackId.Equals(GET_FOLLOWER_LIST_ID)){
        FTapError error = {80080,"TapSDk getFollower List Error"};
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnGetFollowerListError.Broadcast(error);
            return;
        }

        TSharedRef<TJsonReader<TCHAR>> followerReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> followerRoot;
        bool followerParseEnable = FJsonSerializer::Deserialize(followerReader,followerRoot);
        if(followerParseEnable){
            int followerCode = followerRoot->GetIntegerField(TEXT("getFollowerList"));

            if(followerCode == 0){
                TArray<FTapUserRelationShip> followerList;

                const TArray<TSharedPtr<FJsonValue>>* members;

                if(followerRoot->TryGetArrayField(TEXT("wrapper"),members)){

                    for(int32 i = 0;i<(*members).Num();i++){
                        TSharedPtr<FJsonObject> jsonMember = (*members)[i]->AsObject();
                        struct FTapUserRelationShip relationShip;
                        relationShip.userId = jsonMember->GetStringField(TEXT("userId"));
                        relationShip.avatar = jsonMember->GetStringField(TEXT("avatar"));
                        relationShip.gender = jsonMember->GetIntegerField(TEXT("gender"));
                        relationShip.relationShip = jsonMember->GetStringField(TEXT("relationShip"));
                        relationShip.name = jsonMember->GetStringField(TEXT("name"));
                        relationShip.mutualAttention = jsonMember->GetBoolField(TEXT("mutualAttention"));
                        followerList.Add(relationShip);
                    }
                    FTapFriendsModule::OnGetFollowerListSuccess.Broadcast(followerList);
                }
            }else{
                FTapError getFollowerListError;
                FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(followerRoot->GetStringField(TEXT("wrapper")),&getFollowerListError,0,0);
                FTapFriendsModule::OnGetFollowerListError.Broadcast(getFollowerListError);
            }
            return;
        }
        FTapFriendsModule::OnGetFollowerListError.Broadcast(error);
        return;
    }

    if(tapResult.callbackId.Equals(GET_BLOCK_USER_LIST_ID)){
        FTapError error = {80080,"TapSDk BlockUser List Error"};
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnGetBlockUserListError.Broadcast(error);
            return;
        }
        
        TSharedRef<TJsonReader<TCHAR>> blockListReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> blockListRoot;
        bool blockListParseEnable = FJsonSerializer::Deserialize(blockListReader,blockListRoot);
        if(blockListParseEnable){

            int blockListCode = blockListRoot->GetIntegerField(TEXT("getBlockList"));

            if(blockListCode == 0){

                TArray<FTapUserRelationShip> blockList;

                const TArray<TSharedPtr<FJsonValue>>* members;

                if(blockListRoot->TryGetArrayField(TEXT("wrapper"),members)){

                    for(int32 i = 0;i<(*members).Num();i++){
                        TSharedPtr<FJsonObject> jsonMember = (*members)[i]->AsObject();
                        struct FTapUserRelationShip relationShip;
                        relationShip.userId = jsonMember->GetStringField(TEXT("userId"));
                        relationShip.avatar = jsonMember->GetStringField(TEXT("avatar"));
                        relationShip.gender = jsonMember->GetIntegerField(TEXT("gender"));
                        relationShip.relationShip = jsonMember->GetStringField(TEXT("relationShip"));
                        relationShip.name = jsonMember->GetStringField(TEXT("name"));
                        relationShip.mutualAttention = jsonMember->GetBoolField(TEXT("mutualAttention"));
                        blockList.Add(relationShip);
                    }

                    FTapFriendsModule::OnBlockUserListCallback.Broadcast(blockList);
                }
            }else{
                FTapError blockListError;
                FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(blockListRoot->GetStringField(TEXT("wrapper")),&blockListError,0,0);
                FTapFriendsModule::OnGetBlockUserListError.Broadcast(blockListError);
            }
            return;
        }
        FTapFriendsModule::OnGetBlockUserListError.Broadcast(error);
        return;
    }

    if(tapResult.callbackId.Equals(SEARCH_USER_ID)){
        FTapError error = {80080,"TapSDk Search Error"};
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnSearchError.Broadcast(error);
            return;
        }

        TSharedRef<TJsonReader<TCHAR>> searchReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> searchRoot;
        bool serachParseEnable = FJsonSerializer::Deserialize(searchReader,searchRoot);
        if(serachParseEnable){
            FString errorJson = searchRoot->GetStringField(TEXT("error"));
            FString searchWrapper = searchRoot->GetStringField(TEXT("result"));

            if(errorJson.Len()>0){
                FTapError searchError;
                FJsonObjectConverter::JsonObjectStringToUStruct<FTapError>(errorJson,&searchError,0,0);
                FTapFriendsModule::OnSearchError.Broadcast(searchError);
                return;
            }

            FTapUserRelationShip relationShip;
            FJsonObjectConverter::JsonObjectStringToUStruct<FTapUserRelationShip>(searchWrapper,&relationShip,0,0);
            FTapFriendsModule::OnSearchCallback.Broadcast(relationShip);
            return;
        }
        FTapFriendsModule::OnSearchError.Broadcast(error);
        return;
    }

    if(tapResult.callbackId.Equals(REGISTER_MESSAGE_LISTENER_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnMessage.Broadcast(80080,TEXT("TapSDK Register message Listener error"));
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> messageReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> messageRoot;
        bool messageEnable = FJsonSerializer::Deserialize(messageReader,messageRoot);
        if(messageEnable){
            int32 messageCode = messageRoot->GetIntegerField(TEXT("messageCallbackCode"));
            FString wrapper = messageRoot->GetStringField(TEXT("wrapper"));
            FTapFriendsModule::OnMessage.Broadcast(messageCode,wrapper);
            return;
        }
        FTapFriendsModule::OnMessage.Broadcast(80080,TEXT("TapSDK Register message Listener error"));
        return;
    }

    if(tapResult.callbackId.Equals(GenerateFriendInvitation_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnGenerateFriendInvitationCallback.Broadcast(TEXT("TapSDK GenerateFriendInvitation Listener error"));
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> generateReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> generateRoot;
        bool generateEnable = FJsonSerializer::Deserialize(generateReader,generateRoot);
        if(generateEnable){
            FTapFriendsModule::OnGenerateFriendInvitationCallback.Broadcast(generateRoot->GetStringField(TEXT("result")));
            return;
        }
        FTapFriendsModule::OnGenerateFriendInvitationCallback.Broadcast(TEXT("TapSDK GenerateFriendInvitation error"));
        return;
    }

    if(tapResult.callbackId.Equals(SendFriendInvitation_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapFriendsModule::OnSendFriendInvitationCallback.Broadcast(TEXT("TapSDK SendFriendInvitation Listener error"));
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> sendReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> sendRoot;
        bool generateEnable = FJsonSerializer::Deserialize(sendReader,sendRoot);
        if(generateEnable){
            FTapFriendsModule::OnSendFriendInvitationCallback.Broadcast(sendRoot->GetStringField(TEXT("result")));
            return;
        }
        FTapFriendsModule::OnSendFriendInvitationCallback.Broadcast(TEXT("TapSDK SendFriendInvitation error"));
        return;
    }

#endif
}

