// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define TapUEDB_VERSION_NUMBER "31801001"
#define TapUEDB_VERSION "3.18.1"

class FTapDBModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	

};
