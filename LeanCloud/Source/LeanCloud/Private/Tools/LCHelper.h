#pragma once

class LCHelper {
public:
	static void PerformOnGameThread(TFunction<void()> Function);

};
