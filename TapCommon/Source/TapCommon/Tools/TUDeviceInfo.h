#pragma once

class TAPCOMMON_API TUDeviceInfo
{
public:
	
	// Intel(R) Core(TM) i9-9880H CPU @ 2.30GHz
	static FString GetCPU();
	
	// AMD Radeon Pro 5500M
	static FString GetGPU();

	// ++UE4+Release-4.27-CL-18319896
	static FString GetBuildVersion();

	// 4.27.2-18319896+++UE4+Release-4.27
	static FString GetEngineVersion();

	// 4.27.0-17155196+++UE4+Release-4.27
	static FString GetCompatibleEngineVersion();
	
	//OS: macOS 10.16 (21E258)
	static FString GetOSVersion();

	static int GetScreenWidth();
	
	static int GetScreenHeight();

	// 236caff059420a236f355dab40d051c0
	// 可以当成GetMachineId或者IDFA 如果没有满足需求，那么自己建一个FGuid::NewGuid()，然后存在本地
	static FString GetLoginId();

	// 236caff059420a236f355dab40d051c0
	// 安转应用的ID，每次重装会更新
	static FString GetInstallId();

	// 88:66:5a:28:20:9e
	// static FString GetMacAddress();

	// macOS
	static FString GetPlatform();

	static FString GetProjectName();

	static FString GetDeviceId();
	
	static FString GetIpv4();

	static FString GetIpv6();
};
