#pragma once
#include "TUType.h"

class TAPCOMMON_API TULanguage {
public:

	static ELanguageType GetNativeType();

	// 获得当前语言
	static ELanguageType GetCurrentType();
	// 设置当前语言
	static void SetCurrentType(ELanguageType Type);

	// DECLARE_EVENT_OneParam(TdsLanguage, LanguageChangedEvent, ELanguageType);
	// LanguageChangedEvent& OnLanguageChanged() { return LanguageChangedEvent; }


private:
	static TULanguage& Get();
	static TULanguage *SingleInstance;

	TMap<FString, ELanguageType> LanguageMap;
	void ParseLanguages();
	ELanguageType CurrentType = ELanguageType::AUTO;
	// LanguageChangedEvent LanguageChangedEvent;
	
};
