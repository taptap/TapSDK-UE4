#pragma once

#include "CoreMinimal.h"
#include "TapUser.generated.h"

USTRUCT(BlueprintType)
struct FTapUser
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
