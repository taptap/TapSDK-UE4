#pragma once
#include "TULoginImpl.h"

class TULoginMobileImpl: public TULoginImpl{
public:
	TULoginMobileImpl();
	
	virtual void Init(FTULoginConfig _Config) override;
	virtual TSharedPtr<FTULoginProfileModel> GetProfile() override;
	virtual void FetchProfile(
		TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) override;
	virtual TSharedPtr<FTUAccessToken> GetAccessToken() override;
	virtual void Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) override;
	virtual void Logout() override;
	virtual void GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) override;
	virtual void QueryMutualList(FString Cursor, int Size,
		TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack) override;
};
