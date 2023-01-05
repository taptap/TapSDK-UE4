#include "TapBootstrapObjcHelper.h"

#include "IOSHelper.h"


TSharedPtr<FTDSUser> TapBootstrapObjcHelper::Convert(TDSUser *user) {
	if (!user) {
		return nullptr;
	}
	TSharedPtr<FJsonObject> ServerData = IOSHelper::Convert([user dictionaryForObject]);
	TSharedPtr<FTDSUser> User_UE = MakeShareable(new FTDSUser(ServerData));
	return User_UE;
}

TSharedPtr<FTDSUser> TapBootstrapObjcHelper::Convert(LCUser *user) {
	if (!user) {
		return nullptr;
	}
	TSharedPtr<FJsonObject> ServerData = IOSHelper::Convert([user dictionaryForObject]);
	TSharedPtr<FTDSUser> User_UE = MakeShareable(new FTDSUser(ServerData));
	return User_UE;
}

TDSUser * TapBootstrapObjcHelper::Convert(const FTDSUser& User) {
	NSDictionary * Dic = IOSHelper::Convert(TSharedPtr<FJsonObject>(User.GetServerData()));
	TDSUser * user = (TDSUser *)[TDSUser objectWithDictionary:Dic];
	return user;
}


