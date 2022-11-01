#include "TUSupportMobileImpl.h"

#include "TapUESupport.h"
#include "TUDebuger.h"
#include "TUJsonHelper.h"
#include "TUMobileBridge.h"
#include "TUSupportUnreadStatusResult.h"


#define TapSupportService "TDSTapSupportService"
#define TapSupport_SERVICE_CLZ "com.tds.tapsupport.wrapper.TapSupportService"
#define TapSupport_SERVICE_IMPL "com.tds.tapsupport.wrapper.TapSupportServiceImpl"



TUSupportMobileImpl::TUSupportMobileImpl() {
	TUMobileBridge::Register(TapSupport_SERVICE_CLZ, TapSupport_SERVICE_IMPL);
}

void TUSupportMobileImpl::Init(FTapSupportConfig _Config) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("server", _Config.ServerUrl);
	JsonObject->SetStringField("productID", _Config.ProductID);

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("config"), TUJsonHelper::GetJsonString(JsonObject));
	Writer->WriteObjectEnd();
	Writer->Close();

	TUMobileBridge::AsyncPerform(TapSupportService, "setConfig", JsonOutString, [](const FString& ResultJsonStr) {
		auto UnreadStatusResult = TUJsonHelper::GetUStruct<FTUSupportUnreadStatusResult>(ResultJsonStr);
		if (!UnreadStatusResult.IsValid()) {
			TUDebuger::ErrorLog("TUSupportMobileImpl::Init CallBack Error");
			return;
		}
		if (UnreadStatusResult->IsSuccessful()) {
			TapUESupport::OnUnreadStatusChanged.ExecuteIfBound(UnreadStatusResult->HasUnread());
		} else {
			TapUESupport::OnErrorCallBack.ExecuteIfBound(FTUError(UnreadStatusResult->code, UnreadStatusResult->errorMsg));
		}
	}, false);
}

void TUSupportMobileImpl::SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("setDefaultMetaData"), TUJsonHelper::GetJsonString(MetaData));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TapSupportService, "setDefaultMetaData", JsonOutString);
}

void TUSupportMobileImpl::SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("setDefaultFieldsData"), TUJsonHelper::GetJsonString(FieldsData));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TapSupportService, "setDefaultFieldsData", JsonOutString);
}

void TUSupportMobileImpl::LoginAnonymously(const FString& UserID) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("loginAnonymously"), UserID);
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TapSupportService, "loginAnonymously", JsonOutString);
}

void TUSupportMobileImpl::Logout() {
	TUMobileBridge::AsyncPerform(TapSupportService, "logout", "");
}

FString TUSupportMobileImpl::GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> MetaData,
	TSharedPtr<FJsonObject> FieldsData) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("path"), Path);
	Writer->WriteValue(TEXT("metaData"), TUJsonHelper::GetJsonString(MetaData));
	Writer->WriteValue(TEXT("fieldsData"), TUJsonHelper::GetJsonString(FieldsData));
	Writer->WriteObjectEnd();
	Writer->Close();
	auto ResultStr = TUMobileBridge::Perform(TapSupportService, "getSupportWebUrl", JsonOutString);
	auto ResultJsonObject = TUJsonHelper::GetJsonObject(ResultStr);
	FString Url = "";
	ResultJsonObject->TryGetStringField("url", Url);
	return Url;
}

void TUSupportMobileImpl::OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> MetaData,
	TSharedPtr<FJsonObject> FieldsData) {
	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<
		TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("openSupportView"), Path);
	Writer->WriteValue(TEXT("metaData"), TUJsonHelper::GetJsonString(MetaData));
	Writer->WriteValue(TEXT("fieldsData"), TUJsonHelper::GetJsonString(FieldsData));
	Writer->WriteObjectEnd();
	Writer->Close();
	TUMobileBridge::AsyncPerform(TapSupportService, "openSupportView", JsonOutString);
}

void TUSupportMobileImpl::CloseSupportView() {
	TUMobileBridge::AsyncPerform(TapSupportService, "closeSupportView", "");
}

void TUSupportMobileImpl::Resume() {
	TUMobileBridge::AsyncPerform(TapSupportService, "resume", "");
}

void TUSupportMobileImpl::Pause() {
	TUMobileBridge::AsyncPerform(TapSupportService, "pause", "");
}

void TUSupportMobileImpl::FetchUnReadStatus() {
	TUMobileBridge::AsyncPerform(TapSupportService, "fetchUnreadStatus", "");
}
