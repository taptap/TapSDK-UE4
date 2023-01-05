#include "LCLeaderboardImpl.h"
#include "Tools/LCDebuger.h"

#if PLATFORM_IOS

#include "iOS/LCLeaderboard_iOS.h"
typedef FLCLeaderboard_iOS LCLeaderboardCommon;

#elif PLATFORM_ANDROID

#include "Android/LCLeaderboard_Android.h"
typedef FLCLeaderboard_Android LCLeaderboardCommon;

#else
typedef FLCLeaderboardImpl LCLeaderboardCommon;
#endif

#define UnsupportedPlatformsLog LCDebuger::ErrorLog(FString::Printf(TEXT("LeanCloud Unsupported Platforms Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));

TSharedPtr<FLCLeaderboardImpl> FLCLeaderboardImpl::Instance;

TSharedPtr<FLCLeaderboardImpl> FLCLeaderboardImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShared<LCLeaderboardCommon>();
	}
	return Instance;
}

FLCLeaderboardImpl::~FLCLeaderboardImpl() {
}

void FLCLeaderboardImpl::UpdateCurrentUserStatistics(const TMap<FString, double> Values,
	FLCLeaderboard::FStatisticsDelegate OnSuccess, FLCError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FLCLeaderboardImpl::GetStatistics(const FString& Target, FLCLeaderboard::FStatisticsDelegate OnSuccess,
	FLCError::FDelegate OnError, TArray<FString> StatisticNames, FLCLeaderboard::Type InMemberType) {
	UnsupportedPlatformsLog
}

void FLCLeaderboardImpl::GetResults(const FLCLeaderboard& Query, FLCLeaderboard::FRankingsDelegate OnSuccess,
	FLCError::FDelegate OnError) {
	UnsupportedPlatformsLog
}

void FLCLeaderboardImpl::GetAroundResults(const FLCLeaderboard& Query, const FString& TargetID,
	FLCLeaderboard::FRankingsDelegate OnSuccess, FLCError::FDelegate OnError) {
	UnsupportedPlatformsLog
}
