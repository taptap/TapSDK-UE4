#pragma once
#include "TUType.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTapBootstrapInit, const FTUConfig& /** Config */);

class TAPBOOTSTRAP_API FTapBootstrap {
public:
	/**
	 * @brief TapBootstrap 初始化方法
	 * @param InConfig 初始化配置参数
	 */
	static void Init(const FTUConfig& InConfig);

	static FTapBootstrapInit OnBootstrapInit;
};
