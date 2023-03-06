#include "TDSUser.h"
#include "JsonObjectConverter.h"
#include "TUJsonHelper.h"
#include "TapBootstrapImpl.h"


FString FTDSUser::KeyNickName = "nickname";
FString FTDSUser::KeyAvatar = "avatar";
FString FTDSUser::KeyShortID = "shortId";


FTDSUser::FTDSUser(TSharedPtr<FJsonObject> InServerData)
	: FLCUser(InServerData)
{
}

FString FTDSUser::GetAvatar() const {
	return TUJsonHelper::GetStringField(ServerData, KeyAvatar);
}

FString FTDSUser::GetNickName() const {
	return TUJsonHelper::GetStringField(ServerData, KeyNickName);
}

FString FTDSUser::GetShortID() const {
	return TUJsonHelper::GetStringField(ServerData, KeyShortID);
}

TSharedPtr<FTDSUser> FTDSUser::GetCurrentUser() {
	return FTapBootstrapImpl::Get()->GetCurrentUser();
}

void FTDSUser::Logout() {
	FTapBootstrapImpl::Get()->Logout();
}

void FTDSUser::LoginAnonymously(FDelegate OnSuccess, FTUError::FDelegate OnError) {
	FTapBootstrapImpl::Get()->LoginAnonymously(OnSuccess, OnError);
}

void FTDSUser::LoginWithTapTap(const TArray<FString>& Permissions, FDelegate OnSuccess, FTUError::FDelegate OnError) {
	FTapBootstrapImpl::Get()->LoginWithTapTap(Permissions, OnSuccess, OnError);
}

void FTDSUser::BecomeWithSessionToken(const FString& SessionToken, FDelegate OnSuccess, FTUError::FDelegate OnError) {
	FTapBootstrapImpl::Get()->BecomeWithSessionToken(SessionToken, OnSuccess, OnError);
}

void FTDSUser::LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData, FDelegate OnSuccess,
	FTUError::FDelegate OnError) {
	FTapBootstrapImpl::Get()->LoginWithAuthData(Platform, AuthData, OnSuccess, OnError);
}

void FTDSUser::AssociateWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData, FDelegate OnSuccess,
	FTUError::FDelegate OnError) {
	FTapBootstrapImpl::Get()->AssociateWithAuthData(*this, Platform, AuthData, OnSuccess, OnError);
}

void FTDSUser::DisassociateAuthData(const FString& Platform, FDelegate OnSuccess, FTUError::FDelegate OnError) {
	FTapBootstrapImpl::Get()->DisassociateAuthData(*this, Platform, OnSuccess, OnError);
}

void FTDSUser::RetrieveShortToken(const FStringSignature& OnSuccess, const FLCError::FDelegate& OnFailed) {
#if PLATFORM_IOS || PLATFORM_ANDROID
	FTapBootstrapImpl::Get()->RetrieveShortToken(*this, OnSuccess, OnFailed);
#else
	FLCUser::RetrieveShortToken(OnSuccess, OnFailed);
#endif
	
}

TSharedPtr<FTDSUser> FTDSUser::CreateCurrentUser(const FString& UserID, const FString& SessionToken) {
	return FTapBootstrapImpl::Get()->CreateCurrentUser(UserID, SessionToken);
}

void FTDSUser::SaveUser() {
	FTapBootstrapImpl::Get()->SaveUser(*this);
}


