#pragma once
#include "CoreMinimal.h"
#include "AAUPlayableModel.generated.h"

/*
1.接近限制时才会返回限制类型，若剩余时长充足，不会返回限制 
2.返回剩余时间更小的限制类型，如距离宵禁18分钟，距离时长限制30分钟，此时限制类型为宵禁1
3.剩余时长较小时才会返回限制，如当天剩余时长小于20分钟
*/
USTRUCT()
struct FAAUPlayableModel
{

	GENERATED_BODY()
	
	UPROPERTY()
	int restrict_type = 0; // 0-不限制1-宵禁 2-时长

	UPROPERTY()
	int remain_time = 0;  //   防沉迷剩余时间，单位秒

	UPROPERTY()
	FString title; // 实名的宵禁临近文案

	UPROPERTY()
	FString description; // 距离健康保护时间还剩余14分钟，请注意适当休息。

	UPROPERTY()
	int cost_time = 0; //  今日游戏已玩时长，单位秒

	UPROPERTY()
	int status = 0;
	
};
