package com.tds;

import android.util.Log;
// import com.tapsdk.antiaddictionui.AntiAddictionUIKit;
import com.tds.common.net.util.HostReplaceUtil;
import java.util.Locale;

public class TUDebugerUnreal4 {

    public static void addReplacedHostPair(String oldHost, String newHost) {
//         print(String.format(Locale.CHINA, "执行 replace oldHost: %s,  newHost: %s", oldHost, newHost));
//         if (oldHost.startsWith("http")){
            HostReplaceUtil.getInstance().addReplacedHostPair(oldHost, newHost);
//         }else{
//             AntiAddictionUIKit.setRndEnvironment(newHost);
//         }
    }

    public static void removeReplacedHostPair(String oldHost) {
        print("执行 remove host: " + oldHost);
        HostReplaceUtil.getInstance().clearReplacedHostPair(oldHost);
    }

    public static void clearAllReplacedHostPairs() {
        print("执行 clear host");
        HostReplaceUtil.getInstance().clear();
    }

    private static void print(String msg) {
        Log.i("TUDebugerUnreal4:", msg);
    }
}
