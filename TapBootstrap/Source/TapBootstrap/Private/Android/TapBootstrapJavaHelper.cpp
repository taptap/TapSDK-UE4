#include "TapBootstrapJavaHelper.h"

#include "TapJavaHelper.h"
#include "TUJsonHelper.h"


TSharedPtr<FTDSUser> TapBootstrapJavaHelper::ConvertUser(const TapJNI::JNI& JNI, const TapJNI::Object& tds_user) {
	if (!tds_user) {
		return nullptr;
	}
	auto map = JNI.CallObjectMethod(tds_user, "getServerData", "()Ljava/util/concurrent/ConcurrentMap;");
	TSharedPtr<FJsonObject> ServerData = TUJsonHelper::GetJsonObject(TapJavaHelper::GetJsonStrFromMap(JNI, map));
	TSharedPtr<FTDSUser> User_UE = MakeShareable(new FTDSUser(ServerData));
	return User_UE;
}

TapJNI::Object TapBootstrapJavaHelper::ConvertUser(const TapJNI::JNI& JNI, const FTDSUser& User) {
	auto map = TapJavaHelper::GetMapFromJsonObject(JNI, User.GetServerData());
	auto Class = JNI.FindClass("com/tapsdk/bootstrap/account/TDSUser");
	auto Java_User = JNI.NewObject(Class, "()V");
	if (map) {
		JNI.CallVoidMethod(Java_User, "resetServerData", "(Ljava/util/Map;)V", *map);
	}
	return Java_User;
}
