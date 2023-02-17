// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBillboardCommon.h"

#include "UI/TapBillboardBrowserSplash.h"
#include "UI/TapBillboardMarquee.h"
#include "UI/TapBillboardBrowserNavigate.h"
#include "UI/TapBillboardBrowserPopup.h"

#include "JsonObjectConverter.h"
#include "JsonWrapper.h"

#include "TUDeviceInfo.h"
#include "TULanguage.h"
#include "TapCommon.h"
#include "TUDebuger.h"
#include "TUError.h"
#include "TUSettings.h"
#include "Engine/Font.h"
#include "Engine/FontFace.h"


template<typename CharType, typename PrintPolicy, typename FValue>
void WriteValue(const TSharedRef<TJsonWriter<CharType, PrintPolicy>>& Writer, const TMap<FString, FValue>& ValueMap)
{
	Writer->WriteArrayStart();
	for (const TTuple<FString, FValue>& T : ValueMap)
	{
		Writer->WriteObjectStart();
		Writer->WriteValue(T.Key, T.Value);
		Writer->WriteObjectEnd();
	}
	Writer->WriteArrayEnd();
}

template<typename CharType, typename PrintPolicy, typename FValue>
void WriteValue(const TSharedRef<TJsonWriter<CharType, PrintPolicy>>& Writer, const FString& Identifier, const TMap<FString, FValue>& ValueMap)
{
	Writer->WriteArrayStart(Identifier);
	for (const TTuple<FString, FValue>& T : ValueMap)
	{
		Writer->WriteObjectStart();
		Writer->WriteValue(T.Key, T.Value);
		Writer->WriteObjectEnd();
	}
	Writer->WriteArrayEnd();
}

#define BASE_SERVER_URL_CN "https://%s.billboard.tds1.tapapis.cn"
#define BASE_SERVER_URL_IO "https://%s.billboard.ap-sg.tapapis.com"

#define BASE_WEB_URL_IO "https://%s.billboard.ap-sg.tapapis.com"

FTapBillboardCommon::FTapBillboardCommon()
	: NavigateBrowser(nullptr)
	, SplashBrowser(nullptr)
	, MarqueeWidget(nullptr)
	, MarqueeIconTexture(nullptr)
	, NavigateBrowserClass(FString(TEXT("WidgetBlueprint'/TapBillboard/BillboardWebBrowserNavigate.BillboardWebBrowserNavigate_C'")))
	, SplashBrowserClass(FString(TEXT("WidgetBlueprint'/TapBillboard/BillboardWebBrowserSplash.BillboardWebBrowserSplash_C'")))
	, MarqueeClass(FString(TEXT("WidgetBlueprint'/TapBillboard/BillboardMarquee.BillboardMarquee_C'")))
	, PopupBrowserClass(FString(TEXT("WidgetBlueprint'/TapBillboard/BillboardWebBrowserPopup.BillboardWebBrowserPopup_C'")))
	, CanGetBadgeSecond(0.f)
	, bSplashDataCallback(false)
	, bCachedBadge(false)
	, DownloadFont(nullptr)
{
}

void FTapBillboardCommon::Init(const FTUConfig& InConfig)
{
	if (!InConfig.BillboardConfig)
	{
		ensure(false);
		return;
	}
	if (!InConfig.ClientID.IsEmpty())
	{
		FTUConfig::Get()->ClientID = InConfig.ClientID;
	}
	if (!InConfig.ClientToken.IsEmpty())
	{
		FTUConfig::Get()->ClientToken = InConfig.ClientToken;
	}
	FTUConfig::Get()->RegionType = InConfig.RegionType;
	FTUConfig::Get()->BillboardConfig = InConfig.BillboardConfig;

	if (InConfig.RegionType == ERegionType::Global)
	{
		FTUConfig::Get()->BillboardConfig->BillboardUrl = FString::Printf(TEXT(BASE_WEB_URL_IO), *FTUConfig::Get()->ClientID.Left(8));
	}

	ServerUrl = FTUConfig::Get()->RegionType == ERegionType::CN ? FString::Printf(TEXT(BASE_SERVER_URL_CN), *FTUConfig::Get()->ClientID.Left(8)) : FString::Printf(TEXT(BASE_SERVER_URL_IO), *FTUConfig::Get()->ClientID.Left(8));
	ServerUrl = TUDebuger::GetReplacedHost(ServerUrl);
#if PLATFORM_WINDOWS || PLATFORM_MAC
	Rest_FetchMarqueeStyle(FAnnouncementStyleDataResult::CreateRaw(this, &FTapBillboardCommon::InternalFetchMarqueeStyleCallback), FTapFailed());

	Rest_FetchUnreadAnnouncementsGeneralData(ETapBillboardTemplate::Splash,
		FAnnouncementGeneralDataResult::CreateRaw(this, &FTapBillboardCommon::InternalFetchUnreadSplashCallback),
		FTapFailed());
#endif
}

void FTapBillboardCommon::OpenPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose)
{
	if (IsValid(NavigateBrowser) && NavigateBrowser->IsInViewport())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Already exists one.")));
		return;
	}
	if (TSharedPtr<FTUError> Error = CheckConfig())
	{
		OnFailed.ExecuteIfBound(*Error);
		return;
	}
	if (!TUSettings::GetGameInstance().IsValid())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Failed display billboard widget. no game instance.")));
		return;
	}
	
	if (!IsValid(NavigateBrowser))
	{
		NavigateBrowser = CreateWidget<UTapBillboardBrowserNavigate>(TUSettings::GetGameInstance().Get(), NavigateBrowserClass.Get());
		NavigateBrowser->OnAudioOutputStateChanged.BindRaw(this, &FTapBillboardCommon::InternalAudioOutputStateChangedCallback, ETapBillboardTemplate::Navigate);
		NavigateBrowser->OnCustomLinkClicked.BindRaw(this, &FTapBillboardCommon::InternalCustomUrlClickedCallback, ETapBillboardTemplate::Navigate);
	}

	if (NavigateBrowser->GetState() != EBillboardBrowserState::Loaded)
	{
		NavigateBrowser->OnLoadComplete = OnSuccess;
		NavigateBrowser->OnLoadFailed = OnFailed;
		NavigateBrowser->OnBillboardBrowserClosed = OnClose;
	}
	
	switch(NavigateBrowser->GetState())
	{
	case EBillboardBrowserState::NotLoad:
	case EBillboardBrowserState::LoadFailed:
		{
			const FString Url = GenerateBillboardUrl(FTUConfig::Get()->BillboardConfig->Dimensions);
			NavigateBrowser->LoadUrl(Url);
		}
		break;
	case EBillboardBrowserState::Loading:
		break;
	case EBillboardBrowserState::Loaded:
		OnSuccess.ExecuteIfBound();
		NavigateBrowser->AddToViewport(TUSettings::GetUILevel());
		return;
	default: ;
	}
	
	NavigateBrowser->SetDisplayWhenReady(true);
}

void FTapBillboardCommon::GetBadgeDetails(const FTapBadgeDetailsResult& OnSuccess, const FTapFailed& OnFailed)
{
	if (const TSharedPtr<FTUError> Error = CheckConfig())
	{
		OnFailed.ExecuteIfBound(*Error);
		return;
	}

	if (bCachedBadge)
	{
		if (const UGameInstance* GI = TUSettings::GetGameInstance().Get())
		{
			if (const UWorld* World = GI->GetWorld())
			{
				if (World->GetTimeSeconds() < CanGetBadgeSecond)
				{
					OnSuccess.ExecuteIfBound(CachedBadgeDetails);
					return;
				}
			}
		}
	}
	
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	const FString Url = FString::Printf(TEXT("%s/billboard/rest-api/v1/dot/read?client_id=%s"),
		*ServerUrl,
		*FTUConfig::Get()->ClientID);
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("POST"));
	
	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLCSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("uuid"), TUDeviceInfo::GetLoginId());
	Writer->WriteValue(TEXT("template"), ETapBillboardTemplate::Navigate);
	WriteValue(Writer, TEXT("dimension_list"), FTUConfig::Get()->BillboardConfig->Dimensions);
	Writer->WriteObjectEnd();
	Writer->Close();
	HttpRequest->SetContentAsString(JsonString);
	
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBillboardCommon::NetGetBadgeDetailsCallback, OnSuccess, OnFailed);

	TAP_LOG_REQUEST(HttpRequest);
	if (HttpRequest->ProcessRequest())
	{
		CanGetBadgeSecond += 5.f * 60.f;
	}
	else
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error")));
	}
}

FDelegateHandle FTapBillboardCommon::RegisterCustomLinkListener(const FOnCustomLinkClicked::FDelegate& LinkClickedCallback)
{
	return OnCustomLinkClicked.Add(LinkClickedCallback);	
}

void FTapBillboardCommon::UnregisterCustomLinkListener(FDelegateHandle Handle)
{
	OnCustomLinkClicked.Remove(Handle);
}

void FTapBillboardCommon::OpenSplashPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose)
{
	if (IsValid(SplashBrowser) && SplashBrowser->IsInViewport())
	{
		OnFailed.Execute(FTUError(-1, TEXT("Already exists one.")));
		return;
	}
	if (TSharedPtr<FTUError> Error = CheckConfig())
	{
		OnFailed.ExecuteIfBound(*Error);
		return;
	}
	if (!TUSettings::GetGameInstance().IsValid())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Failed display splash billboard widget. no game instance")));
		return;
	}

	if (!IsValid(SplashBrowser))
	{
		SplashBrowser = CreateWidget<UTapBillboardBrowserSplash>(TUSettings::GetGameInstance().Get(), SplashBrowserClass.Get());
		SplashBrowser->OnAudioOutputStateChanged.BindRaw(this, &FTapBillboardCommon::InternalAudioOutputStateChangedCallback, ETapBillboardTemplate::Splash);
		SplashBrowser->OnCustomLinkClicked.BindRaw(this, &FTapBillboardCommon::InternalCustomUrlClickedCallback, ETapBillboardTemplate::Splash);
	}

	if (SplashBrowser->GetState() != EBillboardBrowserState::Loaded)
	{
		SplashBrowser->OnLoadComplete = OnSuccess;
		SplashBrowser->OnLoadFailed = OnFailed;
		SplashBrowser->OnBillboardBrowserClosed = OnClose;
	}

	switch(SplashBrowser->GetState())
	{
	case EBillboardBrowserState::NotLoad:
	case EBillboardBrowserState::LoadFailed:
		{
			SplashBrowser->LoadSplash();
		}
		break;
	case EBillboardBrowserState::Loading:
		break;
	case EBillboardBrowserState::Loaded:
		OnSuccess.ExecuteIfBound();
		SplashBrowser->AddToViewport(TUSettings::GetUILevel());
		return;
	default: ;
	}
	
	SplashBrowser->SetDisplayWhenReady(true);
}

void FTapBillboardCommon::CloseSplashPanel()
{
	if (IsValid(SplashBrowser))
	{
		SplashBrowser->RemoveFromParent();
		SplashBrowser = nullptr;
	}
}

void FTapBillboardCommon::StartFetchMarqueeData()
{
	if (FetchMarqueeTimer.IsValid())
	{
		return;
	}
	
	if (!TUSettings::GetGameInstance().IsValid())
	{
		return;
	}

	if (UWorld* World = TUSettings::GetGameInstance()->GetWorld())
	{
		FTimerDelegate Delegate;
		Delegate.BindRaw(this, &FTapBillboardCommon::TimerFetchMarqueeData);
		World->GetTimerManager().SetTimer(FetchMarqueeTimer, Delegate, 300.f, true, 0.f);

		Rest_SendTraceEvent(ETapBillboardTemplate::Marquee, {{TEXT("action"), TEXT("app_init")}, {TEXT("init_from"), TEXT("sdk")}});
	}
}

void FTapBillboardCommon::StopFetchMarqueeData(bool bCloseNow)
{
	if (TUSettings::GetGameInstance().IsValid())
	{
		if (UWorld* World = TUSettings::GetGameInstance()->GetWorld())
		{
			World->GetTimerManager().ClearTimer(FetchMarqueeTimer);
			FetchMarqueeTimer.Invalidate();
		}
	}
	if (bCloseNow && IsValid(MarqueeWidget))
	{
		if (MarqueeWidget->IsInViewport())
		{
			MarqueeWidget->RemoveFromParent();
		}
		MarqueeWidget = nullptr;
	}
}

void FTapBillboardCommon::RegisterOutputStateListener(const FAudioOutputStateChanged& OnAudioStateChanged)
{
	OnAudioOutputStateChanged = OnAudioStateChanged;	
}

void FTapBillboardCommon::UnregisterOutputStateListener()
{
	OnAudioOutputStateChanged.Unbind();
}

void FTapBillboardCommon::Rest_SubmitReadingRecord()
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	const FString Url = FString::Printf(TEXT("%s/billboard/rest-api/v1/dot/submit?client_id=%s"),
		*ServerUrl,
		*FTUConfig::Get()->ClientID);
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("POST"));
	
	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLCSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("uuid"), TUDeviceInfo::GetLoginId());
	Writer->WriteValue(TEXT("client_id"), FTUConfig::Get()->ClientID);
	Writer->WriteValue(TEXT("read_all"), 1);
	Writer->WriteObjectEnd();
	Writer->Close();
	HttpRequest->SetContentAsString(JsonString);

	if (HttpRequest->ProcessRequest())
	{
		TAP_LOG_REQUEST(HttpRequest);
		CachedBadgeDetails.show_red_dot = false;
	}
}

TSharedPtr<FTUError> FTapBillboardCommon::CheckConfig() const
{
	const TSharedPtr<FTUConfig> Config = FTUConfig::Get();
	check(Config);
	if (Config->ClientID.IsEmpty())
		return MakeShared<FTUError>(-1, TEXT("Need ClientID"));
	
	if (Config->ClientToken.IsEmpty())
		return MakeShared<FTUError>(-1, TEXT("Need ClientToken"));
	
	if (Config->RegionType == ERegionType::CN && (!Config->BillboardConfig.IsValid() || Config->BillboardConfig->BillboardUrl.IsEmpty()))
		return MakeShared<FTUError>(-1, TEXT("ERegionType::CN Need BillboardConfig.BillboardUrl"));
#if PLATFORM_WINDOWS || PLATFORM_MAC
	ensure(NavigateBrowserClass);
	ensure(SplashBrowserClass);
	ensure(MarqueeClass);
#endif
	return nullptr;
}

FString FTapBillboardCommon::GenerateBillboardUrl(const TMap<FString, FString>& DimensionsMap) const
{
	return FString::Printf(TEXT("%s/webapp/%s?business=%s&track=%s"),
					*FTUConfig::Get()->BillboardConfig->BillboardUrl,
					*FTUConfig::Get()->ClientID,
					*GenerateBusinessParams(DimensionsMap),
					*GenerateTrackParams());
}

FString FTapBillboardCommon::GenerateSplashUrl(const TMap<FString, FString>& DimensionsMap, int64 SplashID)
{
	return FString::Printf(TEXT("%s/webapp/%s?business=%s&track=%s"),
					*FTUConfig::Get()->BillboardConfig->BillboardUrl,
					*FTUConfig::Get()->ClientID,
					*GenerateBusinessParamsSplash(DimensionsMap, SplashID),
					*GenerateTrackParams());
}

UTexture2DDynamic* FTapBillboardCommon::GetMarqueeIconTexture() const
{
	return MarqueeIconTexture;
}

const FAnnouncementStyleData& FTapBillboardCommon::GetMarqueeStyle() const
{
	return CachedMarqueeStyle;
}

TSubclassOf<UTapBillboardBrowserPopup> FTapBillboardCommon::GetPopupBrowserClass() const
{
	return PopupBrowserClass.LoadSynchronous();
}

const FAnnouncementDetailData* FTapBillboardCommon::FindNextMarqueeData(const TArray<int64>& IgnoreIds) const
{
	for (int32 i = PendingDetailsReverse.Num() - 1; i >= 0; --i)
	{
		if (IgnoreIds.Contains(PendingDetailsReverse[i].id))
		{
			continue;
		}

		return &PendingDetailsReverse[i];
	}

	return nullptr;
}

void FTapBillboardCommon::PopMarqueeData(int64 MarqueeId)
{
	if (PendingDetailsReverse.Num() > 0)
	{
		if (PendingDetailsReverse.Top().id == MarqueeId)
		{
			PendingDetailsReverse.Pop();
		}
	}
}

void FTapBillboardCommon::DownloadWebFont(const FString& Url)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	HttpRequest->SetURL(Url);
	
	HttpRequest->SetVerb(TEXT("GET"));

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBillboardCommon::NetDownloadWebFontCallback);
	
	if (!HttpRequest->ProcessRequest())
	{
		
	}
}

const UFont* FTapBillboardCommon::GetDownloadFont() const
{
	return DownloadFont;
}

void FTapBillboardCommon::UpdateMarqueeScrollSpeed(float NewSpeed)
{
	MarqueeSpeed = NewSpeed;
	if (MarqueeWidget)
	{
		MarqueeWidget->UpdateScrollSpeed(NewSpeed);
	}
}

float FTapBillboardCommon::GetMarqueeSpeed() const
{
	return MarqueeSpeed;
}

void FTapBillboardCommon::Rest_FetchMarqueeStyle(const FAnnouncementStyleDataResult& OnSuccess, const FTapFailed& OnFailed)
{
	if (TSharedPtr<FTUError> Error = CheckConfig())
	{
		OnFailed.ExecuteIfBound(*Error);
		return;
	}
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	const FString Url = FString::Printf(TEXT("%s/billboard/rest-api/v1/pattern/detail?template=%s&client_id=%s"),
		*ServerUrl,
		ETapBillboardTemplate::Marquee,
		*FTUConfig::Get()->ClientID);
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("GET"));

	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLCSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBillboardCommon::NetFetchMarqueeStyleCallback, OnSuccess, OnFailed);

	TAP_LOG_REQUEST(HttpRequest);
	if (!HttpRequest->ProcessRequest())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error")));
	}
}

void FTapBillboardCommon::Rest_FetchUnreadAnnouncementsGeneralData(const TCHAR* Template, const FAnnouncementGeneralDataResult& OnSuccess, const FTapFailed& OnFailed)
{
	if (TSharedPtr<FTUError> Error = CheckConfig())
	{
		OnFailed.ExecuteIfBound(*Error);
		return;
	}
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	const FString Url = FString::Printf(TEXT("%s/billboard/rest-api/v1/announcement/unread?client_id=%s"),
		*ServerUrl,
		*FTUConfig::Get()->ClientID);
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("POST"));

	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLCSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	Writer->WriteObjectStart();
	WriteValue(Writer, TEXT("dimension_list"), FTUConfig::Get()->BillboardConfig->Dimensions);
	Writer->WriteValue(TEXT("template"), Template);
	Writer->WriteValue(TEXT("uuid"), TUDeviceInfo::GetLoginId());
	Writer->WriteObjectEnd();
	Writer->Close();
	HttpRequest->SetContentAsString(JsonString);
	
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBillboardCommon::NetFetchGeneralDataCallback, OnSuccess, OnFailed);

	TAP_LOG_REQUEST(HttpRequest);
	if (!HttpRequest->ProcessRequest())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error")));
	}
}

void FTapBillboardCommon::Rest_FetchAnnouncementsDetailData(const TArray<int64>& Ids, const FAnnouncementDetailsDataResult& OnSuccess, const FTapFailed& OnFailed)
{
	if (TSharedPtr<FTUError> Error = CheckConfig())
	{
		OnFailed.ExecuteIfBound(*Error);
		return;
	}
	
	if (Ids.Num() == 0)
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Need Ids")));
		return;
	}

	FString IDStr;
	for (int32 i = 0; i < Ids.Num(); ++i)
	{
		if (i == Ids.Num() - 1)
		{
			IDStr += FString::Printf(TEXT("%lld"), Ids[i]);
		}
		else
		{
			IDStr += FString::Printf(TEXT("%lld,"), Ids[i]);
		}
	}

	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	const FString Url = FString::Printf(TEXT("%s/billboard/rest-api/v1/announcement/detail/multi?client_id=%s&ids=%s&uuid=%s&lang=%s"),
		*ServerUrl,
		*FTUConfig::Get()->ClientID,
		*IDStr,
		*TUDeviceInfo::GetLoginId(),
		*TULanguage::GetLanguageString());
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("GET"));

	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLCSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBillboardCommon::NetFetchDetailDataCallback, OnSuccess, OnFailed);

	TAP_LOG_REQUEST(HttpRequest);
	if (!HttpRequest->ProcessRequest())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error")));
	}
}

void FTapBillboardCommon::Rest_AnnouncementsMarkRead(const TArray<int64>& Ids, const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed)
{
	if (TSharedPtr<FTUError> Error = CheckConfig())
	{
		OnFailed.ExecuteIfBound(*Error);
		return;
	}
	
	if (Ids.Num() == 0)
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Need param id")));
		return;
	}
	
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	const FString Url = FString::Printf(TEXT("%s/billboard/rest-api/v1/announcement/mark?client_id=%s"),
		*ServerUrl,
		*FTUConfig::Get()->ClientID);
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("POST"));

	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLCSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("uuid"), *TUDeviceInfo::GetLoginId());
	Writer->WriteValue(TEXT("ids"), Ids);
	Writer->WriteValue(TEXT("template"), ETapBillboardTemplate::Marquee);
	Writer->WriteObjectEnd();
	Writer->Close();
	HttpRequest->SetContentAsString(JsonString);
	
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FTapBillboardCommon::NetMarkReadCallback, OnSuccess, OnFailed);

	TAP_LOG_REQUEST(HttpRequest);
	if (!HttpRequest->ProcessRequest())
	{
		OnFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error")));
	}
}

void FTapBillboardCommon::Rest_SendTraceEvent(const TCHAR* Template, const TMap<FString, FString>& TraceParams)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	FString Url = FTUConfig::Get()->RegionType == ERegionType::CN
			? TEXT("https://tap-tds-data-bj.cn-beijing.log.aliyuncs.com")
			: TEXT("https://tap-tds-data-hk.log-global.aliyuncs.com");
	Url = TUDebuger::GetReplacedHost(Url);
	Url += TEXT("/logstores/web_tds_logs/track");
	
	TMap<FString, FString> Params;
	Params.Add(TEXT("APIVersion"), TEXT("0.6.0"));
	Params.Add(TEXT("XUA"), GenerateXUA());
	Params.Add(TEXT("client_id"), FTUConfig::Get()->ClientID);
	Params.Add(TEXT("os"), TUDeviceInfo::GetPlatform());
	Params.Add(TEXT("sv"), TUDeviceInfo::GetOSVersion());
	Params.Add(TEXT("dv"), FPlatformMisc::GetCPUVendor());
	Params.Add(TEXT("md"), FPlatformMisc::GetCPUBrand());
	Params.Add(TEXT("time"), FString::Printf(TEXT("%lld"), FDateTime::Now().ToUnixTimestamp() * 1000));
	Params.Add(TEXT("t_log_id"), FGuid::NewGuid().ToString());
	Params.Add(TEXT("sdk_vc"), TEXT(TapBillboard_VERSION_NUMBER));
#if PLATFORM_IOS || PLATFORM_ANDROID
	EDeviceScreenOrientation Orientation = FPlatformMisc::GetDeviceOrientation();
	FString OrientationStr = Orientation == EDeviceScreenOrientation::Portrait || Orientation == EDeviceScreenOrientation::PortraitUpsideDown ? TEXT("1") : TEXT("2");
	Params.Add(TEXT("orientation"), OrientationStr);
#else
	Params.Add(TEXT("orientation"), TEXT("1"));
#endif
	Params.Add(TEXT("template"), Template);
	Params.Add(TEXT("area"), FTUConfig::Get()->RegionType == ERegionType::CN ? TEXT("cn") : TEXT("intl"));
	Params.Append(TraceParams);
	
	int32 i = 0;
	FString Result = TEXT("?");
	for (const TTuple<FString, FString>& T : Params)
	{
		Result += FPlatformHttp::UrlEncode(T.Key);
		Result += TEXT("=");
		Result += FPlatformHttp::UrlEncode(T.Value);
		if (i++ < Params.Num() - 1)
		{
			Result += TEXT("&");
		}
	}
	Url += Result;
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("GET"));
	
	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLCSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	HttpRequest->ProcessRequest();
}

void FTapBillboardCommon::Test_UnmarkCurrentDevice()
{
	if (TSharedPtr<FTUError> Error = CheckConfig())
	{
		return;
	}
	
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	const FString Url = FString::Printf(TEXT("%s/billboard/rest-api/v1/announcement/unmark?client_id=%s"),
		*ServerUrl,
		*FTUConfig::Get()->ClientID);
	HttpRequest->SetURL(Url);

	HttpRequest->SetVerb(TEXT("POST"));

	HttpRequest->SetHeader(TEXT("X-LC-Id"), FTUConfig::Get()->ClientID);
	HttpRequest->SetHeader(TEXT("X-LC-Sign"), GetLCSyncString());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	Writer->WriteObjectStart();
	Writer->WriteValue(TEXT("uuid"), *TUDeviceInfo::GetLoginId());
	Writer->WriteObjectEnd();
	Writer->Close();
	HttpRequest->SetContentAsString(JsonString);
	
	TAP_LOG_REQUEST(HttpRequest);
	HttpRequest->ProcessRequest();
}

void FTapBillboardCommon::LoadBrowserClass()
{
	NavigateBrowserClass.LoadSynchronous();
	SplashBrowserClass.LoadSynchronous();
	MarqueeClass.LoadSynchronous();
	PopupBrowserClass.LoadSynchronous();
}

FString FTapBillboardCommon::GenerateBusinessParams(const TMap<FString, FString>& DimensionsMap) const
{
	TMap<FString, FString> Params;
	Params.Add(TEXT("lang"), TULanguage::GetLanguageString());
	
	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	WriteValue(Writer, DimensionsMap);
	Writer->Close();
	Params.Add(TEXT("dimension_list"), JsonString);
	Params.Add(TEXT("template"), ETapBillboardTemplate::Navigate);
	Params.Add(TEXT("query_nameless"), FTUConfig::Get()->ClientToken);
	
	int32 i = 0;
	FString Result;
	for (const TTuple<FString, FString>& T : Params)
	{
		Result += T.Key;
		Result += TEXT("=");
		Result += T.Value;
		if (i++ < Params.Num() - 1)
		{
			Result += TEXT("&");
		}
	}
	return FBase64::Encode(Result);
}

FString FTapBillboardCommon::GenerateBusinessParamsSplash(const TMap<FString, FString>& DimensionsMap, int64 SplashId) const
{
	TMap<FString, FString> Params;
	Params.Add(TEXT("lang"), TULanguage::GetLanguageString());
	
	FString JsonString;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString);
	WriteValue(Writer, DimensionsMap);
	Writer->Close();
	Params.Add(TEXT("dimension_list"), JsonString);
	Params.Add(TEXT("template"), ETapBillboardTemplate::Splash);
	Params.Add(TEXT("query_nameless"), FTUConfig::Get()->ClientToken);
	Params.Add(TEXT("splash_id"), FString::Printf(TEXT("%lld"), SplashId));
	
	int32 i = 0;
	FString Result;
	for (const TTuple<FString, FString>& T : Params)
	{
		Result += T.Key;
		Result += TEXT("=");
		Result += T.Value;
		if (i++ < Params.Num() - 1)
		{
			Result += TEXT("&");
		}
	}
	return FBase64::Encode(Result);
}

FString FTapBillboardCommon::GenerateTrackParams() const
{
	TMap<FString, FString> Params;
	Params.Add(TEXT("OS"), TUDeviceInfo::GetPlatform());
	Params.Add(TEXT("SV"), TUDeviceInfo::GetOSVersion());
	// Params.Add(TEXT("DEB"),TEXT("Google"));
	// Params.Add(TEXT("DEM"), TEXT("Sdk-Google-platform"));
	Params.Add(TEXT("SDKVC"), TEXT(TapBillboard_VERSION_NUMBER));
	Params.Add(TEXT("DEVICEID"), TUDeviceInfo::GetDeviceId());
	
	int32 i = 0;
	FString Result;
	for (const TTuple<FString, FString>& T : Params)
	{
		Result += T.Key;
		Result += TEXT("=");
		Result += T.Value;
		if (i++ < Params.Num() - 1)
		{
			Result += TEXT("&");
		}
	}
	return FBase64::Encode(Result);
}

FString FTapBillboardCommon::GenerateXUA() const
{
	TMap<FString, FString> Params;
	Params.Add(TEXT("V"), TEXT("1"));
	if (FTUConfig::Get()->RegionType == ERegionType::CN)
	{
		Params.Add(TEXT("PN"), TEXT("tdsBillboard"));
	}
	else
	{
		Params.Add(TEXT("PN"), TEXT("tdsBillboardIntl"));
	}
	Params.Add(TEXT("LANG"), TULanguage::GetLanguageString());
	Params.Add(TEXT("UID"), FGuid::NewGuid().ToString());
	Params.Add(TEXT("PLT"), TUDeviceInfo::GetPlatform());
	Params.Add(TEXT("OSV"), TUDeviceInfo::GetOSVersion());
	Params.Add(TEXT("SDKVC"), TEXT(TapBillboard_VERSION_NUMBER));
	Params.Add(TEXT("CID"), FTUConfig::Get()->ClientID);
	
	int32 i = 0;
	FString Result;
	for (const TTuple<FString, FString>& T : Params)
	{
		
		Result += T.Key;
		Result += TEXT("=");
		Result += T.Value;
		if (i++ < Params.Num() - 1)
		{
			Result += TEXT("&");
		}
	}
	return Result;
}

void FTapBillboardCommon::InternalCustomUrlClickedCallback(const FString& Url, const TCHAR* Template)
{
	OnCustomLinkClicked.Broadcast(Url);
}

void FTapBillboardCommon::InternalAudioOutputStateChangedCallback(bool bPlaying, const TCHAR* Template)
{
	OnAudioOutputStateChanged.ExecuteIfBound(bPlaying);
}

void FTapBillboardCommon::NetGetBadgeDetailsCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FTapBadgeDetailsResult OnSuccess, FTapFailed OnFailed)
{
	TAP_LOG_RESPONSE(HttpResponse);
	if (TSharedPtr<FTUError> Error = ParseTapHttpResponse(HttpResponse, CachedBadgeDetails))
	{
		OnFailed.ExecuteIfBound(*Error);	
	}
	else
	{
		bCachedBadge = true;
		OnSuccess.ExecuteIfBound(CachedBadgeDetails);
	}
}

void FTapBillboardCommon::NetFetchMarqueeStyleCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FAnnouncementStyleDataResult OnSuccess, FTapFailed OnFailed)
{
	TAP_LOG_RESPONSE(HttpResponse);
	if (const TSharedPtr<FTUError> Error = ParseTapHttpResponse(HttpResponse, CachedMarqueeStyle))
	{
		OnFailed.ExecuteIfBound(*Error);	
	}
	else
	{
		OnSuccess.ExecuteIfBound(CachedMarqueeStyle);
	}
}

void FTapBillboardCommon::NetFetchGeneralDataCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FAnnouncementGeneralDataResult OnSuccess, FTapFailed OnFailed)
{
	TAP_LOG_RESPONSE(HttpResponse);
	FAnnouncementGeneralDataList Wrapper;
	if (TSharedPtr<FTUError> Error = ParseTapHttpResponse(HttpResponse, Wrapper))
	{
		OnFailed.ExecuteIfBound(*Error);	
	}
	else
	{
		OnSuccess.ExecuteIfBound(Wrapper.list);
	}
}

void FTapBillboardCommon::NetFetchDetailDataCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FAnnouncementDetailsDataResult OnSuccess, FTapFailed OnFailed)
{
	TAP_LOG_RESPONSE(HttpResponse);
	FAnnouncementDetailDataList Wrapper;
	if (TSharedPtr<FTUError> Error = ParseTapHttpResponse(HttpResponse, Wrapper))
	{
		OnFailed.ExecuteIfBound(*Error);	
	}
	else
	{
		OnSuccess.ExecuteIfBound(Wrapper.list);
	}
}

void FTapBillboardCommon::NetMarkReadCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSimpleDelegate OnSuccess, FTapFailed OnFailed)
{
	TAP_LOG_RESPONSE(HttpResponse);
	FAnnouncementMarkRead Wrapper;
	if (TSharedPtr<FTUError> Error = ParseTapHttpResponse(HttpResponse, Wrapper))
	{
		OnFailed.ExecuteIfBound(*Error);	
	}
	else if(Wrapper.msg == TEXT("ok"))
	{
		OnSuccess.ExecuteIfBound();
	}
	else
	{
		OnFailed.ExecuteIfBound(FTUError(-1, FString::Printf(TEXT("Net error, error content, response content: %s"), *HttpResponse->GetContentAsString())));
	}
}

void FTapBillboardCommon::NetDownloadWebFontCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (HttpResponse && EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()) && HttpResponse->GetContentLength() > 0)
	{
		TArray<uint8> Data = HttpResponse->GetContent();
		UFontFace* FontFace = NewObject<UFontFace>();
		FontFace->LoadingPolicy = EFontLoadingPolicy::Inline;
		FontFace->FontFaceData = FFontFaceData::MakeFontFaceData(MoveTemp(Data));
	
		DownloadFont = NewObject<UFont>(GetTransientPackage(), NAME_None, RF_Transient);
		DownloadFont->FontCacheType = EFontCacheType::Runtime;
		FTypefaceEntry& TypefaceEntry = DownloadFont->CompositeFont.DefaultTypeface.Fonts.AddDefaulted_GetRef();
		TypefaceEntry.Font = FFontData(FontFace);
	}
}

void FTapBillboardCommon::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(NavigateBrowser);
	Collector.AddReferencedObject(SplashBrowser);
	Collector.AddReferencedObject(MarqueeWidget);
	
	Collector.AddReferencedObject(MarqueeIconTexture);
	Collector.AddReferencedObject(DownloadFont);
}

void FTapBillboardCommon::InternalFetchUnreadSplashCallback(const TArray<FAnnouncementGeneralData>& GeneralData)
{
	bSplashDataCallback = true;
	if (GeneralData.Num() > 0)
	{
		if (!SplashBrowser)
		{
			SplashBrowser = CreateWidget<UTapBillboardBrowserSplash>(TUSettings::GetGameInstance().Get(), SplashBrowserClass.Get());
			SplashBrowser->OnAudioOutputStateChanged.BindRaw(this, &FTapBillboardCommon::InternalAudioOutputStateChangedCallback, ETapBillboardTemplate::Splash);
			SplashBrowser->OnCustomLinkClicked.BindRaw(this, &FTapBillboardCommon::InternalCustomUrlClickedCallback, ETapBillboardTemplate::Splash);
			SplashBrowser->LoadSplashWithID(GeneralData[0].id);
		}

		SplashBrowser->SetExpireTime(GeneralData[0].expire_time);
	}
	else
	{
		if (!NavigateBrowser)
		{
			NavigateBrowser = CreateWidget<UTapBillboardBrowserNavigate>(TUSettings::GetGameInstance().Get(), NavigateBrowserClass.Get());
			NavigateBrowser->OnAudioOutputStateChanged.BindRaw(this, &FTapBillboardCommon::InternalAudioOutputStateChangedCallback, ETapBillboardTemplate::Navigate);
			NavigateBrowser->OnCustomLinkClicked.BindRaw(this, &FTapBillboardCommon::InternalCustomUrlClickedCallback, ETapBillboardTemplate::Navigate);
		}
	}
}

void FTapBillboardCommon::InternalFetchMarqueeStyleCallback(const FAnnouncementStyleData& StyleData)
{
	CachedMarqueeStyle = StyleData;
	if (!MarqueeIconTexture)
	{
		FTapCommonModule::AsyncDownloadImage(
			CachedMarqueeStyle.icon.FindRef(TEXT("url")),
			FAsyncDownloadImage::CreateRaw(this, &FTapBillboardCommon::InternalDownloadMarqueeImageCallback));
	}
	if (!CachedMarqueeStyle.use_system_font)
	{
		DownloadWebFont(CachedMarqueeStyle.custom_font);
	}
}

void FTapBillboardCommon::InternalDownloadMarqueeImageCallback(UTexture2DDynamic* Texture)
{
	MarqueeIconTexture = Texture;
}

void FTapBillboardCommon::TimerFetchMarqueeData()
{
	Rest_FetchUnreadAnnouncementsGeneralData(ETapBillboardTemplate::Marquee,
		FAnnouncementGeneralDataResult::CreateRaw(this, &FTapBillboardCommon::HandleFetchUnreadMarqueeGeneralData),
		FTapFailed());
}

void FTapBillboardCommon::HandleFetchUnreadMarqueeGeneralData(const TArray<FAnnouncementGeneralData>& GeneralData)
{
	if (GeneralData.Num() > 0)
	{
		TArray<int64> Ids;
		for (const FAnnouncementGeneralData& Data : GeneralData)
		{
			Ids.Add(Data.id);
		}
		Rest_FetchAnnouncementsDetailData(Ids, FAnnouncementDetailsDataResult::CreateRaw(this, &FTapBillboardCommon::HandleFetchMarqueeDetailData), FTapFailed());
	}
	else
	{
		PendingDetailsReverse.Reset();
	}
}

void FTapBillboardCommon::HandleFetchMarqueeDetailData(const TArray<FAnnouncementDetailData>& DetailsData)
{
	PendingDetailsReverse.Reset();
	for (int32 i = DetailsData.Num() - 1; i >= 0; --i)
	{
		PendingDetailsReverse.Add_GetRef(DetailsData[i]);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, DetailsData[i].content);
	}
	
	if (PendingDetailsReverse.Num() != 0)
	{
		if (!IsValid(MarqueeWidget))
		{
			MarqueeWidget = CreateWidget<UTapBillboardMarquee>(TUSettings::GetGameInstance().Get(), MarqueeClass.Get());
		}

		if (!MarqueeWidget->IsInViewport())
		{
			MarqueeWidget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}
