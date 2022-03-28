#pragma once

#include "CoreMinimal.h"

class TAPCOMMON_API IBridge
{

public:
    virtual void Init() = 0;

public:
    virtual void Register(FString serviceClz, FString serviceImpl) = 0;

public:
    virtual void CallHandler(FString command) = 0;
};

IBridge *GetBridge();