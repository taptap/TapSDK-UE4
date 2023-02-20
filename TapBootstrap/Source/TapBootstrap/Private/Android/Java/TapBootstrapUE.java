package com.tds;
import android.app.Activity;
import android.os.Handler;
import android.os.Looper;

import com.tapsdk.bootstrap.Callback;
import com.tapsdk.bootstrap.TapBootstrap;
import com.tapsdk.bootstrap.account.TDSUser;
import com.tapsdk.bootstrap.exceptions.TapError;
import com.tapsdk.bootstrap.leaderboard.TDSLeaderBoardRanking;
import com.tds.common.entities.Pair;
import com.tds.common.entities.TapBillboardConfig;
import com.tds.common.entities.TapConfig;
import com.tds.common.entities.TapDBConfig;
import com.tds.common.models.TapRegionType;

import org.jetbrains.annotations.NotNull;

import java.util.List;
import java.util.Map;
import java.util.HashSet;
import java.util.Set;

import cn.leancloud.LCUser;
import cn.leancloud.json.JSONObject;
import io.reactivex.Observer;
import io.reactivex.disposables.Disposable;

public class TapBootstrapUE {
    public static void init(Activity activity, String clientID, String clientToken, String serverUrl, boolean isCN, boolean dbEnable, String channel, String gameVersion, boolean billboardEnable, String billboardUrl, String[] dimensionString) {
        Handler mainHandler = new Handler(Looper.getMainLooper());
        mainHandler.post(new Runnable() {
            @Override
            public void run() {
                TapDBConfig dbConfig = new TapDBConfig();
                dbConfig.setEnable(dbEnable);
                dbConfig.setChannel(channel);
                dbConfig.setGameVersion(gameVersion);

                TapConfig.Builder builder = new TapConfig.Builder().withAppContext(activity)
                        .withServerUrl(serverUrl)
                        .withClientId(clientID)
                        .withClientToken(clientToken)
                        .withRegionType(isCN ? TapRegionType.CN : TapRegionType.IO)
                        .withTapDBConfig(dbConfig);

                if(billboardEnable){
                    Set<Pair<String, String>> dimensionSet = new HashSet<>();
                    for (int i = 0; i < dimensionString.length; i += 2)
                    {
                        dimensionSet.add(Pair.create(dimensionString[i], dimensionString[i+1]));
                    }

                    TapBillboardConfig billboardConfig = new TapBillboardConfig.Builder()
                            .withDimensionSet(dimensionSet)
                            .withServerUrl(billboardUrl)
                            .build();

                    builder.withBillboardConfig(billboardConfig);
                }

                TapConfig tapConfig = builder.build();

                TapBootstrap.init(activity, tapConfig);
            }
        });
    }

    public static TDSUser getCurrentUser() {
        return TDSUser.getCurrentUser();
    }

    public static void logout() {
        TDSUser.logOut();
    }

    public static void loginAnonymously(int callBackID) {
        TDSUser.logInAnonymously().subscribe(new Observer<TDSUser>() {
            @Override
            public void onSubscribe(@NotNull Disposable d) {

            }

            @Override
            public void onNext(@NotNull TDSUser tdsUser) {
                new TapBootstrapUE().onUserSuccess(tdsUser, callBackID);
            }

            @Override
            public void onError(@NotNull Throwable e) {
                new TapBootstrapUE().onUserError(-1, e.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {

            }
        });
    }

    public static void loginWithTapTap(Activity activity, String[] permissions, int callBackID) {
        Handler mainHandler = new Handler(Looper.getMainLooper());
        mainHandler.post(new Runnable() {
            @Override
            public void run() {
                TDSUser.loginWithTapTap(activity, new Callback<TDSUser>() {
                    @Override
                    public void onSuccess(TDSUser result) {
                        new TapBootstrapUE().onUserSuccess(result, callBackID);
                    }

                    @Override
                    public void onFail(TapError error) {
                        new TapBootstrapUE().onUserError(error.code, error.detailMessage, callBackID);
                    }
                }, permissions);
            }
        });
    }

    public static void becomeWithSessionToken(String sessionToken, int callBackID) {
        TDSUser.becomeWithSessionTokenInBackground(sessionToken, true).subscribe(new Observer<TDSUser>() {
            @Override
            public void onSubscribe(@NotNull Disposable d) {

            }

            @Override
            public void onNext(@NotNull TDSUser tdsUser) {
                new TapBootstrapUE().onUserSuccess(tdsUser, callBackID);
            }

            @Override
            public void onError(@NotNull Throwable e) {
                new TapBootstrapUE().onUserError(-1, e.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {

            }
        });
    }

    public static void loginWithAuthData(Map authdata, String platfrom, int callBackID) {
        TDSUser.loginWithAuthData(authdata, platfrom).subscribe(new Observer<TDSUser>() {
            @Override
            public void onSubscribe(@NotNull Disposable d) {

            }

            @Override
            public void onNext(@NotNull TDSUser tdsUser) {
                new TapBootstrapUE().onUserSuccess(tdsUser, callBackID);
            }

            @Override
            public void onError(@NotNull Throwable e) {
                new TapBootstrapUE().onUserError(-1, e.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {

            }
        });
    }

    public static void associateWithAuthData(TDSUser user, Map authdata, String platfrom, int callBackID) {
        user.associateWithAuthData(authdata, platfrom).subscribe(new Observer<TDSUser>() {
            @Override
            public void onSubscribe(@NotNull Disposable d) {

            }

            @Override
            public void onNext(@NotNull TDSUser tdsUser) {
                new TapBootstrapUE().onUserSuccess(tdsUser, callBackID);
            }

            @Override
            public void onError(@NotNull Throwable e) {
                new TapBootstrapUE().onUserError(-1, e.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {

            }
        });
    }

    public static void dissociateWithAuthData(TDSUser user, String platfrom, int callBackID) {
        user.dissociateWithAuthData(platfrom).subscribe(new Observer<LCUser>() {
            @Override
            public void onSubscribe(@NotNull Disposable d) {

            }

            @Override
            public void onNext(@NotNull LCUser lcUser) {
                TDSUser tdsUser = new TDSUser();
                tdsUser.resetServerData(lcUser.getServerData());
                new TapBootstrapUE().onUserSuccess(tdsUser, callBackID);
            }

            @Override
            public void onError(@NotNull Throwable e) {
                new TapBootstrapUE().onUserError(-1, e.getMessage(), callBackID);
            }

            @Override
            public void onComplete() {

            }
        });
    }
    public static void save(TDSUser user) {
        user.saveInBackground();
    }

    public static void queryTapFriendsLeaderBoard(String name, int from, int limit, int callBackID) {
        TDSLeaderBoardRanking.queryTapFriendsLeaderBoard(name, from, limit, new Callback<List<TDSLeaderBoardRanking>>() {
            @Override
            public void onSuccess(List<TDSLeaderBoardRanking> result) {
                new TapBootstrapUE().onRankingsSuccess(result, callBackID);
            }

            @Override
            public void onFail(TapError error) {
                new TapBootstrapUE().onRankingsError(error.code, error.detailMessage, callBackID);
            }
        });
    }

    public static void retrieveShortToken(String sessionToken, int callBackID) {
        TDSUser.retrieveShortTokenInBackground(sessionToken).subscribe(new Observer<JSONObject>() {
            @Override
            public void onSubscribe(Disposable d) {

            }

            @Override
            public void onNext(JSONObject jsonObject) {
                String credential = jsonObject.getString("identityToken");
                onRetrieveShortTokenSuccess(credential, callBackID);
            }

            @Override
            public void onError(Throwable error) {
                onRetrieveShortTokenError(-1, error.toString(), callBackID);
            }

            @Override
            public void onComplete() {

            }
        });
    }

    public native void onUserError(int code, String Message, int callBackID);

    public native void onUserSuccess(TDSUser user, int callBackID);

    public native void onRankingsError(int code, String Message, int callBackID);

    public native void onRankingsSuccess(List<TDSLeaderBoardRanking> result, int callBackID);

    public native static void onRetrieveShortTokenError(int code, String Message, int callBackID);

    public native static void onRetrieveShortTokenSuccess(String credential, int callBackID);

}
