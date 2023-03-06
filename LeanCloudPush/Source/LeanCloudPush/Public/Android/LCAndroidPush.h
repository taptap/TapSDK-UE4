#pragma once
#include "LeanCloudPush.h"

class LEANCLOUDPUSH_API FLCAndroidPush : FLeanCloudPush {
public:
	static void RegisterHuaWei();
	static void RegisterHonor();
	static void RegisterOPPO(const FString& AppKey, const FString& AppSecret);
	static void RegisterVIVO();
	static void RegisterMeiZu(const FString& AppId, const FString& AppKey);
	static void RegisterXiaoMi(const FString& AppId, const FString& AppKey);

	static FString GetDeviceName();
};
