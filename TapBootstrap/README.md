# TapBootstrap 启动器

TapBootstrap 用于 TapSDK 中的内建账户登陆、用户信息获取等功能

## 支持平台

* Android
* iOS

## 前提条件

* 安装 **UE 4.26** 及以上版本
* IOS **12** 或更高版本 
* Android MinSDK 为 **API21** 或更高版本

## 安装插件

* 下载 **TapSDK.zip** 解压后将 `TapBootstrap`、 `TapLogin`、`TapCommon` 文件夹 `Copy` 到项目的 `Plugins` 目录中
* 重启 Unreal Editor
* 打开 **编辑 > 插件 > 项目 > TapTap**，开启 `TapBootstrap`、 `TapLogin` 模块


### iOS 配置

在 **项目设置 > Platform > iOS > Additional Plist data** 中可以填入一个字符串，复制以下代码并且替换其中的`ClientId`以及授权文案。

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
```

如果有 `TapDB` 模块，那么还需要加上：
```xml
<key>NSUserTrackingUsageDescription<key>
<string>{数据追踪权限申请文案} </string>
```

## TapBootstrap使用

### 依赖所需模块
在 **Project.Build.cs** 中添加所需模块:
```c#
PublicDependencyModuleNames.AddRange(new string[] { "Core",
	"CoreUObject",
	"Engine",
	"Json",
	"InputCore",
	"JsonUtilities",
	"SlateCore",
	"TapCommon",
	"TapBootstrap",
	"TapLogin"
});
```

### 导入头文件
```cpp
#include "TapUEBootstrap.h"
#include "TapUECommon.h"
```

### 初始化

`TapBootstrap`会把`TapLogin`模块一起初始化，如果插件中包含`TapDB`并且`DBConfig.Enable = true`，那么也会把`TapDB`初始化，相关模块无需再次初始化。
```cpp
FTUConfig Config;
Config.ClientID = ClientID;
Config.ClientToken = ClientToken;
Config.ServerURL = ServerURL;
Config.RegionType = RegionType;
Config.DBConfig.Enable = true;  // 如果该项为true，那么会将TapDB一起初始化了
Config.DBConfig.Channel = Channel;
Config.DBConfig.GameVersion = GameVersion;
Config.DBConfig.AdvertiserIDCollectionEnabled = AdvertiserIDCollectionEnabled;
TapUEBootstrap::Init(Config);
```

### TapTap登录
```cpp
TapUEBootstrap::Login({TUType::PermissionScope::Profile}, [](const FTDSUser& User) {
	// 登录成功
}, [](const FTUError& Error) {
	// 登录失败
});
```

### 匿名登录
```cpp
TapUEBootstrap::AnonymouslyLogin([](const FTDSUser& User) {
	// 登录成功
}, [](const FTUError& Error) {
	// 登录失败
});
```

### 登出
```cpp
TapUEBootstrap::Logout();
```

### 获取当前用户
```cpp
TSharedPtr<FTDSUser> UserPtr = TapUEBootstrap::GetUser();
if (UserPtr.IsValid()) {
	// TDS User
} else {
	// No User
}
```

### 设置多语言
```cpp
UENUM(BlueprintType)
enum class ELanguageType : uint8
{
	AUTO = 0,   // 国内默认简体中文
	ZH,			// 简体中文
	EN,			// 英文，海外默认语言
	ZHTW,		// 繁体中文
	JA,			// 日语
	KO,			// 韩语
	TH,			// 泰文
	ID,			// 印尼文
};
TapUEBootstrap::SetPreferLanguage(LangType);
```
