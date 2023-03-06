package com.lc;

import android.app.Activity;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;

import cn.leancloud.LCException;
import cn.leancloud.LCOPPOPushAdapter;
import cn.leancloud.callback.LCCallback;

public class LCMixPushUE {
    public static void registerHMSPush(final Activity activity) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                cn.leancloud.LCMixPushManager.registerHMSPush(activity.getApplication());
                cn.leancloud.LCMixPushManager.connectHMS(activity);
//                 cn.leancloud.LCMixPushManager.turnOnHMSPush(activity, new LCCallback<Void>() {
//                     @Override
//                     protected void internalDone0(Void aVoid, LCException LCException) {
//                         if (null != LCException) {
//                             System.out.println("failed to turn on HMS Push, cause: " + LCException.getMessage());
//                         } else {
//                             System.out.println("succeed to turn on HMS Push.");
//                         }
//                     }
//                 });
            }
        });
    }

    public static void registerHonorPush(final Activity activity) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                cn.leancloud.LCMixPushManager.registerHonorPush(activity);
            }
        });
    }

    public static void registerXiaomiPush(final Activity activity, String miAppId, String miAppKey) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                cn.leancloud.LCMixPushManager.registerXiaomiPush(activity.getApplicationContext(), miAppId, miAppKey);
            }
        });
    }

    public static void registerFlymePush(final Activity activity, String flymeId, String flymeKey) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                cn.leancloud.LCMixPushManager.registerFlymePush(activity.getApplicationContext(), flymeId, flymeKey);
            }
        });
    }

    public static void registerVIVOPush(final Activity activity) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                cn.leancloud.LCMixPushManager.registerVIVOPush(activity.getApplication());
                cn.leancloud.LCMixPushManager.turnOnVIVOPush(new LCCallback<Boolean>() {
                    @Override
                    protected void internalDone0(Boolean aBoolean, LCException e) {
                        if (null != e) {
                            System.out.println("failed to turn on vivo push. cause:");
                            e.printStackTrace();
                        } else {
                            System.out.println("succeed to turn on vivo push.");
                        }
                    }
                });
            }
        });
    }

    public static void registerOppoPush(final Activity activity, String appKey, String appSecret) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                cn.leancloud.LCMixPushManager.registerOppoPush(activity.getApplicationContext(), appKey, appSecret, new LCOPPOPushAdapter() {

                });
            }
        });

    }

    public static String getDeviceName() {
        final String model = !TextUtils.isEmpty(Build.MODEL) ? Build.MODEL : "unknown";
        final String brand = !TextUtils.isEmpty(Build.BRAND) ? Build.BRAND : "unknown";
        return model + " " + brand;

    }
    
    public static void onResume(final Activity activity) {
           String content = activity.getIntent().getStringExtra("content");
            System.out.println("received content from Intent Extra: " + content);
    }

}
