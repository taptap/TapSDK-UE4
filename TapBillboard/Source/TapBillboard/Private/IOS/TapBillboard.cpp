// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBillboard.h"

#include "TUHelper.h"

@interface TapBillboardOutputStateDelegateImpl : NSObject <TapBillboardOutputStateDelegate>

@property (nonatomic, strong) dispatch_block_t playVoiceBlock;
@property (nonatomic, strong) dispatch_block_t stopVoiceBlock;

@end

@implementation TapBillboardOutputStateDelegateImpl

- (void)onPlayVoice {
	if (self.playVoiceBlock) {
		self.playVoiceBlock();
	}
}

- (void)onStopVoice {
	if (self.stopVoiceBlock) {
		self.stopVoiceBlock();
	}
}

@end



FTapBillboard::FTapBillboard()
{
	[TapBillboard registerCustomLinkListener:^(NSString * _Nullable customUrl) {
		FString Url = FString(customUrl);
		TUHelper::PerformOnGameThread([this, Url](){ OnCustomLinkClicked.Broadcast(Url); });
	}];

	static TapBillboardOutputStateDelegateImpl *OutputStateDelegateImpl = nil;
	OutputStateDelegateImpl = [TapBillboardOutputStateDelegateImpl new];
	OutputStateDelegateImpl.playVoiceBlock = ^ () {
		TUHelper::PerformOnGameThread([this](){ OnAudioOutputStateChanged.ExecuteIfBound(true); });
	};
	OutputStateDelegateImpl.stopVoiceBlock = ^ () {
		TUHelper::PerformOnGameThread([this](){ OnAudioOutputStateChanged.ExecuteIfBound(false); });
	};
	
	[TapBillboard regsiterOutputStateListener:OutputStateDelegateImpl];
}

void FTapBillboard::Init(const FTUConfig& InConfig)
{
	FTapBillboardCommon::Init(InConfig);
	if (CheckConfig().IsValid())
	{
		return;
	}
    	
	if (InConfig.RegionType == ERegionType::CN)
	{
		NSMutableSet <NSArray *> *dimensionSet = [[NSMutableSet alloc] init];
		if (InConfig.BillboardConfig->Dimensions.Num() > 0)
		{
			for (TTuple<FString, FString>& T : InConfig.BillboardConfig->Dimensions)
			{
				[dimensionSet addObject:[NSArray arrayWithObjects:T.Key.GetNSString(), T.Value.GetNSString(), nil]];
			}
		}

		TapBillboardConfig *billboardCnConfig = [TapBillboardConfig new];
		billboardCnConfig.diemensionSet = dimensionSet; // 可选项
		billboardCnConfig.serverUrl = InConfig.BillboardConfig->BillboardUrl.GetNSString();

		TapConfig *config = [TapConfig new];
		config.clientId = InConfig.ClientID.GetNSString();
		config.clientToken = InConfig.ClientToken.GetNSString();
		config.region = TapSDKRegionTypeCN; // TapSDKRegionTypeCN：中国大陆，TapSDKRegionTypeIO：其他国家或地区
		config.serverURL = FTUConfig::Get()->ServerURL.GetNSString();
		config.tapBillboardConfig = billboardCnConfig;
		[TapBillboard initWithConfig:config];
	}
	else if(InConfig.RegionType == ERegionType::Global)
	{
		NSMutableSet <NSArray *> *dimensionSet = [[NSMutableSet alloc] init];
		if (InConfig.BillboardConfig->Dimensions.Num() > 0)
		{
			for (TTuple<FString, FString>& T : InConfig.BillboardConfig->Dimensions)
			{
				[dimensionSet addObject:[NSArray arrayWithObjects:T.Key.GetNSString(), T.Value.GetNSString(), nil]];
			}
		}

		TapBillboardConfig *billboardCnConfig = [TapBillboardConfig new];
		billboardCnConfig.diemensionSet = dimensionSet; // 可选项
		billboardCnConfig.serverUrl = InConfig.BillboardConfig->BillboardUrl.GetNSString();

		TapConfig *config = [TapConfig new];
		config.clientId = InConfig.ClientID.GetNSString();
		config.clientToken = InConfig.ClientToken.GetNSString();
		config.region = TapSDKRegionTypeIO; 
		config.serverURL = FTUConfig::Get()->ServerURL.GetNSString();
		config.tapBillboardConfig = billboardCnConfig;
		[TapBillboard initWithConfig:config];
	}
}

void FTapBillboard::OpenPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose)
{
	FSimpleDelegate OnSuccessCopy = OnSuccess;
	FTapFailed OnFailedCopy = OnFailed;
	FSimpleDelegate OnCloseCopy = OnClose;
	dispatch_async(dispatch_get_main_queue(), ^
	{
		[TapBillboard openPanel:^(bool _, NSError *_Nullable error)
		{
			if (error)
			{
				FTUError Error = FTUError(error.code, FString(error.localizedDescription));
				TUHelper::PerformOnGameThread([OnFailedCopy, Error](){ OnFailedCopy.ExecuteIfBound(Error); });
			}
			else
			{
				TUHelper::PerformOnGameThread([OnSuccessCopy](){ OnSuccessCopy.ExecuteIfBound(); });
			}
		}
		closeCallback:^(void){
			TUHelper::PerformOnGameThread([OnCloseCopy](){ OnCloseCopy.ExecuteIfBound(); });
		}];
	});
}

void FTapBillboard::OpenSplashPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose)
{
	FSimpleDelegate OnSuccessCopy = OnSuccess;
	FTapFailed OnFailedCopy = OnFailed;
	FSimpleDelegate OnCloseCopy = OnClose;
	dispatch_async(dispatch_get_main_queue(), ^
	{
		[TapBillboard openSplashPanel:^(bool _, NSError *_Nullable error)
		{
			if (error)
			{
				FTUError Error = FTUError(error.code, FString(error.localizedDescription));
				TUHelper::PerformOnGameThread([OnFailedCopy, Error](){ OnFailedCopy.ExecuteIfBound(Error); });
			}
			else
			{
				TUHelper::PerformOnGameThread([OnSuccessCopy](){ OnSuccessCopy.ExecuteIfBound(); });
			}
		}
		closeCallback:^(void){
			TUHelper::PerformOnGameThread([OnCloseCopy](){ OnCloseCopy.ExecuteIfBound(); });
		}];
	});
}

void FTapBillboard::CloseSplashPanel()
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		[TapBillboard closeSplashPanel];
	});
}

void FTapBillboard::StartFetchMarqueeData()
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		[TapBillboard startFetchMarqueeData];
	});
}

void FTapBillboard::StopFetchMarqueeData(bool bCloseNow)
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		[TapBillboard stopFetchMarqueeData:bCloseNow];
	});
}

