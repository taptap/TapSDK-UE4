# TapLicense 付费认证服务

> Android 独占模块

### 设置认证成功回调

```c++
DECLARE_MULTICAST_DELEGATE(FTapLicenseCallback);

UPROPERTY(BlueprintAssignable,Category = "TapLicense")
static FTapLicenseCallback OnLicenseSuccess;
```

###  资格检查

```c++
UFUNCTION(BlueprintCallable,Category = "TapLicense")
static void Check();
```

### DLC 查询
```c++
UFUNCTION(BlueprintCallable,Category = "TapLicense")
static void QueryDLC(TArray<FString> skus);

// DLC 查询委托定义
DECLARE_MULTICAST_DELEGATE_TwoParams(FTapLicenseDLCQueryCallback,const int,const FString &);

UPROPERTY(BlueprintAssignable,Category = "TapLicense")
static FTapLicenseDLCQueryCallback OnQueryCallback;

```

### DLC 购买

```c++
UFUNCTION(BlueprintCallable,Category = "TapLicense")
static void PurchaseDLC(FString sku);

// DLC 购买委托定义
DECLARE_MULTICAST_DELEGATE_TwoParams(FTapLicenseOrderCallback,const FString& ,const int);

UPROPERTY(BlueprintAssignable,Category = "TapLicense")
static FTapLicenseOrderCallback OnOrderCallback;
```