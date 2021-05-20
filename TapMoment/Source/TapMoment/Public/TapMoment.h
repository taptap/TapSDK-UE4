// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FTapMomentModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    DECLARE_MULTICAST_DELEGATE_TwoParams(FTapMomentCallback, const int, const FString &);

    UPROPERTY(BlueprintAssignable, Category = "TapMoment")
    static FTapMomentCallback OnMomentCallback;


};
