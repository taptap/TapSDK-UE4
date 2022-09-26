#pragma once
#include "AAUType.h"

class AAUHelper {
public:
	static bool ValidateCardID(const FString& CardID);
	static int GetAge(const FString& CardID); // 调用前，先调用ValidateCardID

	// 拿服务时间与本地时间的差值，如果差值小于1分钟，那么认为本地时间是可信的，直接返回0；
	static void GetTimeSpanWithSeverTime(TFunction<void(int64 TimeSpan)> CallBack);

	// FDateTime对象本身不带时区的概念。
	static FDateTime GetChinaCurrentTime();
	static FDateTime GetVietnamCurrentTime();

	static TArray<int> ParseCurfewTimeString(const FString& ServerTime);

	// 距离宵禁的时长，0表示在宵禁
	static int64 IntervalForCurfew(FDateTime ChinaTime);

	// 
	static bool IsHoliday(FDateTime ChinaTime);

	static FString RemoveHtmlTag(const FString& Content);

	static FString ReplaceHtmlTag(FString Content);

	static EAAUAgeLimit MakeAgeLimit(int64 Age);
};
