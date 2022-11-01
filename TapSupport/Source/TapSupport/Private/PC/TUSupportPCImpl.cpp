#include "TUSupportPCImpl.h"
#include "TapUESupport.h"
#include "TUDebuger.h"
#include "TUDeviceInfo.h"
#include "TUHelper.h"
#include "TUJsonHelper.h"
#include "TUSettings.h"
#include "TUSupportNet.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "TUWebViewJavascriptBridge/TUWebViewJavascriptBridge_JS.h"
#include "UI/TUSupportWebView.h"

TUSupportPCImpl::TUSupportPCImpl() {
	NextDelay = 10;
}

TUSupportPCImpl::~TUSupportPCImpl() {
	CancelTimer();
}

void TUSupportPCImpl::Init(FTapSupportConfig _Config) {
	Config = _Config;
}

void TUSupportPCImpl::SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData) {
	DefaultMetaData = MetaData;
}

void TUSupportPCImpl::SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) {
	DefaultFieldsData = FieldsData;
}

void TUSupportPCImpl::LoginAnonymously(const FString& UserID) {
	if (Config.ServerUrl.IsEmpty()) {
		return;
	}
	check(!UserID.IsEmpty());
	if (!AnonymousID.IsEmpty() && UserID != AnonymousID) {
		Logout();
	}
	AnonymousID = UserID;
	Pause();
}

FString TUSupportPCImpl::GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> MetaData,
	TSharedPtr<FJsonObject> FieldsData) {
	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return "";
	}
	if (AnonymousID.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return "";
	}
	TSharedPtr<FJsonObject> RealMetaData = MetaData;
	if (!RealMetaData.IsValid() || RealMetaData->Values.Num() == 0) {
		RealMetaData = DefaultMetaData;
	}
	TSharedPtr<FJsonObject> RealFieldsData = FieldsData;
	if (!RealFieldsData.IsValid() || RealFieldsData->Values.Num() == 0) {
		RealFieldsData = DefaultFieldsData;
	}

	TSharedPtr<FJsonObject> ParasObject = MakeShareable(new FJsonObject);
	if (RealMetaData.IsValid() && RealMetaData->Values.Num() > 0) {
		ParasObject->SetStringField("meta", TUJsonHelper::GetJsonString(RealMetaData));
	}
	if (RealFieldsData.IsValid() && RealFieldsData->Values.Num() > 0) {
		ParasObject->SetStringField("fields", TUJsonHelper::GetJsonString(RealFieldsData));
	}
	
	FString Url = Config.ServerUrl / "in-app/v1/products" / Config.ProductID;
	if (!Path.IsEmpty()) {
		Url = Url / Path;
	}
	FString ParasStr = TUHelper::CombinParameters(ParasObject);
	if (!ParasStr.IsEmpty()) {
		Url = Url + "?" + ParasStr;
	}
	Url = Url + "#anonymous-id=" + FGenericPlatformHttp::UrlEncode(GetAnonymousID());
	return Url;
}

void TUSupportPCImpl::OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> MetaData,
	TSharedPtr<FJsonObject> FieldsData) {
	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return;
	}
	if (AnonymousID.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return;
	}
	FString Url = GetSupportWebUrl(Path, MetaData, FieldsData);
	TUHelper::LaunchURL(*Url, NULL, NULL);
	// UTUSupportWebView::Show(Url);
}

void TUSupportPCImpl::CloseSupportView() {
	// TUDebuger::DisplayLog(TUWebViewJavascriptBridge_js());
	
	// TUSupportImpl::CloseSupportView();
}

void TUSupportPCImpl::Resume() {
	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return;
	}
	if (AnonymousID.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return;
	}
	StopNotification = false;
	StartNext();
}

void TUSupportPCImpl::Pause() {
	StopNotification = true;
	CancelTimer();
	NextDelay = 10;
}

void TUSupportPCImpl::Logout() {
	Pause();
	CloseSupportView();
	AnonymousID = "";
}

void TUSupportPCImpl::FetchUnReadStatus() {
	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return;
	}
	if (AnonymousID.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return;
	}
	TUSupportNet::FetchUnReadStatus(GetAnonymousID(), [=](const FString& Model, const FTUError& Error) {
		if (Model.IsEmpty()) {
			TapUESupport::OnErrorCallBack.ExecuteIfBound(Error);
		} else {
			if (Model != UnreadStatus) {
				NextDelay = 10;
				TapUESupport::OnUnreadStatusChanged.ExecuteIfBound(Model == "true");
			}
			UnreadStatus = Model;
		}
		StartNext();
	});
}

void TUSupportPCImpl::StartNext() {
	if (StopNotification) {
		return;
	}
	TUSettings::GetGameInstance()->GetTimerManager().SetTimer(TimerHandle, [=]() {
		FetchUnReadStatus();
	}, FMath::Max<int>(10, NextDelay), false);
	if (NextDelay < 300) {
		NextDelay += 10;
	}
}

void TUSupportPCImpl::CancelTimer() {
	if (TimerHandle.IsValid()) {
		TUSettings::GetGameInstance()->GetTimerManager().ClearTimer(TimerHandle);
	}
	TimerHandle.Invalidate();
}

FString TUSupportPCImpl::GetAnonymousID() {
	// if (AnonymousID.IsEmpty()) {
	// 	AnonymousID = TUDeviceInfo::GetLoginId();
	// }
	return AnonymousID;
}

