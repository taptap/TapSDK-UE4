#pragma once

#include "CoreMinimal.h"
#include "TapUser.generated.h"

USTRUCT(BlueprintType)
struct FTapUser
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

};
