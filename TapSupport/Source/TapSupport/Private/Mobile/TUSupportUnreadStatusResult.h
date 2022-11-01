#pragma once
#include "CoreMinimal.h"
#include "TUSupportUnreadStatusResult.generated.h"

USTRUCT(BlueprintType)
struct FTUSupportUnreadStatusResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString errorMsg;

	UPROPERTY()
	uint64 code;

	UPROPERTY()
	uint64 hasUnRead;

	bool HasUnread() {
		return hasUnRead == 1;
	}

	bool IsSuccessful() {
		return code == 0;
	}
};