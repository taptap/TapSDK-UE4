package com.lc;
import org.jetbrains.annotations.NotNull;

import java.util.List;
import java.util.Map;

import cn.leancloud.LCLeaderboard;
import cn.leancloud.LCLeaderboardResult;
import cn.leancloud.LCRanking;
import cn.leancloud.LCStatistic;
import cn.leancloud.LCStatisticResult;
import cn.leancloud.LCUser;
import io.reactivex.Observer;
import io.reactivex.disposables.Disposable;

public class LCLeaderboardUE {
    public static void updateCurrentUserStatistics(Map<String, Double> values, int callBackID) {
        System.out.println("method: " + "updateCurrentUserStatistics");
        System.out.println("values: " + values);
        System.out.println("callBackID: " + callBackID);
        LCLeaderboard.updateStatistic(LCUser.currentUser(), values).subscribe(new Observer<LCStatisticResult>() {
            @Override
            public void onSubscribe(@NotNull Disposable disposable) {}

            @Override
            public void onNext(@NotNull LCStatisticResult jsonObject) {
                new LCLeaderboardUE().onStatisticsSuccess(jsonObject.getResults(), callBackID);
            }

            @Override
            public void onError(@NotNull Throwable throwable) {
                new LCLeaderboardUE().onStatisticsError(-1, throwable.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {}
        });
    }

    public static void getMemberStatistics(int memberType, String memberId, List<String> statisticNames, int callBackID) {
        System.out.println("method: " + "getMemberStatistics");
        System.out.println("memberType: " + memberType);
        System.out.println("memberId: " + memberId);
        System.out.println("statisticNames: " + statisticNames);
        System.out.println("callBackID: " + callBackID);
        String memberTypeStr = "_Object";
        if (memberType == 0) {
            memberTypeStr = LCLeaderboard.MEMBER_TYPE_USER;
        } else if (memberType == 1) {
            memberTypeStr = LCLeaderboard.MEMBER_TYPE_ENTITY;
        }
        LCLeaderboard.getMemberStatistics(memberTypeStr, memberId, statisticNames).subscribe(new Observer<LCStatisticResult>() {
            @Override
            public void onSubscribe(@NotNull Disposable disposable) {}

            @Override
            public void onNext(@NotNull LCStatisticResult jsonObject) {
                new LCLeaderboardUE().onStatisticsSuccess(jsonObject.getResults(), callBackID);
            }

            @Override
            public void onError(@NotNull Throwable throwable) {
                new LCLeaderboardUE().onStatisticsError(-1, throwable.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {}
        });
    }

    public static void getRankingResults(String StatisticName, String TargetID, int memberType, int version, int skip, int limit, List<String> selectMemberKeys, List<String> includeStatistics, boolean withCount, int callBackID) {
        System.out.println("method: " + "getRankingResults");
        System.out.println("StatisticName: " + StatisticName);
        System.out.println("TargetID: " + TargetID);
        System.out.println("memberType: " + memberType);
        System.out.println("version: " + version);
        System.out.println("skip: " + skip);
        System.out.println("limit: " + limit);
        System.out.println("selectMemberKeys: " + selectMemberKeys);
        System.out.println("includeStatistics: " + includeStatistics);
        System.out.println("withCount: " + withCount);
        System.out.println("callBackID: " + callBackID);
        String memberTypeStr = "_Object";
        if (memberType == 0) {
            memberTypeStr = LCLeaderboard.MEMBER_TYPE_USER;
        } else if (memberType == 1) {
            memberTypeStr = LCLeaderboard.MEMBER_TYPE_ENTITY;
        }
        LCLeaderboard leaderboard = LCLeaderboard.createWithoutData(StatisticName, memberTypeStr);
        leaderboard.setVersion(version);
        Observer<LCLeaderboardResult> observer = new Observer<LCLeaderboardResult>() {
            @Override
            public void onSubscribe(@NotNull Disposable disposable) {}

            @Override
            public void onNext(@NotNull LCLeaderboardResult leaderboardResult) {
                List<LCRanking> rankings = leaderboardResult.getResults();
                new LCLeaderboardUE().onRankingsSuccess(rankings, leaderboardResult.getCount(), callBackID);
            }

            @Override
            public void onError(@NotNull Throwable throwable) {
                new LCLeaderboardUE().onRankingsError(-1, throwable.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {}
        };
        if (TargetID != null && TargetID.length() != 0) {
            leaderboard.getAroundResults(TargetID, skip, limit, selectMemberKeys, includeStatistics).subscribe(observer);
        } else {
            leaderboard.getResults(skip, limit, selectMemberKeys, includeStatistics, withCount).subscribe(observer);
        }

    }

    public native void onStatisticsError(int code, String Message, int callBackID);

    public native void onStatisticsSuccess(List<LCStatistic> statistics, int callBackID);

    public native void onRankingsError(int code, String Message, int callBackID);

    public native void onRankingsSuccess(List<LCRanking> rankings, int count, int callBackID);

}
