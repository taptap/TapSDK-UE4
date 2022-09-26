#include "AAUVietnamConfigModel.h"
#include "TUDebuger.h"
#include "Server/AAUStorage.h"

TSharedPtr<FAAUVietnamConfigModel> FAAUVietnamConfigModel::CurrentModel = nullptr;

static FString AAVietnamConfigJsonPath = FPaths::ProjectPluginsDir() / TEXT("AntiAddiction/Content") / TEXT("Assets/VietnamConfig.json");

void FAAUVietnamConfigModel::SaveToLocal(TSharedPtr<FAAUVietnamConfigModel> Model) {
	CurrentModel = Model;
	TUDataStorage<FAAUStorage>::SaveStruct(FAAUStorage::AAVienamConfigFile, Model, true);
}

TSharedPtr<FAAUVietnamConfigModel> FAAUVietnamConfigModel::GetLocalModel() {
	if (CurrentModel.IsValid()) {
		return CurrentModel;
	}
	// 先读取上次保存服务的配置，如果没有的话，在读取插件中预置的配置。
	TSharedPtr<FAAUVietnamConfigModel> ModelPtr = TUDataStorage<FAAUStorage>::LoadStruct<FAAUVietnamConfigModel>(FAAUStorage::AAVienamConfigFile);
	if (!ModelPtr.IsValid()) {
		FString JsonStr;
		if (FFileHelper::LoadFileToString(JsonStr, *AAVietnamConfigJsonPath)) {
			ModelPtr = TUJsonHelper::GetUStruct<FAAUVietnamConfigModel>(JsonStr);
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
