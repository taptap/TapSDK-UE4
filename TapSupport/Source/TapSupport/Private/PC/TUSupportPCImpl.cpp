#include "TUSupportPCImpl.h"

#include "TapUESupport.h"
#include "TUDebuger.h"
#include "TUHelper.h"
#include "TUJsonHelper.h"
#include "TUSettings.h"
#include "TUSupportNet.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "UI/TUSupportWebView.h"


TUSupportPCImpl::TUSupportPCImpl() {
	NextDelay = LOOP_INC_SECONDS;
}

TUSupportPCImpl::~TUSupportPCImpl() {
}

void TUSupportPCImpl::Init(FTapSupportConfig _Config) {
	Config = _Config;
}

void TUSupportPCImpl::SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData) {
	DefaultFieldsData = FieldsData;
}

void TUSupportPCImpl::UpdateDefaultField(const FString& Key, const TSharedPtr<FJsonValue>& NewValue)
{
	if (!DefaultFieldsData)
	{
		DefaultFieldsData = MakeShared<FJsonObject>();
	}
	DefaultFieldsData->SetField(Key, NewValue);
}

void TUSupportPCImpl::LoginAnonymously(const FString& UserID) {
	if (Config.ServerUrl.IsEmpty()) {
		return;
	}
	check(!UserID.IsEmpty());
	if (!CachedId.IsEmpty() && UserID != CachedId) {
		Logout();
	}
	CachedId = UserID;
	LoginType = ESupportLoginType::Anonymous;
	FetchLoop();
}

void TUSupportPCImpl::LoginWithCustomCredential(const FString& Credential)
{
	CachedId = Credential;
	LoginType = ESupportLoginType::Custom;
	FetchLoop();
}

void TUSupportPCImpl::LoginWithTDSCredential(const FString& Credential, const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	const FString Url = FString::Printf(TEXT("%s/api/2/users/tds/token"), *Config.ServerUrl);
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("POST"));
	
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("jwt"), Credential);
	Writer->WriteObjectEnd();
	Writer->Close();
	HttpRequest->SetContentAsString(JsonString);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &TUSupportPCImpl::NetTDSLoginCallback, OnSuccess, OnFailed);
	
	if (!HttpRequest->ProcessRequest())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Network error.")));
	}
}

FString TUSupportPCImpl::GetSupportWebUrl(const FString& Path, TSharedPtr<FJsonObject> FieldsData) {
	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return "";
	}
	if (CachedId.IsEmpty() || LoginType == ESupportLoginType::None) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return "";
	}

	TSharedPtr<FJsonObject> RealFieldsData = MakeShared<FJsonObject>();
	if (DefaultFieldsData)
	{
		RealFieldsData->Values = DefaultFieldsData->Values;
	}
	if (FieldsData)
	{
		RealFieldsData->Values.Append(FieldsData->Values);
	}

	TSharedPtr<FJsonObject> ParasObject = MakeShareable(new FJsonObject);
	if (RealFieldsData.IsValid() && RealFieldsData->Values.Num() > 0) {
		ParasObject->SetStringField("fields", TUJsonHelper::GetJsonString(RealFieldsData));
	}
	
	FString Url = Config.ServerUrl / "in-app/v1/products" / Config.ProductID;
	if (!Path.IsEmpty()) {
		Url = Url / Path;
	}

	FString Key;
	switch (LoginType) {
	case ESupportLoginType::Anonymous: 
		Key = TEXT("#anonymous-id=");
		break;
	case ESupportLoginType::Custom: 
		Key = TEXT("#credential=");
		break;
	case ESupportLoginType::TDS: 
		Key = TEXT("#tds-credential=");
		break;
	default:
		ensure(false);
	}
	Url = Url + Key + FGenericPlatformHttp::UrlEncode(CachedId);
	
	FString ParasStr = TUHelper::CombinParameters(ParasObject);
	if (!ParasStr.IsEmpty()) {
		Url = Url + "&" + ParasStr;
	}
	return Url;
}

void TUSupportPCImpl::OpenSupportView(const FString& Path, TSharedPtr<FJsonObject> FieldsData) {
	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return;
	}
	if (CachedId.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return;
	}
	FString Url = GetSupportWebUrl(Path, FieldsData);
	OpenFullUrl(Url);
	bWebOpen = true;
	Resume();
}

void TUSupportPCImpl::CloseSupportView()
{
	TUDebuger::ErrorShow(FString::Printf(TEXT("TUSupportImpl Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
}

void TUSupportPCImpl::Resume() {
	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return;
	}
	if (CachedId.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return;
	}
	if (TimerHandle.IsValid())
	{
		TUSettings::GetGameInstance()->GetTimerManager().ClearTimer(TimerHandle);
	}

	NextDelay = LOOP_INC_SECONDS;

	TUSettings::GetGameInstance()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateRaw(this, &TUSupportPCImpl::FetchLoop),
		(bUnregister && !bWebOpen) ? UNREGISTER_SECONDS : NextDelay,
		false);
	bWebOpen = false;
}

void TUSupportPCImpl::Pause() {
	if (TimerHandle.IsValid()) {
		TUSettings::GetGameInstance()->GetTimerManager().ClearTimer(TimerHandle);
	}
	TimerHandle.Invalidate();
}

void TUSupportPCImpl::Logout() {
	Pause();
	CloseSupportView();
	CachedId = "";
	LoginType = ESupportLoginType::None;
	UnreadStatus.Reset();
}

void TUSupportPCImpl::FetchUnReadStatus() {

	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return;
	}
	if (CachedId.IsEmpty() || LoginType == ESupportLoginType::None) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return;
	}
	
	TUSupportNet::FetchUnReadStatus(
		LoginType,
		CachedId,
		[=](const FString& Model, const FTUError& Error)
		{
			if (Model.IsEmpty())
			{
				bUnregister = Error.error_description == TEXT("USER_NOT_REGISTERED");
				TapUESupport::OnErrorCallBack.ExecuteIfBound(Error);
			}
			else
			{
				if (Model != UnreadStatus)
				{
					Resume();
					TapUESupport::OnUnreadStatusChanged.ExecuteIfBound(Model == "true");
				}
				bUnregister = false;
				UnreadStatus = Model;
			}
		});
}

void TUSupportPCImpl::OpenFullUrl(const FString& Path) {
	TUHelper::LaunchURL(*Path, NULL, NULL);
}

void TUSupportPCImpl::NetTDSLoginCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSimpleDelegate OnSuccess, FTapFailed OnFailed)
{
	if (!HttpResponse)
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error")));
		return;
	}

	const FString ContentString = HttpResponse->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(ContentString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		OnFailed.ExecuteIfBound(FTUError(HttpResponse->GetResponseCode(), FString::Printf(TEXT("Net error, Deserialize failed, response content: %s"), *ContentString)));
		return;
	}
	
	if (JsonObject->TryGetStringField(TEXT("jwt"), CachedId))
	{
		LoginType = ESupportLoginType::TDS;
		FetchLoop();
		OnSuccess.ExecuteIfBound();
		return;
	}
	OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error")));
}

void TUSupportPCImpl::FetchLoop()
{
	if (Config.ServerUrl.IsEmpty()) {
		TUDebuger::WarningLog("TapSupport Server Url is Empty");
		return;
	}
	if (CachedId.IsEmpty() || LoginType == ESupportLoginType::None) {
		TUDebuger::WarningLog("TapSupport: Please Login first");
		return;
	}
	
	TUSupportNet::FetchUnReadStatus(
		LoginType,
		CachedId,
		[this](const FString& Model, const FTUError& Error)
		{
			NextDelay = FMath::Min(LOOP_MAX_SECONDS, NextDelay + LOOP_INC_SECONDS);

			if (Model.IsEmpty())
			{
				bUnregister = Error.error_description == TEXT("USER_NOT_REGISTERED");
				TapUESupport::OnErrorCallBack.ExecuteIfBound(Error);
			}
			else
			{
				if (Model != UnreadStatus)
				{
					Resume();
					TapUESupport::OnUnreadStatusChanged.ExecuteIfBound(Model == "true");
				}
				UnreadStatus = Model;
				bUnregister = false;
			}

			TUSettings::GetGameInstance()->GetTimerManager().SetTimer(
				TimerHandle,
				FTimerDelegate::CreateRaw(this, &TUSupportPCImpl::FetchLoop),
				bUnregister ? UNREGISTER_SECONDS : NextDelay,
				false);
		});
}

