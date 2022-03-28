// Fill out your copyright notice in the Description page of Project Settings.

#include "TapFriendshipRequest.h"

#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#include "IOSHelper.h"
#import <LeanCloudObjc/LeanCloudObjc.h>
#import <TapFriendSDK/TapFriendSDK.h>

FTapFriendshipRequest::FTapFriendshipRequest(NSObject *request)
{
	if ([request isKindOfClass:[LCFriendshipRequest class]]) {
		LCFriendshipRequest *realRequest = (LCFriendshipRequest *)request;
		
		objectId = IOSHelper::convertString(realRequest.objectId);

		LCUser *User = realRequest[@"friend"];
		NSDictionary *dic = [[User dictionaryForObject] copy];
		friendInfo = IOSHelper::getJson(dic);

		User = realRequest[@"user"];
		dic = [[User dictionaryForObject] copy];
		user = IOSHelper::getJson(dic);
		
		status = realRequest[@"status"];
	}
}
#endif
