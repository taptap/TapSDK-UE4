// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define AntiAddictionUE_VERSION_NUMBER "31602001"
#define AntiAddictionUE_VERSION "3.16.2"

class FAntiAddictionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
