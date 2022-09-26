#include "TapUECommon.h"

#include "TUCommonImpl.h"

void TapUECommon::SetLanguage(ELanguageType LanguageType) {
	TUCommonImpl::Get()->SetLanguage(LanguageType);
}
