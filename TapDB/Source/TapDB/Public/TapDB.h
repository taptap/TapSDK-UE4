// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define TapUEDB_VERSION_NUMBER "31605001"
#define TapUEDB_VERSION "3.16.5"

class FTapDBModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	

};
