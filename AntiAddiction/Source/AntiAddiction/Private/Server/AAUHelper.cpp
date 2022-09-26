#include "AAUHelper.h"

#include "AAUNet.h"
#include "TUCrypto.h"
#include "TUDebuger.h"

bool AAUHelper::ValidateCardID(const FString& CardID) {

	const int weight[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
	const char validate[] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};
	int sum = 0;
	int mode = 0;
	TArray<uint8> CardIDData = TUCrypto::UTF8Encode(CardID);
	const int CardIDCount = 18;
	
	if (CardIDData.Num() != CardIDCount) {
		return false;
	}

	if (CardIDData[10] < '0' || CardIDData[10] > '1') {
		return false;
	}

	if (CardIDData[12] < '0' || CardIDData[12] > '3') {
		return false;
	}
	
	for (int i = 0; i < CardIDCount - 1; i++) {
		if (CardIDData[i] < '0' || CardIDData[i] > '9') {
			return false;
		}
		sum = sum + (CardIDData[i] - '0') * weight[i];
	}
	mode = sum % 11;
	return validate[mode] == CardIDData[17];
}

int AAUHelper::GetAge(const FString& CardID) {
	
	int32 BirthYear = FCString::Atoi(*CardID.Mid(6, 4));
	int32 BirthMonth = FCString::Atoi(*CardID.Mid(10, 2));
	int32 BirthDay = FCString::Atoi(*CardID.Mid(12, 2));

	FDateTime DateTime = FDateTime::Now();
	int32 CurrentYear = DateTime.GetYear();
	int32 CurrentMonth = DateTime.GetMonth();
	int32 CurrentDay = DateTime.GetDay();

	int Age = CurrentYear - BirthYear;
	//如果当前日月<出生日月
	if ((BirthMonth > CurrentMonth) || (BirthMonth == CurrentMonth && BirthDay > CurrentDay)){
		Age -= 1;
	}
	return Age;
}

void AAUHelper::GetTimeSpanWithSeverTime(TFunction<void(int64 TimeSpan)> CallBack) {
	if (CallBack == nullptr) {
		return;
	}
	AAUNet::GetServerTime([=](TSharedPtr<FAAUServerTimeModel> ModelPtr, const FTUError& Error) {
		int64 LocalTimestamp = FDateTime::UtcNow().ToUnixTimestamp();
		if (ModelPtr.IsValid() && ModelPtr->timestamp > 0 &&
			FMath::Abs(ModelPtr->timestamp - LocalTimestamp) > 60) {
			CallBack(ModelPtr->timestamp - LocalTimestamp);
			return;
		}
		CallBack(0);
	});
}

FDateTime AAUHelper::GetChinaCurrentTime() {
	auto Time = FDateTime::UtcNow();
	Time += 8 * ETimespan::TicksPerHour; // 东八区
	return Time;
}

FDateTime AAUHelper::GetVietnamCurrentTime() {
	auto Time = FDateTime::UtcNow();
	Time += 7 * ETimespan::TicksPerHour; // 东七区
	return Time;
}

TArray<int> AAUHelper::ParseCurfewTimeString(const FString& ServerTime) {
	TArray<FString> OutArray;
	ServerTime.ParseIntoArray(OutArray, TEXT(":"));
	TArray<int> TimeArray;
	for (auto Str : OutArray) {
		TimeArray.Add(FCString::Atoi(*Str));
	}
	return TimeArray;
}

int64 AAUHelper::IntervalForCurfew(FDateTime ChinaTime) {
	// 如果配置出错了，是能一直玩还是不能玩
	int64 RetainSeconds = INT64_MAX;
	// int64 RetainSeconds = 0;
	int64 NowMins = ChinaTime.GetHour() * 60 + ChinaTime.GetMinute();
	auto ConfigModel = FAAUChinaConfigModel::GetLocalModel();
	auto Start = ParseCurfewTimeString(ConfigModel->child_protected_config.night_strict_start);
	if (Start.Num() != 2) {
		return RetainSeconds;
	}
	int64 StartMins = Start[0] * 60 + Start[1];
	auto End = ParseCurfewTimeString(ConfigModel->child_protected_config.night_strict_end);
	if (End.Num() != 2) {
		return RetainSeconds;
	}
	int64 EndMins = End[0] * 60 + End[1];
	// TUDebuger::DisplayLog(FString::Printf(TEXT("niu: %lld-%lld-%lld-%d"), NowMins, StartMins, EndMins, ChinaTime.GetSecond()));
	if (StartMins > EndMins) {
		if (NowMins >= EndMins && NowMins < StartMins) {
			return (StartMins - NowMins) * 60 - ChinaTime.GetSecond();
		} else {
			return 0;
		}
	} else if (StartMins < EndMins) {
		if (NowMins >= StartMins && NowMins < EndMins) {
			return 0;
		} else {
			return (StartMins - NowMins) * 60 - ChinaTime.GetSecond();
		}
	} else {
		return RetainSeconds;
	}
}

bool AAUHelper::IsHoliday(FDateTime ChinaTime) {
	FString TodayStr = FString::Printf(TEXT("%02i.%02i"), ChinaTime.GetMonth(), ChinaTime.GetDay());
	for (auto Holiday : FAAUChinaConfigModel::GetLocalModel()->holiday) {
		if (TodayStr == Holiday) {
			return true;
		}
	}
	return false;
}

FString AAUHelper::RemoveHtmlTag(const FString& Content) {
	FString PatternString(TEXT("(<.*?>)"));
	FRegexPattern Pattern(PatternString);
	FRegexMatcher Matcher(Pattern, Content);
	TArray<int32> Indexs;
	while (Matcher.FindNext()) {
		Indexs.Add(Matcher.GetMatchBeginning());
		Indexs.Add(Matcher.GetMatchEnding());
		TUDebuger::DisplayLog(Matcher.GetCaptureGroup(0));
		TUDebuger::DisplayLog(FString::Printf(TEXT("%i-%i"), Matcher.GetMatchBeginning(), Matcher.GetMatchEnding()));
	}
	FString Result = Content;
	int Tail = Indexs.Num();
	while (Tail > 0) {
		Result.RemoveAt(Indexs[Tail-2], Indexs[Tail-1] - Indexs[Tail-2], true);
		Tail -= 2;
	}
	return Result;
	
}

FString AAUHelper::ReplaceHtmlTag(FString Content) {
	Content.ReplaceInline(TEXT("<span color=\"#888888\">"), TEXT(""));
	Content.ReplaceInline(TEXT("</span>"), TEXT(""));
	Content.ReplaceInline(TEXT("<b>"), TEXT(""));
	Content.ReplaceInline(TEXT("</b>"), TEXT(""));
	Content.ReplaceInline(TEXT("<br>"), TEXT("\n"));
	Content.ReplaceInline(TEXT("<font color=\"#FF8156\">"), TEXT("<TapColor>"));
	Content.ReplaceInline(TEXT("</font>"), TEXT("</>"));
	return Content;
}

EAAUAgeLimit AAUHelper::MakeAgeLimit(int64 Age) {
	if (Age < 0) {
		return EAAUAgeLimit::Unknown;
	} else if (Age < 8) {
		return EAAUAgeLimit::Child;
	} else if (Age < 16) {
		return EAAUAgeLimit::Teen;
	} else if (Age < 18) {
		return EAAUAgeLimit::Young;
	} else {
		return EAAUAgeLimit::Adult;
	}
}


