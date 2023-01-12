package com.lc;

import cn.leancloud.LeanCloud;
import android.app.Activity;

import android.os.Handler;
import android.os.Looper;

public class LeanCloudUE {
   public static void initialize(final Activity activity, String clientID, String clientToken, String serverURL) {
        Handler mainHandler = new Handler(Looper.getMainLooper());
        mainHandler.post(new Runnable() {
            @Override
            public void run() {
                LeanCloud.initialize(activity, clientID, clientToken, serverURL);
            }
        });
    }
}