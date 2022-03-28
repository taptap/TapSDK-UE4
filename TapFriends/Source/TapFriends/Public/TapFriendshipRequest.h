// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapFriendshipRequest.generated.h"

USTRUCT(BlueprintType)
struct FTapFriendshipRequest
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString objectId;
	
	UPROPERTY()
	FString friendInfo;

	UPROPERTY()
	FString user;

	UPROPERTY()
	FString status;

#if PLATFORM_IOS
	FTapFriendshipRequest() = default;
	FTapFriendshipRequest(NSObject *request);
#endif
	
};