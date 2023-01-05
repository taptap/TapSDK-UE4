#pragma once

#include "CoreMinimal.h"
#include "TapResult.generated.h"

USTRUCT(BlueprintType)
struct FTapResult
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString callbackId;

    UPROPERTY()
    FString content;

    UPROPERTY()
    FString message;

    UPROPERTY()
    int code = 0;

    UPROPERTY()
    bool onceTime = true;

};