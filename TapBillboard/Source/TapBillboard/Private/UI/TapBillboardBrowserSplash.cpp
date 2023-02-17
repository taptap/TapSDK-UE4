// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBillboardBrowserSplash.h"

#include "SWebBrowserView.h"
#include "TapBillboardCommon.h"
#include "TapBillboardModule.h"
#include "TUHelper.h"
#include "TUType.h"
#include "Components/NativeWidgetHost.h"
#include "IWebBrowserWindow.h"
#include "TapBillboardBrowserPopup.h"
#include "TUSettings.h"
#include "Components/SizeBox.h"

void UTapBillboardBrowserSplash::LoadUrl(const FString& Url)
{
	BrowserView->LoadURL(Url);
	State = EBillboardBrowserState::Loading;
}

void UTapBillboardBrowserSplash::SetExpireTime(int64 Time)
{
	if (UWorld* World = GetWorld())
	{
		if (ExpireRemoveTimer.IsValid())
		{
			World->GetTimerManager().ClearTimer(ExpireRemoveTimer);
			ExpireRemoveTimer.Invalidate();
		}

		FDateTime ExpireTime = FDateTime::FromUnixTimestamp(Time);
		float Second = (ExpireTime - FDateTime::UtcNow()).GetTotalSeconds();
		if (Second > 0.f)
		{
			World->GetTimerManager().SetTimer(ExpireRemoveTimer, this, &UTapBillboardBrowserSplash::TimerRemoveSelf, Second);
		}
		else
		{
			TimerRemoveSelf();
		}
	}
}

void UTapBillboardBrowserSplash::LoadSplash()
{
	if (const FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		Billboard->Rest_FetchUnreadAnnouncementsGeneralData(ETapBillboardTemplate::Splash,
			FAnnouncementGeneralDataResult::CreateUObject(this, &UTapBillboardBrowserSplash::InternalFetchUnreadSplashCallback),
			OnLoadFailed);
	}
}

void UTapBillboardBrowserSplash::LoadSplashWithID(int64 SplashID)
{
	if (const FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		const FString Url = Billboard->GenerateSplashUrl(FTUConfig::Get()->BillboardConfig->Dimensions, SplashID);
		LoadUrl(Url);
	}
}

void UTapBillboardBrowserSplash::RemoveFromParent()
{
	if (FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		Billboard->Rest_SendTraceEvent(ETapBillboardTemplate::Splash, {{TEXT("action"), TEXT("click")}, {TEXT("type"), TEXT("close")}});
	}
	Super::RemoveFromParent();
}

void UTapBillboardBrowserSplash::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (const FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		SAssignNew(BrowserView, SWebBrowserView)
			.InitialURL(TEXT(""))
			.BackgroundColor(FColor(0,0,0,0))
			.SupportsTransparency(true)
			.ShowErrorMessage(false)
			.BrowserFrameRate(60.f)
			.OnBeforePopup_UObject(this, &UTapBillboardBrowserSplash::OnBeforePopup)
			.OnBeforeNavigation_UObject(this, &UTapBillboardBrowserSplash::OnBeforeNavigate)
			.OnLoadCompleted_UObject(this, &UTapBillboardBrowserSplash::OnLoadCompleted)
			.OnLoadError_UObject(this, &UTapBillboardBrowserSplash::OnLoadError);

		NativeWidget->SetContent(BrowserView.ToSharedRef());

		if (ITextInputMethodSystem* InputSys = FSlateApplication::Get().GetTextInputMethodSystem())
		{
			BrowserView->BindInputMethodSystem(InputSys);
		}
	}

#if PLATFORM_WINDOWS || PLATFORM_MAC
	SizeBox->SetWidthOverride(1000.f);
	SizeBox->SetHeightOverride(600.f);
#else
	SizeBox->SetWidthOverride(1464.f);
	SizeBox->SetHeightOverride(1208.f);
#endif
}

bool UTapBillboardBrowserSplash::OnBeforePopup(FString Url, FString FrameName)
{
	if (BrowserView->GetAddressBarUrlText().ToString() == Url)
	{
		return false;
	}
	TWeakObjectPtr<UTapBillboardBrowserSplash> WeakThis(this);

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

bool UTapBillboardBrowserSplash::OnBeforeNavigate(const FString& Url, const FWebNavigationRequest& Request)
{
	if (BrowserView->GetAddressBarUrlText().ToString() == Url)
	{
		return false;
	}
	TWeakObjectPtr<UTapBillboardBrowserSplash> WeakThis(this);

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

void UTapBillboardBrowserSplash::OnLoadCompleted()
{
	if (GetState() == EBillboardBrowserState::Loaded || GetState() == EBillboardBrowserState::LoadFailed)
	{
		return;
	}
	TWeakObjectPtr<UTapBillboardBrowserSplash> WeakThis(this);
	State = EBillboardBrowserState::Loaded;
	AsyncTask(ENamedThreads::GameThread, [WeakThis]()
	{
		if(WeakThis.IsValid())
		{
			WeakThis->OnLoadComplete.ExecuteIfBound();
			WeakThis->OnLoadComplete.Unbind();
			WeakThis->OnLoadFailed.Unbind();
			if (WeakThis->bDisplayWhenRead)
			{
				WeakThis->AddToViewport(TUSettings::GetUILevel());
			}
		}
	});
}

void UTapBillboardBrowserSplash::OnLoadError()
{
	if (GetState() == EBillboardBrowserState::Loaded || GetState() == EBillboardBrowserState::LoadFailed)
	{
		return;
	}
	TWeakObjectPtr<UTapBillboardBrowserSplash> WeakThis(this);
	State = EBillboardBrowserState::LoadFailed;
	AsyncTask(ENamedThreads::GameThread, [WeakThis]()
	{
		if(WeakThis.IsValid())
		{
			WeakThis->OnLoadFailed.ExecuteIfBound(FTUError(-1, TEXT("Net error.")));
			WeakThis->OnLoadComplete.Unbind();
			WeakThis->OnLoadFailed.Unbind();
		}
	});
}

void UTapBillboardBrowserSplash::TimerRemoveSelf()
{
	ExpireRemoveTimer.Invalidate();
	if (FTapBillboardPtr Billboard = FTapBillboardModule::GetTapBillboardInterface())
	{
		Billboard->CloseSplashPanel();
	}
}

void UTapBillboardBrowserSplash::InternalFetchUnreadSplashCallback(const TArray<FAnnouncementGeneralData>& GeneralData)
{
	if (GeneralData.Num() > 0)
	{
		LoadSplashWithID(GeneralData[0].id);
		SetExpireTime(GeneralData[0].expire_time);
	}
	else
	{
		OnLoadFailed.ExecuteIfBound(FTUError(-1, TEXT("Can not found splash data.")));
		TimerRemoveSelf();
	}
}

