#pragma once

#include "CoreMinimal.h"
#include "LoginTapFriendInfo.generated.h"

USTRUCT(BlueprintType)
struct  FLoginTapFriendInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString name;

	UPROPERTY()
	FString avatar;

	UPROPERTY()
	FString openid;

};
