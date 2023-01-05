#pragma once
#include "CoreMinimal.h"
#include "AAUSettingsModel.generated.h"


USTRUCT()
struct FAAUSettingsModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool stand_alone_mode = false;

};
