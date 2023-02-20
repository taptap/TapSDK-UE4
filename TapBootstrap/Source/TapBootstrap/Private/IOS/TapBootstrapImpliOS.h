#pragma once
#include "TapBootstrapImpl.h"

class FTapBootstrapImpliOS: public FTapBootstrapImpl{
public:
	virtual ~FTapBootstrapImpliOS() override;
	virtual void Init(const FTUConfig& InConfig) override;
	virtual TSharedPtr<FTDSUser> GetCurrentUser() override;
	virtual void Logout() override;
	virtual void LoginAnonymously(FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) override;
	virtual void LoginWithTapTap(const TArray<FString>& Permissions, FTDSUser::FDelegate OnSuccess,
		FTUError::FDelegate OnError) override;
	virtual void BecomeWithSessionToken(const FString& SessionToken, FTDSUser::FDelegate OnSuccess,
		FTUError::FDelegate OnError) override;
	virtual void LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData,
		FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) override;
	virtual void AssociateWithAuthData(const FTDSUser& InUser, const FString& Platform, TSharedPtr<FJsonObject> AuthData,
		FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) override;
	virtual void DisassociateAuthData(const FTDSUser& InUser, const FString& Platform, FTDSUser::FDelegate OnSuccess,
		FTUError::FDelegate OnError) override;
	virtual void SaveUser(const FTDSUser& InUser) override;
	virtual void QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
		FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) override;
	virtual void RetrieveShortToken(const FLCUser& InUser, const FStringSignature& OnSuccess,
		const FLCError::FDelegate& OnFailed) override;
};
