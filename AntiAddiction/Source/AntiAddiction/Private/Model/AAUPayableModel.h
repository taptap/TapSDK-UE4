#pragma once
#include "CoreMinimal.h"
#include "AAUPayableModel.generated.h"


USTRUCT()
struct FAAUPayableModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool status = false; 
	
	UPROPERTY()
	FString title; 

	UPROPERTY()
	FString description;

	UPROPERTY()
	FString message;
	
};


USTRUCT()
struct FAAUPaymentModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString message;
};