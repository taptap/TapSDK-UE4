#pragma once

#define TapUESupport_VERSION_NUMBER "31700001"
#define TapUESupport_VERSION "3.17.0"
#include "TUError.h"
#include "TUSupportType.h"

class TAPSUPPORT_API TapUESupport {
public:
	
	DECLARE_DELEGATE_OneParam(FErrorDelegate, const FTUError&);
	DECLARE_DELEGATE_OneParam(FUnreadStatusChangedDelegate, bool);

	/// 初始化
	/// @param Config 配置信息
	static void Init(FTapSupportConfig Config);

	static FErrorDelegate OnErrorCallBack;
	static FUnreadStatusChangedDelegate OnUnreadStatusChanged;

	static void SetDefaultMetaData(TSharedPtr<FJsonObject> MetaData);
	
	static void SetDefaultFieldsData(TSharedPtr<FJsonObject> FieldsData);

	/// 匿名用户登录
	/// @param UserID 匿名用户id，不能为空
	static void LoginAnonymously(const FString& UserID);

	/// 登出
	static void Logout();

	/// 获取网页地址
	/// @param Path 路径
	/// @param MetaData meta 参数
	/// @param FieldsData fields 参数
	static FString GetSupportWebUrl(const FString& Path = "", TSharedPtr<FJsonObject> MetaData = nullptr, TSharedPtr<FJsonObject> FieldsData = nullptr);

	/// 打开客服网页
	/// @param Path 路径
	/// @param MetaData meta 参数
	/// @param FieldsData fields 参数
	static void OpenSupportView(const FString& Path = "", TSharedPtr<FJsonObject> MetaData = nullptr, TSharedPtr<FJsonObject> FieldsData = nullptr);

	/// 关闭客服网页
	static void CloseSupportView();

	/// 开始轮询获取未读状态
	static void Resume();

	/// 结束轮询获取未读状态
	static void Pause();

	/// 单次获取未读状态
	static void FetchUnReadStatus();

};
