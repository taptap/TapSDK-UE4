// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define TapUECommon_VERSION_NUMBER "31501001"
#define TapUECommon_VERSION "3.15.1"

class FSlateStyleSet;

class TAPCOMMON_API FTapCommonModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


	static inline FTapCommonModule &Get()
	{
		return FModuleManager::LoadModuleChecked<FTapCommonModule>("TapCommon");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("TapCommon");
	}

	DECLARE_MULTICAST_DELEGATE_OneParam(FBridgeCallback, const FString&);

	DECLARE_MULTICAST_DELEGATE_OneParam(FGetRegionCode, const bool);

	UPROPERTY(BlueprintAssignable, Category = "TapCommon")
	static FBridgeCallback OnBridgeCallback;

	UPROPERTY(BlueprintAssignable, Category= "TapCommon")
	static FGetRegionCode OnRegionCode;

	/** 是否安装 TapTap */
	DECLARE_MULTICAST_DELEGATE_OneParam(FIsTapTapInstalled,const bool);
	
	UPROPERTY(BlueprintAssignable,Category = "TapCommon")
	static FIsTapTapInstalled OnTapTapInstalled;

	/** 是否安装 Tap IO */
	DECLARE_MULTICAST_DELEGATE_OneParam(FIsTapGlobalInstalled,const bool);

	UPROPERTY(BlueprintAssignable,Category = "TapCommon")
	static FIsTapGlobalInstalled OnTapGlobalInstalled;

	/** 在 TapTap 更新 Game */
	DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateGameInTapTap,const bool);

	UPROPERTY(BlueprintAssignable,Category = "TapCommon")
	static FUpdateGameInTapTap OnUpdateGameInTapTap;

	/** 在 TapTap iO 更新 Game*/
	DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateGameInTapGlobal,const bool);

	UPROPERTY(BlueprintAssignable,Category = "TapCommon")
	static FUpdateGameInTapGlobal OnUpdateGameInTapGlobal;

	/** 在 TapTap 打开 Game */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOpenReviewInTapTap,const bool);

	UPROPERTY(BlueprintAssignable,Category = "TapCommon")
	static FOpenReviewInTapTap OnOpenReviewInTapTap;

	/** 在 Tap IO 打开 Game */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOpenReviewInTapGlobal,const bool);
	
	UPROPERTY(BlueprintAssignable,Category = "TapCommon")
	static FOpenReviewInTapGlobal OnOpenReviewInTapGlobal;

	static void TapThrobberShowWait();
 
	static void TapThrobberShowWaitAndToast(const FString& Toast);
	 
	static void TapThrobberDismiss();
 
	static void TapThrobberShowToast(const FString& Toast, float TimeInterval = 3.f);
 
	static void OnTapThrobberRemoveSelf(const TSharedRef<class STapThrobber>& Throbber);

	TSharedPtr<class STapThrobber> TapThrobber;
};
