#pragma once
#include "TUDebuger.h"
#include "Blueprint/UserWidget.h"

class TAPCOMMON_API TUHelper
{
public:

	static void LaunchURL(const TCHAR* URL, const TCHAR* Param, FString* Error);

	static void PerformOnGameThread(TFunction<void()> Function);
	
	template <class UIType>
	static UIType* GetWidgetUI(UUserWidget *widget, FName name)
	{
		UIType* view = Cast<UIType>(widget->GetWidgetFromName(name));
		if (view == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s cant find"), *name.ToString());
		}
		return view;
	}


	static FString GetRandomStr(int len);

	static void GenerateBrush(FSlateBrush& Brush, const FString& TexturePath);

	// 把Adder加到Object中。
	static void JsonObjectAppend(TSharedPtr<FJsonObject>& Object, const TSharedPtr<FJsonObject>& Adder);

	// 把Adder加到Object中。
	static void JsonObjectAddNotEmptyString(TSharedPtr<FJsonObject>& Object, const FString& Key, const FString& Value);

	template<typename TReturn, typename... TArgs>
	static TReturn InvokeFunction(FString ClassName, FString FunctionName, TArgs&&... args)
	{
		TReturn Value = TReturn();
		if (ClassName.IsEmpty()) {
			TUDebuger::ErrorLog("InvokeFunction ClassName:" + ClassName + " is Empty");
			return Value;
		}
		if (FunctionName.IsEmpty()) {
			TUDebuger::ErrorLog("InvokeFunction FunctionName:" + FunctionName + " is Empty");
			return Value;
		}
#if ENGINE_MAJOR_VERSION > 4
		UClass* ResultClass = FindObject<UClass>(nullptr, *ClassName);
#else
		UClass* ResultClass = FindObject<UClass>(ANY_PACKAGE, *ClassName);
#endif
		if (ResultClass)
		{
			UFunction* Function = ResultClass->FindFunctionByName(FName(*FunctionName));
			if (Function)
			{
				TTuple<TArgs..., TReturn> params(Forward<TArgs>(args)..., Value);
				ResultClass->ProcessEvent(Function, &params);
				//copy back out params
				for (TFieldIterator<FProperty> i(Function); i; ++i)
				{
				    FProperty* prop = *i;
				    if (prop->PropertyFlags & CPF_OutParm)
				    {
				        void* propBuffer = prop->ContainerPtrToValuePtr<void*>(&params);
				        prop->CopyCompleteValue(&Value, propBuffer);
				    }
				}
				return Value;
			}
		}
		TUDebuger::ErrorLog("InvokeFunction ClassName:" + ClassName + " FunctionName:" + FunctionName + " Fail");
		return Value;
	}

	template<typename... TArgs>
	static void InvokeNoReturnFunction(FString ClassName, FString FunctionName, TArgs&&... args)
	{
		if (ClassName.IsEmpty()) {
			TUDebuger::ErrorLog("InvokeFunction ClassName:" + ClassName + " is Empty");
			return;
		}
		if (FunctionName.IsEmpty()) {
			TUDebuger::ErrorLog("InvokeFunction FunctionName:" + FunctionName + " is Empty");
			return;
		}
#if ENGINE_MAJOR_VERSION > 4
		UClass* ResultClass = FindObject<UClass>(nullptr, *ClassName);
#else
		UClass* ResultClass = FindObject<UClass>(ANY_PACKAGE, *ClassName);
#endif
		if (ResultClass)
		{
			UFunction* Function = ResultClass->FindFunctionByName(FName(*FunctionName));
			if (Function)
			{
				TTuple<TArgs...> params(Forward<TArgs>(args)...);
				ResultClass->ProcessEvent(Function, &params);
				return;
			}
		}
		TUDebuger::ErrorLog("InvokeFunction ClassName:" + ClassName + " FunctionName:" + FunctionName + " Fail");
	}

	static FString CombinParameters(const TSharedPtr<FJsonObject>& parameters, bool isEncode = true);

#if PLATFORM_MAC || PLATFORM_WINDOWS
	
	static UTexture2D* GenerateQrCode(const FString& string);
	
	static void ActivateItself();
#endif


	

};
