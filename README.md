# TapSDK2-UE 

## 支持平台

* Android
* iOS
* Windows（部分插件支持）
* macOS（部分插件支持）


## 前提条件

* 安装 **UE 4.26** 及以上版本
* IOS **12** 或更高版本 
* Android MinSDK 为 **API21** 或更高版本
* macOS 为 **10.14.0** 或更高版本
* Windows 为 **Windows 7** 或更高版本

## 安装插件

* 下载 **TapSDK.zip** 解压后按需将所需模块的文件夹 Copy 到项目的 Plugins 目录中

## iOS 配置

下列配置仅供参考，具体根据插件需要配置

在 项目设置 > Platform > iOS > Additional Plist data 中可以填入一个字符串，复制以下代码并且替换其中的 `ClientId` 以及授权文案。

⚠️ 注意 `<string>tt{ClientId}</string>` 中的 `tt` 要保留，仅替换 `{ClientId}` 为游戏在开发者中心应用配置中的 Client ID。

```xml
<key>CFBundleURLTypes</key>
    <array>
        <dict>
            <key>CFBundleTypeRole</key>
            <string>Editor</string>
            <key>CFBundleURLName</key>
            <string>taptap</string>
            <key>CFBundleURLSchemes</key>
            <array>
                <string>tt{ClientId}</string>
            </array>
        </dict>
    </array>
<key>LSApplicationQueriesSchemes</key>
<array>
    <string>tapiosdk</string>
    <string>tapsdk</string>
</array>
<key>NSPhotoLibraryUsageDescription</key>
<string>{相册权限申请文案}</string>
<key>NSCameraUsageDescription</key>
<string>{相机权限申请文案}</string>
<key>NSMicrophoneUsageDescription</key>
<string>{麦克风权限申请文案}</string>
<key>NSUserTrackingUsageDescription<key>
<string>{数据追踪权限申请文案} </string>
```

权限描述

> NSPhotoLibraryUsageDescription: 相册权限，用于内嵌动态

> NSCameraUsageDescription: 相机权限，用于内嵌动态

> NSMicrophoneUsageDescription: 麦克风权限，用于内嵌动态 

> NSUserTrackingUsageDescription: 数据追踪，用于 TapDB

## **TapSDK** 目前包含插件

* [TapBootstrap](TapBootstrap/README.md)
* [TapLogin](TapLogin/README.md)
* [TapDB](TapDB/README.md)
* [AntiAddiction](AntiAddiction/README.md)




