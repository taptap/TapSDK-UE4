// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FTapCommonModule : public IModuleInterface
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

};
