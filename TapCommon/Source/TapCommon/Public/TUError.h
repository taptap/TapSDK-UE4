#pragma once

#include "CoreMinimal.h"
#include "TUError.generated.h"
// #if PLATFORM_IOS
// #import <Foundation/Foundation.h>
// #include "IOSHelper.h"
// #endif


USTRUCT()
struct FTUError {
	GENERATED_BODY()

	enum Code {
		// 未知错误
		ERROR_CODE_UNDEFINED = 80000,

		// SDK 未初始化
		ERROR_CODE_UNINITIALIZED = 80001,

		// 绑定取消
		ERROR_CODE_BIND_CANCEL = 80002,

		// 绑定错误
		ERROR_CODE_BIND_ERROR = 80003,

		// 登陆错误
		ERROR_CODE_LOGOUT_INVALID_LOGIN_STATE = 80004,

		// 登陆被踢出
		ERROR_CODE_LOGOUT_KICKED = 80007,

		// 桥接回调错误
		ERROR_CODE_BRIDGE_EXECUTE = 80080,

		// 登录取消
		ERROR_CODE_LOGIN_CANCEL = 80081,

		// 登录取消
		ERROR_CODE_LOGIN_Platforms_Unsupported = 10001,
	};

	UPROPERTY()
	int code = 0;

	UPROPERTY()
	FString error_description = TEXT("");

	UPROPERTY()
	FString msg = TEXT("");

	FString GetErrorReason() const {
		if (!error_description.IsEmpty()) {
			return error_description;
		}
		if (!msg.IsEmpty()) {
			return msg;
		}
		return "No Reason";
	}

	FTUError() = default;

	FTUError(int Code, FString ErrorDescription) : code(Code), error_description(ErrorDescription) {
	};
};
