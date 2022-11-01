#pragma once
#include "TUType.h"

class TAPCOMMON_API TUSettings {
	
public:
	// 设置Widget控件的层级。
	static void SetUILevel(int32 Level);
	static int32 GetUILevel();

	// 设置当前的GameInstance
	static void SetGameInstance(TWeakObjectPtr<UGameInstance> GameInstance);
	static TWeakObjectPtr<UGameInstance> GetGameInstance();

	// 设置打开外部浏览器前的回调（用于解决打开外部浏览器出现意外的状态，比如全屏游戏会覆盖浏览器）
	static void SetBlockBeforeLaunchUrl(TFunction<void()> Block);
	static TFunction<void()> GetBlockBeforeLaunchUrl();
	
private:
	int32 UILevel = 9999;
	TWeakObjectPtr<UGameInstance> GameInstancePtr;
	TFunction<void()> BlockBeforeLaunchUrl = nullptr;
	static TUSettings * Instance;
	static TUSettings& Get();
};
