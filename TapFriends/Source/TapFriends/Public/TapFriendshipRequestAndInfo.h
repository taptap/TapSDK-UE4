// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "TapFriendshipRequest.h"
#include "TapFriendInfo.h"
#include "TapFriendshipRequestAndInfo.generated.h"

USTRUCT(BlueprintType)
struct FTapFriendshipRequestAndInfo
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FTapFriendshipRequest request;

	UPROPERTY()
	FTapFriendInfo user;

#if PLATFORM_IOS
	FTapFriendshipRequestAndInfo() = default;
	FTapFriendshipRequestAndInfo(NSObject *infoRequest);
#endif
};
