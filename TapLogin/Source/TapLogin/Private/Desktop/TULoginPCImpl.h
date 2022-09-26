#pragma once
#include "TULoginImpl.h"

class TULoginPCImpl: public TULoginImpl {
public:
	virtual void Init(FTULoginConfig _Config) override;
	virtual TSharedPtr<FTULoginProfileModel> GetProfile() override;
	virtual void FetchProfile(
		TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) override;
	virtual TSharedPtr<FTUAccessToken> GetAccessToken() override;
	virtual void Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) override;
	virtual void Logout() override;
	virtual void GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) override;
};
