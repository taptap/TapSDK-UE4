#include "LCLeaderboard.h"
#include "LCLeaderboardImpl.h"

FLCLeaderboard::FLCLeaderboard(const FString& Name, const Type& InMemberType) {
	StatisticName = Name;
	MemberType = InMemberType;
}

void FLCLeaderboard::UpdateCurrentUserStatistics(const TMap<FString, double> Values, FStatisticsDelegate OnSuccess,
                                                 FLCError::FDelegate OnError) {
	FLCLeaderboardImpl::Get()->UpdateCurrentUserStatistics(Values, OnSuccess, OnError);
}

void FLCLeaderboard::GetResults(FRankingsDelegate OnSuccess, FLCError::FDelegate OnError) {
	FLCLeaderboardImpl::Get()->GetResults(*this, OnSuccess, OnError);
}

void FLCLeaderboard::GetAroundResults(const FString& TargetID, FRankingsDelegate OnSuccess, FLCError::FDelegate OnError) {
	FLCLeaderboardImpl::Get()->GetAroundResults(*this, TargetID, OnSuccess, OnError);
}

void FLCLeaderboard::GetStatistics(const FString& Target, FStatisticsDelegate OnSuccess, FLCError::FDelegate OnError,
                                   TArray<FString> StatisticNames, const Type& InMemberType) {
	FLCLeaderboardImpl::Get()->GetStatistics(Target, OnSuccess, OnError, StatisticNames, InMemberType);

}

FString FLCLeaderboard::GetStatisticName() const{
	return StatisticName;
}

FLCLeaderboard::Type FLCLeaderboard::GetMemberType() const{
	return MemberType;
}
