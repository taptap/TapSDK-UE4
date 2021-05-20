#pragma once

#include "CoreMinimal.h"
#include "TapUserRelationShip.generated.h"

USTRUCT(BlueprintType)
struct FTapUserRelationShip
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FString userId;

    UPROPERTY()
    FString name;

    UPROPERTY()
    FString avatar;

    UPROPERTY()
    int32 gender;

    UPROPERTY()
    bool mutualAttention;

    UPROPERTY()
    FString relationShip;

};