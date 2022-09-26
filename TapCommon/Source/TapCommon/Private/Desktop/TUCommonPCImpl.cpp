#include "TUCommonPCImpl.h"

#include "TULanguage.h"

void TUCommonPCImpl::SetLanguage(ELanguageType LanguageType) {
	TULanguage::SetCurrentType(LanguageType);
}
