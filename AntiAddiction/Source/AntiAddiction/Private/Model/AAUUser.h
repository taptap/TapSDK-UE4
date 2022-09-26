#pragma once
#include "AAUType.h"
#include "AAUUser.generated.h"



USTRUCT()
struct FAAUUser {
	
	GENERATED_BODY()
	
	UPROPERTY()
	FString UserID;

	UPROPERTY()
	FString AccessToken;

	UPROPERTY()
	EAAUAgeLimit AgeLimit = EAAUAgeLimit::Unknown;


	FAAUUser() = default;
	FAAUUser(const FString& _UserID, const FString& _AccessToken, EAAUAgeLimit _AgeLimit)
		: UserID(_UserID), AccessToken(_AccessToken), AgeLimit(_AgeLimit) {}

private:
	
};

