#include "iOS/LCIOSPush.h"
#include "IOS/IOSAppDelegate.h"
#include "Tools/LCDebuger.h"
#include "iOS/LCObjcHelper.h"

#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <objc/runtime.h>
#import <LeanCloudObjc/LeanCloudObjc.h>

static NSData *NotificationDeviceToken = nil;
static NSString *TeamID = nil;

void InitPushToken() {
	if (NotificationDeviceToken == nil) {
		return;
	}
	if (TeamID.length <= 0) {
		return;
	}
	[[LCInstallation defaultInstallation] setDeviceTokenFromData:NotificationDeviceToken teamId:TeamID];
	[[LCInstallation defaultInstallation] saveInBackground];
	TeamID = nil;
    
}

void FLCIOSPush::Register(const FString& InTeamID) {
    
    TeamID = LCObjcHelper::Convert(InTeamID);
    InitPushToken();
    LCDebuger::DisplayLog("hyf----------FLCIOSPush::Register: " + InTeamID);
// #if !PLATFORM_TVOS && !NOTIFICATIONS_ENABLED
    dispatch_async(dispatch_get_main_queue(), ^{
        NSLog(@"hyf----------开始注册通知");
    UNUserNotificationCenter *Center = [UNUserNotificationCenter currentNotificationCenter];
    [Center requestAuthorizationWithOptions:(UNAuthorizationOptionBadge | UNAuthorizationOptionSound | UNAuthorizationOptionAlert)
                completionHandler:^(BOOL granted, NSError * _Nullable error) {
                    NSLog(@"hyf----------注册通知回调：%@, %@", @(granted), error);
                  if (error)
                  {
                      LCDebuger::WarningLog(TEXT("Failed to register for notifications."));
                  }
                  else
                  {
                    int32 types = (int32)granted;
                                      if (granted)
                                      {
                                          UIApplication* application = [UIApplication sharedApplication];
                                          [application registerForRemoteNotifications];
                                          
                                      }
                    FFunctionGraphTask::CreateAndDispatchWhenReady([types]()
                                           {
                                             FCoreDelegates::ApplicationRegisteredForUserNotificationsDelegate.Broadcast(types);
                                           }, TStatId(), NULL, ENamedThreads::GameThread);
                    
                  }
                }];
    });
// #endif

}

// void FLCIOSPush::SubscribeChannel(const FString& ChannelName) {
//     NSString *channelName = LCObjcHelper::Convert(ChannelName);
//     [[LCInstallation defaultInstallation] addUniqueObject:channelName forKey:@"channels"];
//     [[LCInstallation defaultInstallation] saveInBackground];
// }
//
// void FLCIOSPush::UnsubscribeChannel(const FString& ChannelName) {
//     NSString *channelName = LCObjcHelper::Convert(ChannelName);
//     [[LCInstallation defaultInstallation] removeObject:channelName forKey:@"channels"];
//     [[LCInstallation defaultInstallation] saveInBackground];
// }
//
// void FLCIOSPush::PushUserInfo(NSDictionary* userInfo) {
//     NSLog(@"hyf----------FLCIOSPush::PushUserInfo: %@", userInfo);
//     if (userInfo) {
//         PushContent(LCObjcHelper::Convert(userInfo));
//     }
// }



@interface IOSAppDelegate (LeanCloud)

@end

static NSMutableSet *MissMethod;

@implementation IOSAppDelegate (LeanCloud)

+ (void)swizzleInstanceMethodWithOriginSelector:(SEL)originSelector swizzledSelector:(SEL)swizzledSelector {
    Method originalMethod = class_getInstanceMethod(self, originSelector);
    Method swizzledMethod = class_getInstanceMethod(self, swizzledSelector);
    if (originalMethod && swizzledMethod) {
        method_exchangeImplementations(originalMethod, swizzledMethod);
    } else if (originalMethod) {
        [MissMethod addObject:NSStringFromSelector(swizzledSelector)];
        class_addMethod(self, swizzledSelector, method_getImplementation(originalMethod), method_getTypeEncoding(originalMethod));
    } else if (swizzledMethod) {
        [MissMethod addObject:NSStringFromSelector(originSelector)];
        class_addMethod(self, originSelector, method_getImplementation(swizzledMethod), method_getTypeEncoding(swizzledMethod));
    } else {
        return;
    }
}


+ (void)load {
    MissMethod = [[NSMutableSet alloc] initWithCapacity:4];
    [self swizzleInstanceMethodWithOriginSelector:@selector(application:didFinishLaunchingWithOptions:) swizzledSelector:@selector(leancould_application:didFinishLaunchingWithOptions:)];
    [self swizzleInstanceMethodWithOriginSelector:@selector(application:didRegisterForRemoteNotificationsWithDeviceToken:) swizzledSelector:@selector(leancould_application:didRegisterForRemoteNotificationsWithDeviceToken:)];
    // [self swizzleInstanceMethodWithOriginSelector:@selector(leancould_userNotificationCenter:willPresentNotification:withCompletionHandler:) swizzledSelector:@selector(userNotificationCenter:willPresentNotification:withCompletionHandler:)];
    // [self swizzleInstanceMethodWithOriginSelector:@selector(leancould_userNotificationCenter:didReceiveNotificationResponse:withCompletionHandler:) swizzledSelector:@selector(userNotificationCenter:didReceiveNotificationResponse:withCompletionHandler:)];
    
}


- (BOOL)leancould_application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // FLCIOSPush::PushUserInfo(launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey]);
    [self leancould_application:application didFinishLaunchingWithOptions:launchOptions];
    NSLog(@"hyf----------leancould_application");
    return YES;
}


- (void)leancould_application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    NSLog(@"hyf----------didRegisterForRemoteNotificationsWithDeviceToken");
    NotificationDeviceToken = deviceToken;
    InitPushToken();
    if (![MissMethod containsObject:NSStringFromSelector(@selector(application:didRegisterForRemoteNotificationsWithDeviceToken:))]) {
        [self leancould_application:application didRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
    }
    NSUInteger dataLength = deviceToken.length;
    if (dataLength == 0) {
        return;
    }
    const unsigned char *dataBuffer = (const unsigned char *)deviceToken.bytes;
    NSMutableString *hexTokenString  = [NSMutableString stringWithCapacity:(dataLength * 2)];
    for (int i = 0; i < dataLength; ++i) {
        [hexTokenString appendFormat:@"%02x", dataBuffer[i]];
    }
    NSLog(@"push DeviceToken token:%@", hexTokenString);
}

// - (void)leancould_userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler {
//     NSLog(@"hyf----------willPresentNotification");
//     if([notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]){
//         FLCIOSPush::PushUserInfo(notification.request.content.userInfo);
//     }else{
//         //应用处于前台时的本地推送接受
//     }
//     if (![MissMethod containsObject:NSStringFromSelector(@selector(userNotificationCenter:willPresentNotification:withCompletionHandler:))]) {
//         [self leancould_userNotificationCenter:center willPresentNotification:notification withCompletionHandler:completionHandler];
//     }
// }
//
// - (void)leancould_userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)())completionHandler {
//     NSLog(@"hyf----------didReceiveNotificationResponse");
//     if([response.notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]){
//         FLCIOSPush::PushUserInfo(response.notification.request.content.userInfo);
//     }else{
//         //应用处于后台时的本地推送接受
//     }
//     if (![MissMethod containsObject:NSStringFromSelector(@selector(userNotificationCenter:didReceiveNotificationResponse:withCompletionHandler:))]) {
//         [self leancould_userNotificationCenter:center didReceiveNotificationResponse:response withCompletionHandler:completionHandler];
//     }
// }

@end


