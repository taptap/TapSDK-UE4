#pragma once

#include "CoreMinimal.h"
#include "TapUserDetail.generated.h"


USTRUCT(BlueprintType)
struct FTapUserCenterEntry
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    bool isMomentEnable;
};

USTRUCT(BlueprintType)
struct FTapUserDetail
{
    GENERATED_USTRUCT_BODY()

 UPROPERTY()
    FString uuid;

    UPROPERTY()
    FString userName;

    UPROPERTY()
    FString email;

    UPROPERTY()
    FString phoneNumber;

    UPROPERTY()
    FString sessionToken;

    UPROPERTY()
    bool isAnonymous;

};

