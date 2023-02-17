// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TapPlatformButton.h"
#import <UIKit/UIKit.h>

#include "IOS/IOSAppDelegate.h"

class FButtonHandle;

@interface IOSPlatformButton : NSObject
{
	FButtonHandle* NativeHandle;//生命周期和Handle同步
}
@property(strong) UIImageView* IOSButton;
@property(strong) UIView* ViewContainer;
@property CGRect DesiredFrame;
@property(copy) NSURL* ImageUrl;

-(void)Create:(FButtonHandle*)InNativeHandle;
-(void)Show;
-(void)Close;
-(void)UpdateTexture:(NSString*)InUrlString;
-(void)UpdateLayout:(CGRect)InFrame;
-(void)SetVisible:(bool)NewVisible;
-(void)TapDetected;
@end

namespace BillboardIOSHelper
{
	void SetScrollable(bool bNewScrollable);
}

class TAPBILLBOARD_API FButtonHandle : public IPlatformButtonHandle
{
public:
	FButtonHandle(UTapPlatformButton* Button);
	
	virtual ~FButtonHandle() override;

	virtual void Show() override;

	virtual void Close() override;

	virtual void UpdateTexture(const FString IconUrl) override;

	virtual void UpdateLayout(const FGeometry& PlaceHolderGeometry) override;

	virtual void SetVisible(bool bNewVisible) override;

	void NotifyButtonClicked();

protected:
	TWeakObjectPtr<UTapPlatformButton> SavedButton;

	mutable __strong IOSPlatformButton* PlatformButton;
};
