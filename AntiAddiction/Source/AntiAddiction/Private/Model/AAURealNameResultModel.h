#pragma once
#include "CoreMinimal.h"
#include "AAUType.h"
#include "AAURealNameResultModel.generated.h"


USTRUCT()
struct FAAURealNameResultModel
{
	enum AuthState {
		Success = 0,                     // 认证成功
		Verifying = 1,                   // 认证中
		Fail = 2,                        // 认证失败
	};

	GENERATED_BODY()
	
	UPROPERTY()
	int status;

	UPROPERTY()
	int age_limit;

	UPROPERTY()
	FString anti_addiction_token;

	UPROPERTY()
	bool has_auth_record;

	AuthState GetAuthState() {
		if (status == Success) {
			return Success;
		} else if (status == Verifying) {
			return Verifying;
		} else if (status == Fail) {
			return Fail;
		} else {
			return Success;
		}
	}
	
};
