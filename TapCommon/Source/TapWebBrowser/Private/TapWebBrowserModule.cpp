// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapWebBrowserModule.h"
#include "Modules/ModuleManager.h"
#include "TUWebBrowserAssetManager.h"
#include "WebBrowserModule.h"
#include "IWebBrowserSingleton.h"
#include "Materials/Material.h"

//////////////////////////////////////////////////////////////////////////

class FTapWebBrowserModule : public ITapWebBrowserModule
{
public:
	virtual void StartupModule() override
	{
		if (WebBrowserAssetMgr == nullptr)
		{
			WebBrowserAssetMgr = NewObject<UTUWebBrowserAssetManager>((UObject*)GetTransientPackage(), NAME_None, RF_Transient | RF_Public);
			WebBrowserAssetMgr->LoadDefaultMaterials();

			IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
			if (WebBrowserSingleton)
			{
				WebBrowserSingleton->SetDefaultMaterial(WebBrowserAssetMgr->GetDefaultMaterial());
				WebBrowserSingleton->SetDefaultTranslucentMaterial(WebBrowserAssetMgr->GetDefaultTranslucentMaterial());
			}
		}
	}

	virtual void ShutdownModule() override
	{
	}
	
private:
	UTUWebBrowserAssetManager* WebBrowserAssetMgr = nullptr;
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FTapWebBrowserModule, TapWebBrowser);
