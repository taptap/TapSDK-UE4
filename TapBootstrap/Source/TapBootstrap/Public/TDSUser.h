#pragma once
#include "LCUser.h"
#include "TUError.h"

/**
 * TapTap Developer Services user
 */
/* Old -> New
 * uuid -> GetObjectId()
 * userName -> GetUsername()
 * email -> GetEmail()
 * phoneNumber -> GetMobilePhoneNumber()
 * sessionToken -> GetSessionToken()
 * isAnonymous -> IsAnonymous()
 * avatar -> GetAvatar()
 * nickName -> GetNickName()
 * shortId -> GetShortID()
 * UpdateAt -> GetUpdatedAt()
 * CreateAt -> GetCreatedAt()
 * bMobilePhoneVerified -> IsMobilePhoneVerified()
 */
struct TAPBOOTSTRAP_API FTDSUser: public FLCUser
{
public:
	DECLARE_DELEGATE_OneParam(FDelegate, const FTDSUser& User);
	
	FTDSUser(TSharedPtr<FJsonObject> InServerData = nullptr);
	
	FString GetAvatar() const;
	FString GetNickName() const;
	FString GetShortID() const;

	/**
	 * Return last login user from cached memory or load from local storage
	 */
	static TSharedPtr<FTDSUser> GetCurrentUser();

	/**
	 * Clear current user in memory and local storage
	 */
	static void Logout();

	/**
	 * Anonymously login to LeanCloud
	 * 
	 */
	static void LoginAnonymously(FDelegate OnSuccess, FTUError::FDelegate OnError);

	/**
	 * Login LeanCloud with auth data (this auth data is TapTap),
	 * This evokes a TapTap login flow
	 * 
	 * @see LoginWithAuthData
	 * 
	 * @param Permissions @see TapUELogin::Login
	 */
	static void LoginWithTapTap(const TArray<FString>& Permissions, FDelegate OnSuccess, FTUError::FDelegate OnError);

	/**
	* Fetch LeanCloud user with LeanCloud's session token
	* 
	* @param SessionToken The session token pass-in
	*/
	static void BecomeWithSessionToken(const FString& SessionToken, FDelegate OnSuccess, FTUError::FDelegate OnError);

	/**
	 * Login LeanCloud with other platform account
	 * 
	 * @param Platform The other platform to auth with
	 * @param AuthData @see https://leancloud.cn/docs/rest_api.html#hash621643170
	 */
	static void LoginWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData, FDelegate OnSuccess, FTUError::FDelegate OnError);

	/**
	 * Bind other platform account to TapTap Developer Services user
	 * 
	 * @param Platform The other platform to auth with
	 * @param AuthData @see https://leancloud.cn/docs/rest_api.html#hash621643170
	 */
	void AssociateWithAuthData(const FString& Platform, TSharedPtr<FJsonObject> AuthData, FDelegate OnSuccess, FTUError::FDelegate OnError);

	/**
	 * Unbind other platform account to TapTap Developer Services user
	 * 
	 * @param Platform The other platform to unbind
	 */
	void DisassociateAuthData(const FString& Platform, FDelegate OnSuccess, FTUError::FDelegate OnError);
	
	virtual void RetrieveShortToken(const FStringSignature& OnSuccess, const FLCError::FDelegate& OnFailed) override;
	
protected:

	static FString KeyAvatar;
	static FString KeyNickName;
	static FString KeyShortID;

	void SaveUser();
};
