// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapCommon.h"

#include "Slate/STapThrobber.h"
#include "Slate/TapStyleCommon.h"

#define LOCTEXT_NAMESPACE "FTapCommonModule"

void FTapCommonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FTapStyleCommon::Initialize();
}

void FTapCommonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FTapStyleCommon::Shutdown();
}



void FTapCommonModule::TapThrobberShowWait()
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	if (!Module.TapThrobber)
	{
		SAssignNew(Module.TapThrobber, STapThrobber);
		if (Module.TapThrobber && GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->AddViewportWidgetContent(Module.TapThrobber.ToSharedRef(), MAX_int16);
		}
	}
}

void FTapCommonModule::TapThrobberShowWaitAndToast(const FString& Toast)
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	const FText NewContent = FText::FromString(Toast);
	if (Module.TapThrobber)
	{
		Module.TapThrobber->UpdateContent(NewContent);
	}
	else
	{
		SAssignNew(Module.TapThrobber, STapThrobber)
		.Content(NewContent);
		if (Module.TapThrobber && GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->AddViewportWidgetContent(Module.TapThrobber.ToSharedRef(), MAX_int16);
		}
	}
}

void FTapCommonModule::TapThrobberDismiss()
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	if (Module.TapThrobber && GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(Module.TapThrobber.ToSharedRef());
		Module.TapThrobber.Reset();
	}
}

void FTapCommonModule::TapThrobberShowToast(const FString& Toast, float TimeInterval)
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	const FText NewContent = FText::FromString(Toast);
	if (Module.TapThrobber)
	{
		Module.TapThrobber->ShowThrobber(false);
		Module.TapThrobber->UpdateContent(NewContent);
		Module.TapThrobber->UpdateTimeInterval(TimeInterval);
		Module.TapThrobber->UpdateRemoveSelfDelegate(FOnTapThrobberRemoveSelf::CreateStatic(&FTapCommonModule::OnTapThrobberRemoveSelf));
	}
	else
	{
		SAssignNew(Module.TapThrobber, STapThrobber)
		.HasThrobber(false)
		.Content(NewContent)
		.TimeInterval(TimeInterval)
		.OnRemoveSelf(FOnTapThrobberRemoveSelf::CreateStatic(&FTapCommonModule::OnTapThrobberRemoveSelf));
		if (Module.TapThrobber && GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->AddViewportWidgetContent(Module.TapThrobber.ToSharedRef(), MAX_int16);
		}
	}
}

void FTapCommonModule::OnTapThrobberRemoveSelf(const TSharedRef<STapThrobber>& Throbber)
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	Module.TapThrobber.Reset();
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