#pragma once
#include "TUType.h"

class TUCommonImpl {
public:

	virtual ~TUCommonImpl() = default;

	static TSharedPtr<TUCommonImpl>& Get();
	
	virtual void SetLanguage(ELanguageType LanguageType);

private:
	static TSharedPtr<TUCommonImpl> Instance;
	
};
