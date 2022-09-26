#pragma once
#include "CoreMinimal.h"
#include "TUHttpResponse.h"

class TAPCOMMON_API TUHttpRequest
{
	friend class TUHttpManager;
public:
	TUHttpRequest();
	virtual ~TUHttpRequest() = default;
	// virtual ~TDUHttpRequest();

	enum Type
	{
		Get,	// application/x-www-form-urlencoded
		Post,	// application/json
	};

	enum Form
	{
		Default,
		Json,
	};
	
	FString URL;
	
	TMap<FString, FString> Headers;

	// 参数，get请求会把参数拼接到url后面，post请求会放在body里。
	TSharedPtr<FJsonObject> Parameters;

	// Url中Path的参数，会替换URL中{}的内容。
	TMap<FString, FString> PathParameters;

	// post请求把参数拼在url后面，明明post请求可以把参数放在body里的。。特殊改的需求
	TSharedPtr<FJsonObject> PostUrlParameters;

	// post请求自己生成Body，TSharedPtr<FJsonObject>竟然不支持[[1,2,3]]格式。。。后面得换json object
	FString PostBodyString;

	int RepeatCount = 1;

	float TimeoutSecs = 10;
	
	Type Type = Get;

	Form Form = Default;

	DECLARE_DELEGATE_OneParam(CompletedBlock, TSharedPtr<TUHttpResponse>);
	CompletedBlock onCompleted;

	FString GetFinalUrl();

	// 请求的时候，是否不加入CommonHeaders和CommonParameters。
	bool isPure = false; 
	virtual TMap<FString, FString> CommonHeaders();
	virtual TSharedPtr<FJsonObject> CommonParameters();

	// virtual void DoSomeingAfterCombinHeadersAndParas(){  };
	// 真正的request已经配置完，这里给个时机重新设置一下
	virtual bool ResetHeadersBeforeRequest(){ return false; };
	// virtual bool ResetParametersBeforeRequest(){ return false; };

private:
	int tryCount = 0;
	FString FinalURL;
};
