// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define TapUEBootstrap_VERSION_NUMBER "31700001"
#define TapUEBootstrap_VERSION "3.17.0"


class TAPBOOTSTRAP_API FTapBootstrapModule : public IModuleInterface
{
public:
	
protected:
    virtual void StartupModule() override;
    
    virtual void ShutdownModule() override;

};

