#include "Tools/LCHelper.h"


void LCHelper::PerformOnGameThread(TFunction<void()> Function) {
	if (!Function) {
		return;
	}
	if (IsInGameThread())
	{
		Function();
	}
	else
	{
		AsyncTask(ENamedThreads::GameThread, Function);
	}
}