#pragma once
#include "CoreMinimal.h"
// #include "TDUHttpRequest.h"

DEFINE_LOG_CATEGORY_STATIC(TDUHttpLog, Log, All);

class TUHttpRequest;
class TAPCOMMON_API TUHttpResponse
{
	
public:
	// ~TDUHttpResponse();
	enum State
	{
		success,      // 200 - 206
		clientError,  // other
		serverError,  // >= 500
		networkError, // connection error
	};

	State state = clientError;

	int32 httpCode;

	// TArray<uint8> content;
	FString contentString;
	/**
	 * Return all headers in an array in "Name: Value" format.
	 *
	 * @return the header array of strings
	 */
	TArray<FString> headers;

	TSharedPtr<TUHttpRequest> request;

	FString GenerateDebugString();
};
