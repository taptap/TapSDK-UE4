#pragma once
#include "TUBootStrapImpl.h"

class TUBootStrapPCImpl: public TUBootStrapImpl {
public:
	virtual void Init(const FTUConfig& Config) override;
	virtual void Login(TArray<FString> Permissions, TFunction<void(const FTDSUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock) override;
	virtual void AnonymouslyLogin(TFunction<void(const FTDSUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock) override;
	virtual void Logout() override;
	virtual TSharedPtr<FTDSUser> GetUser() override;
	virtual void SetPreferLanguage(ELanguageType LangType) override;
};
