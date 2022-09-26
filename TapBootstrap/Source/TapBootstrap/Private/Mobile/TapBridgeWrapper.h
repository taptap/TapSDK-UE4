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
struct FTDSUserStatusWrapper
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString wrapper;

    UPROPERTY()
    uint64 userStatusCallbackCode;

};

USTRUCT(BlueprintType)
struct FTDSUserInfoWrapper
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString wrapper;

    UPROPERTY()
    uint64 getUserInfoCode;

};

USTRUCT(BlueprintType)
struct FTDSUserDetailInfoWrapper
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString wrapper;

    UPROPERTY()
    uint64 getUserDetailInfoCode;

};


