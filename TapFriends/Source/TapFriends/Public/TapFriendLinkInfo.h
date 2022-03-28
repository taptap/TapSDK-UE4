#pragma once

#include "CoreMinimal.h"
#include "TapFriendLinkInfo.generated.h"

USTRUCT(BlueprintType)
struct FTapFriendLinkInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString queries;

	UPROPERTY()
	FString roleName;

	UPROPERTY()
	FString identity;
	
};
