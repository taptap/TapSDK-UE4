#pragma once

class AAULoginResult {
public:
	enum State {
		SuccessWithNoLimit,		// 登录成功，成年人，没有时长限制
		SuccessWithLimit,		// 登录成功，未成年人，有时长限制
		FailWithCurfew,			// 登录失败，宵禁时间，未成年不能玩游戏
		FailWithTimeout,		// 登录失败，未成年人可玩时长耗尽，目前国内以宵禁时间为主，暂时用不到这个字段
		FailWithOther,          // 一些其他原因的登录失败，比如被服务端踢出，或者网络请求失败等
	};
	State LoginState;

	int RemainTime = 0;   //   未成年人可玩剩余时长，单位秒

	FString Title; // 实名的宵禁临近文案

	FString Description; // 距离健康保护时间还剩余14分钟，请注意适当休息。
};
