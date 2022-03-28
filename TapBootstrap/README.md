# TapBootstrap 启动器

TapBootstrap 用于 TapSDK 中的内建账户登陆、用户信息获取等功能

```c++
#include "TapBootstrap.h"
#include "TapBootstrapBPLibrary.h"
```

## 接口描述

### 初始化

如果未在 `Unreal Editor` 开启 `TapBootstrap Auto Init` 选项的话，则需要手动调用该方法

入参描述参考 [启用 TapSDK 插件](../README.md)

```c++
UFUNCTION(BlueprintCallable, meta=(DisplayName = "TapBootstrap init"), Category = "TapBootstrap")
static void Init(FString clientID, FString clientToken, FString serverUrl, bool bIsCN, bool bTapDBEnable,
	                 bool bAdvertiserIDCollectionEnabled, FString gameVersion, FString gameChannel);
```

### TDS 内建账户登陆

在你使用 TDS 内建登陆时调用以下接口:

```c++
/**
 * 调用 TDS 内建账户登陆 (TapTap 登录)
 * @param permissions 登陆权限
 */
UFUNCTION(BlueprintCallable, meta = (DisplayName = "Login by TapTap"), Category = "TapBootstrap")
static void Login(TArray<FString> permissions);

/**
 * 游客(匿名)登录
 */
UFUNCTION(BlueprintCallable, meta = (DisplayName = "Anonymously Login by TapTap"), Category = "TapBootstrap")
static void AnonymouslyLogin();

```

TapTap 登陆提供以下几种委托用于接口登陆回调

```c++
// 登陆成功
DECLARE_MULTICAST_DELEGATE_OneParam(FLoginSuccess, const FTapAccessToken);

// 登陆失败
DECLARE_MULTICAST_DELEGATE_OneParam(FLoginError, const FTapError);

// 登陆取消
DECLARE_MULTICAST_DELEGATE(FLoginCancel);

UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
static FLoginSuccess OnLoginSuccess;

UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
static FLoginError OnLoginError;

UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
static FLoginCancel OnLoginCancel;

```

你需要在你的头文件中定义接口用于绑定以上几种委托

```c++
void OnLoginSuccess(const FTapUser token);

void OnLoginError(const FTapError error);

void OnLoginCancel();
```

接下来，你需要和 TapBootstrapModule 绑定以上定义的接口,这样就能收到 TapBootstrap 登陆接口的回调。

```c++
FTapBootstrapModule::OnLoginSuccess.AddUbObject(this,&YourObject::OnLoginSuccess);
FTapBootstrapModule::OnLoginError.AddUbObject(this,&YourObject::OnLoginError);
FTapBootstrapModule::OnLoginCancel.AddUbObject(this,&YourObject::OnLoginCancel);
```

### 退出登录

```c++
// 接口定义
UFUNCTION(BlueprintCallable, meta = (DisplayName = "TapTap Logout"), Category = "TapBootstrap")
static void Logout();

// 委托定义
DECLARE_MULTICAST_DELEGATE_OneParam(FLogout, const FTapError);

UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
static FLogout OnLogout;

```

### 获取 TDS Token

```c++        
// 导入头文件
#include "TapAccessToken.h"

// 接口定义
UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get TapTap AccessToken"), Category = "TapBootstrap")
static void GetAccessToken();

// 委托定义
DECLARE_MULTICAST_DELEGATE_OneParam(FGetAccessToken, const FTapAccessToken);
    
UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
static FGetAccessToken OnGetAccessToken;

```

### 获取 TDS 用户信息

```c++
// 导入头文件
#include "TapUser.h"

// 接口定义
UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get TapTap Userinfo"), Category = "TapBootstrap")
static void GetUser();

// 获取用户信息成功
DECLARE_MULTICAST_DELEGATE_OneParam(FGetUserSuccess,const FTapUser);

// 获取用户信息失败
DECLARE_MULTICAST_DELEGATE_OneParam(FGetUserError,const FTapError);

UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
static FGetUserSuccess OnGetUserSuccess;

UPROPERTY(BlueprintAssignable,Category = "TapBootstrap")
static FGetUserError OnGetUserError;

```