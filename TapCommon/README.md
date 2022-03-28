# TapCommon 基础服务

### 地区判断

```c++
UFUNCTION(BlueprintCallable, Category="TapCommon")
static void GetRegionCode();

UPROPERTY(BlueprintAssignable, Category= "TapCommon")
static FGetRegionCode OnRegionCode;
```

## 以下为 Android 独占方法

### 是否安装 TapTap

```c++
UFUNCTION(BlueprintCallable,Category = "TapCommon")
static void IsTapTapInstalled();

/** 是否安装 TapTap */
DECLARE_MULTICAST_DELEGATE_OneParam(FIsTapTapInstalled,const bool);

UPROPERTY(BlueprintAssignable,Category = "TapCommon")
static FIsTapTapInstalled OnTapTapInstalled;
```

### 是否安装 Tap.IO

```c++
UFUNCTION(BlueprintCallable,Category = "TapCommon")
static void IsTapGlobalInstalled();

DECLARE_MULTICAST_DELEGATE_OneParam(FIsTapGlobalInstalled,const bool);

UPROPERTY(BlueprintAssignable,Category = "TapCommon")
static FIsTapGlobalInstalled OnTapGlobalInstalled;
```

### 在 TapTap 中更新游戏

```c++
UFUNCTION(BlueprintCallable,Category = "TapCommon")
static void UpdateGameInTapTap(FString appId);

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateGameInTapTap,const bool);

UPROPERTY(BlueprintAssignable,Category = "TapCommon")
static FUpdateGameInTapTap OnUpdateGameInTapTap;
```

### 在 Tap.IO 中更新游戏

```c++
UFUNCTION(BlueprintCallable,Category = "TapCommon")
static void UpdateGameInTapGlobal(FString appId);

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateGameInTapGlobal,const bool);

UPROPERTY(BlueprintAssignable,Category = "TapCommon")
static FUpdateGameInTapGlobal OnUpdateGameInTapGlobal;
```

### 在 TapTap 打开游戏

```c++
UFUNCTION(BlueprintCallable, Category = "TapCommon")
static void OpenReviewInTapTap(FString appId);

DECLARE_MULTICAST_DELEGATE_OneParam(FOpenReviewInTapTap,const bool);

UPROPERTY(BlueprintAssignable,Category = "TapCommon")
static FOpenReviewInTapTap OnOpenReviewInTapTap;
```

### 在 Tap.IO 打开游戏

```c++
UFUNCTION(BlueprintCallable,Category = "TapCommon")
static void OpenReviewInTapGlobal(FString appId);

DECLARE_MULTICAST_DELEGATE_OneParam(FOpenReviewInTapGlobal,const bool);

UPROPERTY(BlueprintAssignable,Category = "TapCommon")
static FOpenReviewInTapGlobal OnOpenReviewInTapGlobal;
```



