// Fill out your copyright notice in the Description page of Project Settings.


#include "PCButtonHandle.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Components/Button.h"

FPCButtonHandle::FPCButtonHandle(UTapPlatformButton* Button)
	: SavedButton(Button)
{
}

void FPCButtonHandle::UpdateTexture(const FString IconUrl)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->OnProcessRequestComplete().BindSP(this, &FPCButtonHandle::HandleImageRequest);
	HttpRequest->SetURL(IconUrl);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->ProcessRequest();
}

void FPCButtonHandle::HandleImageRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
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

		for (TSharedPtr<IImageWrapper> ImageWrapper : ImageWrappers)
		{
			if ( ImageWrapper.IsValid() && ImageWrapper->SetCompressed(HttpResponse->GetContent().GetData(), HttpResponse->GetContentLength()) )
			{
				TArray64<uint8> RawData;
				const ERGBFormat InFormat = ERGBFormat::BGRA;
				if ( ImageWrapper->GetRaw(InFormat, 8, RawData) )
				{
					const int32 Width = ImageWrapper->GetWidth();
					const int32 Height = ImageWrapper->GetHeight();
					const bool bUseSRGB = true;

					check(Width > 0 && Height > 0 && RawData.Num() > 0);

					// Allocate the new texture
					UTexture2D* NewTexture2D = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

					// Fill in the base mip for the texture we created
					uint8* MipData = (uint8*)NewTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
					for (int32 y = 0; y < Height; y++)
					{
						uint8* DestPtr = &MipData[(Height - 1 - y) * Width * sizeof(FColor)];
						const FColor* SrcPtr = &((FColor*)(RawData.GetData()))[(Height - 1 - y) * Width];
						for (int32 x = 0; x < Width; x++)
						{
							*DestPtr++ = SrcPtr->B;
							*DestPtr++ = SrcPtr->G;
							*DestPtr++ = SrcPtr->R;
							*DestPtr++ = SrcPtr->A;
							SrcPtr++;
						}
					}
					NewTexture2D->PlatformData->Mips[0].BulkData.Unlock();

					// Update the remote texture data
					NewTexture2D->UpdateResource();
					NewTexture2D->Filter = TF_Nearest;

					if (IsValid(SavedButton.Get()))
					{
						SavedButton->Button->WidgetStyle.Normal.SetResourceObject(NewTexture2D);
						SavedButton->Button->WidgetStyle.Hovered.SetResourceObject(NewTexture2D);
						SavedButton->Button->WidgetStyle.Pressed.SetResourceObject(NewTexture2D);
						SavedButton->Button->SetStyle(SavedButton->Button->WidgetStyle);
					}
				}
			}
		}
	}
}
