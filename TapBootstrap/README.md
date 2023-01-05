# TapBootstrap 启动器

TapBootstrap 用于 TapSDK 中的内建账户登陆、用户信息获取等功能

## 支持平台

* Android
* iOS
* macOS
* Windows

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
#include "TapBootstrap.h"
#include "TDSUser.h"
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
FTapBootstrap::Init(Config);
```

### TapTap登录
```cpp
FTDSUser::FDelegate OnSuccess = FTDSUser::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserSuccessCallBack);
FTUError::FDelegate OnError = FTUError::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserErrorCallBack);
FTDSUser::LoginWithTapTap({TUType::PermissionScope::Profile, TUType::PermissionScope::Friend}, OnSuccess, OnError);
```

### 匿名登录
```cpp
FTDSUser::FDelegate OnSuccess = FTDSUser::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserSuccessCallBack);
FTUError::FDelegate OnError = FTUError::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserErrorCallBack);
FTDSUser::LoginAnonymously(OnSuccess, OnError);
```

### 登出
```cpp
FTDSUser::Logout();
```

### 获取当前用户
如果是登出状态，获得结果为`nullptr`
```cpp
TSharedPtr<FTDSUser> User = FTDSUser::GetCurrentUser()
```

### 使用 `session token` 登录一个用户
下面的代码使用 `session token` 登录一个用户（云端会验证 `session token` 是否有效）：
```cpp
FTDSUser::FDelegate OnSuccess = FTDSUser::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserSuccessCallBack);
FTUError::FDelegate OnError = FTUError::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserErrorCallBack);

FString Token = ETB_User_SessionToken->GetText().ToString();
FTDSUser::BecomeWithSessionToken(Token, OnSuccess, OnError);
```

### 检查 session token 是否有效
```cpp
TSharedPtr<FTDSUser> User = FTDSUser::GetCurrentUser();
if (User.IsValid()) {
	User->isAuthenticated();
} else {
}
```

### 第三方账户登录

```cpp
FTDSUser::FDelegate OnSuccess = FTDSUser::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserSuccessCallBack);
FTUError::FDelegate OnError = FTUError::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserErrorCallBack);
	
const FString Platform = ETB_Login_Platform->GetText().ToString();
TSharedPtr<FJsonObject> AuthData = MakeShared<FJsonObject>();
AuthData->SetStringField("openid", "OPENID");
AuthData->SetStringField("access_token", "ACCESS_TOKEN");
AuthData->SetNumberField("expires_in", 7200);
AuthData->SetStringField("refresh_token", "REFRESH_TOKEN");
AuthData->SetStringField("scope", "SCOPE");
FTDSUser::LoginWithAuthData(Platform, AuthData, OnSuccess, OnError);
```

### 绑定第三方账户

```cpp
FTDSUser::FDelegate OnSuccess = FTDSUser::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserSuccessCallBack);
FTUError::FDelegate OnError = FTUError::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserErrorCallBack);
	
UserPtr->AssociateWithAuthData(Platform, AuthData, OnSuccess, OnError);
```

### 解除与第三方账户的关联
```cpp
FTDSUser::FDelegate OnSuccess = FTDSUser::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserSuccessCallBack);
FTUError::FDelegate OnError = FTUError::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserErrorCallBack);
	
UserPtr->DisassociateAuthData(Platform, OnSuccess, OnError);
```

### TapTap 好友排行榜列表
```cpp
FString Name = "world";
int From = 0;
int Limit = 10;
FTDSLeaderBoardRanking::FRankingsDelegate OnSuccess = FTDSLeaderBoardRanking::FRankingsDelegate::CreateLambda([](const TArray<FTDSLeaderBoardRanking>& Rankings) {

});
FTUError::FDelegate OnError = FTUError::FDelegate::CreateUObject(this, &UServiceWidgetBootstrap::OnUserErrorCallBack);
FTDSLeaderBoardRanking::QueryTapFriendsLeaderBoard(Name, From, Limit, OnSuccess, OnError);
```


