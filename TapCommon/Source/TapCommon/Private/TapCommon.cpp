// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapCommon.h"

#define LOCTEXT_NAMESPACE "FTapCommonModule"

void FTapCommonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FTapCommonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTapCommonModule, TapCommon)

FTapCommonModule::FBridgeCallback FTapCommonModule::OnBridgeCallback;
FTapCommonModule::FGetRegionCode FTapCommonModule::OnRegionCode;
FTapCommonModule::FIsTapTapInstalled FTapCommonModule::OnTapTapInstalled;
FTapCommonModule::FIsTapGlobalInstalled FTapCommonModule::OnTapGlobalInstalled;
FTapCommonModule::FUpdateGameInTapTap FTapCommonModule::OnUpdateGameInTapTap;
FTapCommonModule::FUpdateGameInTapGlobal FTapCommonModule::OnUpdateGameInTapGlobal;
FTapCommonModule::FOpenReviewInTapGlobal FTapCommonModule::OnOpenReviewInTapGlobal;
FTapCommonModule::FOpenReviewInTapTap FTapCommonModule::OnOpenReviewInTapTap;