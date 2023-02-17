// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBillboardBrowserNavigate.h"

#include "SWebBrowserView.h"
#include "TapBillboardModule.h"
#include "TapBillboardTypes.h"
#include "TapPlatformButton.h"
#include "TUHelper.h"
#include "TUType.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/NativeWidgetHost.h"
#include "Components/ScaleBox.h"
#include "IWebBrowserWindow.h"
#include "TapBillboardBrowserPopup.h"
#include "TUSettings.h"
#include "Components/SizeBox.h"
#if PLATFORM_IOS
#include "ButtonHandle.h"
#endif

void UTapBillboardBrowserNavigate::LoadUrl(const FString& Url)
{
	BrowserView->LoadURL(Url);
	State = EBillboardBrowserState::Loading;
}

void UTapBillboardBrowserNavigate::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (const FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		const FString Url = Billboard->GenerateBillboardUrl(FTUConfig::Get()->BillboardConfig->Dimensions);
		
		SAssignNew(BrowserView, SWebBrowserView)
			.InitialURL(Url)
			.BackgroundColor(FColor(0,0,0,0))
			.SupportsTransparency(true)
			.ShowErrorMessage(false)
			.BrowserFrameRate(60.f)
			.OnBeforePopup_UObject(this, &UTapBillboardBrowserNavigate::OnBeforePopup)
			.OnBeforeNavigation_UObject(this, &UTapBillboardBrowserNavigate::OnBeforeNavigate)
			.OnLoadCompleted_UObject(this, &UTapBillboardBrowserNavigate::OnLoadCompleted)
			.OnLoadError_UObject(this, &UTapBillboardBrowserNavigate::OnLoadError);

		State = EBillboardBrowserState::Loading;
		NativeWidget->SetContent(BrowserView.ToSharedRef());

		if (ITextInputMethodSystem* InputSys = FSlateApplication::Get().GetTextInputMethodSystem())
		{
			BrowserView->BindInputMethodSystem(InputSys);
		}

		CloseButton->OnClicked.BindUObject(this, &UTapBillboardBrowserNavigate::Close);
		CloseButton->SetVisibility(ESlateVisibility::Hidden);
		Billboard->GetBadgeDetails(FTapBadgeDetailsResult::CreateUObject(this, &UTapBillboardBrowserNavigate::GetBadgeDetailSuccess),
			FTapFailed::CreateUObject(this, &UTapBillboardBrowserNavigate::OnGetBadgeDetailFailed));
	}

#if PLATFORM_WINDOWS || PLATFORM_MAC
	SizeBox->SetWidthOverride(1000.f);
	SizeBox->SetHeightOverride(600.f);
#else
	SizeBox->SetWidthOverride(1464.f);
	SizeBox->SetHeightOverride(1208.f);
#endif
}

void UTapBillboardBrowserNavigate::NativeConstruct()
{
	Super::NativeConstruct();
#if PLATFORM_IOS
	BillboardIOSHelper::SetScrollable(false);
#endif
}

void UTapBillboardBrowserNavigate::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (TSharedPtr<SWidget> Widget = GetCachedWidget())
	{
		if (TSharedPtr<SWindow> WidgetWindow = FSlateApplicationBase::Get().FindWidgetWindow(Widget.ToSharedRef()))
		{
			const float NativeScale = WidgetWindow->GetNativeWindow()->GetDPIScaleFactor();
			check(NativeScale > 0.f);
			const FVector2D NativeSize = NativeWidget->GetCachedGeometry().GetAbsoluteSize() / NativeScale;
			const float Scale = NativeSize.X / (NativeSize.X > NativeSize.Y ? 584.f : 336.f);
			UpdateCloseButtonDPIScale(Scale * NativeScale);
		}
	}
}

bool UTapBillboardBrowserNavigate::OnBeforePopup(FString Url, FString FrameName)
{
	if (BrowserView->GetAddressBarUrlText().ToString() == Url)
	{
		return false;
	}
	TWeakObjectPtr<UTapBillboardBrowserNavigate> WeakThis(this);
	
	ETapBillboardUrlType Type = GetBillboardUrlType(Url);

	switch (Type)
	{
	case ETapBillboardUrlType::Custom:
		AsyncTask(ENamedThreads::GameThread, [WeakThis, Url]() { if(WeakThis.IsValid()) { WeakThis->OnCustomLinkClicked.ExecuteIfBound(Url); }});
		break;
	case ETapBillboardUrlType::AudioOn:
		AsyncTask(ENamedThreads::GameThread, [WeakThis]() { if(WeakThis.IsValid()) { WeakThis->OnAudioOutputStateChanged.ExecuteIfBound(true); }});
		break;
	case ETapBillboardUrlType::AudioOff:
		AsyncTask(ENamedThreads::GameThread, [WeakThis]() { if(WeakThis.IsValid()) { WeakThis->OnAudioOutputStateChanged.ExecuteIfBound(false); }});
		break;
	case ETapBillboardUrlType::LaunchSystemBrowser:
		AsyncTask(ENamedThreads::GameThread, [Url]() { TUHelper::LaunchURL(*Url, nullptr, nullptr); });
		break;
	case ETapBillboardUrlType::LaunchInnerBrowser:
		AsyncTask(ENamedThreads::GameThread, [Url]() { UTapBillboardBrowserPopup::OpenUrl(Url); });
		break;
	default:
		return false;
	}
	return true;
}

bool UTapBillboardBrowserNavigate::OnBeforeNavigate(const FString& Url, const FWebNavigationRequest& Request)
{
	if (BrowserView->GetAddressBarUrlText().ToString() == Url)
	{
		return false;
	}
	TWeakObjectPtr<UTapBillboardBrowserNavigate> WeakThis(this);
	
	ETapBillboardUrlType Type = GetBillboardUrlType(Url);

	switch (Type)
	{
	case ETapBillboardUrlType::Custom:
		AsyncTask(ENamedThreads::GameThread, [WeakThis, Url]() { if(WeakThis.IsValid()) { WeakThis->OnCustomLinkClicked.ExecuteIfBound(Url); }});
		break;
	case ETapBillboardUrlType::AudioOn:
		AsyncTask(ENamedThreads::GameThread, [WeakThis]() { if(WeakThis.IsValid()) { WeakThis->OnAudioOutputStateChanged.ExecuteIfBound(true); }});
		break;
	case ETapBillboardUrlType::AudioOff:
		AsyncTask(ENamedThreads::GameThread, [WeakThis]() { if(WeakThis.IsValid()) { WeakThis->OnAudioOutputStateChanged.ExecuteIfBound(false); }});
		break;
	case ETapBillboardUrlType::LaunchSystemBrowser:
		AsyncTask(ENamedThreads::GameThread, [Url]() { TUHelper::LaunchURL(*Url, nullptr, nullptr); });
		break;
	case ETapBillboardUrlType::LaunchInnerBrowser:
		AsyncTask(ENamedThreads::GameThread, [Url]() { UTapBillboardBrowserPopup::OpenUrl(Url); });
		break;
	default:
		return false;
	}
	return true;
}

void UTapBillboardBrowserNavigate::OnLoadCompleted()
{
	if (GetState() == EBillboardBrowserState::Loaded || GetState() == EBillboardBrowserState::LoadFailed)
	{
		return;
	}
	TWeakObjectPtr<UTapBillboardBrowserNavigate> WeakThis(this);
	State = EBillboardBrowserState::Loaded;
	AsyncTask(ENamedThreads::GameThread, [WeakThis]()
	{
		if(WeakThis.IsValid())
		{
			WeakThis->OnLoadComplete.ExecuteIfBound();
			WeakThis->OnLoadComplete.Unbind();
			WeakThis->OnLoadFailed.Unbind();
			WeakThis->CloseButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			WeakThis->CloseButton->ShowPlatformButtonNextFrame();
			if (WeakThis->bDisplayWhenRead)
			{
				WeakThis->AddToViewport(TUSettings::GetUILevel());
			}
		}
	});
}

void UTapBillboardBrowserNavigate::OnLoadError()
{
	if (GetState() == EBillboardBrowserState::Loaded || GetState() == EBillboardBrowserState::LoadFailed)
	{
		return;
	}
	TWeakObjectPtr<UTapBillboardBrowserNavigate> WeakThis(this);
	State = EBillboardBrowserState::LoadFailed;
	AsyncTask(ENamedThreads::GameThread, [WeakThis]()
	{
		if(WeakThis.IsValid())
		{
			WeakThis->OnLoadFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error.")));
			WeakThis->OnLoadComplete.Unbind();
			WeakThis->OnLoadFailed.Unbind();
			WeakThis->CloseButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			WeakThis->CloseButton->ShowPlatformButtonNextFrame();
		}
	});
}

void UTapBillboardBrowserNavigate::Close()
{
	OnBillboardBrowserClosed.ExecuteIfBound();

	SetVisibility(ESlateVisibility::Hidden);//接收小红点 暂时不关闭

	CloseButton->ClosePlatformButton();

	BrowserView->BindUObject(TEXT("browser"), this, false);
	const TCHAR* JSScript = TEXT("if (window.localStorage==null){window.ue.browser.jsnotifyreddot('submit')}else{window.ue.browser.jsnotifyreddot(window.localStorage.getItem('local_red_dot'))}");
	BrowserView->ExecuteJavascript(JSScript);
	if (FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		Billboard->Rest_SendTraceEvent(ETapBillboardTemplate::Navigate, {{TEXT("action"), TEXT("click")}, {TEXT("type"), TEXT("close")}});
		Billboard->NavigateBrowser = nullptr;//@TODO
	}
}

void UTapBillboardBrowserNavigate::JSNotifyRedDot(const FString& ResultString)
{
	FString JsonString;
	if (ResultString == TEXT("submit"))
	{
		if (FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
		{
			Billboard->Rest_SubmitReadingRecord();
		}
	}
	else if (!ResultString.IsEmpty() && FBase64::Decode(ResultString, JsonString))
	{
		JsonString = FGenericPlatformHttp::UrlDecode(JsonString);
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			bool bResult = true;
			if (JsonObject->TryGetBoolField(FTUConfig::Get()->ClientID, bResult) && !bResult)
			{
				if (FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
				{
					Billboard->Rest_SubmitReadingRecord();
				}
			}
		}
	}

	RemoveFromParent();
}

void UTapBillboardBrowserNavigate::GetBadgeDetailSuccess(const FBadgeDetails& BadgeDetails)
{
	CloseButton->UpdateButtonTexture(BadgeDetails.close_button_img);
}

void UTapBillboardBrowserNavigate::OnGetBadgeDetailFailed(const FTUError& Error)
{
	OnLoadFailed.ExecuteIfBound(Error);
}

void UTapBillboardBrowserNavigate::UpdateCloseButtonDPIScale(float NewScale)
{
	if (DPIBox && GEngine && GEngine->GameViewport)
	{
		const float DPIScale = UWidgetLayoutLibrary::GetViewportScale(GEngine->GameViewport);
		DPIBox->SetUserSpecifiedScale(NewScale / DPIScale);
	}
}
