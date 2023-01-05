#pragma once
#include "TUType.h"

class TAPBOOTSTRAP_API FTapBootstrap {
public:
	/**
	 * @brief TapBootstrap 初始化方法
	 * @param InConfig 初始化配置参数
	 */
	static void Init(const FTUConfig& InConfig);
};
