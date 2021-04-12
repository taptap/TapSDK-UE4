#pragma once

#include "Core.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"
#include "Android/AndroidJNI.h"
#include "TapBridge.h"

class AndroidBridge : public IBridge
{

public:

    AndroidBridge();
    ~AndroidBridge();

    jobject GetJBridge();
    
    void Init();

    void Register(FString serviceClz, FString serviceImpl);

    void CallHandler(FString command);

};