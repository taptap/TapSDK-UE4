#pragma once

#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#import <Foundation/Foundation.h>
#import <TapFriendSDK/TapFriendSDK.h>

@interface TapFriendsNotificationDelegator : NSObject <TDSFriendNotificationDelegate>

@end

FOUNDATION_EXPORT TapFriendsNotificationDelegator *friendDelegator;

#endif