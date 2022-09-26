// Copyright Epic Games, Inc. All Rights Reserved.

#include "TUWebBrowser.h"

#include <iostream>

#include "SWebBrowser.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Async/TaskGraphInterfaces.h"
#include "UObject/ConstructorHelpers.h"

#if WITH_EDITOR
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialExpressionMaterialFunctionCall.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionTextureSampleParameter2D.h"
#include "Materials/MaterialFunction.h"
#include "Factories/MaterialFactoryNew.h"
#include "AssetRegistryModule.h"
#include "PackageHelperFunctions.h"
#endif

#define LOCTEXT_NAMESPACE "WebBrowser"

/////////////////////////////////////////////////////
// UWebBrowser

UTUWebBrowser::UTUWebBrowser(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
}

void UTUWebBrowser::LoadURL(FString NewURL)
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->LoadURL(NewURL);
	}
}

void UTUWebBrowser::LoadString(FString Contents, FString DummyURL)
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->LoadString(Contents, DummyURL);
	}
}

void UTUWebBrowser::ExecuteJavascript(const FString& ScriptText)
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->ExecuteJavascript(ScriptText);
	}
}

FText UTUWebBrowser::GetTitleText() const
{
	if ( WebBrowserWidget.IsValid() )
	{
		return WebBrowserWidget->GetTitleText();
	}

	return FText::GetEmpty();
}

FString UTUWebBrowser::GetUrl() const
{
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->GetUrl();
	}

	return FString();
}

void UTUWebBrowser::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	WebBrowserWidget.Reset();
}

TSharedRef<SWidget> UTUWebBrowser::RebuildWidget()
{
	if ( IsDesignTime() )
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Web Browser", "Web Browser"))
			];
	}
	else {
		WebBrowserWidget = SNew(SWebBrowser)
			.InitialURL(InitialURL)
			.ShowControls(false)
			.ShowErrorMessage(false)
			.SupportsTransparency(bSupportsTransparency)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged))
			.OnTitleChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTitleChanged))
			.OnLoadCompleted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadCompleted))
			.OnLoadError(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadError))
			.OnLoadStarted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadStarted))
			// .OnCreateWindow(OnCreateWindow) 这种写法不对，还是模仿上面的写法
			// .OnCloseWindow(OnCloseWindow)
			.OnBeforeNavigation(BIND_UOBJECT_DELEGATE(SWebBrowser::FOnBeforeBrowse, HandleOnBeforeNavigation))
			// .OnLoadUrl(OnLoadUrl)
			// .OnShowDialog(OnShowDialog)
			// .OnDismissAllDialogs(OnDismissAllDialogs)
			// .OnSuppressContextMenu(OnSuppressContextMenu)
			// .OnDragWindow(OnDragWindow)
			.BrowserFrameRate(60.f)
			.OnBeforePopup(BIND_UOBJECT_DELEGATE(FOnBeforePopupDelegate, HandleOnBeforePopup));

		if (ITextInputMethodSystem* InputSys = FSlateApplication::Get().GetTextInputMethodSystem())
		{
			WebBrowserWidget->BindInputMethodSystem(InputSys);
		}
		return WebBrowserWidget.ToSharedRef();
	}
}

bool UTUWebBrowser::CanGoBack() const {
	if (WebBrowserWidget.IsValid()) {
		return WebBrowserWidget->CanGoBack();
	}
	return false;
}

void UTUWebBrowser::GoBack() {
	if (WebBrowserWidget.IsValid()) {
		WebBrowserWidget->GoBack();
	}
}

bool UTUWebBrowser::CanGoForward() const {
	if (WebBrowserWidget.IsValid()) {
		return WebBrowserWidget->CanGoForward();
	}
	return false;
}

void UTUWebBrowser::GoForward() {
	if (WebBrowserWidget.IsValid()) {
		WebBrowserWidget->GoForward();
	}
}

void UTUWebBrowser::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if ( WebBrowserWidget.IsValid() )
	{

	}
}

void UTUWebBrowser::HandleOnUrlChanged(const FText& InText)
{
	OnUrlChanged.Broadcast(InText.ToString());
	const TCHAR* JSCode = TEXT("window.oncontextmenu = function(event){	event.preventDefault();	event.stopPropagation();	return false; };");
	ExecuteJavascript(JSCode);
}

void UTUWebBrowser::HandleOnTitleChanged(const FText& Text) {
	OnTitleChanged.Broadcast(Text.ToString());
}

bool UTUWebBrowser::HandleOnBeforePopup(FString URL, FString Frame)
{
	if (OnBeforePopup.IsBound())
	{
		if (IsInGameThread())
		{
			OnBeforePopup.Broadcast(URL, Frame);
		}
		else
		{
			// Retry on the GameThread.
			TWeakObjectPtr<UTUWebBrowser> WeakThis = this;
			FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, URL, Frame]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->HandleOnBeforePopup(URL, Frame);
				}
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		}

		return true;
	}

	return false;
}

void UTUWebBrowser::HandleOnLoadCompleted() {
	OnLoadCompleted.Broadcast();
}

void UTUWebBrowser::HandleOnLoadError() {
	OnLoadError.Broadcast();
}

void UTUWebBrowser::HandleOnLoadStarted() {
	OnLoadStarted.Broadcast();
}

bool UTUWebBrowser::HandleOnBeforeNavigation(const FString& Url, const FWebNavigationRequest& Request) {
	if (OnBeforeNavigation.IsBound()) {
		return OnBeforeNavigation.Execute(Url, Request);
	}
	return false;
}


#if WITH_EDITOR

const FText UTUWebBrowser::GetPaletteCategory()
{
	return LOCTEXT("TapTap", "TapTap");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
