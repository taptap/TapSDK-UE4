#pragma once
#include "CoreMinimal.h"
#include "AAUVietnamConfigModel.generated.h"

USTRUCT()
struct FAAUVietnamRealnameInfoUI
{
	GENERATED_BODY()

	UPROPERTY()
	FString title;

	UPROPERTY()
	FString description;

	UPROPERTY()
	FString button;

	UPROPERTY()
	FString submit_success_message;

	UPROPERTY()
	FString birthdate_invalidate_message;
	
};

USTRUCT()
struct FAAUVietnamHealthReminderUI
{
	GENERATED_BODY()

	UPROPERTY()
	FString title;

	UPROPERTY()
	FString description;

	UPROPERTY()
	FString button_exit;

	UPROPERTY()
	FString button_switch;
	
};

USTRUCT()
struct FAAUVietnamPayReminderUI
{
	GENERATED_BODY()

	UPROPERTY()
	FString button_confirm;
	
};


USTRUCT()
struct FAAUVietnamUIConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FAAUVietnamRealnameInfoUI input_realname_info;

	UPROPERTY()
	FAAUVietnamHealthReminderUI health_reminder;

	UPROPERTY()
	FAAUVietnamPayReminderUI pay_reminder;
	
};

USTRUCT()
struct FAAUVietnamConfigModel
{
	GENERATED_BODY()

	UPROPERTY()
	bool upload_user_action = false;
	
	UPROPERTY()
	FAAUVietnamUIConfig ui_config;

	
	static void SaveToLocal(TSharedPtr<FAAUVietnamConfigModel> Model);
	// 理论上一定是有值的，可以不用判空。
	static TSharedPtr<FAAUVietnamConfigModel> GetLocalModel();

private:
	static TSharedPtr<FAAUVietnamConfigModel> CurrentModel;

};
