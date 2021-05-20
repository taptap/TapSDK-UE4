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
    FString user_id;

    UPROPERTY()
    FString name;

    UPROPERTY()
    FString avatar;

    UPROPERTY()
    FString taptap_user_id;

    UPROPERTY()
    bool isGuest;

    UPROPERTY()
    uint64 gender;

    UPROPERTY()
    FTapUserCenterEntry entry;

};

