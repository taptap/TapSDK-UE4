// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TapBillboardBrowserPopup.generated.h"

class UNativeWidgetHost;
class UButton;
class SWebBrowserView;

UCLASS(Abstract)
class TAPBILLBOARD_API UTapBillboardBrowserPopup : public UUserWidget
{
	GENERATED_BODY()
public:
	static UTapBillboardBrowserPopup* OpenUrl(const FString& Url);

protected:
	virtual void NativeOnInitialized() override;

	void OnLoadCompleted();

	UPROPERTY(Meta = (BindWidget))
	UNativeWidgetHost* NativeWidget;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* CloseButton;

	TSharedPtr<SWebBrowserView> BrowserView;
};
