#pragma once

#include "CoreMinimal.h"
#include "TapFriendInfo.h"
#include "TapThirdPartyFriendInfo.generated.h"
#if PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif

USTRUCT(BlueprintType)
struct FTapThirdPartyFriendInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString userId;
	
	UPROPERTY()
	FString avatar;
	
	UPROPERTY()
	FString userName;

	UPROPERTY()
	FTapFriendInfo userInfo;

#if PLATFORM_IOS
	FTapThirdPartyFriendInfo() = default;
	FTapThirdPartyFriendInfo(NSObject *info);
#endif
};