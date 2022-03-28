#pragma once

#include "CoreMinimal.h"
#include "TapUser.h"
#include "TapFriendInfo.generated.h"
#if PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif

USTRUCT(BlueprintType)
struct FTapFriendInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool online;

	UPROPERTY()
	FTapUser tdsUser;

	UPROPERTY()
	FString richPresence;

#if PLATFORM_IOS
	FTapFriendInfo() = default;
	FTapFriendInfo(NSObject *info);
#endif

};
