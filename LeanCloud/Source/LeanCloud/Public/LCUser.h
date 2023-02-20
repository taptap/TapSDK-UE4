#pragma once
#include "LCError.h"
#include "LCObject.h"
#include "Http.h"
#include "LeanCloudType.h"

struct LEANCLOUD_API FLCUser: public FLCObject {
public:
	FLCUser(TSharedPtr<FJsonObject> InServerData = nullptr);

	FString GetEmail() const;
	FString GetUsername() const;
	FString GetPassword() const;
	FString GetMobilePhoneNumber() const;
	FString GetSessionToken() const;
	bool IsAnonymous() const;
	bool IsMobilePhoneVerified() const;
	bool IsAuthenticated() const;
	virtual void RetrieveShortToken(const FStringSignature& OnSuccess, const FLCError::FDelegate& OnFailed);

protected:
	static FString KeyUserName;
	static FString KeyPassword;
	static FString KeyEmail;
	static FString KeyMobilePhone;
	static FString KeyMobilePhoneVerified;
	static FString KeyAuthData;
	static FString KeyAuthDataAnonymous;
	static FString KeySessionToken;
	static FString CLASS_NAME;

	void OnRetrieveShortTokenCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, FStringSignature OnSuccess, FLCError::FDelegate OnError);
	
	TSharedPtr<FJsonObject> GetAuthData() const;
};
