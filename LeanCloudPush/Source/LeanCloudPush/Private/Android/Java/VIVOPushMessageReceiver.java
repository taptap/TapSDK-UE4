package com.lc;

import android.content.Context;

import com.vivo.push.model.UPSNotificationMessage;
import com.vivo.push.model.UnvarnishedMessage;

import cn.leancloud.LCVIVOPushMessageReceiver;

public class VIVOPushMessageReceiver extends LCVIVOPushMessageReceiver {

    @Override
    public void onTransmissionMessage(Context context, UnvarnishedMessage unvarnishedMessage) {
        super.onTransmissionMessage(context, unvarnishedMessage);
        onVIVOMessage(unvarnishedMessage.getMessage());
    }

    public void onNotificationMessageClicked(Context context, UPSNotificationMessage unvarnishedMessage) {
        super.onNotificationMessageClicked(context, unvarnishedMessage);
        onVIVOMessage(unvarnishedMessage.toString());
    }

    public native void onVIVOMessage(String Message);
}
