#include "TapUESupport.h"

#include "TUSupportImpl.h"

TapUESupport::FErrorDelegate TapUESupport::OnErrorCallBack;
TapUESupport::FUnreadStatusChangedDelegate TapUESupport::OnUnreadStatusChanged;

void TapUESupport::Init(FTapSupportConfig Config) {
	TUSupportImpl::Get()->Init(Config);
}

void TapUESupport::SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData) {
	TUSupportImpl::Get()->SetDefaultMetaData(MetaData);
}

void TapUESupport::SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) {
	TUSupportImpl::Get()->SetDefaultFieldsData(FieldsData);
}

void TapUESupport::LoginAnonymously(const FString& UserID) {
	TUSupportImpl::Get()->LoginAnonymously(UserID);
}

void TapUESupport::Logout() {
	TUSupportImpl::Get()->Logout();
}

FString TapUESupport::GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> MetaData,
                                       TSharedPtr<FJsonObject> FieldsData) {
	return TUSupportImpl::Get()->GetSupportWebUrl(Path, MetaData, FieldsData);
}

void TapUESupport::OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> MetaData,
	TSharedPtr<FJsonObject> FieldsData) {
	TUSupportImpl::Get()->OpenSupportView(Path, MetaData, FieldsData);
}

void TapUESupport::CloseSupportView() {
	TUSupportImpl::Get()->CloseSupportView();
}

void TapUESupport::Resume() {
	TUSupportImpl::Get()->Resume();
}

void TapUESupport::Pause() {
	TUSupportImpl::Get()->Pause();
}

void TapUESupport::FetchUnReadStatus() {
	TUSupportImpl::Get()->FetchUnReadStatus();
}
