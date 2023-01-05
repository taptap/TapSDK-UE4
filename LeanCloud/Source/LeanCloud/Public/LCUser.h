#pragma once
#include "LCObject.h"

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
	bool isAuthenticated() const;

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

	TSharedPtr<FJsonObject> GetAuthData() const;
};
