#include "TapThirdPartyFriendInfo.h"

#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#include "IOSHelper.h"
#import <TapFriendSDK/TDSFriendInfo.h>
#import <TapFriendSDK/TDSThirdPartyFriend.h>

FTapThirdPartyFriendInfo::FTapThirdPartyFriendInfo(NSObject *info) {
	if ([info isKindOfClass:[TDSThirdPartyFriend class]]) {
		TDSThirdPartyFriend *realInfo = (TDSThirdPartyFriend *)info;
		userId = IOSHelper::convertString(realInfo.userId);
		avatar = IOSHelper::convertString(realInfo.userAvatar);
		userName = IOSHelper::convertString(realInfo.userName);
		userInfo = FTapFriendInfo((NSObject *)realInfo.tdsFriendInfo);
	}
}
#endif