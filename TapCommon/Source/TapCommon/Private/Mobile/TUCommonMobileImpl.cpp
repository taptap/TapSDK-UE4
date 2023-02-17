#include "TUCommonMobileImpl.h"
#include "TULanguage.h"
#include "TUMobileBridge.h"

#define TAPCOMMON_SERVICE "TDSCommonService"
#define TAPCOMMON_SERVICE_CLZ "com.tds.common.wrapper.TDSCommonService"
#define TAPCOMMON_SERVICE_IMPL "com.tds.common.wrapper.TDSCommonServiceImpl"

TUCommonMobileImpl::TUCommonMobileImpl() {
	TUMobileBridge::Register(TAPCOMMON_SERVICE_CLZ, TAPCOMMON_SERVICE_IMPL);
}

void TUCommonMobileImpl::SetLanguage(ELanguageType LanguageType) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("preferredLanguage"), (int)LanguageType);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TAPCOMMON_SERVICE, "setPreferredLanguage", JsonOutString);
	TULanguage::SetCurrentType(LanguageType);
}
