// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapBootstrapEditor.h"
#include "TapBootstrapSettings.h"

#include "Modules/ModuleManager.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/Class.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "TapBootstrapEditor"

void FTapBootstrapEditorModule::StartupModule()
{
	RegisterSettings();
}

void FTapBootstrapEditorModule::ShutdownModule()
{
	UnregisterSettings();
}

void FTapBootstrapEditorModule::RegisterSettings()
{
	if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "TapTap Bootstrap",
			LOCTEXT("RuntimeSettingsName", "TapTap Bootstrap"),
			LOCTEXT("RuntimeSettingsDescription", "Configure TapTap Bootstrap"),
			GetMutableDefault<UTapBootstrapSettings>());
	}
}

void FTapBootstrapEditorModule::UnregisterSettings()
{
	if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "TapTap Bootstrap");
	}
}

IMPLEMENT_MODULE(FTapBootstrapEditorModule, TapBootstrapEditor);

#undef LOCTEXT_NAMESPACE
