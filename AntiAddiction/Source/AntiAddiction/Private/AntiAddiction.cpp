// Copyright Epic Games, Inc. All Rights Reserved.

#include "AntiAddiction.h"

#include "Slate/TapStyleAntiAddiction.h"

#define LOCTEXT_NAMESPACE "FAntiAddictionModule"

void FAntiAddictionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FTapStyleAntiAddiction::Initialize();
}

void FAntiAddictionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FTapStyleAntiAddiction::Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAntiAddictionModule, AntiAddiction)