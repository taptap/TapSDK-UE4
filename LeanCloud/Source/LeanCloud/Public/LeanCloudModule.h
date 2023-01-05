// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LeanCloud.h"
#include "Modules/ModuleManager.h"

class FLeanCloudModule : public IModuleInterface
{
public:
protected:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
