#include "TapBootstrapImpl.h"
#if PLATFORM_IOS
#include "iOS/TapBootstrapImpliOS.h"
typedef FTapBootstrapImpliOS TapBootstrapImplCommon;
#elif PLATFORM_ANDROID
#include "Android/TapBootstrapImplAndroid.h"
typedef FTapBootstrapImplAndroid TapBootstrapImplCommon;
#elif PLATFORM_MAC || PLATFORM_WINDOWS
#include "PC/TapBootstrapImplPC.h"
typedef FTapBootstrapImplPC TapBootstrapImplCommon;
#else
typedef FTapBootstrapImpl TapBootstrapImplCommon;
#endif

#include "TUDebuger.h"

#define UnsupportedPlatformsLog TUDebuger::ErrorLog(FString::Printf(TEXT("FTapBootstrapImpl Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<FTapBootstrapImpl> FTapBootstrapImpl::Instance;

TSharedPtr<FTapBootstrapImpl> FTapBootstrapImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShared<TapBootstrapImplCommon>();
	}
	return Instance;
}

FTapBootstrapImpl::~FTapBootstrapImpl() {
}

void FTapBootstrapImpl::Init(const FTUConfig& InConfig) {
	UnsupportedPlatformsLog
}

TSharedPtr<FTDSUser> FTapBootstrapImpl::GetCurrentUser() {
	UnsupportedPlatformsLog
	return nullptr;
}

void FTapBootstrapImpl::Logout() {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::LoginAnonymously(FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::LoginWithTapTap(const TArray<FString>& Permissions, FTDSUser::FDelegate OnSuccess,
	FTUError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::BecomeWithSessionToken(const FString& SessionToken, FTDSUser::FDelegate OnSuccess,
	FTUError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData,
	FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::AssociateWithAuthData(const FTDSUser& InUser, const FString& Platform,
	TSharedPtr<FJsonObject> AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::DisassociateAuthData(const FTDSUser& InUser, const FString& Platform,
	FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::SaveUser(const FTDSUser& InUser) {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
	FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FTapBootstrapImpl::RetrieveShortToken(const FLCUser& InUser, const FStringSignature& OnSuccess,
	const FLCError::FDelegate& OnFailed) {
	UnsupportedPlatformsLog
}

TSharedPtr<FTDSUser> FTapBootstrapImpl::CreateCurrentUser(const FString& UserID, const FString& SessionToken) {
	UnsupportedPlatformsLog
	return nullptr;
}
