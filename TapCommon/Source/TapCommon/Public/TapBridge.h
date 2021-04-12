#pragma once

#include "CoreMinimal.h"

class IBridge
{

public:
    virtual void Init();

public:
    virtual void Register(FString serviceClz, FString serviceImpl) = 0;

public:
    virtual void CallHandler(FString command) = 0;
};

IBridge *GetBridge();