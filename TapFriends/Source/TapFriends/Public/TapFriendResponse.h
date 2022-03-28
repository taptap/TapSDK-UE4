#pragma once

#include "CoreMinimal.h"
#include "TapFriendResponse.generated.h"

USTRUCT(BlueprintType)
struct FTapFriendResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString content;

	UPROPERTY()
	bool success;

	UPROPERTY()
	int64 code;
	
};
