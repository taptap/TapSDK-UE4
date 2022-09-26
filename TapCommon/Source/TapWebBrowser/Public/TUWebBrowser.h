// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IWebBrowserWindow.h"
#include "Components/Widget.h"
#include "TUWebBrowser.generated.h"


/**
 * 
 */
UCLASS()
class TAPWEBBROWSER_API UTUWebBrowser : public UWidget
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTUOnTextChanged, const FString&, Text);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTUOnBeforePopup, FString, URL, FString, Frame);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTUSimpleDelegate);

	/**
	 * Load the specified URL
	 *
	 * @param NewURL New URL to load
	 */
	UFUNCTION(BlueprintCallable, Category="Web Browser")
	void LoadURL(FString NewURL);

	/**
	 * Load a string as data to create a web page
	 *
	 * @param Contents String to load
	 * @param DummyURL Dummy URL for the page
	 */
	UFUNCTION(BlueprintCallable, Category="Web Browser")
	void LoadString(FString Contents, FString DummyURL);

	/**
	* Executes a JavaScript string in the context of the web page
	*
	* @param ScriptText JavaScript string to execute
	*/
	UFUNCTION(BlueprintCallable, Category = "Web Browser")
	void ExecuteJavascript(const FString& ScriptText);

	/**
	 * Get the current title of the web page
	 */
	UFUNCTION(BlueprintCallable, Category="Web Browser")
	FText GetTitleText() const;

	/**
	* Gets the currently loaded URL.
	*
	* @return The URL, or empty string if no document is loaded.
	*/
	UFUNCTION(BlueprintCallable, Category = "Web Browser")
	FString GetUrl() const;

	/** Called when the Url changes. */
	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	FTUOnTextChanged OnUrlChanged;

	/** Called when document title changed. */
	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	FTUOnTextChanged OnTitleChanged;

	/** Called when a popup is about to spawn. */
	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	FTUOnBeforePopup OnBeforePopup;

	/** Called when document loading completed. */
	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	FTUSimpleDelegate OnLoadCompleted;

	/** Called when document loading failed. */
	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	FTUSimpleDelegate OnLoadError;

	/** Called when document loading started. */
	UPROPERTY(BlueprintAssignable, Category = "Web Browser|Event")
	FTUSimpleDelegate OnLoadStarted;

	// /** Called when the browser requests the creation of a new window */
	// FOnCreateWindowDelegate OnCreateWindow;
	//
	// /** Called when a browser window close event is detected */
	// FOnCloseWindowDelegate OnCloseWindow;

	/** Called before browser navigation. return true 可以拦截跳转 */ 
	SWebBrowser::FOnBeforeBrowse OnBeforeNavigation;

	// /** Called to allow bypassing page content on load. */
	// SWebBrowser::FOnLoadUrl OnLoadUrl;
	//
	// /** Called when the browser needs to show a dialog to the user. */
	// SWebBrowser::FOnShowDialog OnShowDialog;
	//
	// /** Called to dismiss any dialogs shown via OnShowDialog. */
	// FSimpleDelegate OnDismissAllDialogs;
	//
	// FOnSuppressContextMenu OnSuppressContextMenu;
	//
	// /** Called when drag is detected in a web page area tagged as a drag region. */
	// SWebBrowser::FOnDragWindow OnDragWindow;

	/** Returns true if the browser can navigate backwards. */
	bool CanGoBack() const;

	/** Navigate backwards. */
	void GoBack();

	/** Returns true if the browser can navigate forwards. */
	bool CanGoForward() const;

	/** Navigate forwards. */
	void GoForward();

public:

	//~ Begin UWidget interface
	virtual void SynchronizeProperties() override;
	// End UWidget interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	/** URL that the browser will initially navigate to. The URL should include the protocol, eg http:// */
	UPROPERTY(EditAnywhere, Category=Appearance)
	FString InitialURL;

	/** Should the browser window support transparency. */
	UPROPERTY(EditAnywhere, Category=Appearance)
	bool bSupportsTransparency = false;

protected:
	TSharedPtr<class SWebBrowser> WebBrowserWidget;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

	void HandleOnUrlChanged(const FText& Text);
	void HandleOnTitleChanged(const FText& Text);
	bool HandleOnBeforePopup(FString URL, FString Frame);
	void HandleOnLoadCompleted();
	void HandleOnLoadError();
	void HandleOnLoadStarted();
	bool HandleOnBeforeNavigation(const FString& Url, const FWebNavigationRequest& Request);

	
};
