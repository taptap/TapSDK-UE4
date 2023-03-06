#pragma once
#include <jni.h>

#define LC_JNIObjectCallMethodArgs const Object& object, const char* methodName, const char* methodsig, ...
#define LC_JNIClassCallMethodArgs const Class& Class, const char* methodName, const char* methodsig, ...


namespace LCJNI {

	// Helper class that automatically calls DeleteLocalRef on the passed-in Java object when goes out of scope
	template <typename T>
	class LEANCLOUD_API ScopedJavaObject
	{
	public:
		ScopedJavaObject(JNIEnv* InEnv, const T& InObjRef, bool InNeedDelete = true) :
		Env(InEnv),
		ObjRef(InObjRef),
		NeedDelete(InNeedDelete)
		{}
	
		ScopedJavaObject(ScopedJavaObject&& Other) :
		Env(Other.Env),
		ObjRef(Other.ObjRef),
		NeedDelete(Other.NeedDelete)
		{
			Other.Env = nullptr;
			Other.ObjRef = nullptr;
		}
	
		ScopedJavaObject(const ScopedJavaObject& Other) = delete;
		ScopedJavaObject& operator=(const ScopedJavaObject& Other) = delete;
	
		~ScopedJavaObject()
		{
			if (*this && NeedDelete)
			{
				Env->DeleteLocalRef(ObjRef);
			}
		}
	
		// Returns the underlying JNI pointer
		T operator*() const { return ObjRef; }
	
		operator bool() const
		{
			if (!Env || !ObjRef || Env->IsSameObject(ObjRef, NULL))
			{
				return false;
			}
		
			return true;
		}
	
	private:
		JNIEnv* Env = nullptr;
		T ObjRef = nullptr;
		bool NeedDelete = true;
	};

	/**
	 Helper function that allows template deduction on the java object type, for example:
	 auto ScopeObject = NewScopedJavaObject(Env, JavaString);
	 instead of ScopedJavaObject<jstring> ScopeObject(Env, JavaString);
	 */
	template <typename T>
	ScopedJavaObject<T> MakeScopedJavaObject(JNIEnv* InEnv, const T& InObjRef, bool InNeedDelete = true)
	{
		return ScopedJavaObject<T>(InEnv, InObjRef, InNeedDelete);
	}
	typedef ScopedJavaObject<jclass> Class;
	typedef ScopedJavaObject<jobject> Object;
	
	class LEANCLOUD_API JNI {
	public:
		JNI();
		JNI(JNIEnv *in_env);

		// Returns the underlying JNI pointer
		JNIEnv * operator*() const { return Env; }

		Class FindClass(const char* name) const;
		Class GetObjectClass(const Object& object) const;
		jmethodID GetMethodID(const Class& Class, const char* methodName, const char* methodsig) const;
		jmethodID GetStaticMethodID(const Class& Class, const char* methodName, const char* methodsig) const;

		Object GetJavaObject(int Value) const;
		Object GetJavaObject(double Value) const;
		Object GetActivity() const;

		Object NewObject(const Class& Class, const char* CtorSig, ...) const;
		
		Object CallObjectMethod(LC_JNIObjectCallMethodArgs) const;
		bool CallBooleanMethod(LC_JNIObjectCallMethodArgs) const;
		char CallCharMethod(LC_JNIObjectCallMethodArgs) const;
		short CallShortMethod(LC_JNIObjectCallMethodArgs) const;
		int CallIntMethod(LC_JNIObjectCallMethodArgs) const;
		long CallLongMethod(LC_JNIObjectCallMethodArgs) const;
		float CallFloatMethod(LC_JNIObjectCallMethodArgs) const;
		double CallDoubleMethod(LC_JNIObjectCallMethodArgs) const;
		void CallVoidMethod(LC_JNIObjectCallMethodArgs) const;

		Object CallStaticObjectMethod(LC_JNIClassCallMethodArgs) const;
		bool CallStaticBooleanMethod(LC_JNIClassCallMethodArgs) const;
		char CallStaticCharMethod(LC_JNIClassCallMethodArgs) const;
		short CallStaticShortMethod(LC_JNIClassCallMethodArgs) const;
		int CallStaticIntMethod(LC_JNIClassCallMethodArgs) const;
		long CallStaticLongMethod(LC_JNIClassCallMethodArgs) const;
		float CallStaticFloatMethod(LC_JNIClassCallMethodArgs) const;
		double CallStaticDoubleMethod(LC_JNIClassCallMethodArgs) const;
		void CallStaticVoidMethod(LC_JNIClassCallMethodArgs) const;
          
		// Converts the java string to FString and calls DeleteLocalRef on the passed-in java string reference
		FString GetFString(const Object& JavaString) const;

		// Converts FString into a Java string wrapped in ScopedJavaObject
		Object ToJavaString(const FString& UnrealString) const;
    
		// Converts the java string to FString, does NOT modify the passed-in java string reference
		FString GetFStringFromParam(jstring JavaString) const;
	
	private:
		JNIEnv *Env;

		// Converts the java string to FString and calls DeleteGlobalRef on the passed-in java string reference
		FString GetStringFromGlobalRef(jstring JavaString) const;
	};
}

