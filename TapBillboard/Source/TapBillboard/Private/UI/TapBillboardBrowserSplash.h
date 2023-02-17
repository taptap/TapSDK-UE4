// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TapBillboardTypes.h"
#include "TapBillboardBrowserSplash.generated.h"

struct FWebNavigationRequest;
class USizeBox;
/**
 * 
 */
UCLASS(Abstract)
class TAPBILLBOARD_API UTapBillboardBrowserSplash : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnCustomLinkClicked::FDelegate OnCustomLinkClicked;
	FAudioOutputStateChanged OnAudioOutputStateChanged;
	
	FSimpleDelegate OnLoadComplete;
	FTapFailed OnLoadFailed;
	FSimpleDelegate OnBillboardBrowserClosed;

	void LoadUrl(const FString& Url);

	FORCEINLINE EBillboardBrowserState GetState() const
	{
		return State;
	}

	FORCEINLINE void SetDisplayWhenReady(bool bDisplay)
	{
		bDisplayWhenRead = bDisplay;
	}

	void SetExpireTime(int64 Time);

	void LoadSplash();

	void LoadSplashWithID(int64 SplashID);
	
	virtual void RemoveFromParent() override;

protected:
	virtual void NativeOnInitialized() override;

	bool OnBeforePopup(FString Url, FString FrameName);

	bool OnBeforeNavigate(const FString& Url, const FWebNavigationRequest& Request);

	void OnLoadCompleted();

	void OnLoadError();

	void TimerRemoveSelf();

	void InternalFetchUnreadSplashCallback(const TArray<FAnnouncementGeneralData>& GeneralData);

	
	UPROPERTY(Meta = (BindWidget))
	class UNativeWidgetHost* NativeWidget;
	
	UPROPERTY(Meta = (BindWidget))
	USizeBox* SizeBox;

	UPROPERTY()
	EBillboardBrowserState State = EBillboardBrowserState::NotLoad;

	UPROPERTY()
	bool bDisplayWhenRead;

	FTimerHandle ExpireRemoveTimer;
	
	TSharedPtr<class SWebBrowserView> BrowserView;
};

 