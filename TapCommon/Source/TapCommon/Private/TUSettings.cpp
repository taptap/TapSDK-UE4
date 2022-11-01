#include "TUSettings.h"

TUSettings * TUSettings::Instance = nullptr;

void TUSettings::SetUILevel(int32 Level) {
	Get().UILevel = Level;
}

int32 TUSettings::GetUILevel() {
	return Get().UILevel;
}

void TUSettings::SetGameInstance(TWeakObjectPtr<UGameInstance> GameInstance) {
	Get().GameInstancePtr = GameInstance;
}

TWeakObjectPtr<UGameInstance> TUSettings::GetGameInstance() {
	TWeakObjectPtr<UGameInstance> Ptr = Get().GameInstancePtr;
	if (Ptr.IsValid()) {
		return Ptr;
	}
	if (GEngine) {
		const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
		for (const FWorldContext& WorldContext: WorldContexts) {
			if (WorldContext.OwningGameInstance) {
				Ptr = WorldContext.OwningGameInstance;
				break;
			}
		}
		Get().GameInstancePtr = Ptr;
	}
	return Ptr;
}

void TUSettings::SetBlockBeforeLaunchUrl(TFunction<void()> Block) {
	Get().BlockBeforeLaunchUrl = Block;
}

TFunction<void()> TUSettings::GetBlockBeforeLaunchUrl() {
	return Get().BlockBeforeLaunchUrl;
}

TUSettings& TUSettings::Get() {
	if (Instance == nullptr) {
		Instance = new TUSettings();
	}
	return *Instance;
}
