#include "TDSLeaderBoardRanking.h"
#include "TapBootstrapImpl.h"

#if PLATFORM_IOS || PLATFORM_ANDROID
void FTDSLeaderBoardRanking::QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit,
														FRankingsDelegate OnSuccess, FTUError::FDelegate OnError) {
	FTapBootstrapImpl::Get()->QueryTapFriendsLeaderBoard(Name, From, Limit, OnSuccess, OnError);
}
#endif

