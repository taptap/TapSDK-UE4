#pragma once

#include "CoreMinimal.h"
#include "TapCommand.generated.h"

USTRUCT(BlueprintType)
struct FTapCommand
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString service;

    UPROPERTY()
    FString method;

    UPROPERTY()
    FString args;

    UPROPERTY()
    bool callback;

    UPROPERTY()
    bool onceTime;

    UPROPERTY()
    FString callbackId;

};
