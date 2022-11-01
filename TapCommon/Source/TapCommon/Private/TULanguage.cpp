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
				LanguageMap.Add(Name, ELanguageType::ZHTW);
			} else {
				LanguageMap.Add(Name, ELanguageType::ZH);
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

FString TULanguage::GetLanguageString()
{
	const ELanguageType Type = GetCurrentType();
	switch (Type)
	{
	case ELanguageType::AUTO:
		return TEXT("zh_CN");
	case ELanguageType::ZH:
		return TEXT("zh_CN");
	case ELanguageType::EN:
		return TEXT("en_US");
	case ELanguageType::ZHTW:
		return TEXT("zh-TW");
	case ELanguageType::JA: 
		return TEXT("ja_JP");
	case ELanguageType::KO: 
		return TEXT("ko_KR");
	case ELanguageType::TH: 
		return TEXT("th_TH");
	case ELanguageType::ID: 
		return TEXT("id_ID");
	default:
		ensure(false);
		return TEXT("");
	}
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




