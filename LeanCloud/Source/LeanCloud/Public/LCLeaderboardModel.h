#pragma once
#include "LCUser.h"

struct LEANCLOUD_API FLCLeaderboardStatistic {
	/// The name of the leaderboard.
	FString Name;
	/// The version of the leaderboard.
	int64 Version;
	/// The value of this statistic.
	double Value;
	/// If this statistic belongs to one user, this property is nonnull.
	TSharedPtr<FLCUser> User;
	/// If this statistic belongs to one object, this property is nonnull.
	TSharedPtr<FLCObject> Object;
	/// If this statistic belongs to one entity, this property is nonnull.
	FString Entity;
};

struct LEANCLOUD_API FLCLeaderboardRanking {
	/// The ranking on the leaderboard.
	int64 Rank;
	/// If this ranking belongs to one user, this property is nonnull.
	TSharedPtr<FLCUser> User;
	/// If this ranking belongs to one object, this property is nonnull.
	TSharedPtr<FLCObject> Object;
	/// If this ranking belongs to one entity, this property is nonnull.
	FString EntityId;
	/// The value of the statistic.
	double Value;
	/// The statistics on the other leaderboards.
	TArray<FLCLeaderboardStatistic> Statistics;
};
