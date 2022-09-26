#include "TapUEBootstrap.h"

#include "TUBootStrapImpl.h"
#include "TUDebuger.h"


// static bool IsInitialized = false;

void TapUEBootstrap::Init(const FTUConfig& Config) {
	// if (IsInitialized) {
	// 	TUDebuger::WarningShow("Has Initialized");
	// 	return;
	// }
	// IsInitialized = true;
	TUBootStrapImpl::Get()->Init(Config);
}

void TapUEBootstrap::Login(TArray<FString> Permissions, TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	// if (!IsInitialized) {
	// 	TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
	// 	return;
	// }
	TUBootStrapImpl::Get()->Login(Permissions, SuccessBlock, FailBlock);
}

void TapUEBootstrap::AnonymouslyLogin(TFunction<void(const FTDSUser& User)> SuccessBlock,
	TFunction<void(const FTUError& Error)> FailBlock) {
	// if (!IsInitialized) {
	// 	TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
	// 	return;
	// }
	TUBootStrapImpl::Get()->AnonymouslyLogin(SuccessBlock, FailBlock);
}

void TapUEBootstrap::Logout() {
	// if (!IsInitialized) {
	// 	TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
	// 	return;
	// }
	TUBootStrapImpl::Get()->Logout();
}

TSharedPtr<FTDSUser> TapUEBootstrap::GetUser() {
	// if (!IsInitialized) {
	// 	TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
	// 	return nullptr;
	// }
	return TUBootStrapImpl::Get()->GetUser();
}

void TapUEBootstrap::SetPreferLanguage(ELanguageType LangType) {
	// if (!IsInitialized) {
	// 	TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
	// 	return;
	// }
	TUBootStrapImpl::Get()->SetPreferLanguage(LangType);
}
