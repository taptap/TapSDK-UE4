#include "LCLeaderboard_iOS.h"
#include "iOS/LCObjcHelper.h"
#include "Tools/LCHelper.h"
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wnonnull"
#import <LeanCloudObjc/LCLeaderboard.h>

FLCLeaderboardStatistic FLCLeaderboard_Covert(LCLeaderboardStatistic * statistic) {
	FLCLeaderboardStatistic UE_Statistic;
	UE_Statistic.Name = LCObjcHelper::Convert(statistic.name);
	UE_Statistic.Version = statistic.version;
	UE_Statistic.Value = statistic.value;
	UE_Statistic.Entity = LCObjcHelper::Convert(statistic.entity);
	UE_Statistic.User = LCObjcHelper::Convert(statistic.user);;
	UE_Statistic.Object = LCObjcHelper::Convert(statistic.object);;
	return UE_Statistic;
}

TArray<FLCLeaderboardStatistic> FLCLeaderboard_Covert(NSArray<LCLeaderboardStatistic *> * statistics) {
	TArray<FLCLeaderboardStatistic> UE_Statistics;
	for (LCLeaderboardStatistic *statistic in statistics) {
		UE_Statistics.Add(FLCLeaderboard_Covert(statistic));     
	}
	return UE_Statistics;
}

FLCLeaderboardRanking FLCLeaderboard_Covert(LCLeaderboardRanking * ranking) {
	FLCLeaderboardRanking UE_Ranking;
	UE_Ranking.Rank = ranking.rank;
	UE_Ranking.Value = ranking.value;
	UE_Ranking.EntityId = LCObjcHelper::Convert(ranking.entity);
	UE_Ranking.User = LCObjcHelper::Convert(ranking.user);
	UE_Ranking.Object = LCObjcHelper::Convert(ranking.object);
	UE_Ranking.Statistics = FLCLeaderboard_Covert(ranking.includedStatistics);
	return UE_Ranking;
}

TArray<FLCLeaderboardRanking> FLCLeaderboard_Covert(NSArray<LCLeaderboardRanking *> * rankings) {
	TArray<FLCLeaderboardRanking> UE_Rankings;
	for (LCLeaderboardRanking *ranking in rankings) {
		UE_Rankings.Add(FLCLeaderboard_Covert(ranking));     
	}
	return UE_Rankings;
}

FLCLeaderboard_iOS::~FLCLeaderboard_iOS() {
}

void FLCLeaderboard_iOS::UpdateCurrentUserStatistics(const TMap<FString, double> Values,
	FLCLeaderboard::FStatisticsDelegate OnSuccess, FLCError::FDelegate OnError) {
	NSMutableDictionary *dic = [[NSMutableDictionary alloc] init];
	for (auto Value : Values) {
		NSString *key = LCObjcHelper::Convert(Value.Key);
		dic[key] = @(Value.Value);
	}
	
	[LCLeaderboard updateCurrentUserStatistics:dic callback:^(NSArray<LCLeaderboardStatistic *> * _Nullable statistics, NSError * _Nullable error) {
		if (error) {
			FLCError Error_UE = LCObjcHelper::Convert(error);
			LCHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error_UE);
			});
		} else {
			TArray<FLCLeaderboardStatistic> Statistics_UE = FLCLeaderboard_Covert(statistics);
			LCHelper::PerformOnGameThread([=]() {
				OnSuccess.ExecuteIfBound(Statistics_UE);
			});
		}
	}];
}

void FLCLeaderboard_iOS::GetStatistics(const FString& Target, FLCLeaderboard::FStatisticsDelegate OnSuccess,
	FLCError::FDelegate OnError, TArray<FString> StatisticNames, FLCLeaderboard::Type InMemberType) {
	FLCLeaderboardImpl::GetStatistics(Target, OnSuccess, OnError, StatisticNames, InMemberType);
	void(^resultBlock)(NSArray<LCLeaderboardStatistic *> * _Nullable statistics, NSError * _Nullable error) = ^(NSArray<LCLeaderboardStatistic *> * _Nullable statistics, NSError * _Nullable error) {
		if (error) {
			FLCError Error_UE = LCObjcHelper::Convert(error);
			LCHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error_UE);
			});
		} else {
			TArray<FLCLeaderboardStatistic> Statistics_UE = FLCLeaderboard_Covert(statistics);
			LCHelper::PerformOnGameThread([=]() {
				OnSuccess.ExecuteIfBound(Statistics_UE);
			});
		}
	};
	NSString * oc_value = LCObjcHelper::Convert(Target);
	NSArray<NSString *> *oc_statisticNames = nil;
	if (StatisticNames.Num() > 0) {
		oc_statisticNames = LCObjcHelper::Convert(StatisticNames);
	}
	if (InMemberType == FLCLeaderboard::User) {
		[LCLeaderboard getStatisticsWithUserId:oc_value statisticNames:oc_statisticNames callback:resultBlock];
	} else if (InMemberType == FLCLeaderboard::Entity) {
		[LCLeaderboard getStatisticsWithEntity:oc_value statisticNames:oc_statisticNames callback:resultBlock];
	} else {
		[LCLeaderboard getStatisticsWithObjectId:oc_value statisticNames:oc_statisticNames option:nil callback:resultBlock];
	}
}

void FLCLeaderboard_iOS::GetResults(const FLCLeaderboard& Query, FLCLeaderboard::FRankingsDelegate OnSuccess,
	FLCError::FDelegate OnError) {
	GetAroundResults(Query, "", OnSuccess, OnError);
}

void FLCLeaderboard_iOS::GetAroundResults(const FLCLeaderboard& Query, const FString& TargetID,
	FLCLeaderboard::FRankingsDelegate OnSuccess, FLCError::FDelegate OnError) {
	void(^resultBlock)(NSArray *rankings, NSInteger count, NSError *error) = ^(NSArray<LCLeaderboardRanking *> * rankings, NSInteger count, NSError *error) {
		if (error) {
			FLCError Error_UE = LCObjcHelper::Convert(error);
			LCHelper::PerformOnGameThread([=]() {
				OnError.ExecuteIfBound(Error_UE);
			});
		} else {
			TArray<FLCLeaderboardRanking> Rankings_UE = FLCLeaderboard_Covert(rankings);
			LCHelper::PerformOnGameThread([=]() {
				OnSuccess.ExecuteIfBound(Rankings_UE, count);
			});
		}
	};
	
	NSString * StatisticName = LCObjcHelper::Convert(Query.GetStatisticName());
	LCLeaderboard *leaderboard = [[LCLeaderboard alloc] initWithStatisticName:StatisticName];
	leaderboard.limit = Query.Limit;
	leaderboard.skip = Query.Skip;
	leaderboard.version = Query.Version;
	leaderboard.returnCount = Query.WithCount;
	if (Query.IncludeStatistics.Num() > 0) {
		leaderboard.includeStatistics = LCObjcHelper::Convert(Query.IncludeStatistics);
	}
	LCLeaderboardQueryOption *queryOption = nil;
	if (Query.SelectMemberKeys.Num() > 0) {
		queryOption = [LCLeaderboardQueryOption new];
		queryOption.selectKeys = LCObjcHelper::Convert(Query.SelectMemberKeys);
	}
	
	auto MemberType = Query.GetMemberType();
	if (TargetID.IsEmpty()) {
		if (MemberType == FLCLeaderboard::User) {
			[leaderboard getUserResultsWithOption:queryOption callback:resultBlock];
		} else if (MemberType == FLCLeaderboard::Entity) {
			[leaderboard getEntityResultsWithCallback:resultBlock];
		} else {
			[leaderboard getObjectResultsWithOption:queryOption callback:resultBlock];
		}
	}
	else {
		NSString * TargetID_OC = LCObjcHelper::Convert(TargetID);
		if (MemberType == FLCLeaderboard::User) {
			[leaderboard getUserResultsAroundUser:TargetID_OC option:queryOption callback:resultBlock];
		} else if (MemberType == FLCLeaderboard::Entity) {
			[leaderboard getEntityResultsAroundEntity:TargetID_OC callback:resultBlock];
		} else {
			[leaderboard getObjectResultsAroundObject:TargetID_OC option:queryOption callback:resultBlock];
		}
	}

}

