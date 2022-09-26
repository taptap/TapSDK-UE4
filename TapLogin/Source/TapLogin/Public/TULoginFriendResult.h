#pragma once

#include "CoreMinimal.h"
#include "TULoginFriendResult.generated.h"

USTRUCT(BlueprintType)
struct FTULoginFriendInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString name;

	UPROPERTY()
	FString avatar;

	UPROPERTY()
	FString openid;

};

USTRUCT(BlueprintType)
struct FTULoginFriendResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FTULoginFriendInfo> data;

	UPROPERTY()
	FString cursor;

};