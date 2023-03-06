#pragma once
#include "TapBootstrapImpl.h"
#include "TUAccessToken.h"
#include "TULoginProfileModel.h"
#include "Interfaces/IHttpRequest.h"

class FTapBootstrapImplPC: public FTapBootstrapImpl{
public:
	virtual ~FTapBootstrapImplPC() override;
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
	virtual void AssociateWithAuthData(const FTDSUser& InUser, const FString& Platform,
		TSharedPtr<FJsonObject> AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError) override;
	virtual void DisassociateAuthData(const FTDSUser& InUser, const FString& Platform, FTDSUser::FDelegate OnSuccess,
		FTUError::FDelegate OnError) override;
	virtual void SaveUser(const FTDSUser& InUser) override;
	virtual TSharedPtr<FTDSUser> CreateCurrentUser(const FString& UserID, const FString& SessionToken) override;

private:
	void OnUserInfoCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);

	void OnAssociateWithAuthDataCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FString Platform, TSharedPtr<FJsonObject> AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);
	
	void OnDisassociateAuthDataCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FString Platform, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);

	void InternalLoginWithTapTap(const TSharedPtr<FTUAccessToken>& TapAccessToken, 	const TSharedPtr<FTULoginProfileModel>& TapProfile, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);

	// /**
	//  * Fetch LeanCloud user with LeanCloud's session token
	//  * 
	//  * @param SessionToken The session token pass-in
	//  */
	// void BecomeWithSessionTokenInBackground(const FString& SessionToken, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);
	//
	// /**
	//  * Login LeanCloud with other platform account
	//  * 
	//  * @param Platform The other platform to auth with
	//  * @param AuthData @see https://leancloud.cn/docs/rest_api.html#hash621643170
	//  */
	// void LoginWithAuthData(const TCHAR* Platform, const TMap<FString, FString>& AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);
	//
	// /**
	//  * Bind other platform account to TapTap Developer Services user
	//  * 
	//  * @param InUser The user to bind
	//  * @param Platform The other platform to auth with
	//  * @param AuthData @see https://leancloud.cn/docs/rest_api.html#hash621643170
	//  */
	// void AssociateWithAuthData(const FTDSUser& InUser, const TCHAR* Platform, const TMap<FString, FString>& AuthData, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);
	//
	// /**
	//  * Unbind other platform account to TapTap Developer Services user
	//  * 
	//  * @param InUser The user to unbind
	//  * @param Platform The other platform to unbind
	//  */
	// void DisassociateAuthData(const FTDSUser& InUser, const TCHAR* Platform, FTDSUser::FDelegate OnSuccess, FTUError::FDelegate OnError);
	//
	// void SaveUser(const FTDSUser& InUser);
	
	
	TSharedPtr<FTDSUser> CachedUser;
};


