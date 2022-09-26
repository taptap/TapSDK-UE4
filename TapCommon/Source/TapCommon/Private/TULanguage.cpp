#include "TULanguage.h"
#include "TUType.h"
#include "Internationalization/Culture.h"

TULanguage* TULanguage::SingleInstance = nullptr;

TULanguage& TULanguage::Get() {
	if (SingleInstance == nullptr) {
		SingleInstance = new TULanguage();
		SingleInstance->ParseLanguages();
		// FInternationalization::Get().OnCultureChanged().AddLambda([]() {
		// 	SingleInstance->LanguageChangedEvent.Broadcast(SingleInstance->GetCurrentType());
		// });
	}
	return *SingleInstance;
}

void TULanguage::ParseLanguages() {
	TArray<FString> Names = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
	for (auto Name : Names) {
		if (Name.StartsWith("zh")) {
			if (Name == "zh-TW") {
				LanguageMap.Add(Name, ELanguageType::ZH);
			} else {
				LanguageMap.Add(Name, ELanguageType::ZHTW);
			}
		} else if (Name.StartsWith("en")) {
			LanguageMap.Add(Name, ELanguageType::EN);
		} else if (Name.StartsWith("id")) {
			LanguageMap.Add(Name, ELanguageType::ID);
		} else if (Name.StartsWith("ja")) {
			LanguageMap.Add(Name, ELanguageType::JA);
		} else if (Name.StartsWith("ko")) {
			LanguageMap.Add(Name, ELanguageType::KO);
		} else if (Name.StartsWith("th")) {
			LanguageMap.Add(Name, ELanguageType::TH);
		} 
	}
}

ELanguageType TULanguage::GetNativeType() {
	FString Name = FTextLocalizationManager::Get().GetNativeCultureName(ELocalizedTextSourceCategory::Game);
	const ELanguageType * FindKey = Get().LanguageMap.Find(Name);
	if (FindKey == nullptr) {
		if (FTUConfig::Get()->RegionType == ERegionType::CN) {
			return ELanguageType::ZH;
		} else {
			return ELanguageType::EN;
		}
	}
	return *FindKey;
}

ELanguageType TULanguage::GetCurrentType() {
	if (Get().CurrentType != ELanguageType::AUTO) {
		return Get().CurrentType;
	}
	FString Name = FInternationalization::Get().GetCurrentCulture().Get().GetName();
	const ELanguageType * FindKey = Get().LanguageMap.Find(Name);
	if (FindKey == nullptr) {
		if (FTUConfig::Get()->RegionType == ERegionType::CN) {
			Get().CurrentType = ELanguageType::ZH;
		} else {
			Get().CurrentType = ELanguageType::EN;
		}
		return Get().CurrentType;
	}
	return *FindKey;
}

void TULanguage::SetCurrentType(ELanguageType Type) {
	Get().CurrentType = Type;
}




