#pragma once

class LEANCLOUD_API LCHelper {
public:
	static void PerformOnGameThread(TFunction<void()> Function);

};
