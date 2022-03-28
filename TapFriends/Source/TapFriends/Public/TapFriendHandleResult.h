#pragma once

#include "CoreMinimal.h"
#include "TapFriendHandleResult.generated.h"

USTRUCT(BlueprintType)
struct FTapFriendHandleResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int64 code = 0;

	UPROPERTY()
	FString message = "";

};
