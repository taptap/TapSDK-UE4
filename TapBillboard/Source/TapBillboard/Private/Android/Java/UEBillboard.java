package com.tds;

import com.tapsdk.billboard.TapBillboard;

import com.tapsdk.billboard.exceptions.TapBillboardException;
import com.tds.common.entities.Pair;
import com.tds.common.entities.TapBillboardConfig;
import com.tds.common.entities.TapConfig;
import com.tds.common.entities.TapDBConfig;
import com.tds.common.models.TapRegionType;
import com.tds.common.net.util.HostReplaceUtil;

import com.tapsdk.billboard.utils.TapBillboardLogger;
import com.tapsdk.billboard.CustomLinkListener;
import com.tapsdk.billboard.Callback;
import com.tapsdk.billboard.constants.Constants;
import com.tapsdk.billboard.OutputStateListener;
import com.tapsdk.billboard.PanelShowStateListener;
import com.tapsdk.bootstrap.TapBootstrap;

import com.epicgames.ue4.GameActivity;
import java.util.HashSet;
import java.util.Set;

public class UEBillboard
{
    public long nativePtr;
    
    public UEBillboard(long inNativePtr)
    {
        nativePtr = inNativePtr;
        TapBillboard.registerCustomLinkListener(new CustomLinkListener() {
            @Override
            public void onCustomUrlClick(String url) {
                NotifyCustomUrl(nativePtr, url);
            }
        });
        TapBillboard.registerOutputStateListener(new OutputStateListener() {
            @Override
            public void onPlayVoice() {
                NotifyAudioStatusChanged(nativePtr, true);
            }
            @Override
            public void onStopVoice() {
                NotifyAudioStatusChanged(nativePtr, false);
            }
        });
    }
    
    public void init(String clientId, String clientToken, String serverUrl, String billboardUrl, String[] dimensionString, int region)
    {
         Set<Pair<String, String>> dimensionSet = new HashSet<>();
         for (int i = 0; i < dimensionString.length; i += 2)
         {
             dimensionSet.add(Pair.create(dimensionString[i], dimensionString[i+1]));
         }
         
         TapBillboardConfig billboardConfig = new TapBillboardConfig.Builder()
                 .withDimensionSet(dimensionSet) 
                 .withServerUrl(billboardUrl)
                 .build();
         
         TapConfig.Builder builder = new TapConfig.Builder()
                 .withAppContext(GameActivity.Get())
                 .withClientId(clientId)
                 .withClientToken(clientToken)
                 .withServerUrl(serverUrl)
                 .withBillboardConfig(billboardConfig)
                 .withRegionType(region == 0 ? TapRegionType.CN : TapRegionType.IO);
                 
         TapConfig tdsConfig = builder.build();
         
         GameActivity.Get().runOnUiThread(new Runnable()
         {
             @Override
             public void run()
             {
                TapBillboard.init(tdsConfig);
             }
         });
    }
    
    public void openPanel()
    {
        GameActivity.Get().runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
               TapBillboard.openPanel(GameActivity.Get(), new Callback<Void>() {
                   @Override
                   public void onError(TapBillboardException tapBillboardException) {
                       NotifyNavigateOpenFailed(nativePtr, tapBillboardException.code, tapBillboardException.message);
                   }
               
                   @Override
                   public void onSuccess(Void result) {
                       NotifyNavigateOpenSuccess(nativePtr);
                   }
               },new PanelShowStateListener() {
                   @Override
                   public void onClose() {
                       NotifyNavigateClosed(nativePtr);
                   }
               });
            }
        });
    }
    
    public void openSplashPanel()
    {
         GameActivity.Get().runOnUiThread(new Runnable()
         {
             @Override
             public void run()
             {
                TapBillboard.openSplashPanel(GameActivity.Get(), new Callback<Void>() {
                                @Override
                                public void onError(TapBillboardException tapBillboardException) {
                                    NotifySplashOpenFailed(nativePtr, tapBillboardException.code, tapBillboardException.message);
                                }
                            
                                @Override
                                public void onSuccess(Void result) {
                                    NotifySplashOpenSuccess(nativePtr);
                                }
                            },new PanelShowStateListener() {
                                @Override
                                public void onClose() {
                                    NotifySplashClosed(nativePtr);
                                }
                            });
             }
         });
    }
    
    public void closeSplashPanel()
    {
                 GameActivity.Get().runOnUiThread(new Runnable()
                 {
                     @Override
                     public void run()
                     {
         TapBillboard.closeSplashPanel();
                     
                     }
                     });
    }
    
    public void startFetchMarqueeData()
    {
                     GameActivity.Get().runOnUiThread(new Runnable()
                     {
                         @Override
                         public void run()
                         {
        TapBillboard.startFetchMarqueeData(GameActivity.Get());
                         }
                         });
    }
    
    public void stopFetchMarqueeData(boolean closeNow)
    {
              GameActivity.Get().runOnUiThread(new Runnable()
              {
                  @Override
                  public void run()
                  {
         TapBillboard.stopFetchMarqueeData(closeNow);
                  }
                  });
    }
    
    public native void NotifyCustomUrl(long nativePtr, String Url);
    
    public native void NotifyAudioStatusChanged(long nativePtr, boolean newPlaying);
    
    public native void NotifyNavigateOpenSuccess(long nativePtr);
    public native void NotifyNavigateOpenFailed(long nativePtr, int code, String message);
    public native void NotifyNavigateClosed(long nativePtr);
    
    public native void NotifySplashOpenSuccess(long nativePtr);
    public native void NotifySplashOpenFailed(long nativePtr, int code, String message);
    public native void NotifySplashClosed(long nativePtr);
}

