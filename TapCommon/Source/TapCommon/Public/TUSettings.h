#pragma once
#include "TUType.h"

class TAPCOMMON_API TUSettings {
	
public:
	// 设置Widget控件的层级。
	static void SetUILevel(int32 Level);
	static int32 GetUILevel();

	static void SetGameInstance(TWeakObjectPtr<UGameInstance> GameInstance);
	static TWeakObjectPtr<UGameInstance> GetGameInstance();
	
private:
	int32 UILevel = 10;
	TWeakObjectPtr<UGameInstance> GameInstancePtr;
	static TUSettings * Instance;
	static TUSettings& Get();
};
