#pragma once

#include "CoreMinimal.h"
#include "TDSUser.generated.h"
#if PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif

USTRUCT(BlueprintType)
struct FTDSUser
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString uuid;

	UPROPERTY()
	FString userName;

	UPROPERTY()
	FString email;

	UPROPERTY()
	FString phoneNumber;

	UPROPERTY()
	FString sessionToken;

	UPROPERTY()
	bool isAnonymous;

	UPROPERTY()
	FString avatar;

	UPROPERTY()
	FString nickName;

	UPROPERTY()
	FString shortId;

	UPROPERTY()
	FString serverData;

#if PLATFORM_IOS
	FTDSUser() = default;
	FTDSUser(NSObject *user);
#endif
};
