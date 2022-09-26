#pragma once

class TAPCOMMON_API TUDebuger
{
public:

	static void AddReplacedHostPair(const FString& OriginHost, const FString& ReplacedHost);
	static void RemoveReplacedHostPair(const FString& OriginHost);
	static void ClearAllReplacedHostPairs();
	
	static FString GetReplacedHost(const FString& OriginHost);
	static FString GetReplacedUrl(const FString& OriginUrl); // 这个会替换url中的host；
	
	static bool IsTest;
	
	static void DisplayLog(const FString& Info);
	static void WarningLog(const FString& Info);
	static void ErrorLog(const FString& Info);

	static void DisplayShow(const FString& Info, float Duration = 3.f);
	static void WarningShow(const FString& Info, float Duration = 3.f);
	static void ErrorShow(const FString& Info, float Duration = 3.f);

private:
	static TMap<FString, FString> ReplaceHosts;
};
