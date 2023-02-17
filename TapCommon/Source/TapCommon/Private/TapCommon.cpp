// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapCommon.h"

#if PLATFORM_MAC
#include "WebBrowserModule.h"
#endif
#include "Slate/STapThrobber.h"
#include "Slate/TapStyleCommon.h"
#include "Http.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Engine/Texture2DDynamic.h"

#define LOCTEXT_NAMESPACE "FTapCommonModule"

void FTapCommonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if PLATFORM_MAC
	IWebBrowserModule::Get().GetSingleton();
#endif
	FTapStyleCommon::Initialize();
}

void FTapCommonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FTapStyleCommon::Shutdown();
}

void WriteRawToTexture_RenderThread(FTexture2DDynamicResource* TextureResource, TArray64<uint8>* RawData, bool bUseSRGB = true)
{
	check(IsInRenderingThread());

	if (TextureResource)
	{
		FRHITexture2D* TextureRHI = TextureResource->GetTexture2DRHI();

		int32 Width = TextureRHI->GetSizeX();
		int32 Height = TextureRHI->GetSizeY();

		uint32 DestStride = 0;
		uint8* DestData = reinterpret_cast<uint8*>(RHILockTexture2D(TextureRHI, 0, RLM_WriteOnly, DestStride, false, false));

		for (int32 y = 0; y < Height; y++)
		{
			uint8* DestPtr = &DestData[((int64)Height - 1 - y) * DestStride];

			const FColor* SrcPtr = &((FColor*)(RawData->GetData()))[((int64)Height - 1 - y) * Width];
			for (int32 x = 0; x < Width; x++)
			{
				*DestPtr++ = SrcPtr->B;
				*DestPtr++ = SrcPtr->G;
				*DestPtr++ = SrcPtr->R;
				*DestPtr++ = SrcPtr->A;
				SrcPtr++;
			}
		}

		RHIUnlockTexture2D(TextureRHI, 0, false, false);
	}

	delete RawData;
}

void HandleImageRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FAsyncDownloadImage Callback)
{
	if ( bSucceeded && HttpResponse.IsValid() && HttpResponse->GetContentLength() > 0 )
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrappers[3] =
		{
			ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
			ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
			ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
		};

		for ( auto ImageWrapper : ImageWrappers )
		{
			if ( ImageWrapper.IsValid() && ImageWrapper->SetCompressed(HttpResponse->GetContent().GetData(), HttpResponse->GetContentLength()) )
			{
				TArray64<uint8>* RawData = new TArray64<uint8>();
				const ERGBFormat InFormat = ERGBFormat::BGRA;
				if ( ImageWrapper->GetRaw(InFormat, 8, *RawData) )
				{
					if ( UTexture2DDynamic* Texture = UTexture2DDynamic::Create(ImageWrapper->GetWidth(), ImageWrapper->GetHeight()) )
					{
						Texture->SRGB = true;
						Texture->UpdateResource();

						FTexture2DDynamicResource* TextureResource = static_cast<FTexture2DDynamicResource*>(Texture->Resource);
						if (TextureResource)
						{
							ENQUEUE_RENDER_COMMAND(FWriteRawDataToTexture)(
								[TextureResource, RawData](FRHICommandListImmediate& RHICmdList)
								{
									WriteRawToTexture_RenderThread(TextureResource, RawData);
								});
						}
						else
						{
							delete RawData;
						}
						Callback.ExecuteIfBound(Texture);						
						return;
					}
				}
			}
		}
	}

	Callback.ExecuteIfBound(nullptr);
}

void FTapCommonModule::AsyncDownloadImage(const FString& Url, const FAsyncDownloadImage& Callback)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->OnProcessRequestComplete().BindStatic(&HandleImageRequest, Callback);
	HttpRequest->SetURL(Url);
	HttpRequest->SetVerb(TEXT("GET"));
	if (!HttpRequest->ProcessRequest())
	{
		Callback.ExecuteIfBound(nullptr);
	}
}

void FTapCommonModule::TapThrobberShowWait()
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	if (!Module.TapThrobber)
	{
		SAssignNew(Module.TapThrobber, STapThrobber);
		if (Module.TapThrobber && GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->AddViewportWidgetContent(Module.TapThrobber.ToSharedRef(), MAX_int16);
		}
	}
}

void FTapCommonModule::TapThrobberShowWaitAndToast(const FString& Toast)
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	const FText NewContent = FText::FromString(Toast);
	if (Module.TapThrobber)
	{
		Module.TapThrobber->UpdateContent(NewContent);
	}
	else
	{
		SAssignNew(Module.TapThrobber, STapThrobber)
		.Content(NewContent);
		if (Module.TapThrobber && GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->AddViewportWidgetContent(Module.TapThrobber.ToSharedRef(), MAX_int16);
		}
	}
}

void FTapCommonModule::TapThrobberDismiss()
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	if (Module.TapThrobber && GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(Module.TapThrobber.ToSharedRef());
		Module.TapThrobber.Reset();
	}
}

void FTapCommonModule::TapThrobberShowToast(const FString& Toast, float TimeInterval)
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	const FText NewContent = FText::FromString(Toast);
	if (Module.TapThrobber)
	{
		Module.TapThrobber->ShowThrobber(false);
		Module.TapThrobber->UpdateContent(NewContent);
		Module.TapThrobber->UpdateTimeInterval(TimeInterval);
		Module.TapThrobber->UpdateRemoveSelfDelegate(FOnTapThrobberRemoveSelf::CreateStatic(&FTapCommonModule::OnTapThrobberRemoveSelf));
	}
	else
	{
		SAssignNew(Module.TapThrobber, STapThrobber)
		.HasThrobber(false)
		.Content(NewContent)
		.TimeInterval(TimeInterval)
		.OnRemoveSelf(FOnTapThrobberRemoveSelf::CreateStatic(&FTapCommonModule::OnTapThrobberRemoveSelf));
		if (Module.TapThrobber && GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->AddViewportWidgetContent(Module.TapThrobber.ToSharedRef(), MAX_int16);
		}
	}
}

void FTapCommonModule::OnTapThrobberRemoveSelf(const TSharedRef<STapThrobber>& Throbber)
{
	FTapCommonModule& Module = FModuleManager::GetModuleChecked<FTapCommonModule>("TapCommon");
	Module.TapThrobber.Reset();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTapCommonModule, TapCommon)

FTapCommonModule::FBridgeCallback FTapCommonModule::OnBridgeCallback;
FTapCommonModule::FGetRegionCode FTapCommonModule::OnRegionCode;
FTapCommonModule::FIsTapTapInstalled FTapCommonModule::OnTapTapInstalled;
FTapCommonModule::FIsTapGlobalInstalled FTapCommonModule::OnTapGlobalInstalled;
FTapCommonModule::FUpdateGameInTapTap FTapCommonModule::OnUpdateGameInTapTap;
FTapCommonModule::FUpdateGameInTapGlobal FTapCommonModule::OnUpdateGameInTapGlobal;
FTapCommonModule::FOpenReviewInTapGlobal FTapCommonModule::OnOpenReviewInTapGlobal;
FTapCommonModule::FOpenReviewInTapTap FTapCommonModule::OnOpenReviewInTapTap;