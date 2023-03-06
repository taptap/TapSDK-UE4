#include "LeanCloudPush.h"
#include "Tools/LCHelper.h"

// static FLeanCloudPush::FNotificationDelegate NotificationDelegater;
// TSharedPtr<FJsonObject> FLeanCloudPush::CacheContent;
//
// void FLeanCloudPush::SetReceiveNotificationDelegater(const FNotificationDelegate& Delegater) {
// 	NotificationDelegater = Delegater;
// 	LCHelper::PerformOnGameThread([=]() {
// 		if (CacheContent.IsValid()) {
// 			NotificationDelegater.ExecuteIfBound(CacheContent);
// 			CacheContent = nullptr;
// 		}
// 	});
// }
//
// void FLeanCloudPush::PushContent(TSharedPtr<FJsonObject> Content) {
// 	LCHelper::PerformOnGameThread([=]() {
// 		if (!NotificationDelegater.ExecuteIfBound(Content)) {
// 			CacheContent = Content;
// 		}
// 	});
// }
