#include "LCUser.h"

#include "Tools/LCJsonHelper.h"

FString FLCUser::KeyUserName = "username";
FString FLCUser::KeyPassword = "password";
FString FLCUser::KeyEmail = "email";
FString FLCUser::KeyMobilePhone = "mobilePhoneNumber";
FString FLCUser::KeyMobilePhoneVerified = "mobilePhoneVerified";
FString FLCUser::KeyAuthData = "authData";
FString FLCUser::KeyAuthDataAnonymous = "anonymous";
FString FLCUser::KeySessionToken = "sessionToken";
FString FLCUser::CLASS_NAME = "_User";

FLCUser::FLCUser(TSharedPtr<FJsonObject> InServerData): FLCObject(CLASS_NAME, InServerData) {
}

FString FLCUser::GetEmail() const {
	return LCJsonHelper::GetStringField(ServerData, KeyEmail);
}

FString FLCUser::GetUsername() const {
	return LCJsonHelper::GetStringField(ServerData, KeyUserName);
}

FString FLCUser::GetPassword() const {
	return LCJsonHelper::GetStringField(ServerData, KeyPassword);
}

FString FLCUser::GetMobilePhoneNumber() const {
	return LCJsonHelper::GetStringField(ServerData, KeyMobilePhone);
}

FString FLCUser::GetSessionToken() const {
	return LCJsonHelper::GetStringField(ServerData, KeySessionToken);
}

bool FLCUser::IsAnonymous() const {
	auto AuthDataPtr = GetAuthData();
	if (!AuthDataPtr.IsValid()) {
		return false;
	}
	return AuthDataPtr->Values.Contains(KeyAuthDataAnonymous);
}

bool FLCUser::IsMobilePhoneVerified() const {
	return LCJsonHelper::GetBoolField(ServerData, KeyMobilePhoneVerified);
}

bool FLCUser::IsAuthenticated() const {
	return !GetSessionToken().IsEmpty();
}

TSharedPtr<FJsonObject> FLCUser::GetAuthData() const {
	const TSharedPtr<FJsonObject> * TempPtr = nullptr;
	ServerData->TryGetObjectField(KeyAuthData, TempPtr);
	return *TempPtr;
}
