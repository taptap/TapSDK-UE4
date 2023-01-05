# LeanCloud

LeanCloud模块

## 支持平台

* Android
* iOS

## 前提条件

* 安装 **UE 4.26** 及以上版本
* IOS **12** 或更高版本 
* Android MinSDK 为 **API21** 或更高版本

## 安装插件

* 下载 **TapSDK.zip** 解压后将 `LeanCloud` 文件夹 `Copy` 到项目的 `Plugins` 目录中
* 重启 Unreal Editor
* 打开 **编辑 > 插件 > 项目 > LeanCloud**，开启 `LeanCloud`模块


## LeanCloud使用

### 依赖所需模块
在 **Project.Build.cs** 中添加所需模块:
```c#
PublicDependencyModuleNames.AddRange(new string[] { "Core",
	"CoreUObject",
	"Engine",
	"Json",
	"InputCore",
	"JsonUtilities",
	"SlateCore",
	"LeanCloud"
});
```

### 导入头文件
```cpp
#include "LCLeaderboard.h"
```

### 更新用户成绩

```cpp
TMap<FString, double> Values;
kills
FLCLeaderboard::FStatisticsDelegate OnSuccess = FLCLeaderboard::FStatisticsDelegate::CreateLambda([=](TArray<FLCLeaderboardStatistic> Statistics) {
});
FLCError::FDelegate OnError = FLCError::FDelegate::CreateLambda([=](const FLCError& Error) {
});
FLCLeaderboard::UpdateCurrentUserStatistics(Values, OnSuccess, OnError);
```

### 获取排行榜结果指定区间的排名
```cpp
FString Name = "world";
FLCLeaderboard Leaderboard(Name, FLCLeaderboard::User);
Leaderboard.Version = 28;
Leaderboard.Skip = 0;
Leaderboard.Limit = 10;
FLCLeaderboard::FRankingsDelegate OnSuccess = FLCLeaderboard::FRankingsDelegate::CreateLambda([=](TArray<FLCLeaderboardRanking> Rankings, int64 Count) {

});
FLCError::FDelegate OnError = FLCError::FDelegate::CreateLambda([=](const FLCError& Error) {
	
});
Leaderboard.GetResults(OnSuccess, OnError);
```

### 获取排行榜结果当前用户附近的排名
```cpp
FString Name = "world";
FString TargetID = "UserID";
FLCLeaderboard Leaderboard(Name, FLCLeaderboard::User);
Leaderboard.Version = 28;
Leaderboard.Skip = 0;
Leaderboard.Limit = 10;
FLCLeaderboard::FRankingsDelegate OnSuccess = FLCLeaderboard::FRankingsDelegate::CreateLambda([=](TArray<FLCLeaderboardRanking> Rankings, int64 Count) {

});
FLCError::FDelegate OnError = FLCError::FDelegate::CreateLambda([=](const FLCError& Error) {
	
});
Leaderboard.GetAroundResults(TargetID, OnSuccess, OnError);
```

### 查询排行榜成员成绩
```cpp
FLCLeaderboard::FStatisticsDelegate OnSuccess = FLCLeaderboard::FStatisticsDelegate::CreateLambda([=](TArray<FLCLeaderboardStatistic> Statistics) {
		
});
FLCError::FDelegate OnError = FLCError::FDelegate::CreateLambda([=](const FLCError& Error) {
	
});
FString Value = ESW_MemberStatistics_Name->GetString();
FLCLeaderboard::GetStatistics(Value, OnSuccess, OnError, NULL, FLCLeaderboard::User);
```

