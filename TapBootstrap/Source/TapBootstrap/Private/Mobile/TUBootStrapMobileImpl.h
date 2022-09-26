#pragma once
#include "TUBootStrapImpl.h"

#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#import <TapBootstrapSDK/TapBootstrapSDK.h>
#include "IOSHelper.h"
#endif

class TUBootStrapMobileImpl: public TUBootStrapImpl{
public:
	TUBootStrapMobileImpl();
	virtual void Init(const FTUConfig& Config) override;
	virtual void Login(TArray<FString> Permissions, TFunction<void(const FTDSUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock) override;
	virtual void AnonymouslyLogin(TFunction<void(const FTDSUser& User)> SuccessBlock,
		TFunction<void(const FTUError& Error)> FailBlock) override;
	virtual void Logout() override;
	virtual TSharedPtr<FTDSUser> GetUser() override;
	virtual void SetPreferLanguage(ELanguageType LangType) override;

private:
	void DealLoginCallBack(const FString& ResultStr, TFunction<void(const FTDSUser& User)> SuccessBlock,
	                              TFunction<void(const FTUError& Error)> FailBlock);

#if PLATFORM_IOS
	void DealLoginCallBack(TDSUser* user, NSError* error, TFunction<void(const FTDSUser& User)> SuccessBlock,
								  TFunction<void(const FTUError& Error)> FailBlock);
#endif
	

	
};


