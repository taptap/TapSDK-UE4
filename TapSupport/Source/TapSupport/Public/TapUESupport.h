#pragma once

#define TapUESupport_VERSION_NUMBER "31801001"
#define TapUESupport_VERSION "3.18.1"
#include "TUError.h"
#include "TUSupportType.h"

class TAPSUPPORT_API TapUESupport {
public:
	
	DECLARE_DELEGATE_OneParam(FErrorDelegate, const FTUError&);
	DECLARE_DELEGATE_OneParam(FUnreadStatusChangedDelegate, bool);


	/**
	 * @brief Init support module 
	 * @param Config Support module configuration data
	 */
	static void Init(FTapSupportConfig Config);

	/**
	 * @brief Error callback when fetch unread message state
	 */
	static FErrorDelegate OnErrorCallBack;
	
	/**
	 * @brief Unread message state changed delegate
	 */
	static FUnreadStatusChangedDelegate OnUnreadStatusChanged;

	UE_DEPRECATED(4.0, "'SetDefaultMetaData' is deprecated. Please use 'SetDefaultFieldsData' instead!")
	static void SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData);
	
	/**
	 * @brief Set default field data to DC
	 * @param FieldsData The default key-value field data
	 */
	static void SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData);

	/**
	 * @brief Update specify field data
	 */
	static void UpdateDefaultField(const FString& Key, const TSharedPtr<FJsonValue>& NewValue);


	/**
	 * @brief Anonymously login
	 * @param UserID At least 32 characters
	 */
	static void LoginAnonymously(const FString& UserID);

	/**
	 * @brief Custom login
	 */
	static void LoginWithCustomCredential(const FString& Credential);

	/**
	 * @brief Async login with TDSUser
	 * @param Credential @see FTDSUser::RetrieveShortToken
	 */
	static void LoginWithTDSCredential(const FString& Credential, const FSimpleDelegate& OnSuccess, const FTapFailed& OnFailed);
	
	/**
	 * @brief Clear login user data
	 */
	static void Logout();

	UE_DEPRECATED(4.0, "'MetaData' is deprecated. Please use GetSupportWebUrl(Two params) instead!")
	static FString GetSupportWebUrl(const FString& Path = "", TSharedPtr<FJsonObject> MetaData = nullptr, TSharedPtr<FJsonObject> FieldsData = nullptr);

	UE_DEPRECATED(4.0, "'MetaData' is deprecated. Please use OpenSupportView(Two params) instead!")
	static void OpenSupportView(const FString& Path = "", TSharedPtr<FJsonObject> MetaData = nullptr, TSharedPtr<FJsonObject> FieldsData = nullptr);

	/**
	 * @brief Generate web url
	 */
	static FString GetSupportWebUrlV2(const FString& Path = TEXT("/"), TSharedPtr<FJsonObject> Fields = nullptr);
	
	/**
	 * @brief Open support web view in system browser
	 */
	static void OpenSupportViewV2(const FString& Path = TEXT("/"), TSharedPtr<FJsonObject> Fields = nullptr);

	/**
	 * @brief Close opened system web browser (Mobile only)
	 */
	static void CloseSupportView();

	/**
	 * @brief Resume timer fetch unread message
	 */
	static void Resume();

	/**
	 * @brief Stop timer fetch unread message
	 */
	static void Pause();

	/**
	 * @brief Fetch unread message immediately, and trigger callback if need
	 */
	static void FetchUnReadStatus();
};
