#include "TapUser.h"


#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#import <TapBootstrapSDK/TDSUser.h>
#include "IOSHelper.h"

FTapUser::FTapUser(NSObject *user) {
	if ([user isKindOfClass:[TDSUser class]]) {
		TDSUser *realUser = (TDSUser *)user;
		uuid = IOSHelper::convertString(realUser.objectId);
		userName = IOSHelper::convertString(realUser.username);
		email = IOSHelper::convertString(realUser.email);
		phoneNumber = IOSHelper::convertString(realUser.mobilePhoneNumber);
		sessionToken = IOSHelper::convertString(realUser.sessionToken);
		avatar = IOSHelper::convertString(realUser[@"avatar"]);
		nickName = IOSHelper::convertString(realUser[@"nickname"]);
		shortId = IOSHelper::convertString(realUser[@"shortId"]);
		isAnonymous = [realUser isAnonymous];
		NSDictionary *dic = [[realUser dictionaryForObject] copy];
		serverData = IOSHelper::getJson(dic);
	}
}
#endif