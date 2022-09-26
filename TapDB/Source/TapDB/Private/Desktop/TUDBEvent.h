#pragma once
#include "TUDBEnum.h"

// namespace TUDBEventKey
// {
// 	static FString const NAME_KEY                 = "name";
// 	static FString const PROPERTIES_KEY           = "properties";
// 	static FString const CUSTOM_PROPERTIES_KEY    = "customProperties";
//
// 	static FString const MODULE_KEY               = "module";
// 	static FString const TYPE_KEY                 = "type";
//
// 	static FString const OPENID_KEY               = "open_id";
// 	static FString const CLIENTID_KEY             = "client_id";
// 	static FString const LOGIN_TYPE_KEY           = "login_type";
// }

class TUDBEvent
{
public:
	
	class Key {
		/*
		 * 系统字段
		 */
	public:
		static FString const IPV6;			// 可传 SDK 获取的本机 ip v6 地址，获取不到可以不传
		static FString const UserID;		// 可传 用户标识符
		static FString const DeviceID;		// 可传 设备标识符
		static FString const OpenID;		// 可传 圈人测试相关，和登录的 TapTap 账号有关，只有 taptap 登录才会有
		static FString const Name;			// 可传 当且仅当 type 类型为 track 时才会出现，表示追踪的事件名。
		static FString const Type;			// 必传，SDK 上传的事件类型，可选值为 [track, update, initialise, add]。
		static FString const ClientID;		// 必传，TapDB 的项目ID / TDS 的项目 ID （二选一）
		static FString const Properties;	// 必传，子属性
		/*
		 * 预置属性
		 */
		static FString const OS;			// 必传 目前iOS、Android、Mac、Windows
		static FString const DeviceModel;	// 必传 设备型号
		static FString const DeviceID1;		// 可传 IMEI 或者 IDFA 
		static FString const DeviceID2;		// 可传    Google AD ID
		static FString const DeviceID3;		// 可传    Android ID
		static FString const DeviceID4;		// 可传    OAID
		static FString const InstallID;		// 必传    标记每次安装 
		static FString const PersistID;		// 必传    每次安装都一样
		static FString const Channel;		// 必传    分包渠道
		static FString const Width;			// 必传    屏幕宽度
		static FString const Height;		// 必传    屏幕高度
		static FString const OSVersion;		// 必传    操作系统版本
		static FString const Provider;		// 必传    网络运营商
		static FString const Network;		// 必传    网络类型，可选值为： [0,1,2,3,4,5,6,7] 0=Unknown 1=Ethernet(局域网) 2=WIFI 3=Cellular Network – Unknown Generation(移动网络，但是不确定是几代) 4=2G 5=3G 6=4G 7=5G
		static FString const AppVersion;	// 必传  接入的应用版本
		static FString const SDKVersion;	// 必传  接入的应用版本
		/*
		 * user_login 事件特有
		 *	TapTap=TapTap登录
			WeiXin=微信登录
			QQ=QQ登录
			Tourist=游客登录
			Apple=Apple登录
			Alipay=支付宝登录
			Facebook=facebook登录
			Google=Google登录
			Twitter=Twitter登录
			PhoneNumber=手机号登录
			Custom=用户自定义登录类型
		 */
		static FString const LoginType;		// 必传    登录方式 
		/*
		 * charge 事件特有
		 */
		static FString const OrderID;		// 可传    订单号
		static FString const Product;		// 可传    商品
		static FString const Amount;		// 必传    金额
		static FString const VirtualCurrencyAmount;		// 可传    虚拟金额
		static FString const CurrencyType;		// 可传    货币类型
		static FString const Payment;		// 可传    付费方式
		/*
		 * play_game 事件特有
		 */
		static FString const Duration;		// 必传 时长
	};
	
	TUDBEvent(const FString& _ClientID);
	void SetUser(const FString& _UserID); // 登录的时候设置
	void SetTapTapID(const FString& TapTapID); // taptap 登录 独有的设置
	void ClearUser();

	// TrackEvent 事件会用到
	TSharedPtr<FJsonObject> CustomStaticProperties;
	TFunction<TSharedPtr<FJsonObject>()> CustomDynamicPropertiesCaculator;

	void TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties, TFunction<void()> SuccessBlock = nullptr);
	void DeviceInitialize(TSharedPtr<FJsonObject> Properties);
	void DeviceUpdate(TSharedPtr<FJsonObject> Properties);
	void DeviceAdd(TSharedPtr<FJsonObject> Properties);
	void UserInitialize(TSharedPtr<FJsonObject> Properties);
	void UserUpdate(TSharedPtr<FJsonObject> Properties);
	void UserAdd(TSharedPtr<FJsonObject> Properties);
	
private:
	FString ClientID;
	FString UserID;
	FString DeviceID;

	void GenerateSysProperties();
	void GenerateCommonProperties();
	TSharedPtr<FJsonObject> SysProperties;
	TSharedPtr<FJsonObject> CommonProperties;
	
};
