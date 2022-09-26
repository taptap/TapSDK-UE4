#pragma once

namespace TUDBEnum
{
// 	UENUM(BlueprintType)
// 	enum class LoginType: uint8
// 	{
// 		Default = uint8(-1) UMETA(DisplayName = "Default"),
// 		Guest  = 0 UMETA(DisplayName = "Guest"),
// 		TapTap = 5 UMETA(DisplayName = "TapTap"),
// 	};
	
	enum EventType
	{
		Normal = 0,           // 普通事件
		Custom,               // 自定义事件
		Identify,             // 设备/用户 登录事件
	};

	enum OperationType
	{
		Track = 0,            // 事件追踪
		Initial,              // 初始操作
		Add,                  // 累加操作
		Update,               // 更新操作
	};

	
}
