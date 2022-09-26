#include "AAUChinaConfigModel.h"

#include "TUDebuger.h"

#include "Server/AAUStorage.h"

TSharedPtr<FAAUChinaConfigModel> FAAUChinaConfigModel::CurrentModel = nullptr;

static FString AAConfigJsonPath = FPaths::ProjectPluginsDir() / TEXT("AntiAddiction/Content") / TEXT("Assets/AntiAddictionConfig.json");


FAAUChinaHealthReminderWordTip FAAUChinaConfigModel::GetNonageHealthTip(AAUHealthTipType Type) {
	for (auto Health_Reminder_Word : ui_config.health_reminder_words) {
		if (Health_Reminder_Word.account_type == AAURealNameAccountTypeChild ||
			Health_Reminder_Word.account_type == AAURealNameAccountTypeTeen ||
			Health_Reminder_Word.account_type == AAURealNameAccountTypeYoung) {
			for (auto NonageHealthTip : Health_Reminder_Word.tips) {
				if (NonageHealthTip.type == Type) {
					return NonageHealthTip;
				}
			}
		}
	}
	return FAAUChinaHealthReminderWordTip();
}

void FAAUChinaConfigModel::SaveToLocal(TSharedPtr<FAAUChinaConfigModel> Model) {
	CurrentModel = Model;
	TUDataStorage<FAAUStorage>::SaveStruct(FAAUStorage::AAConfigFile, Model, true);
}

TSharedPtr<FAAUChinaConfigModel> FAAUChinaConfigModel::GetLocalModel() {
	if (CurrentModel.IsValid()) {
		return CurrentModel;
	}
	// 先读取上次保存服务的配置，如果没有的话，在读取插件中预置的配置。
	TSharedPtr<FAAUChinaConfigModel> ModelPtr = TUDataStorage<FAAUStorage>::LoadStruct<FAAUChinaConfigModel>(FAAUStorage::AAConfigFile);
	if (!ModelPtr.IsValid()) {
		FString JsonStr;
		if (FFileHelper::LoadFileToString(JsonStr, *AAConfigJsonPath)) {
			ModelPtr = TUJsonHelper::GetUStruct<FAAUChinaConfigModel>(JsonStr);
		}
		else {
			TUDebuger::ErrorLog("Can not found AntiAddictionConfig json");
		}
	}

	if (!ModelPtr.IsValid()) {
		TUDebuger::ErrorLog("AntiAddiction Can not Get Local Model");
	}
	CurrentModel = ModelPtr;
	return ModelPtr;
}
