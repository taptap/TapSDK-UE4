// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapLogin.h"

#define LOCTEXT_NAMESPACE "FTapLoginModule"

void FTapLoginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FTapLoginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTapLoginModule, TapLogin)

FTapLoginModule::FLoginSuccess FTapLoginModule::OnLoginSuccess;
FTapLoginModule::FLoginCancel FTapLoginModule::OnLoginCancel;
FTapLoginModule::FLoginError FTapLoginModule::OnLoginError;
FTapLoginModule::FGetAccessToken FTapLoginModule::OnGetAccessTokenSuccess;
FTapLoginModule::FGetAccessTokenError FTapLoginModule::OnGetAccessTokenError;
FTapLoginModule::FGetTestQualification FTapLoginModule::OnGetTestQualification;
FTapLoginModule::FGetTestQualificationError FTapLoginModule::OnGetTestQualificationError;
FTapLoginModule::FGetProfile FTapLoginModule::OnGetProfileSuccess;
FTapLoginModule::FGetProfileError FTapLoginModule::OnGetProfileError;
FTapLoginModule::FQueryMutualListCallBack FTapLoginModule::OnQueryMutualList;
