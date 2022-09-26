#pragma once
#include "CoreMinimal.h"
#include "TUDataStorage.h"
#include "TULoginStorage.generated.h"

USTRUCT()
struct FTULoginStorage
{
	GENERATED_BODY()
	static FString AccessToken;
	static FString Profile;
};

