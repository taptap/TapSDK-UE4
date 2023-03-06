// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
struct FTUConfig;
typedef TSharedPtr<class FTapBillboardCommon, ESPMode::ThreadSafe> FTapBillboardPtr;

#define TapBillboard_VERSION_NUMBER "31801001"
#define TapBillboard_VERSION "3.18.1"

class TAPBILLBOARD_API FTapBillboardModule : public IModuleInterface
{
public:
	/**
	 * Get tap billboard interface
	 * @Return Tap billboard interface singleton
	 */
	static FTapBillboardPtr GetTapBillboardInterface();
	
protected:
	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;

	void OnBootstrapInit(const FTUConfig& Config);

	FTapBillboardPtr TapBillboard;

	FDelegateHandle BootstrapInitHandle;
};
