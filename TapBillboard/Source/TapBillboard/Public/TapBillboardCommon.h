// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "TUError.h"
#include "TapBillboardTypes.h"

enum class ELanguageType : uint8;
struct FTUConfig;
class UTapBillboardBrowserNavigate;
class UTapBillboardBrowserSplash;
class UTapBillboardMarquee;
class UTapBillboardBrowserPopup;

DECLARE_DELEGATE_OneParam(FTapBadgeDetailsResult, const FBadgeDetails& /** BadgeDetails */);
DECLARE_DELEGATE_OneParam(FAnnouncementStyleDataResult, const FAnnouncementStyleData& /** StyleData */);
DECLARE_DELEGATE_OneParam(FAnnouncementGeneralDataResult, const TArray<FAnnouncementGeneralData>& /** GeneralData */);
DECLARE_DELEGATE_OneParam(FAnnouncementDetailsDataResult, const TArray<FAnnouncementDetailData>& /** DetailsData */);



/**
 * TapBillboard Interface 
 */
class TAPBILLBOARD_API FTapBillboardCommon : public FGCObject
{
public:
	FTapBillboardCommon();
	
	/**
	 * @brief Init billboard module.
	 * @param InConfig billboard config to use
	 */
	virtual void Init(const FTUConfig& InConfig);

	/**
	 * @brief Create 'navigate' billboard --> load 'navigate' billboard url --> add to viewport when load success
	 * @param OnSuccess Callback after load url successfully
	 * @param OnFailed Callback when not successfully
	 * @param OnClose Callback when panel closed
	 */
	virtual void OpenPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose);
	
	/**
	 * @brief Get badge details, async return FBadgeDetails
	 * @see FBadgeDetails
	 * @param OnSuccess Get detail success
	 * @param OnFailed Get detail failed
	 */
	void GetBadgeDetails(const FTapBadgeDetailsResult& OnSuccess, const FTapFailed& OnFailed);

	/**
	 * @brief Register callback when custom url clicked in 'navigate' or 'splash' panel
	 * @param LinkClickedCallback Custom url clicked callback delegate
	 * @return The delegate handle use for unregister
	 */
	FDelegateHandle RegisterCustomLinkListener(const FOnCustomLinkClicked::FDelegate& LinkClickedCallback);
	
	/**
	 * @brief Unregister
	 * @param Handle The delegate handle returned by RegisterCustomLinkListener
	 */
	void UnregisterCustomLinkListener(FDelegateHandle Handle);

	/**
	 * @brief Create 'splash' panel --> load 'splash' billboard url --> add to viewport when load success
	 * @param OnSuccess Callback after load url successfully
	 * @param OnFailed Callback when not successfully
	 * @param OnClose Callback when call CloseSplashPanel or expired
	 */
	virtual void OpenSplashPanel(const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed, const FSimpleDelegate& OnClose);
	
	/**
	 * @brief Try close and destroy splash panel 
	 */
	virtual void CloseSplashPanel();

	/**
	 * @brief Start loop timer(300s) fetch unread marquee data, show marquee one by one, remove from viewport if all read.
	 * Until StopFetchMarqueeData call
	 */
	virtual void StartFetchMarqueeData();

	/**
	 * @brief Stop fetch timer
	 * @param bCloseNow If True, remove Marquee widget immediately
	 */
	virtual void StopFetchMarqueeData(bool bCloseNow);

	/**
	 * @brief Register callback when audio state changed in billboard
	 * @param OnAudioStateChanged State changed delegate
	 */
	void RegisterOutputStateListener(const FAudioOutputStateChanged& OnAudioStateChanged);

	/**
	 * @brief Unregister audio state changed callback
	 */
	void UnregisterOutputStateListener();
	
	
	void Rest_SubmitReadingRecord();
	
	void Rest_FetchMarqueeStyle(const FAnnouncementStyleDataResult& OnSuccess, const FTapFailed& OnFailed);
	
	void Rest_FetchUnreadAnnouncementsGeneralData(const TCHAR* Template, const FAnnouncementGeneralDataResult& OnSuccess, const FTapFailed& OnFailed);
	
	void Rest_FetchAnnouncementsDetailData(const TArray<int64>& Ids, const FAnnouncementDetailsDataResult& OnSuccess, const FTapFailed& OnFailed);
	
	void Rest_AnnouncementsMarkRead(const TArray<int64>& Ids, const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed);

	void Rest_SendTraceEvent(const TCHAR* Template, const TMap<FString, FString>& TraceParams);

	void Test_UnmarkCurrentDevice();
	
	
	void LoadBrowserClass();
	
	TSharedPtr<FTUError> CheckConfig() const;

	FString GenerateBillboardUrl(const TMap<FString, FString>& DimensionsMap) const;

	FString GenerateSplashUrl(const TMap<FString, FString>& DimensionsMap, int64 SplashID);

	UTexture2DDynamic* GetMarqueeIconTexture() const;

	const FAnnouncementStyleData& GetMarqueeStyle() const;

	TSubclassOf<UTapBillboardBrowserPopup> GetPopupBrowserClass() const;

	const FAnnouncementDetailData* FindNextMarqueeData(const TArray<int64>& IgnoreIds) const;
	
	void PopMarqueeData(int64 MarqueeId);
	
	void DownloadWebFont(const FString& Url);

	const UFont* GetDownloadFont() const;

	/**
	 * @brief Update marquee scroll speed (PC only)
	 * @param NewSpeed Offset / second
	 */
	void UpdateMarqueeScrollSpeed(float NewSpeed);

	float GetMarqueeSpeed() const;

protected:
	FString GenerateBusinessParams(const TMap<FString, FString>& DimensionsMap) const;

	FString GenerateBusinessParamsSplash(const TMap<FString, FString>& DimensionsMap, int64 SplashId) const;

	FString GenerateTrackParams() const;

	FString GenerateXUA() const;
	
	void InternalCustomUrlClickedCallback(const FString& Url, const TCHAR* Template);

	void InternalAudioOutputStateChangedCallback(bool bPlaying, const TCHAR* Template);

	void NetGetBadgeDetailsCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FTapBadgeDetailsResult OnSuccess, FTapFailed OnFailed);

	void NetFetchMarqueeStyleCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FAnnouncementStyleDataResult OnSuccess, FTapFailed OnFailed);
	
	void NetFetchGeneralDataCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FAnnouncementGeneralDataResult OnSuccess, FTapFailed OnFailed);

	void NetFetchDetailDataCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FAnnouncementDetailsDataResult OnSuccess, FTapFailed OnFailed);
	
	void NetMarkReadCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSimpleDelegate OnSuccess, FTapFailed OnFailed);

	void NetDownloadWebFontCallback(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
	FOnCustomLinkClicked OnCustomLinkClicked;

	FAudioOutputStateChanged OnAudioOutputStateChanged;
	
	UTapBillboardBrowserNavigate* NavigateBrowser;
	UTapBillboardBrowserSplash* SplashBrowser;
	UTapBillboardMarquee* MarqueeWidget;

	FAnnouncementStyleData CachedMarqueeStyle;
	FBadgeDetails CachedBadgeDetails;
	
	UTexture2DDynamic* MarqueeIconTexture;

	
	TSoftClassPtr<UTapBillboardBrowserNavigate> NavigateBrowserClass;
	TSoftClassPtr<UTapBillboardBrowserSplash> SplashBrowserClass;
	TSoftClassPtr<UTapBillboardMarquee> MarqueeClass;
	TSoftClassPtr<UTapBillboardBrowserPopup> PopupBrowserClass;
	float CanGetBadgeSecond;
	
	FTimerHandle FetchMarqueeTimer;

	TArray<FAnnouncementDetailData> PendingDetailsReverse;

	FString ServerUrl;
	
	uint8 bSplashDataCallback : 1;
	uint8 bCachedBadge : 1;
	
	UFont* DownloadFont;

	float MarqueeSpeed = 60.f;
	
private:
	void InternalFetchUnreadSplashCallback(const TArray<FAnnouncementGeneralData>& GeneralData);

	void InternalFetchMarqueeStyleCallback(const FAnnouncementStyleData& StyleData);

	void InternalDownloadMarqueeImageCallback(UTexture2DDynamic* Texture);

	void TimerFetchMarqueeData();

	void HandleFetchUnreadMarqueeGeneralData(const TArray<FAnnouncementGeneralData>& GeneralData);

	void HandleFetchMarqueeDetailData(const TArray<FAnnouncementDetailData>& DetailsData);
	
	friend UTapBillboardBrowserNavigate;
};



#if PLATFORM_WINDOWS || PLATFORM_MAC
typedef FTapBillboardCommon FTapBillboard;
#endif
