#pragma once

#include "CoreMinimal.h"
#include "FriendWrapper.generated.h"

USTRUCT(BlueprintType)
struct FFriendWrapper
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString content;

	UPROPERTY()
	bool success = false;

	UPROPERTY()
	int64 code = 0;
	
};