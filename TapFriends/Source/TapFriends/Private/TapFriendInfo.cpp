#include "TapFriendInfo.h"

#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#include "IOSHelper.h"
#import <TapFriendSDK/TDSFriendInfo.h>

FTapFriendInfo::FTapFriendInfo(NSObject *info) {
	if ([info isKindOfClass:[TDSFriendInfo class]]) {
		TDSFriendInfo *realInfo = (TDSFriendInfo *)info;
		online = realInfo.online;
		tdsUser = FTapUser((NSObject *)realInfo.user);
		richPresence = IOSHelper::getJson(realInfo.richPresence);
	}
}
#endif