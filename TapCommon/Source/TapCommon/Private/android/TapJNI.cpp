#include "TapJNI.h"
#include "Android/AndroidApplication.h"
#include "TUDebuger.h"

namespace TapJNI {
	JNI::JNI() {
		Env = FAndroidApplication::GetJavaEnv();
		check(Env);
	}

	JNI::JNI(JNIEnv* in_env) {
		Env = in_env;
		check(Env);
	}

	Class JNI::FindClass(const char* name) const {
		FString Name = FString(UTF8_TO_TCHAR(name));
		jclass Class = NULL;
		if (Name.Find(TEXT("java/")) != INDEX_NONE) {
			TUDebuger::DisplayLog("Env->FindClass: " + Name);
			Class = Env->FindClass(name);
		} else {
			TUDebuger::DisplayLog("FAndroidApplication::FindJavaClass: " + Name);
			Class = FAndroidApplication::FindJavaClass(name);
		}

		if (Class == NULL) {
			TUDebuger::ErrorLog(FString::Printf(TEXT("TapJNI Class: %s not found"), UTF8_TO_TCHAR(name)));
		}
		else {
			TUDebuger::DisplayLog(FString::Printf(TEXT("TapJNI Class: %s"), UTF8_TO_TCHAR(name)));
		}
		return MakeScopedJavaObject(Env, Class);
	}

	Class JNI::GetObjectClass(const Object& object) const {
		jclass Class = Env->GetObjectClass(*object);
		return MakeScopedJavaObject(Env, Class);
	}

	jmethodID JNI::GetMethodID(const Class& Class, const char* name, const char* sig) const {
		jmethodID methodID = Env->GetMethodID(*Class, name, sig);
		if (methodID == NULL) {
			TUDebuger::ErrorLog(FString::Printf(TEXT("TapJNI GetMethodID: %s not found"), UTF8_TO_TCHAR(name)));
		}
		else {
			TUDebuger::DisplayLog(FString::Printf(TEXT("TapJNI GetMethodID: %s"), UTF8_TO_TCHAR(name)));
		}
		return methodID;
	}

	jmethodID JNI::GetStaticMethodID(const Class& Class, const char* name, const char* sig) const {
		jmethodID methodID = Env->GetStaticMethodID(*Class, name, sig);
		if (methodID == NULL) {
			TUDebuger::ErrorLog(FString::Printf(TEXT("TapJNI GetStaticMethodID: %s not found"), UTF8_TO_TCHAR(name)));
		}
		else {
			TUDebuger::DisplayLog(FString::Printf(TEXT("TapJNI GetStaticMethodID: %s"), UTF8_TO_TCHAR(name)));
		}
		return methodID;
	}

	jfieldID JNI::GetFieldID(const Class& Class, const char* name, const char* sig) const {
		jfieldID fieldID = Env->GetFieldID(*Class, name, sig);
		if (fieldID == NULL) {
			TUDebuger::ErrorLog(FString::Printf(TEXT("TapJNI GetFieldID: %s not found"), UTF8_TO_TCHAR(name)));
		}
		else {
			TUDebuger::DisplayLog(FString::Printf(TEXT("TapJNI GetFieldID: %s"), UTF8_TO_TCHAR(name)));
		}
		return fieldID;
	}
	
	jfieldID JNI::GetStaticFieldID(const Class& Class, const char* name, const char* sig) const {
		jfieldID fieldID = Env->GetStaticFieldID(*Class, name, sig);
		if (fieldID == NULL) {
			TUDebuger::ErrorLog(FString::Printf(TEXT("TapJNI GetStaticFieldID: %s not found"), UTF8_TO_TCHAR(name)));
		}
		else {
			TUDebuger::DisplayLog(FString::Printf(TEXT("TapJNI GetStaticFieldID: %s"), UTF8_TO_TCHAR(name)));
		}
		return fieldID;
	}

	Object JNI::GetActivity() const {
		return MakeScopedJavaObject(Env, FAndroidApplication::GetGameActivityThis(), false);
	}

	Object JNI::GetJavaObject(int Value) const {
		Class Interger = FindClass("java/lang/Integer");
		return CallStaticObjectMethod(Interger, "valueOf", "(I)Ljava/lang/Integer;", Value);
	}

	Object JNI::GetJavaObject(double Value) const {
		Class Double = FindClass("java/lang/Double");
		return CallStaticObjectMethod(Double, "valueOf", "(D)Ljava/lang/Double;", Value);
	}

	Object JNI::GetStringArray(const TArray<FString>& Strings) const {
		auto StringClass = FindClass("java/lang/String");
		auto Length = Strings.Num();
		auto Element = ToJavaString("");
		auto Arrays = Env->NewObjectArray(Length, *StringClass, *Element);
		for (int i = 0; i < Length; i++) {
			auto Temp = ToJavaString(Strings[i]);
			Env->SetObjectArrayElement(Arrays, i, *Temp);
		}
		return MakeScopedJavaObject(Env, (jobject)Arrays);
	}

	Object JNI::NewObject(const Class& Class, const char* CtorSig, ...) const {
		jmethodID Constructor = GetMethodID(Class, "<init>", CtorSig);
		va_list ap;
		va_start(ap, CtorSig);
		jobject resultObject = Env->NewObjectV(*Class, Constructor, ap);
		va_end(ap);
		if (resultObject == NULL) {
			TUDebuger::ErrorLog(
				FString::Printf(TEXT("TapJNI NewObject: %s fail"), UTF8_TO_TCHAR(CtorSig)));
		}
		else {
			TUDebuger::DisplayLog(
				FString::Printf(TEXT("TapJNI NewObject: %s success"), UTF8_TO_TCHAR(CtorSig)));
		}
		return MakeScopedJavaObject(Env, resultObject);
	}

	Object JNI::CallObjectMethod(const Object& object, const char* methodName,
	                             const char* methodsig, ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		jobject resultObject = Env->CallObjectMethodV(*object, methodID, ap);
		va_end(ap);
		if (resultObject == NULL) {
			TUDebuger::ErrorLog(
				FString::Printf(TEXT("TapJNI Object Method: %s create object fail"), UTF8_TO_TCHAR(methodName)));
		}
		else {
			TUDebuger::DisplayLog(
				FString::Printf(TEXT("TapJNI Object Method: %s create object success"), UTF8_TO_TCHAR(methodName)));
		}
		return MakeScopedJavaObject(Env, resultObject);
	}


	bool JNI::CallBooleanMethod(const Object& object, const char* methodName, const char* methodsig,
	                            ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		bool result = Env->CallBooleanMethodV(*object, methodID, ap);
		va_end(ap);
		return result;
	}

	char JNI::CallCharMethod(const Object& object, const char* methodName, const char* methodsig,
	                         ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		char result = Env->CallCharMethodV(*object, methodID, ap);
		va_end(ap);
		return result;
	}

	short JNI::CallShortMethod(const Object& object, const char* methodName, const char* methodsig,
	                           ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		short result = Env->CallShortMethodV(*object, methodID, ap);
		va_end(ap);
		return result;
	}

	int JNI::CallIntMethod(const Object& object, const char* methodName, const char* methodsig,
	                       ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		int result = Env->CallIntMethodV(*object, methodID, ap);
		va_end(ap);
		return result;
	}

	long JNI::CallLongMethod(const Object& object, const char* methodName, const char* methodsig,
	                         ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		long result = Env->CallLongMethodV(*object, methodID, ap);
		va_end(ap);
		return result;
	}

	float JNI::CallFloatMethod(const Object& object, const char* methodName, const char* methodsig,
	                           ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		float result = Env->CallFloatMethodV(*object, methodID, ap);
		va_end(ap);
		return result;
	}

	double JNI::CallDoubleMethod(const Object& object, const char* methodName,
	                             const char* methodsig, ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		double result = Env->CallDoubleMethodV(*object, methodID, ap);
		va_end(ap);
		return result;
	}

	void JNI::CallVoidMethod(const Object& object, const char* methodName, const char* methodsig,
	                         ...) const {
		Class Class = GetObjectClass(object);
		jmethodID methodID = GetMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		Env->CallVoidMethodV(*object, methodID, ap);
		va_end(ap);
	}

	Object JNI::CallStaticObjectMethod(const Class& Class,
	                                   const char* methodName, const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		jobject resultObject = Env->CallStaticObjectMethodV(*Class, methodID, ap);
		va_end(ap);
		if (resultObject == NULL) {
			TUDebuger::ErrorLog(
				FString::Printf(TEXT("TapJNI Class Method: %s create object fail"), UTF8_TO_TCHAR(methodName)));
		}
		else {
			TUDebuger::DisplayLog(
				FString::Printf(TEXT("TapJNI Class Method: %s create object success"), UTF8_TO_TCHAR(methodName)));
		}
		return MakeScopedJavaObject(Env, resultObject);
	}

	bool JNI::CallStaticBooleanMethod(const Class& Class, const char* methodName,
	                                  const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		bool result = Env->CallStaticBooleanMethodV(*Class, methodID, ap);
		va_end(ap);
		return result;
	}

	char JNI::CallStaticCharMethod(const Class& Class, const char* methodName,
	                               const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		char result = Env->CallStaticCharMethodV(*Class, methodID, ap);
		va_end(ap);
		return result;
	}

	short JNI::CallStaticShortMethod(const Class& Class, const char* methodName,
	                                 const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		short result = Env->CallStaticShortMethodV(*Class, methodID, ap);
		va_end(ap);
		return result;
	}

	int JNI::CallStaticIntMethod(const Class& Class, const char* methodName, const char* methodsig,
	                             ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		int result = Env->CallStaticIntMethodV(*Class, methodID, ap);
		va_end(ap);
		return result;
	}

	long JNI::CallStaticLongMethod(const Class& Class, const char* methodName,
	                               const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		long result = Env->CallStaticLongMethodV(*Class, methodID, ap);
		va_end(ap);
		return result;
	}

	float JNI::CallStaticFloatMethod(const Class& Class, const char* methodName,
	                                 const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		float result = Env->CallStaticFloatMethodV(*Class, methodID, ap);
		va_end(ap);
		return result;
	}

	double JNI::CallStaticDoubleMethod(const Class& Class, const char* methodName,
	                                   const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		double result = Env->CallStaticDoubleMethodV(*Class, methodID, ap);
		va_end(ap);
		return result;
	}

	void JNI::CallStaticVoidMethod(const Class& Class, const char* methodName, const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		Env->CallStaticVoidMethodV(*Class, methodID, ap);
		va_end(ap);
	}
	
	void JNI::SetObjectField(const Object& object, const char* name, const char* sig, const Object& value) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, sig);
		Env->SetObjectField(*object, fieldID, *value);
	}

	void JNI::SetBooleanField(const Object& object, const char* name, bool value) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "Z");
		Env->SetBooleanField(*object, fieldID, value);
	}

	void JNI::SetCharField(const Object& object, const char* name, char value) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "C");
		Env->SetCharField(*object, fieldID, value);
	}

	void JNI::SetShortField(const Object& object, const char* name, short value) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "S");
		Env->SetShortField(*object, fieldID, value);
	}

	void JNI::SetIntField(const Object& object, const char* name, int value) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "I");
		Env->SetIntField(*object, fieldID, value);
	}

	void JNI::SetLongField(const Object& object, const char* name, long value) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "J");
		Env->SetLongField(*object, fieldID, value);
	}

	void JNI::SetFloatField(const Object& object, const char* name, float value) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "F");
		Env->SetFloatField(*object, fieldID, value);
	}

	void JNI::SetDoubleField(const Object& object, const char* name, double value) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "D");
		Env->SetDoubleField(*object, fieldID, value);
	}

	void JNI::SetFStringField(const Object& object, const char* name, const FString& value) const {
		SetObjectField(object, name, "Ljava/lang/String;", ToJavaString(value));
	}

	FString JNI::GetFStringField(const Object& object, const char* name) const {
		return GetFString(GetObjectField(object, name, "Ljava/lang/String;"));
	}

	void JNI::SetStaticFStringField(const Class& Class, const char* name, const FString& value) const {
		SetStaticObjectField(Class, name, "Ljava/lang/String;", ToJavaString(value));
	}

	FString JNI::GetStaticFStringField(const Class& Class, const char* name) const {
		return GetFString(GetStaticObjectField(Class, name, "Ljava/lang/String;"));
	}

	Object JNI::GetObjectField(const Object& object, const char* name, const char* sig) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, sig);
		jobject resultObject = Env->GetObjectField(*object, fieldID);
		if (resultObject == NULL) {
			TUDebuger::ErrorLog(
				FString::Printf(TEXT("TapJNI GetObjectField: %s fail"), UTF8_TO_TCHAR(name)));
		}
		else {
			TUDebuger::DisplayLog(
				FString::Printf(TEXT("TapJNI GetObjectField: %s success"), UTF8_TO_TCHAR(name)));
		}
		return MakeScopedJavaObject(Env, resultObject);
	}

	bool JNI::GetBooleanField(const Object& object, const char* name) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "Z");
		return Env->GetBooleanField(*object, fieldID);;
	}

	char JNI::GetCharField(const Object& object, const char* name) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "C");
		return Env->GetCharField(*object, fieldID);;
	}

	short JNI::GetShortField(const Object& object, const char* name) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "S");
		return Env->GetShortField(*object, fieldID);;
	}

	int JNI::GetIntField(const Object& object, const char* name) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "I");
		return Env->GetIntField(*object, fieldID);;
	}

	long JNI::GetLongField(const Object& object, const char* name) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "J");
		return Env->GetLongField(*object, fieldID);;
	}

	float JNI::GetFloatField(const Object& object, const char* name) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "F");
		return Env->GetFloatField(*object, fieldID);;
	}

	double JNI::GetDoubleField(const Object& object, const char* name) const {
		Class Class = GetObjectClass(object);
		jfieldID fieldID = GetFieldID(Class, name, "D");
		return Env->GetDoubleField(*object, fieldID);;
	}

	void JNI::SetStaticObjectField(const Class& Class, const char* name, const char* sig, const Object& value) const {
		jfieldID fieldID = GetFieldID(Class, name, sig);
		Env->SetStaticObjectField(*Class, fieldID, *value);
	}

	void JNI::SetStaticBooleanField(const Class& Class, const char* name, bool value) const {
		jfieldID fieldID = GetFieldID(Class, name, "Z");
		Env->SetStaticBooleanField(*Class, fieldID, value);
	}

	void JNI::SetStaticCharField(const Class& Class, const char* name, char value) const {
		jfieldID fieldID = GetFieldID(Class, name, "C");
		Env->SetStaticCharField(*Class, fieldID, value);
	}

	void JNI::SetStaticShortField(const Class& Class, const char* name, short value) const {
		jfieldID fieldID = GetFieldID(Class, name, "S");
		Env->SetStaticShortField(*Class, fieldID, value);
	}

	void JNI::SetStaticIntField(const Class& Class, const char* name, int value) const {
		jfieldID fieldID = GetFieldID(Class, name, "I");
		Env->SetStaticIntField(*Class, fieldID, value);
	}

	void JNI::SetStaticLongField(const Class& Class, const char* name, long value) const {
		jfieldID fieldID = GetFieldID(Class, name, "J");
		Env->SetStaticLongField(*Class, fieldID, value);
	}

	void JNI::SetStaticFloatField(const Class& Class, const char* name, float value) const {
		jfieldID fieldID = GetFieldID(Class, name, "F");
		Env->SetStaticFloatField(*Class, fieldID, value);
	}

	void JNI::SetStaticDoubleField(const Class& Class, const char* name, double value) const {
		jfieldID fieldID = GetFieldID(Class, name, "D");
		Env->SetStaticDoubleField(*Class, fieldID, value);
	}

	Object JNI::GetStaticObjectField(const Class& Class, const char* name, const char* sig) const {
		jfieldID fieldID = GetFieldID(Class, name, sig);
		jobject resultObject = Env->GetStaticObjectField(*Class, fieldID);
		if (resultObject == NULL) {
			TUDebuger::ErrorLog(
				FString::Printf(TEXT("TapJNI GetStaticObjectField: %s fail"), UTF8_TO_TCHAR(name)));
		}
		else {
			TUDebuger::DisplayLog(
				FString::Printf(TEXT("TapJNI GetStaticObjectField: %s success"), UTF8_TO_TCHAR(name)));
		}
		return MakeScopedJavaObject(Env, resultObject);
	}

	bool JNI::GetStaticBooleanField(const Class& Class, const char* name) const {
		jfieldID fieldID = GetFieldID(Class, name, "Z");
		return Env->GetStaticBooleanField(*Class, fieldID);;
	}

	char JNI::GetStaticCharField(const Class& Class, const char* name) const {
		jfieldID fieldID = GetFieldID(Class, name, "C");
		return Env->GetStaticCharField(*Class, fieldID);;
	}

	short JNI::GetStaticShortField(const Class& Class, const char* name) const {
		jfieldID fieldID = GetFieldID(Class, name, "S");
		return Env->GetStaticShortField(*Class, fieldID);;
	}

	int JNI::GetStaticIntField(const Class& Class, const char* name) const {
		jfieldID fieldID = GetFieldID(Class, name, "I");
		return Env->GetStaticIntField(*Class, fieldID);;
	}

	long JNI::GetStaticLongField(const Class& Class, const char* name) const {
		jfieldID fieldID = GetFieldID(Class, name, "J");
		return Env->GetStaticLongField(*Class, fieldID);;
	}

	float JNI::GetStaticFloatField(const Class& Class, const char* name) const {
		jfieldID fieldID = GetFieldID(Class, name, "F");
		return Env->GetStaticFloatField(*Class, fieldID);;
	}

	double JNI::GetStaticDoubleField(const Class& Class, const char* name) const {
		jfieldID fieldID = GetFieldID(Class, name, "D");
		return Env->GetStaticDoubleField(*Class, fieldID);;
	}


	FString JNI::GetFString(const Object& JavaString) const {
		FString ReturnString = GetFStringFromParam((jstring)*JavaString);
		return ReturnString;
	}

	Object JNI::ToJavaString(const FString& UnrealString) const {
		return MakeScopedJavaObject(Env, (jobject)Env->NewStringUTF(TCHAR_TO_UTF8(*UnrealString)));
	}

	FString JNI::GetStringFromGlobalRef(jstring JavaString) const {
		FString ReturnString = GetFStringFromParam(JavaString);
    
		if (Env && JavaString)
		{
			Env->DeleteGlobalRef(JavaString);
		}
    
		return ReturnString;
	}

	FString JNI::GetFStringFromParam(jstring JavaString) const {
		if (!JavaString || Env->IsSameObject(JavaString, NULL))
		{
			return {};
		}
    
		const auto chars = Env->GetStringUTFChars(JavaString, 0);
		FString ReturnString(UTF8_TO_TCHAR(chars));
		Env->ReleaseStringUTFChars(JavaString, chars);
		return ReturnString;
	}
}

