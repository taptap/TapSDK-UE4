// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "JsonObjectConverter.h"
#include "TUError.h"
#include "TUType.h"
#include "TapBillboardTypes.generated.h"

namespace ETapBillboardTemplate
{
	// static const TCHAR* All = TEXT("all");
	static const TCHAR* Navigate = TEXT("navigate");
	// static const TCHAR* Image = TEXT("image"); 
	static const TCHAR* Splash = TEXT("splash");
	static const TCHAR* Marquee = TEXT("marquee");
}

UENUM()
enum class EBillboardBrowserState : uint8
{
	NotLoad,
	Loading,
	Loaded,
	LoadFailed
};

UENUM()
enum class ETapBillboardUrlType : uint8
{
	Custom,
	AudioOn,
	AudioOff,
	LaunchSystemBrowser,
	LaunchInnerBrowser
};

ETapBillboardUrlType GetBillboardUrlType(const FString& Url);

FString GetLCSyncString();

USTRUCT(BlueprintType)
struct FBadgeDetails
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool show_red_dot = false;

	UPROPERTY(BlueprintReadOnly)
	FString close_button_img;
};

USTRUCT(BlueprintType)
struct FAnnouncementLayout
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString scroll_width_type;
	
	UPROPERTY(BlueprintReadOnly)
	float scroll_width = 0;

	UPROPERTY(BlueprintReadOnly)
	FString position;

	UPROPERTY(BlueprintReadOnly)
	float position_offset_x = 0;
	
	UPROPERTY(BlueprintReadOnly)
	float position_offset_y = 0;
	
	UPROPERTY(BlueprintReadOnly)
	float scroll_margin_x = 0;
	
	UPROPERTY(BlueprintReadOnly)
	float scroll_margin_y = 0;
};

USTRUCT(BlueprintType)
struct FAnnouncementStyleData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FString default_text_color;
	
	UPROPERTY(BlueprintReadOnly)
	FString content_background_color;
	
	UPROPERTY(BlueprintReadOnly)
	bool use_system_font = false;
	
	UPROPERTY(BlueprintReadOnly)
	FString custom_font;
	
	UPROPERTY(BlueprintReadOnly)
	int32 repeat_count = 0;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FString> icon = {{TEXT("url"), TEXT("")}};
	
	UPROPERTY(BlueprintReadOnly)
	FAnnouncementLayout horizontal;
	
	UPROPERTY(BlueprintReadOnly)
	FAnnouncementLayout vertical;
};

USTRUCT(BlueprintType)
struct FAnnouncementGeneralData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int64 id = 0;

	UPROPERTY(BlueprintReadOnly)
	int64 publish_time = 0;

	UPROPERTY(BlueprintReadOnly)
	int64 expire_time = 0;
};

USTRUCT(BlueprintType)
struct FAnnouncementDetailData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int64 id = 0;

	UPROPERTY(BlueprintReadOnly)
	FString content;

	UPROPERTY(BlueprintReadOnly)
	FString type;

	UPROPERTY(BlueprintReadOnly)
	FString Template;

	UPROPERTY(BlueprintReadOnly)
	FString short_title;

	UPROPERTY(BlueprintReadOnly)
	FString long_title;

	UPROPERTY(BlueprintReadOnly)
	FString jump_location;

	UPROPERTY(BlueprintReadOnly)
	FString jump_link;

	UPROPERTY(BlueprintReadOnly)
	int64 publish_time = 0;

	UPROPERTY(BlueprintReadOnly)
	int64 expire_time = 0;
};

template<typename StructType, typename ErrorType = FTUError>
TSharedPtr<ErrorType> ParseTapHttpResponse(FHttpResponsePtr Response, StructType& OutStruct)
{
	if (!Response)
	{
		return MakeShared<ErrorType>(-1, TEXT("Net error"));
	}

	const FString ContentString = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(ContentString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		return MakeShared<ErrorType>(Response->GetResponseCode(), FString::Printf(TEXT("Net error, Deserialize failed, response content: %s"), *ContentString));
	}

	bool bSuccess = false;
	if (!JsonObject->TryGetBoolField(TEXT("success"), bSuccess))
	{
		return MakeShared<ErrorType>(Response->GetResponseCode(), FString::Printf(TEXT("Net error, wrong success field, response content: %s"), *ContentString));
	}

	const TSharedPtr<FJsonObject>* DataObjectP = nullptr;
	if (!(JsonObject->TryGetObjectField(TEXT("data"), DataObjectP) && DataObjectP))
	{
		return MakeShared<ErrorType>(Response->GetResponseCode(), FString::Printf(TEXT("Net error, wrong data field, response content: %s"), *ContentString));
	}
	
	if (!bSuccess)
	{
		TSharedPtr<ErrorType> Error = MakeShared<ErrorType>();
		if (FJsonObjectConverter::JsonObjectToUStruct<ErrorType>(DataObjectP->ToSharedRef(), Error.Get()))
		{
			return Error;
		}
		return MakeShared<ErrorType>(Response->GetResponseCode(), FString::Printf(TEXT("Parse json error failed, response content: %s"), *ContentString));
	}
	
	if (!FJsonObjectConverter::JsonObjectToUStruct<StructType>(DataObjectP->ToSharedRef(), &OutStruct))
	{
		return MakeShared<ErrorType>(Response->GetResponseCode(), FString::Printf(TEXT("Parse json data failed, response content: %s"), *ContentString));
	}
	return nullptr;
}

#if UE_BUILD_SHIPPING
#if !defined(TAP_LOG_REQUEST)
	#define TAP_LOG_REQUEST(HttpRequest)
#endif
#if !defined(TAP_LOG_RESPONSE)
	#define TAP_LOG_RESPONSE(HttpResponse)
#endif
#else
#if !defined(TAP_LOG_REQUEST)
	#define TAP_LOG_REQUEST(HttpRequest) \
		UE_LOG(LogTemp, Log, TEXT("----- %s -----"), ANSI_TO_TCHAR(__FUNCTION__)); \
		UE_LOG(LogTemp, Log, TEXT("%s %s"), *HttpRequest->GetVerb(), *HttpRequest->GetURL()); \
		for (const FString& Header : HttpRequest->GetAllHeaders()) \
		{ \
			UE_LOG(LogTemp, Log, TEXT("-H %s"), *Header); \
		} \
		if (HttpRequest->GetContentLength() > 0) \
		{ \
			FUTF8ToTCHAR Data(reinterpret_cast<const ANSICHAR*>(HttpRequest->GetContent().GetData()), HttpRequest->GetContent().Num()); \
			UE_LOG(LogTemp, Log, TEXT("-d ‘%s’"), *FString(Data.Length(), Data.Get())); \
		} \
		UE_LOG(LogTemp, Log, TEXT(" "));
#endif

#if !defined(TAP_LOG_RESPONSE)
	#define TAP_LOG_RESPONSE(HttpResponse) \
		UE_LOG(LogTemp, Log, TEXT("***** %s *****"), ANSI_TO_TCHAR(__FUNCTION__)); \
		if (HttpResponse) \
		{ \
			UE_LOG(LogTemp, Log, TEXT("%d ‘%s’"), HttpResponse->GetResponseCode(), *HttpResponse->GetContentAsString()); \
		} \
		UE_LOG(LogTemp, Log, TEXT(" "));
#endif
#endif
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCustomLinkClicked, const FString&/** Url */);
DECLARE_DELEGATE_OneParam(FAudioOutputStateChanged, bool /** bPlaying */);
