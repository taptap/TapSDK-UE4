// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapBillboardCommon.h"

#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <TapBillboardSDK/TapBillboardSDK.h>
#import <TapBootstrapSDK/TapBootstrapSDK.h>
/**
 * 
 */
class TAPBILLBOARD_API FTapBillboard : public FTapBillboardCommon
{
public:
	FTapBillboard();
	
	virtual void Init(const FTUConfig& InConfig) override;

	virtual void OpenPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose) override;

	virtual void OpenSplashPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose) override;

	virtual void CloseSplashPanel() override;

	virtual void StartFetchMarqueeData() override;

	virtual void StopFetchMarqueeData(bool bCloseNow) override;
};
