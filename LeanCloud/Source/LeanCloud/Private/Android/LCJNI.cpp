#include "Android/LCJNI.h"
#include "Android/AndroidApplication.h"
#include "Tools/LCDebuger.h"

namespace LCJNI {
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
			LCDebuger::DisplayLog("Env->FindClass: " + Name);
			Class = Env->FindClass(name);
		} else {
			LCDebuger::DisplayLog("FAndroidApplication::FindJavaClass: " + Name);
			Class = FAndroidApplication::FindJavaClass(name);
		}

		if (Class == NULL) {
			LCDebuger::ErrorLog(FString::Printf(TEXT("LCJNI Class: %s not found"), UTF8_TO_TCHAR(name)));
		}
		else {
			LCDebuger::DisplayLog(FString::Printf(TEXT("LCJNI Class: %s"), UTF8_TO_TCHAR(name)));
		}
		return MakeScopedJavaObject(Env, Class);
	}

	Class JNI::GetObjectClass(const Object& object) const {
		jclass Class = Env->GetObjectClass(*object);
		return MakeScopedJavaObject(Env, Class);
	}

	jmethodID JNI::GetMethodID(const Class& Class, const char* methodName, const char* methodsig) const {
		jmethodID methodID = Env->GetMethodID(*Class, methodName, methodsig);
		if (methodID == NULL) {
			LCDebuger::ErrorLog(FString::Printf(TEXT("LCJNI Method: %s not found"), UTF8_TO_TCHAR(methodName)));
		}
		else {
			LCDebuger::DisplayLog(FString::Printf(TEXT("LCJNI Method: %s"), UTF8_TO_TCHAR(methodName)));
		}
		return methodID;
	}

	jmethodID JNI::GetStaticMethodID(const Class& Class, const char* methodName, const char* methodsig) const {
		jmethodID methodID = Env->GetStaticMethodID(*Class, methodName, methodsig);
		if (methodID == NULL) {
			LCDebuger::ErrorLog(FString::Printf(TEXT("LCJNI Method: %s not found"), UTF8_TO_TCHAR(methodName)));
		}
		else {
			LCDebuger::DisplayLog(FString::Printf(TEXT("LCJNI Method: %s"), UTF8_TO_TCHAR(methodName)));
		}
		return methodID;
	}

	Object JNI::GetJavaObject(int Value) const {
		Class Interger = FindClass("java/lang/Integer");
		return CallStaticObjectMethod(Interger, "valueOf", "(I)Ljava/lang/Integer;", Value);
	}

	Object JNI::GetJavaObject(double Value) const {
		Class Double = FindClass("java/lang/Double");
		return CallStaticObjectMethod(Double, "valueOf", "(D)Ljava/lang/Double;", Value);
	}

	Object JNI::GetActivity() const {
		return MakeScopedJavaObject(Env, FAndroidApplication::GetGameActivityThis(), false);
	}

	Object JNI::NewObject(const Class& Class, const char* CtorSig, ...) const {
		jmethodID Constructor = GetMethodID(Class, "<init>", CtorSig);
		va_list ap;
		va_start(ap, CtorSig);
		jobject resultObject = Env->NewObjectV(*Class, Constructor, ap);
		va_end(ap);
		if (resultObject == NULL) {
			LCDebuger::ErrorLog(
				FString::Printf(TEXT("LCJNI NewObject: %s fail"), UTF8_TO_TCHAR(CtorSig)));
		}
		else {
			LCDebuger::DisplayLog(
				FString::Printf(TEXT("LCJNI NewObject: %s success"), UTF8_TO_TCHAR(CtorSig)));
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
			LCDebuger::ErrorLog(
				FString::Printf(TEXT("LCJNI Object Method: %s create object fail"), UTF8_TO_TCHAR(methodName)));
		}
		else {
			LCDebuger::DisplayLog(
				FString::Printf(TEXT("LCJNI Object Method: %s create object success"), UTF8_TO_TCHAR(methodName)));
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
			LCDebuger::ErrorLog(
				FString::Printf(TEXT("LCJNI Class Method: %s create object fail"), UTF8_TO_TCHAR(methodName)));
		}
		else {
			LCDebuger::DisplayLog(
				FString::Printf(TEXT("LCJNI Class Method: %s create object success"), UTF8_TO_TCHAR(methodName)));
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

	void JNI::CallStaticVoidMethod(const Class& Class, const char* methodName,
	                               const char* methodsig, ...) const {
		jmethodID methodID = GetStaticMethodID(Class, methodName, methodsig);
		va_list ap;
		va_start(ap, methodsig);
		Env->CallStaticVoidMethodV(*Class, methodID, ap);
		va_end(ap);
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

