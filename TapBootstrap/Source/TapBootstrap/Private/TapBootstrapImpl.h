#pragma once
#include "TDSLeaderBoardRanking.h"
#include "TDSUser.h"
#include "TUError.h"
#include "TUType.h"

class FTapBootstrapImpl {
public:
	static TSharedPtr<FTapBootstrapImpl> Get();
	virtual ~FTapBootstrapImpl();
	
	virtual void Init(const FTUConfig& InConfig);

	virtual TSharedPtr<FTDSUser> GetCurrentUser();
	
	virtual void Logout();

	virtual void LoginAnonymously(FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);

	virtual void LoginWithTapTap(const TArray<FString>& Permissions, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);

	virtual void BecomeWithSessionToken(const FString& SessionToken, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);
	
	virtual void LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);

	virtual void AssociateWithAuthData(const FTDSUser& InUser, const FString& Platform, TSharedPtr<FJsonObject> AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);

	virtual void DisassociateAuthData(const FTDSUser& InUser, const FString& Platform, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);

	virtual TSharedPtr<FTDSUser> CreateCurrentUser(const FString& UserID, const FString& SessionToken);

	virtual void SaveUser(const FTDSUser& InUser);

	virtual void QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
	FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess, FTUError::FDelegate OnError);

	virtual void RetrieveShortToken(const FLCUser& InUser, const FStringSignature& OnSuccess, const FLCError::FDelegate& OnFailed);

private:
	static TSharedPtr<FTapBootstrapImpl> Instance;
};
