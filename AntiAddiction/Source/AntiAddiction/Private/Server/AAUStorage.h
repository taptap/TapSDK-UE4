#pragma once
#include "CoreMinimal.h"
#include "TUDataStorage.h"
#include "AAUStorage.generated.h"

USTRUCT()
struct FAAUStorage
{
	GENERATED_BODY()
	
	static FString AAConfigFile;
	static FString SubmitCardName;
	static FString SubmitCardID;
	static FString HasLoginedUser; //登录过的用户
	static FString LastLoginTime;
	static FString LastCostTime;

	static FString AAVienamConfigFile;
	static FString VienamHasLoginedUser;
};

