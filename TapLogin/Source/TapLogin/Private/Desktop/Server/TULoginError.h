#pragma once
#include "CoreMinimal.h"
#include "TULoginError.generated.h"

USTRUCT()
struct FTULoginError
{
	GENERATED_BODY()
	UPROPERTY()
	int code = 0;

	UPROPERTY()
	FString msg;

	UPROPERTY()
	FString error;

	UPROPERTY()
	FString error_description;
	
};
