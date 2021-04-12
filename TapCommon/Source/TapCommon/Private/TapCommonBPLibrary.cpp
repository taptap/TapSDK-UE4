// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapCommonBPLibrary.h"
#include "TapCommon.h"
#include "TapBridge.h"
#include "TapJson.h"

#define REGION_CODE_ID "TapCommon_GetRegionCode"

UTapCommonBPLibrary::UTapCommonBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    UE_LOG(LogTemp, Warning, TEXT("init TapCommon"));
    Register(TEXT("com.tds.common.wrapper.TDSCommonService"),TEXT("com.tds.common.wrapper.TDSCommonServiceImpl"));
	FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapCommonBPLibrary::OnBridgeCallback);
#endif
}

void UTapCommonBPLibrary::Register(FString serviceClz, FString serviceImpl)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(serviceClz, serviceImpl);
#endif 
}

void UTapCommonBPLibrary::CallHandler(FString command)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->CallHandler(command);
#endif
}

void UTapCommonBPLibrary::GetRegionCode()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"), TEXT("getRegionCode"), TEXT(""), true, TEXT(REGION_CODE_ID),true);
	GetBridge()->CallHandler(commandJson);
#endif
}

void UTapCommonBPLibrary::OnBridgeCallback(const FString &result)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    UE_LOG(LogTemp,Warning,TEXT("TapCommon OnBridgeCallback:%s"),*result);
    TSharedPtr<FJsonObject> JsonObject = TapJson::Parse(result);
	if(JsonObject!=nullptr)
    {   
        UE_LOG(LogTemp,Warning,TEXT("TapCommon Result Parse Finish"));
        int ResultCode = JsonObject->GetIntegerField(TEXT("code"));
        if(ResultCode !=0){
            UE_LOG(LogTemp,Warning,TEXT("TapCommon Result Error"));
            return;
        }
        FString CallbackId = JsonObject->GetStringField(TEXT("callbackId"));
        FString ContentJson = JsonObject->GetStringField(TEXT("content"));
        UE_LOG(LogTemp,Warning,TEXT("TapCommon Result Parse Success:CalllbackId %s"),*CallbackId);
		if(CallbackId.Equals(REGION_CODE_ID))
		{
			UE_LOG(LogTemp,Warning,TEXT("TapCommon Get RegionCode"));
			FTapCommonRegionCode RegionCodeBean;
			bool JsonParseEnable = FJsonObjectConverter::JsonObjectStringToUStruct<FTapCommonRegionCode>(ContentJson,&RegionCodeBean,0,0);
            if(!JsonParseEnable)
            {
                return;
            }
			FTapCommonModule::OnRegionCode.Broadcast(RegionCodeBean.isMainland);
		}
	}
#endif 
}
