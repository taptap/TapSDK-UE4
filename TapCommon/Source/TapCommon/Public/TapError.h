#pragma once

#include "CoreMinimal.h"
#include "TapError.generated.h"
#if PLATFORM_IOS
#import <Foundation/Foundation.h>
#include "IOSHelper.h"
#endif


USTRUCT()
struct FTapError
{
    GENERATED_BODY()
	
	UPROPERTY()
	int code = 0;

	UPROPERTY()
	FString error_description = TEXT("");

};