// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapDBEditor.h"
#include "TapDBSettings.h"

#include "Modules/ModuleManager.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/Class.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "TapDBEditor"

void FTapDBEditorModule::StartupModule()
{
	RegisterSettings();
}

void FTapDBEditorModule::ShutdownModule()
{
	UnregisterSettings();
}

void FTapDBEditorModule::RegisterSettings()
{
	if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "TapTap TapDB",
			LOCTEXT("RuntimeSettingsName", "TapTap TapDB"),
			LOCTEXT("RuntimeSettingsDescription", "Configure TapTap TapDB"),
			GetMutableDefault<UTapDBSettings>());
	}
}

void FTapDBEditorModule::UnregisterSettings()
{
	if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "TapTap TapDB");
	}
}

IMPLEMENT_MODULE(FTapDBEditorModule, TapDBEditor);

#undef LOCTEXT_NAMESPACE
