// Fill out your copyright notice in the Description page of Project Settings.


#include "TapFriendshipRequestAndInfo.h"

#if PLATFORM_IOS
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#include "IOSHelper.h"
#import <LeanCloudObjc/LeanCloudObjc.h>
#import <TapFriendSDK/TDSFriendInfo.h>


FTapFriendshipRequestAndInfo::FTapFriendshipRequestAndInfo(NSObject *infoRequest)
{
	if ([infoRequest isKindOfClass:[TDSFriendshipRequest class]]) {
		TDSFriendshipRequest *realRequest = (TDSFriendshipRequest *)infoRequest;
		
		request = FTapFriendshipRequest((NSObject *)realRequest.lcFriendshipRequest);
		
		user = FTapFriendInfo((NSObject *)realRequest.friendInfo);
	}
}
#endif
