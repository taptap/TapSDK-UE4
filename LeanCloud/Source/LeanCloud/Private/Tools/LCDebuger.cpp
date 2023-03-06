#include "Tools/LCDebuger.h"

DEFINE_LOG_CATEGORY_STATIC(LeanCloudLog, Log, All);


void LCDebuger::DisplayLog(const FString& Info) {
	UE_LOG(LeanCloudLog, Display, TEXT("%s"), *Info);
}

void LCDebuger::WarningLog(const FString& Info) {
	UE_LOG(LeanCloudLog, Warning, TEXT("%s"), *Info);
}

void LCDebuger::ErrorLog(const FString& Info) {
	UE_LOG(LeanCloudLog, Error, TEXT("%s"), *Info);
}

void LCDebuger::DisplayShow(const FString& Info, float Duration) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Green, Info, true, FVector2D(2, 2));
	}
	DisplayLog(Info);
}

void LCDebuger::WarningShow(const FString& Info, float Duration) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Info, true, FVector2D(2, 2));
	}
	WarningLog(Info);
}

void LCDebuger::ErrorShow(const FString& Info, float Duration) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Red, Info, true, FVector2D(2, 2));
	}
	ErrorLog(Info);
}