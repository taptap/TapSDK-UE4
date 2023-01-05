#pragma once
#include "LCLeaderboardImpl.h"

class FLCLeaderboard_iOS: public FLCLeaderboardImpl {
public:
	virtual ~FLCLeaderboard_iOS() override;
	virtual void UpdateCurrentUserStatistics(const TMap<FString, double> Values,
		FLCLeaderboard::FStatisticsDelegate OnSuccess, FLCError::FDelegate OnError) override;
	virtual void GetStatistics(const FString& Target, FLCLeaderboard::FStatisticsDelegate OnSuccess,
		FLCError::FDelegate OnError, TArray<FString> StatisticNames, FLCLeaderboard::Type InMemberType) override;
	virtual void GetResults(const FLCLeaderboard& Query, FLCLeaderboard::FRankingsDelegate OnSuccess,
		FLCError::FDelegate OnError) override;
	virtual void GetAroundResults(const FLCLeaderboard& Query, const FString& TargetID,
		FLCLeaderboard::FRankingsDelegate OnSuccess, FLCError::FDelegate OnError) override;

};
