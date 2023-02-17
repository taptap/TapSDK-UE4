// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapBillboardModule.h"

#include "TapBootstrap.h"
#include "TapBootstrapModule.h"
#if PLATFORM_WINDOWS || PLATFORM_MAC
#include "TapBillboardCommon.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "TapBillboard.h"
#endif

FTapBillboardPtr FTapBillboardModule::GetTapBillboardInterface()
{
	if (FTapBillboardModule* Module = FModuleManager::GetModulePtr<FTapBillboardModule>("TapBillboard"))
	{
		return Module->TapBillboard;
	}
	return nullptr;
}

void FTapBillboardModule::StartupModule()
{
	TapBillboard = MakeShared<FTapBillboard, ESPMode::ThreadSafe>();
#if PLATFORM_WINDOWS || PLATFORM_MAC
	if (TapBillboard)
	{
		TapBillboard->LoadBrowserClass();

		check(FModuleManager::GetModulePtr<FTapBootstrapModule>("TapBootstrap") != nullptr);
		BootstrapInitHandle = FTapBootstrap::OnBootstrapInit.AddRaw(this, &FTapBillboardModule::OnBootstrapInit);
	}
#endif
}

void FTapBillboardModule::ShutdownModule()
{
	FTapBootstrap::OnBootstrapInit.Remove(BootstrapInitHandle);
	TapBillboard.Reset();
}

void FTapBillboardModule::OnBootstrapInit(const FTUConfig& Config)
{
	if (Config.BillboardConfig.IsValid())
	{
		if (TapBillboard)
		{
			TapBillboard->Init(Config);
		}
	}
}

IMPLEMENT_MODULE(FTapBillboardModule, TapBillboard)
