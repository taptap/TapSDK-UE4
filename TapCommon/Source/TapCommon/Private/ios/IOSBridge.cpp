#include "IOSBridge.h"
#include "TapCommon.h"
#include "Engine.h"

IOSBridge::IOSBridge()
{
}

IOSBridge::~IOSBridge()
{
}

void IOSBridge::Init()
{
}

void IOSBridge::Register(FString clz, FString imp)
{
  
}

void IOSBridge::CallHandler(FString command)
{
    UETDSBridgeCallback *ueCallback = [UETDSBridgeCallback new];
    dispatch_async(dispatch_get_main_queue(), ^{
      [[TDSBridge instance] registerHandler:command.GetNSString() bridgeCallback:ueCallback];
    });
}

@implementation UETDSBridgeCallback

- (void)onResult:(NSString *)result
{
    FString resultMsg = UTF8_TO_TCHAR([result UTF8String]);
    FTapCommonModule::OnBridgeCallback.Broadcast(resultMsg);
}
@end