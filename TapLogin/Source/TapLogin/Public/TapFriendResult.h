#pragma once

#include "CoreMinimal.h"
#include "LoginTapFriendInfo.h"
#include "TapFriendResult.generated.h"

USTRUCT(BlueprintType)
struct FTapFriendResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FLoginTapFriendInfo> data;

	UPROPERTY()
	FString cursor;

};