# TapDB 

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

* 下载 **TapSDK.zip** 解压后将 `TapDB`、`TapCommon` 文件夹 `Copy` 到项目的 `Plugins` 目录中
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
	"TapDB"
});
```

## API使用

### 导入头文件
```cpp
#include "TapUEDB.h"
```

### 初始化 TapDB

```cpp
FTUDBConfig Config;
Config.ClientId = TEXT("your client id");
Config.Channel = TEXT("分包渠道名称，可为空");
Config.GameVersion = TEXT("游戏版本");
Config.RegionType = ERegionType::CN; // 国内 or 海外
TapUEDB::Init(Config);
```

### 设置用户

```cpp
FString UserId = TEXT("user id"); // 用户唯一标识
FString LoginType = TUDBType::LoginType::TapTap; // 登录方式，参考TUDBType::LoginType，可以不传
TapUEDB::SetUserWithLoginType(UserId, LoginType);
```


### 清除用户
```cpp
TapUEDB::ClearUser();
```

### 设置 Name

```cpp
FString Name = TEXT("Name"); // 用户游戏昵称
TapUEDB::SetName(Name);
```

### 设置等级

```cpp
int32 Level = 100; // 用户游戏等级
TapUEDB::SetLevel(Level);
```

### 设置服务
```cpp
FString Server = TEXT("Server"); // 服务
TapUEDB::SetServer(Server);
```

### 充值

```cpp
/**
 *
 * @param OrderId 订单id
 * @param Product 商品名称
 * @param Amount 价格 
 * @param CurrencyType 货币名称
 * @param Payment 充值渠道
 * @param Properties 自定义参数
 */
TapUEDB::OnCharge(OrderId, Product, Amount, CurrencyType, Payment, Properties);
```

### 上报自定义事件
```cpp
/*
TapUEDB::TrackEvent(EventName, Properties);
```

### 注册动态事件 
```cpp
TapUEDB::RegisterDynamicProperties(PropertiesBlock);
```

### 注册静态事件
```cpp
TapUEDB::RegisterStaticProperties(Properties);
```

### 取消静态事件
```cpp
TapUEDB::UnregisterStaticProperty(Key);
```

### 清除静态事件
```cpp
TapUEDB::ClearStaticProperties();
```

### 设备初始化
```cpp
TapUEDB::DeviceInitialize(Properties);
```

### 设备更新
```cpp
TapUEDB::DeviceUpdate(Properties);
```

### 添加设备
```cpp
TapUEDB::DeviceAdd(Properties);
```

### 用户初始化
```cpp
TapUEDB::UserInitialize(Properties);
```

### 用户更新
```cpp
TapUEDB::UserUpdate(Properties);
```

### 添加用户
```cpp
TapUEDB::UserAdd(Properties);
```


### 开启 iDFA

> iOS 独占方法

```cpp
TapUEDB::AdvertiserIDCollectionEnabled(true);
```