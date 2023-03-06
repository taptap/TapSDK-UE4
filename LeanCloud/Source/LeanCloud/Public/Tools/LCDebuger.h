#pragma once

class LEANCLOUD_API LCDebuger {
public:
	static void DisplayLog(const FString& Info);
	static void WarningLog(const FString& Info);
	static void ErrorLog(const FString& Info);

	static void DisplayShow(const FString& Info, float Duration = 3.f);
	static void WarningShow(const FString& Info, float Duration = 3.f);
	static void ErrorShow(const FString& Info, float Duration = 3.f);
};
