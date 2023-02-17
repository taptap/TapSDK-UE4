// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonHandle.h"
#include "IOS/IOSView.h"
#include "IOS/IOSAppDelegate.h"
#import "WebKit/WebKit.h"

@implementation IOSPlatformButton

@synthesize IOSButton;
@synthesize ViewContainer;

-(void)Create:(FButtonHandle*)InNativeHandle;
{
	NativeHandle = InNativeHandle;
	dispatch_async(dispatch_get_main_queue(), ^
	{
		ViewContainer = [[UIView alloc]initWithFrame:CGRectMake(1, 1, 100, 100)];
		[self.ViewContainer setOpaque : NO];
		[self.ViewContainer setBackgroundColor : [UIColor clearColor]];

		IOSButton = [[UIImageView alloc]init ];
		[self.IOSButton setOpaque : NO];
		[self.IOSButton setBackgroundColor : [UIColor clearColor]];

		UITapGestureRecognizer *singleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(TapDetected)];
		singleTap.numberOfTapsRequired = 1;
		[IOSButton setUserInteractionEnabled:YES];
		[IOSButton addGestureRecognizer:singleTap];
	});
}

-(void)Show;
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		[self.ViewContainer addSubview : IOSButton];
		[[IOSAppDelegate GetDelegate].IOSView addSubview : ViewContainer];
	});
}
-(void)Close;
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		[self.ViewContainer removeFromSuperview];
		[self.IOSButton removeFromSuperview];
		IOSButton = nil;
		ViewContainer = nil;
	});
}

-(void)UpdateTexture:(NSString*)InUrlString;
{
	self.ImageUrl = [NSURL URLWithString:InUrlString];
	dispatch_async(dispatch_get_main_queue(), ^
	{
		self.IOSButton.image = [UIImage imageWithData:[NSData dataWithContentsOfURL:self.ImageUrl]];
	});
}

-(void)UpdateLayout:(CGRect)InFrame;
{
	self.DesiredFrame = InFrame;

	dispatch_async(dispatch_get_main_queue(), ^
	{
		if (IOSButton != nil)
		{
			ViewContainer.frame = self.DesiredFrame;
			IOSButton.frame = ViewContainer.bounds;
		}
	});
}

-(void)SetVisible:(bool)NewVisible;
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		if (NewVisible)
		{
			[self.ViewContainer setHidden : NO];
		}
		else
		{
			[self.ViewContainer setHidden : YES];
		}
	});
}

-(void)TapDetected;
{
	if(NativeHandle != nullptr)
	{
		NativeHandle->NotifyButtonClicked();
	}
}
@end

void BillboardIOSHelper::SetScrollable(bool bNewScrollable)
{
	for (UIView *subView1 in [IOSAppDelegate GetDelegate].IOSView.subviews)
	{
		for (UIView *subView2 in subView1.subviews)
		{
			if ([subView2 isKindOfClass:[WKWebView class]])
			{
				((WKWebView *)subView2).scrollView.bounces = bNewScrollable;
				((WKWebView *)subView2).scrollView.alwaysBounceVertical = bNewScrollable;
				break;
			}
		}
	}
}

FButtonHandle::FButtonHandle(UTapPlatformButton* Button)
	: SavedButton(Button)
{
	PlatformButton = [IOSPlatformButton alloc];
	[PlatformButton Create : this];
}

FButtonHandle::~FButtonHandle()
{
	PlatformButton = nil;
}

void FButtonHandle::Show()
{
	if (PlatformButton != nil)
	{
		[PlatformButton Show];
	}
}

void FButtonHandle::Close()
{
	if (PlatformButton != nil)
	{
		[PlatformButton Close];
	}
}

void FButtonHandle::UpdateTexture(const FString IconUrl)
{
	if (PlatformButton != nil)
	{
		[PlatformButton UpdateTexture : [NSString stringWithUTF8String : TCHAR_TO_UTF8(*IconUrl)]];
	}
}

void FButtonHandle::UpdateLayout(const FGeometry& PlaceHolderGeometry)
{
	if (PlatformButton != nil)
	{
		UIView* View = [IOSAppDelegate GetDelegate].IOSView;
		CGFloat contentScaleFactor = View.contentScaleFactor;
		FVector2D Position = PlaceHolderGeometry.GetAccumulatedRenderTransform().GetTranslation() / contentScaleFactor;
		FVector2D Size = TransformVector(PlaceHolderGeometry.GetAccumulatedRenderTransform(), PlaceHolderGeometry.GetLocalSize()) / contentScaleFactor;
		CGRect NewFrame;
		NewFrame.origin.x = FMath::RoundToInt(Position.X);
		NewFrame.origin.y = FMath::RoundToInt(Position.Y);
		NewFrame.size.width = FMath::RoundToInt(Size.X);
		NewFrame.size.height = FMath::RoundToInt(Size.Y);

		[PlatformButton UpdateLayout : NewFrame];
	}
}

void FButtonHandle::SetVisible(bool bNewVisible)
{
	if (PlatformButton != nil)
	{
		[PlatformButton SetVisible : bNewVisible];
	}
}

void FButtonHandle::NotifyButtonClicked()
{
	TWeakObjectPtr<UTapPlatformButton> WeakButton = SavedButton.Get();
	AsyncTask(ENamedThreads::GameThread, [WeakButton](){ if (IsValid(WeakButton.Get())) WeakButton->OnClicked.ExecuteIfBound();});
}
