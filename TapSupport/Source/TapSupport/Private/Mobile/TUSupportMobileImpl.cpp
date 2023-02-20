#include "TUSupportMobileImpl.h"

#include "TUJsonHelper.h"
#include "TUMobileBridge.h"



#define TapSupportService "TDSTapSupportService"
#define TapSupport_SERVICE_CLZ "com.tds.tapsupport.wrapper.TapSupportService"
#define TapSupport_SERVICE_IMPL "com.tds.tapsupport.wrapper.TapSupportServiceImpl"



TUSupportMobileImpl::TUSupportMobileImpl() {
	TUMobileBridge::Register(TapSupport_SERVICE_CLZ, TapSupport_SERVICE_IMPL);
}

void TUSupportMobileImpl::OpenFullUrl(const FString& Path) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("openFullUrl"), Path);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TapSupportService, "openFullUrl", JsonOutString);
}

void TUSupportMobileImpl::CloseSupportView() {
	TUMobileBridge::AsyncPerform(TapSupportService, "closeSupportView", "");
}
