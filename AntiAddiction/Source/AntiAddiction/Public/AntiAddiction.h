// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define AntiAddictionUE_VERSION_NUMBER "31501001"
#define AntiAddictionUE_VERSION "3.15.1"

class FAntiAddictionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
