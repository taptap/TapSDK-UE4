#include "LCLeaderboard_Android.h"
#include "LCJavaHelper.h"
#include "Android/LCJNI.h"
#include "Tools/LCDebuger.h"
#include "Tools/LCHelper.h"

#define LCLeaderboardUE "com/lc/LCLeaderboardUE"

typedef TTuple<FLCLeaderboard::FStatisticsDelegate, FLCError::FDelegate> StatisticsDelegate;
typedef TTuple<FLCLeaderboard::FRankingsDelegate, FLCError::FDelegate> RankingsDelegate;
TMap<int, StatisticsDelegate> StatisticsCallBacks;
TMap<int, RankingsDelegate> RankingsCallBacks;

// template <class ValueType>
// void RemovePairsIfExist(TMap<int, ValueType> CallBackMap, int InCallBackID) {
// 	auto CallBackPtr = CallBackMap.Find(InCallBackID);
// 	if (CallBackPtr != nullptr) {
// 		CallBackMap.Remove(InCallBackID);
// 	}
// }

FLCLeaderboard_Android::~FLCLeaderboard_Android() {
}

void FLCLeaderboard_Android::UpdateCurrentUserStatistics(const TMap<FString, double> Values,
	FLCLeaderboard::FStatisticsDelegate OnSuccess, FLCError::FDelegate OnError) {
	int InCallBackID = GetCallBackID();
	StatisticsCallBacks.Add(InCallBackID, MakeTuple(OnSuccess, OnError));
	
	LCJNI::JNI LCJNI;
	auto LCLeaderboardUE_Class = LCJNI.FindClass(LCLeaderboardUE);
	auto Map_Android = LCJavaHelper::ConvertUEMap(LCJNI, Values);
	LCJNI.CallStaticVoidMethod(LCLeaderboardUE_Class, "updateCurrentUserStatistics", "(Ljava/util/Map;I)V", *Map_Android, InCallBackID);
}

void FLCLeaderboard_Android::GetStatistics(const FString& Target, FLCLeaderboard::FStatisticsDelegate OnSuccess,
	FLCError::FDelegate OnError, TArray<FString> StatisticNames, FLCLeaderboard::Type InMemberType) {
	int InCallBackID = GetCallBackID();
	StatisticsCallBacks.Add(InCallBackID, MakeTuple(OnSuccess, OnError));

	LCJNI::JNI LCJNI;
	auto LCLeaderboardUE_Class = LCJNI.FindClass(LCLeaderboardUE);
	int memberType = InMemberType;
	
	auto memberId = LCJNI.ToJavaString(Target);
	auto statisticNames = LCJavaHelper::ConvertUEArray(LCJNI, StatisticNames);
	LCJNI.CallStaticVoidMethod(LCLeaderboardUE_Class, "getMemberStatistics", "(ILjava/lang/String;Ljava/util/List;I)V", memberType, *memberId, *statisticNames, InCallBackID);
}

void FLCLeaderboard_Android::GetResults(const FLCLeaderboard& Query, FLCLeaderboard::FRankingsDelegate OnSuccess,
	FLCError::FDelegate OnError) {
	GetAroundResults(Query, "", OnSuccess, OnError);
}

void FLCLeaderboard_Android::GetAroundResults(const FLCLeaderboard& Query, const FString& TargetID,
	FLCLeaderboard::FRankingsDelegate OnSuccess, FLCError::FDelegate OnError) {
	
	int InCallBackID = GetCallBackID();
	RankingsCallBacks.Add(InCallBackID, MakeTuple(OnSuccess, OnError));

	LCJNI::JNI LCJNI;
	auto LCLeaderboardUE_Class = LCJNI.FindClass(LCLeaderboardUE);
	auto StatisticName = LCJNI.ToJavaString(Query.GetStatisticName());
	auto targetID = LCJNI.ToJavaString(TargetID);
	int memberType = Query.GetMemberType();
	int version = Query.Version;
	int skip = Query.Skip;
	int limit = Query.Limit;
	auto selectMemberKeys = LCJavaHelper::ConvertUEArray(LCJNI, Query.SelectMemberKeys);
	auto includeStatistics = LCJavaHelper::ConvertUEArray(LCJNI, Query.IncludeStatistics);
	bool withCount = Query.WithCount;
	LCJNI.CallStaticVoidMethod(LCLeaderboardUE_Class, "getRankingResults", "(Ljava/lang/String;Ljava/lang/String;IIIILjava/util/List;Ljava/util/List;ZI)V", *StatisticName, *targetID, memberType, version, skip, limit, *selectMemberKeys, *includeStatistics, withCount, InCallBackID);
}

int FLCLeaderboard_Android::GetCallBackID() {
	return CallBackID++;
}

FLCLeaderboardStatistic FLCLeaderboard_CovertStatistic(const LCJNI::JNI& LCJNI, const LCJNI::Object& object) {

	FLCLeaderboardStatistic UE_Statistic;
	if (!object) {
		return UE_Statistic;
	}
	auto Name = LCJNI.CallObjectMethod(object, "getName", "()Ljava/lang/String;");
	UE_Statistic.Name = LCJNI.GetFString(Name);
	auto Entity = LCJNI.CallObjectMethod(object, "getEntity", "()Ljava/lang/String;");
	UE_Statistic.Entity = LCJNI.GetFString(Entity);
	UE_Statistic.Version = LCJNI.CallIntMethod(object, "getVersion", "()I");
	UE_Statistic.Value = LCJNI.CallDoubleMethod(object, "getValue", "()D");
	LCDebuger::DisplayLog(FString::Printf(TEXT("Statistic_UE Value: %.15lf"), UE_Statistic.Value));
	auto User = LCJNI.CallObjectMethod(object, "getUser", "()Lcn/leancloud/LCUser;");
	UE_Statistic.User = LCJavaHelper::ConvertUser(LCJNI, User);
	auto Object = LCJNI.CallObjectMethod(object, "getObject", "()Lcn/leancloud/LCObject;");
	UE_Statistic.Object = LCJavaHelper::ConvertUser(LCJNI, Object);
	
	return UE_Statistic;
}

TArray<FLCLeaderboardStatistic> FLCLeaderboard_CovertStatistics(const LCJNI::JNI& LCJNI, const LCJNI::Object& object) {
	TArray<FLCLeaderboardStatistic> UE_Statistics;
	if (!object) {
		return UE_Statistics;
	}
	LCJavaHelper::ListForEach(LCJNI, object, [&](int Index, const LCJNI::Object& Object) {
		FLCLeaderboardStatistic UE_Statistic = FLCLeaderboard_CovertStatistic(LCJNI, Object);
		UE_Statistics.Add(UE_Statistic);
	});
	return UE_Statistics;
}

FLCLeaderboardRanking FLCLeaderboard_CovertRanking(const LCJNI::JNI& LCJNI, const LCJNI::Object& object) {

	FLCLeaderboardRanking UE_Ranking;
	if (!object) {
		return UE_Ranking;
	}
	UE_Ranking.Rank = LCJNI.CallIntMethod(object, "getRank", "()I");
	UE_Ranking.Value = LCJNI.CallDoubleMethod(object, "getStatisticValue", "()D");
	auto EntityId = LCJNI.CallObjectMethod(object, "getEntityId", "()Ljava/lang/String;");
	UE_Ranking.EntityId = LCJNI.GetFString(EntityId);
	auto User = LCJNI.CallObjectMethod(object, "getUser", "()Lcn/leancloud/LCUser;");
	UE_Ranking.User = LCJavaHelper::ConvertUser(LCJNI, User);
	auto Object = LCJNI.CallObjectMethod(object, "getObject", "()Lcn/leancloud/LCObject;");
	UE_Ranking.Object = LCJavaHelper::ConvertObject(LCJNI, Object);
	auto Statistics = LCJNI.CallObjectMethod(object, "getStatistics", "()Ljava/util/List;");
	UE_Ranking.Statistics = FLCLeaderboard_CovertStatistics(LCJNI, Statistics);

	return UE_Ranking;
}

TArray<FLCLeaderboardRanking> FLCLeaderboard_CovertRankings(const LCJNI::JNI& LCJNI, const LCJNI::Object& object) {
	TArray<FLCLeaderboardRanking> UE_Rankings;
	if (!object) {
		return UE_Rankings;
	}
	LCJavaHelper::ListForEach(LCJNI, object, [&](int Index, const LCJNI::Object& Object) {
		FLCLeaderboardRanking UE_Ranking = FLCLeaderboard_CovertRanking(LCJNI, Object);
		UE_Rankings.Add(UE_Ranking);
	});
	return UE_Rankings;
}

#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT void Java_com_lc_LCLeaderboardUE_onStatisticsError(JNIEnv* jenv, jclass thiz, jint code, jstring Message, jint callBackID) {
		auto CallBackPtr = StatisticsCallBacks.Find(callBackID);
		if (CallBackPtr == nullptr) {
			return;
		}
		LCJNI::JNI LCJNI(jenv);
		FString Msg_UE = LCJNI.GetFStringFromParam(Message);
		FLCError Error(code, Msg_UE);
		FLCError::FDelegate CallBack = (*CallBackPtr).Get<1>();
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(Error);
			StatisticsCallBacks.Remove(callBackID);
		});
	}

	JNIEXPORT void Java_com_lc_LCLeaderboardUE_onStatisticsSuccess(JNIEnv* jenv, jclass thiz, jobject statistics, jint callBackID) {
		auto CallBackPtr = StatisticsCallBacks.Find(callBackID);
		if (CallBackPtr == nullptr) {
			return;
		}
		LCJNI::JNI LCJNI(jenv);
		FLCLeaderboard::FStatisticsDelegate CallBack = (*CallBackPtr).Get<0>();
		auto statistics_obj = LCJNI::MakeScopedJavaObject(jenv, statistics, false);
		TArray<FLCLeaderboardStatistic> UE_Statistics = FLCLeaderboard_CovertStatistics(LCJNI, statistics_obj);
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(UE_Statistics);
			StatisticsCallBacks.Remove(callBackID);
		});
	}

	JNIEXPORT void Java_com_lc_LCLeaderboardUE_onRankingsError(JNIEnv* jenv, jclass thiz, jint code, jstring Message, jint callBackID) {
		auto CallBackPtr = RankingsCallBacks.Find(callBackID);
		if (CallBackPtr == nullptr) {
			return;
		}
		LCJNI::JNI LCJNI(jenv);
		FString Msg_UE = LCJNI.GetFStringFromParam(Message);
		FLCError Error(code, Msg_UE);
		FLCError::FDelegate CallBack = (*CallBackPtr).Get<1>();
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(Error);
			RankingsCallBacks.Remove(callBackID);
		});
	}

	JNIEXPORT void Java_com_lc_LCLeaderboardUE_onRankingsSuccess(JNIEnv* jenv, jclass thiz, jobject rankings, jint count, jint callBackID) {
		auto CallBackPtr = RankingsCallBacks.Find(callBackID);
		if (CallBackPtr == nullptr) {
			return;
		}
		LCJNI::JNI LCJNI(jenv);
		FLCLeaderboard::FRankingsDelegate CallBack = (*CallBackPtr).Get<0>();
		auto rankings_obj = LCJNI::MakeScopedJavaObject(jenv, rankings, false);
		TArray<FLCLeaderboardRanking> UE_Rankings = FLCLeaderboard_CovertRankings(LCJNI, rankings_obj);
		LCHelper::PerformOnGameThread([=]() {
			CallBack.ExecuteIfBound(UE_Rankings, count);
			RankingsCallBacks.Remove(callBackID);
		});

	}


#ifdef __cplusplus
}
#endif
