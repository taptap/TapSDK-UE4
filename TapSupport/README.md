# TapSupport

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

## 如何使用

### 安装插件

* 下载 **TapSDK.zip** 解压后将 `TapSupport`、`TapCommon` 文件夹 `Copy` 到项目的 `Plugins` 目录中
* 重启 Unreal Editor
* 打开 **编辑 > 插件 > 项目 > TapTap**，开启 `TapDB` 模块

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
	"TapSupport"
});
```

## API使用

### 导入头文件
```cpp
#include "TapUESupport.h"
```

### 初始化 TapSupport

在初始化 SDK 之前，有一些准备工作需要完成：

1. 在开发者中心厂商页面启用客服系统。客服系统是厂商维度的，一个厂商下如果有多个游戏启用一次即可。
2. 每个厂商会被分配一个专属域名，需要记下待用。如果你想要使用厂商自己的域名，也可以在开发者中心客服系统模块中绑定。
3. 前往客服后台，为当前游戏创建一个分类（设置 > 管理 > 工单分类），记下这个分类的 ID，我们称其为产品 ID（配置分类需要客服角色，客服需要先在开发者中心邀请）。


然后使用以下代码初始化 TapSupport 模块：
```cpp
FTapSupportConfig Config;
Config.ServerUrl = TEXT("https://please-replace-with-your-customized.domain.com");
Config.ProductID = TEXT("产品 ID");
TapUESupport::Init(Config);
```

上述代码示例中，`please-replace-with-your-customized.domain.com` 就是准备工作 2 中获取或绑定的域名。`产品 ID` 是在准备工作 3 中新建的分类的 ID。

### 匿名登录

匿名登录是指游戏使用一个只有当前玩家能拿到的字符串作为匿名身份标识（ID）登录客服系统。
```cpp
TapUESupport::LoginAnonymously("uuid");
```


### 清除登录状态
```cpp
TapUESupport::Logout();
```

### 打开客服页面

```cpp
TapUESupport::OpenSupportView("/", nullptr, nullptr);
```

### 关闭客服页面(PC无效)

玩家可以在客服页面内点击关闭按钮退出。但在特定场景下，游戏可能需要主动关闭客服页面：
```cpp
TapUESupport::CloseSupportView();
```

### 未读消息通知

当提交的工单有了新的进展（例如有了新的客服回复）时，玩家会产生未读消息。通常在游戏内会在客服入口使用小红点等形式提示玩家有未读消息。SDK 会自动通过轮询获取是否有维度消息，当未读消息状态发生变化时——从无到有或从有到无—— SDK 会通过回调通知开发者：


```cpp
TapUESupport::OnUnreadStatusChanged.BindUObject(this, &YourUObject::OnUnreadStatusChanged);
TapUESupport::OnErrorCallBack.BindUObject(this, &YourUObject::OnErrorCallBack);
```

### 暂停轮询

SDK 内建的轮询机制会智能的调整频率获取未读消息状态。但在有些场景下这些请求依然是没有必要的开销，例如用户在游戏对局期间（此时界面上都没有展示小红点的地方）希望能暂停一切不必要的后台请求。SDK 为此提供了一对 API 来控制轮询：

- `Pause`：暂停轮询
- `Resume`：恢复轮询


> SDK 的轮询策略
> 初始时立即发起一次请求，设定下一次请求间隔 10s。
> 如果某一次未读消息状态与当前状态对比没有变化，增加 10s 间隔时间，直到最大间隔时间 300s。 如果发生了变化，重置间隔时间为 10s。
> 如果用户从未打开过客服页面（WebView），间隔时间增加并恒定为 3600s。
> 调用 Resume 方法会重置轮询为初始状态。
```cpp
TapUESupport::Resume();
TapUESupport::Pause();
```

