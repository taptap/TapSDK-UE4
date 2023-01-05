package com.lc;

import cn.leancloud.LeanCloud;
import android.app.Activity;



public class LeanCloudUE {
   public static void initialize(final Activity activity, String clientID, String clientToken, String serverURL) {
        LeanCloud.initialize(activity, clientID, clientToken, serverURL);
    }
}