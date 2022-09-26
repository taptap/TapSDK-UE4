#include "TULoginPCImpl.h"

#include "TULoginLanguage.h"
#include "Server/TULoginNet.h"
#include "UI/TAULoginWidget.h"

void TULoginPCImpl::Init(FTULoginConfig _Config) {
	FTUConfig::Get()->ClientID = _Config.ClientID;
	FTUConfig::Get()->RegionType = _Config.RegionType;
	this->Config = _Config;
}

TSharedPtr<FTULoginProfileModel> TULoginPCImpl::GetProfile() {
	return FTULoginProfileModel::GetLocalModel();
}

void TULoginPCImpl::FetchProfile(
	TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {

	auto AccessTokenPtr = GetAccessToken();
	if (!AccessTokenPtr.IsValid() && CallBack != nullptr) {
		CallBack(nullptr, FTUError(-1, "no login"));
		return;
	}
	TULoginNet::RequestProfile(*AccessTokenPtr.Get(), [=](TSharedPtr<FTULoginProfileModel> Model, FTULoginError Error) {
		if (CallBack) {
			CallBack(Model, FTUError(Error.code, Error.msg));
		}
	});
}

TSharedPtr<FTUAccessToken> TULoginPCImpl::GetAccessToken() {
	return FTUAccessToken::GetLocalModel();
}

void TULoginPCImpl::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	UTAULoginWidget::ShowLoginUI(Permissions, CallBack);
}

void TULoginPCImpl::Logout() {
	FTULoginProfileModel::ClearLocalModel();
	FTUAccessToken::ClearLocalModel();
}

void TULoginPCImpl::GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) {
	if (!FTUAccessToken::GetLocalModel().IsValid()) {
		if (CallBack) {
			CallBack(false, FTUError(-400, "AccessToken is nil."));
		}
		return;
	}
	TULoginNet::RequestTestQualification([=](TSharedPtr<FTUTestQualificationModel> Model, FTULoginError Error) {
		if (!CallBack) {
			return;
		}
		if (Model.IsValid()) {
			CallBack(Model->in_test, FTUError(Error.code, Error.msg));
		} else {
			CallBack(false, FTUError(Error.code, Error.msg));
		}
	});
}
