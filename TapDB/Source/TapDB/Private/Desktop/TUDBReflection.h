// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TUType.h"
#include "UObject/Object.h"
#include "TUDBReflection.generated.h"

/**
 * 
 */
UCLASS()
class TAPDB_API UTUDBReflection : public UObject {
	GENERATED_BODY()

	UFUNCTION()
	static void Init();
};
