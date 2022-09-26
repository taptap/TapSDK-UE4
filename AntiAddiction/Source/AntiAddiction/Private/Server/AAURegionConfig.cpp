#include "AAURegionConfig.h"

#include "AAUImpl.h"
#include "TUDebuger.h"


class AAURegionConfigCN: public AAURegionConfig
{
public:
	virtual FString BaseHost() {
		return "https://tds-tapsdk.cn.tapapis.com";
	}
	virtual FString GetRSAPublicKey() {
		return "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1pM6yfulomBTXWKiQT5gK9fY4hq11Kv8D+ewum25oPGReuEn6dez7ogA8bEyQlnYYUoEp5cxYPBbIxJFy7q1qzQhTFphuFzoC1x7DieTvfZbh+b60psEottrCD8M0Pa3h44pzyIp5U5WRpxRcQ9iULolGLHZXJr9nW6bpOsyEIFG5tQ7qCBj8HSFoNBKZH+5Cwh3j5cjmyg55WdJTimg9ysbbwZHYmI+TFPuGo/ckHT6j4TQLCmmxI8Qf5pycn3/qJWFhjx/y8zaxgn2hgxbma8hyyGRCMnhM5tISYQv4zlQF+5RashvKa2zv+FHA5DALzIsGXONeTxk6TSBalX5gQIDAQAB";
	}
};

class AAURegionConfigVietnam: public AAURegionConfig
{
public:
	virtual FString BaseHost() {
		return "https://tds-account.intl.tapapis.com";
	}
	virtual FString GetRSAPublicKey() {
		return "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA708T4I+a6wkvT7kn16HY9VrpBE3ay8/nNtaLVFNj/LVBVB6LyIHdU+XIIwi9nj9+I1+a0R2lBds6yKDy6uiwctAwhEHFcKKMmjbdfL0db8bACflASNdrAodw38i7SjzdDrlFiFvJiktkUWnSswaLLPpan/1K3fKo5GgzBtQd8Fe4GQYJ5ghePjA4vVHrpI5rBa9Ca0Ji2YnSOwYv9lFljMCKDYoTzn/Ctsq5vzN+ZGomjz+cATIbA8+zSek+XoGltZvQEWyBtjHDK/pkzb58Kc0zAnEmMQPPtHa0pCU1moMXKIiPvw+YXEVxyvOCUKLAHnzhJNTPlzZzKWtz9VGktQIDAQAB";
	}
};

TSharedPtr<AAURegionConfig> AAURegionConfig::Instance = nullptr;

FString AAURegionConfig::GetRSAPublicKey() {
	return "";
}

FString AAURegionConfig::RealNameUrl()
{
	return BaseHost() + "/real-name/v1/";
}

FString AAURegionConfig::AntiAddictionUrl()
{
	return BaseHost() + "/anti-addiction/v1/";
}

FString AAURegionConfig::TDSPublicConfigUrl()
{
	return "https://tds-public-config-sh.oss-cn-shanghai.aliyuncs.com";
}


TSharedPtr<AAURegionConfig>& AAURegionConfig::Get() {
	if (!Instance.IsValid()) {
		if (AAUImpl::Config.Region == EAAURegion::China) {
			Instance = MakeShareable(new AAURegionConfigCN);
		} else {
			Instance = MakeShareable(new AAURegionConfigVietnam);
		}
	}
	return Instance;
}