#pragma once

#include "CoreMinimal.h"
#include "FLoginWrapper.generated.h"

USTRUCT(BlueprintType)
struct FLoginWrapper
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString wrapper;

	UPROPERTY()
	uint64 loginCallbackCode = 0;
};