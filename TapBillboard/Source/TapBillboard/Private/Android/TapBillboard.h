// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapBillboardCommon.h"
#include "Android/AndroidJava.h"

/**
 * 
 */
class TAPBILLBOARD_API FTapBillboard : public FTapBillboardCommon, public FJavaClassObject
{
public:
	FTapBillboard();

	virtual ~FTapBillboard() override;
	
	virtual void Init(const FTUConfig& InConfig) override;

	virtual void OpenPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose) override;

	virtual void OpenSplashPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose) override;

	virtual void CloseSplashPanel() override;

	virtual void StartFetchMarqueeData() override;

	virtual void StopFetchMarqueeData(bool bCloseNow) override;


	void HandleCustomUrl(const FString& Url);

	void HandleAudioStatusChanged(bool newPlaying);
    
	void HandleNavigateOpenSuccess();
	void HandleNavigateOpenFailed(const FTUError& Error);
	void HandleNavigateClosed();
    
	void HandleSplashOpenSuccess();
	void HandleSplashOpenFailed(const FTUError& Error);
	void HandleSplashClosed();

	static TMap<int64, FTapBillboard*> AllAndroidBillboard;
protected:
	FJavaClassMethod InitMethod;
	FJavaClassMethod OpenPanelMethod;
	FJavaClassMethod OpenSplashPanelMethod;
	FJavaClassMethod CloseSplashPanelMethod;
	FJavaClassMethod StartFetchMarqueeDataMethod;
	FJavaClassMethod StopFetchMarqueeDataMethod;

	FSimpleDelegate NavigateSuccess;
	FTapFailed NavigateFailed;
	FSimpleDelegate NavigateClose;
	
	FSimpleDelegate SplashSuccess;
	FTapFailed  SplashFailed;
	FSimpleDelegate SplashClose;
};

