// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FTapLicenseModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


    DECLARE_MULTICAST_DELEGATE(FTapLicenseCallback);

	UPROPERTY(BlueprintAssignable,Category = "TapLicense")
	static FTapLicenseCallback OnLicenseSuccess;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTapLicenseDLCQueryCallback,const int,const FString &);

	UPROPERTY(BlueprintAssignable,Category = "TapLicense")
	static FTapLicenseDLCQueryCallback OnQueryCallback;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTapLicenseOrderCallback,const FString& ,const int);
	
	UPROPERTY(BlueprintAssignable,Category = "TapLicense")
	static FTapLicenseOrderCallback OnOrderCallback;
};
