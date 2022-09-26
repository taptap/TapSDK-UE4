#pragma once
#include "AAUType.h"

class ANTIADDICTION_API AntiAddictionUE {
public:
	enum ResultHandlerCode {
		LoginSuccess     = 500,		// 登录成功
		Exited			 = 1000,	// 退出登录
		DurationLimit    = 1050,	// 可玩时长耗尽
		PeriodRestrict   = 1030,	// 达到宵禁时长
		RealNameStop     = 9002,	// 实名过程中点击了关闭实名窗
		SwitchAccount    = 1001,	// 切换账号
	};

	DECLARE_DELEGATE_TwoParams( FCallBack, ResultHandlerCode , const FString& );

	// 回调
	static FCallBack OnCallBack;

	// 防沉迷初始化
	static void Init(const FAAUConfig& Config);

	// 启动防沉迷
	static void Startup(const FString& UserID);

	// 防沉迷退出
	static void Exit();

	// 进入游戏，游戏从后台激活时调用该API
	static void EnterGame();

	// 离开游戏，游戏进入后台时调用
	static void LeaveGame();

	// 获取年龄段；
	static EAAUAgeLimit GetAgeRange();

	/// 获取用户剩余时长（单位：分钟）
	static int GetRemainingTimeInMinutes();

	/// 获取用户剩余时长（单位：秒）
	static int GetRemainingTime();
	

	/// 查询能否支付
	/// - Parameter amount: 支付金额，单位分
	/// - Parameter status: true:可以付费 false:限制消费
	static void CheckPayLimit(int Amount,
		TFunction<void(bool Status)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);

	/// 上报消费结果 
	/// - Parameter amount: 支付金额，单位分
	static void SubmitPayResult(int Amount,
		TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);

	// 获取防沉迷Token
	static FString CurrentToken();

	static void Test();
	
};

