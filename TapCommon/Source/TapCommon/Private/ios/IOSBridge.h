#pragma once
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"

#include "Core.h"
#include "TapBridge.h"
#include "IOSAppDelegate.h"

#import <TapCommonSDK/TDSBridge.h>
#import <TapCommonSDK/TapCommonSDK.h>

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