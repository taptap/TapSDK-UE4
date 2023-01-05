#pragma once
#include "TDSUser.h"

struct TAPBOOTSTRAP_API FTDSLeaderBoardRanking {
public:
	DECLARE_DELEGATE_OneParam(FRankingsDelegate, const TArray<FTDSLeaderBoardRanking>& Rankings);
	
	FString StatisticName;
	int StatisticValue = 0;
	int Rank = 0;
	TSharedPtr<FTDSUser> User = nullptr;

#if PLATFORM_IOS || PLATFORM_ANDROID

	/**
	 * @brief TapTap 好友排行榜列表
	 * @param Name 排行榜名称，e.g: "world"
	 * @param From 查询列表的起始位置
	 * @param Limit 查询列表的长度
	 * @param OnSuccess 请求成功
	 * @param OnError 请求失败
	 */
	static void QueryTapFriendsLeaderBoard(const FString& Name, int From, int Limit, FRankingsDelegate OnSuccess, FTUError::FDelegate OnError);
#endif


};
