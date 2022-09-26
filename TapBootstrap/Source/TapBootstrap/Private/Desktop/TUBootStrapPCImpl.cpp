#include "TUBootStrapPCImpl.h"

#include "TapUELogin.h"
#include "TUHelper.h"
#include "TULanguage.h"
#include "TULoginType.h"

void TUBootStrapPCImpl::Init(const FTUConfig& Config) {
	FTUConfig::Init(Config);
	
	// 初始化 TapLogin
	FTULoginConfig LoginConfig;
	LoginConfig.ClientID = Config.ClientID;
	LoginConfig.RegionType = Config.RegionType;
	TapUELogin::Init(LoginConfig);

	// 初始化 TapDB
	if (Config.DBConfig.Enable) {
		TUHelper::InvokeNoReturnFunction("TUDBReflection", "Init");
	}
	
}

void TUBootStrapPCImpl::Login(TArray<FString> Permissions, TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	TUBootStrapImpl::Login(Permissions, SuccessBlock, FailBlock);
}

void TUBootStrapPCImpl::AnonymouslyLogin(TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	TUBootStrapImpl::AnonymouslyLogin(SuccessBlock, FailBlock);
}

void TUBootStrapPCImpl::Logout() {
	TapUELogin::Logout();
}

TSharedPtr<FTDSUser> TUBootStrapPCImpl::GetUser() {
	return TUBootStrapImpl::GetUser();
}

void TUBootStrapPCImpl::SetPreferLanguage(ELanguageType LangType) {
	TULanguage::SetCurrentType(LangType);
}
