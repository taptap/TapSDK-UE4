#pragma once

#include "CoreMinimal.h"
#include "TapBridgeWrapper.generated.h"

USTRUCT(BlueprintType)
struct FTapLoginWrapper
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString wrapper;

    UPROPERTY()
    uint64 loginCallbackCode;

};

USTRUCT(BlueprintType)
struct FTapUserStatusWrapper
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString wrapper;

    UPROPERTY()
    uint64 userStatusCallbackCode;

};

USTRUCT(BlueprintType)
struct FTapUserInfoWrapper
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString wrapper;

    UPROPERTY()
    uint64 getUserInfoCode;

};

USTRUCT(BlueprintType)
struct FTapUserDetailInfoWrapper
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString wrapper;

    UPROPERTY()
    uint64 getUserDetailInfoCode;

};


