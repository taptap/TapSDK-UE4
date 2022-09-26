// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TULoginReflection.generated.h"

/**
 * 
 */
UCLASS()
class TAPLOGIN_API UTULoginReflection : public UObject
{
	GENERATED_BODY()

	UFUNCTION()
	static FString GetOpenID();
};
