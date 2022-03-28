# TapDB 

```c++
#include "TapDB.h"
#include "TapDBBPLibrary.h"
```

### 初始化 TapDB

>  TapDB 初始化可以通过 Unreal Editor 中的 TapTap Bootstrap 进行配置，也可以通过调用接口来进行初始化。

```c++
/**
 *
 * @param clientID TapTap client ID
 * @param channel 游戏渠道
 * @param gameVersion 游戏版本
 * @param isCN 是否是中国大陆
 */ 
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void Init(FString clientID,FString channel,FString gameVersion,bool isCN);
```

### 设置用户

```c++
/**
 * @param userId  用户id
 */ 
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void SetUser(FString userId);

/**
 * 设置 UserId 以及 登陆类型
 * @param userId 用户 ID
 * @param loginType 登陆类型
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void SetUserWithLoginType(FString userId,FString loginType);
```

### 设置 Name

```c++
/**
 * @param name 用户 name
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void SetName(FString name);
```

### 设置等级

```c++
/**
 * @param level 等级
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void SetLevel(int level);
```
### 设置服务
```c++
/**
 * @param server 服务地址
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void SetServer(FString server);
```

### 充值

```c++
/**
 *
 * @param orderÎd 订单id
 * @param product 商品名称
 * @param amount 价格 
 * @param currencyType 货币名称
 * @param payment 充值渠道
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void OnCharge(FString orderId,FString product,int32 amount,FString currencyType,FString payment);
```

### 上报事件
```c++
/*
 * @param eventName 上报事件 key
 * @param properties 上报事件 value （必须为 JSON 字符串）
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void TrackEvent(FString eventName,FString properties);
```

### 注册动态事件 (不支持蓝图)
```c++
/**
 * 注册动态事件
 * @param properties 继承 TapDBDynamicProperties
 */
static void RegisterDynamicProperties(TapDBDynamicProperties* properties);
```

### 注册静态事件
```c++
UFUNCTION(BlueprintCallable,Category = "TapDB")
/**
 * 注册静态事件
 * @param properties 必须为 JSON 字符串
 */
static void RegisterStaticProperties(FString properties);
```

### 取消静态事件
```c++
/**
 * 删除静态事件
 * @param key 必须为已注册的静态事件的 key
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void UnregisterStaticProperty(FString key);
```

### 清除静态事件
```c++
/**
 * 清除所有静态事件
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void ClearStaticProperties();
```

### 设备初始化
```c++
/**
 * @param properties 设置初始化（必须为 JSON 字符串）
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void DeviceInitialize(FString properties);
```

### 设备更新
```c++
/**
 * @param properties 设置更新（必须为 JSON 字符串）
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void DeviceUpdate(FString properties);
```

### 添加设备
```c++
/**
 *
 * @param propertis 添加设备（必须为 JSON 字符串）
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void DeviceAdd(FString properties);
```

### 用户初始化
```c++
/**
 *
 * @param properties 用户初始化 (必须为 JSON 字符串)
 *
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void UserInitialize(FString properties);
```

### 用户更新
```c++
/**
 *
 * @param propertis 用户更新（必须为 JSON 字符串）
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void UserUpdate(FString properties);
```

### 添加用户
```c++
/**
 *
 * @param propertis 添加用户（必须为 JSON 字符串）
 */
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void UserAdd(FString properties);
```

### 清除用户
```c++
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void ClearUser();
```

### 开启 iDFA

> iOS 独占方法

```c++
UFUNCTION(BlueprintCallable,Category = "TapDB")
static void AdvertiserIDCollectionEnabled(bool enable);
```