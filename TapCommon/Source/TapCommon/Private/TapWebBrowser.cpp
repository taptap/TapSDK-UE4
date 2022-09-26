// Fill out your copyright notice in the Description page of Project Settings.


#include "TapWebBrowser.h"

#include "SWebBrowser.h"
#include "TapCommon.h"
#include "Components/Button.h"
#include "Components/NativeWidgetHost.h"

void UTapWebBrowser::LoadURL(const FString& InURL)
{
	if (const TSharedPtr<SWebBrowser> InnerBrowser = GetInnerWebBrowser())
	{
		InnerBrowser->LoadURL(InURL);
	}
}

void UTapWebBrowser::UpdateRetryPanelVisibility(ESlateVisibility NewVisibility)
{
	if (RetryPanel)
	{
		RetryPanel->SetVisibility(NewVisibility);
	}
}

TSharedPtr<SWebBrowser> UTapWebBrowser::GetInnerWebBrowser() const
{
	return StaticCastSharedPtr<SWebBrowser>(WebBrowser->GetContent());
}

bool UTapWebBrowser::CanGoBack() const
{
	if (const TSharedPtr<SWebBrowser> InnerBrowser = GetInnerWebBrowser())
	{
		return InnerBrowser->CanGoBack();
	}
	return false;
}

void UTapWebBrowser::GoBack()
{
	if (const TSharedPtr<SWebBrowser> InnerBrowser = GetInnerWebBrowser())
	{
		InnerBrowser->GoBack();
	}
}

void UTapWebBrowser::Close()
{
	if (OnCloseWebBrowserClicked.IsBound())
	{
		OnCloseWebBrowserClicked.Execute();
	}
	RemoveFromParent();
}

void UTapWebBrowser::Reload()
{
	if (const TSharedPtr<SWebBrowser> InnerBrowser = GetInnerWebBrowser())
	{
		InnerBrowser->Reload();
	}
}

void UTapWebBrowser::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	const TSharedRef<SWebBrowser> Browser = SNew(SWebBrowser)
		.InitialURL(TEXT(""))
		.ShowInitialThrobber(false)
		.ShowControls(false)
		.ShowErrorMessage(false)
		.BrowserFrameRate(60.f)
		.OnUrlChanged(FOnTextChanged::CreateUObject(this, &UTapWebBrowser::OnURLChanged))
		.OnTitleChanged(FOnTextChanged::CreateUObject(this, &UTapWebBrowser::OnTitleChanged))
		.OnLoadStarted(FSimpleDelegate::CreateUObject(this, &UTapWebBrowser::OnLoadStarted))
		.OnLoadCompleted(FSimpleDelegate::CreateUObject(this, &UTapWebBrowser::OnLoadCompleted))
		.OnLoadError(FSimpleDelegate::CreateUObject(this, &UTapWebBrowser::OnLoadError))
		.OnBeforeNavigation(SWebBrowser::FOnBeforeBrowse::CreateUObject(this, &UTapWebBrowser::OnBeforeNavigation));
	
	WebBrowser->SetContent(Browser);

	if (ITextInputMethodSystem* InputSys = FSlateApplication::Get().GetTextInputMethodSystem())
	{
		Browser->BindInputMethodSystem(InputSys);
	}

	if (BTN_GoBack)
	{
		BTN_GoBack->OnClicked.AddDynamic(this, &UTapWebBrowser::GoBack);
	}

	if (BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UTapWebBrowser::Close);
	}

	if (BTN_Retry)
	{
		BTN_Retry->OnClicked.AddDynamic(this, &UTapWebBrowser::Reload);
	}
}

void UTapWebBrowser::OnURLChanged(const FText& NewURL)
{
	const TCHAR* JSCode = TEXT("window.oncontextmenu = function(event){	event.preventDefault();	event.stopPropagation();	return false; };");
	GetInnerWebBrowser()->ExecuteJavascript(JSCode);
}

void UTapWebBrowser::OnTitleChanged(const FText& NewTitle)
{
}

void UTapWebBrowser::OnLoadStarted()
{
	FTapCommonModule::TapThrobberShowWait();
}

void UTapWebBrowser::OnLoadCompleted()
{
	FTapCommonModule::TapThrobberDismiss();
}

void UTapWebBrowser::OnLoadError()
{
	FTapCommonModule::TapThrobberDismiss();
}

bool UTapWebBrowser::OnBeforeNavigation(const FString& URL, const FWebNavigationRequest& Request)
{
	return false;
}
