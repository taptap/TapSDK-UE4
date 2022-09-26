// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TapWebBrowser.generated.h"

struct FWebNavigationRequest;
class SWebBrowser;
class UImage;
class UTextBlock;
class UButton;
class UNativeWidgetHost;

DECLARE_DELEGATE(FOnCloseWebBrowserClicked);

/**
 * 
 */
UCLASS()
class TAPCOMMON_API UTapWebBrowser : public UUserWidget
{
	GENERATED_BODY()
public:
	void LoadURL(const FString& InURL);
	
	void UpdateRetryPanelVisibility(ESlateVisibility NewVisibility);

	FOnCloseWebBrowserClicked OnCloseWebBrowserClicked;

	TSharedPtr<SWebBrowser> GetInnerWebBrowser() const;

	bool CanGoBack() const;
	
	UFUNCTION()
	virtual void GoBack();

	UFUNCTION()
	virtual void Close();

	UFUNCTION()
	virtual void Reload();
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void OnURLChanged(const FText& NewURL);

	virtual void OnTitleChanged(const FText& NewTitle);

	virtual void OnLoadStarted();
	
	virtual void OnLoadCompleted();

	virtual void OnLoadError();

	virtual bool OnBeforeNavigation(const FString& URL, const FWebNavigationRequest& Request);
	
	UPROPERTY(Meta = (BindWidget))
	UNativeWidgetHost* WebBrowser;

	UPROPERTY(Meta = (BindWidgetOptional))
	UButton* BTN_GoBack;

	UPROPERTY(Meta = (BindWidgetOptional))
	UButton* BTN_Close;
	
	UPROPERTY(Meta = (BindWidgetOptional))
	UButton* BTN_Retry;

	UPROPERTY(Meta = (BindWidgetOptional))
	UTextBlock* TB_Retry;

	UPROPERTY(Meta = (BindWidgetOptional))
	UTextBlock* TB_ErrorTip;

	UPROPERTY(Meta = (BindWidgetOptional))
	UImage* ErrorTipIcon;

	UPROPERTY(Meta = (BindWidgetOptional))
	UPanelWidget* RetryPanel;
};

 