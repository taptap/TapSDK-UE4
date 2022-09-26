# TapLogin

## 支持平台

* Android
* iOS
* Windows
* macOS

## 前提条件

* 安装 **UE 4.26** 及以上版本
* IOS **12** 或更高版本 
* Android MinSDK 为 **API21** 或更高版本
* macOS 为 **10.14.0** 或更高版本
* Windows 为 **Windows 7** 或更高版本

## 安装插件

* 下载 **TapSDK.zip** 解压后将 `TapLogin`、`TapCommon` 文件夹 `Copy` 到项目的 `Plugins` 目录中
* 重启 Unreal Editor
* 打开 **编辑 > 插件 > 项目 > TapTap**，开启 `TapLogin` 模块


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

## TapLogin使用

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
	"TapLogin"
});
```

### 导入头文件
```cpp
#include "TapUELogin.h"
#include "TapUECommon.h"
```

### 初始化
```cpp
FTULoginConfig Config;
Config.ClientID = ClientID;
Config.RegionType = RegionType;
TapUELogin::Init(Config);
```

### 登录
```cpp
TapUELogin::Login([](const TUAuthResult& Result) {
    if (Result.GetType() == TUAuthResult::Success) {
        // 登录成功
    }
    else if (Result.GetType() == TUAuthResult::Cancel) {
        // 登录取消
    }
    else {
        // 登录失败
    }
});
```
如果需要指定权限，可以调用：
```cpp
// 权限参考TUType::PermissionScope
TapUELogin::Login({TUType::PermissionScope::BasicInfo}, [](const TUAuthResult& Result) {
    if (Result.GetType() == TUAuthResult::Success) {
        // 登录成功
    }
    else if (Result.GetType() == TUAuthResult::Cancel) {
        // 登录取消
    }
    else {
        // 登录失败
    }
});
```

### 登出
```cpp
TapUELogin::Logout();
```

### 获取Token
```cpp
TSharedPtr<FTUAccessToken> AccessToken = TapUELogin::GetAccessToken();
if (AccessToken.IsValid())
{
	// 获得Token
}
else
{
	// Token无效（未登录状态）
}
```

### 更新用户信息
值得注意的是，如果登录的权限没有：`TUType::Profile`，那么获取不到用户信息。
```cpp
TapUELogin::FetchProfile([](TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error) {
	if (ModelPtr.IsValid())
	{
		// 请求个人信息成功
	}
	else
	{
		// 请求个人信息失败
	}
});
```

### 获取（本地）用户信息

值得注意的是，如果登录的权限没有：`TUType::Profile`，那么获取不到用户信息。
```cpp
	TSharedPtr<FTULoginProfileModel> Profile = TapUELogin::GetProfile();
	if (Profile.IsValid())
	{
		// 获取个人信息成功
	}
	else
	{
		// 获取个人信息失败
	}
```

### 获取篝火测试资格

```cpp
TapUELogin::GetTestQualification([](bool IsQualified, const FTUError& Error)
{
	if (IsQualified)
	{
			// 有资格
	}
	else
	{
			// 没有资格
	}
});
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
TapUECommon::SetLanguage(LangType);
```






