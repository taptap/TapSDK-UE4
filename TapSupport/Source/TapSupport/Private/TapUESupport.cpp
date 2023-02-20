#include "TapUESupport.h"

#include "TUSupportImpl.h"

TapUESupport::FErrorDelegate TapUESupport::OnErrorCallBack;
TapUESupport::FUnreadStatusChangedDelegate TapUESupport::OnUnreadStatusChanged;

void TapUESupport::Init(FTapSupportConfig Config) {
	TUSupportImpl::Get()->Init(Config);
}

void TapUESupport::SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData) {
	ensureMsgf(false, TEXT("'SetDefaultMetaData' is deprecated. Please use 'SetDefaultFieldsData' instead!"));
}

void TapUESupport::SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) {
	TUSupportImpl::Get()->SetDefaultFieldsData(FieldsData);
}

void TapUESupport::UpdateDefaultField(const FString& Key, const TSharedPtr<FJsonValue>& NewValue)
{
	TUSupportImpl::Get()->UpdateDefaultField(Key, NewValue);
}

void TapUESupport::LoginAnonymously(const FString& UserID) {
	TUSupportImpl::Get()->LoginAnonymously(UserID);
}

void TapUESupport::LoginWithCustomCredential(const FString& Credential)
{
	TUSupportImpl::Get()->LoginWithCustomCredential(Credential);
}

void TapUESupport::LoginWithTDSCredential(const FString& Credential, const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed)
{
	TUSupportImpl::Get()->LoginWithTDSCredential(Credential, OnSuccess, OnFailed);
}

void TapUESupport::Logout() {
	TUSupportImpl::Get()->Logout();
}

FString TapUESupport::GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> MetaData,
                                       TSharedPtr<FJsonObject> FieldsData) {
	return TEXT("");
}

void TapUESupport::OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> MetaData,
	TSharedPtr<FJsonObject> FieldsData) {
}

FString TapUESupport::GetSupportWebUrlV2(const FString& Path, TSharedPtr<FJsonObject> Fields)
{
	return TUSupportImpl::Get()->GetSupportWebUrl(Path, Fields);
}

void TapUESupport::OpenSupportViewV2(const FString& Path, TSharedPtr<FJsonObject> Fields)
{
	TUSupportImpl::Get()->OpenSupportView(Path, Fields);
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
