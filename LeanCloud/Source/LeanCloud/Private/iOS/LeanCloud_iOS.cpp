#include "LeanCloud_iOS.h"
#include "iOS/LCObjcHelper.h"
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <LeanCloudObjc/Foundation.h>


FLeanCloud_iOS::~FLeanCloud_iOS() {
}

void FLeanCloud_iOS::Init(const FLCConfig& InConfig) {

	[LCApplication setApplicationId:LCObjcHelper::Convert(InConfig.ClientID)
		clientKey:LCObjcHelper::Convert(InConfig.ClientToken)
		serverURLString:LCObjcHelper::Convert(InConfig.ServerURL)];
}
