#pragma once
#include "LCLeaderboard.h"

class FLCLeaderboardImpl {
public:
	static TSharedPtr<FLCLeaderboardImpl> Get();
	virtual ~FLCLeaderboardImpl();

	virtual void UpdateCurrentUserStatistics(const TMap<FString, double> Values,
	FLCLeaderboard::FStatisticsDelegate OnSuccess, FLCError::FDelegate OnError);
	
	virtual void GetStatistics(const FString& Target, FLCLeaderboard::FStatisticsDelegate OnSuccess, FLCError::FDelegate OnError, TArray<FString> StatisticNames, FLCLeaderboard::Type InMemberType);
	
	virtual void GetResults(const FLCLeaderboard& Query, FLCLeaderboard::FRankingsDelegate OnSuccess, FLCError::FDelegate OnError);

	virtual void GetAroundResults(const FLCLeaderboard& Query, const FString& TargetID, FLCLeaderboard::FRankingsDelegate OnSuccess, FLCError::FDelegate OnError);

private:
	static TSharedPtr<FLCLeaderboardImpl> Instance;
	
};
