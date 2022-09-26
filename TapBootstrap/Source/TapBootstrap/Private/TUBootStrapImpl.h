#pragma once
#include "TDSUser.h"
#include "TUError.h"
#include "TUType.h"

class TUBootStrapImpl {
public:

	virtual ~TUBootStrapImpl() = default;

	static TSharedPtr<TUBootStrapImpl>& Get();
	
	virtual void Init(const FTUConfig& Config);

	virtual void Login(TArray<FString> Permissions, TFunction<void(const FTDSUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock);

	virtual void AnonymouslyLogin(TFunction<void(const FTDSUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock);

	virtual void Logout();

	virtual TSharedPtr<FTDSUser> GetUser();

	virtual void SetPreferLanguage(ELanguageType LangType);

private:
	static TSharedPtr<TUBootStrapImpl> Instance;
};
