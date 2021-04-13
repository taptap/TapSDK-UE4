#pragma once
#include "Core.h"
#include "TapBridge.h"
#include "IOSAppDelegate.h"

#import <TapCommonSDK/TapCommonSDK.h>
#import <TapCommonSDK/TDSBridge.h>

class IOSBridge:public IBridge{

public:
    IOSBridge();
    ~IOSBridge();

    void Init();

    void Register(FString serviceClz, FString serviceImpl);
    
    void CallHandler(FString command);
};

@interface UETDSBridgeCallback:NSObject<TDSBridgeCallback>

@end