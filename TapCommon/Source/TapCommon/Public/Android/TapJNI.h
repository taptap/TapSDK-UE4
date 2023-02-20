#pragma once
#include <jni.h>

#define LC_JNIObjectCallMethodArgs const Object& object, const char* methodName, const char* methodsig, ...
#define LC_JNIClassCallMethodArgs const Class& Class, const char* methodName, const char* methodsig, ...


namespace TapJNI {

	// Helper class that automatically calls DeleteLocalRef on the passed-in Java object when goes out of scope
	template <typename T>
	class ScopedJavaObject
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
	
	class JNI {
	public:
		JNI();
		JNI(JNIEnv *in_env);

		// Returns the underlying JNI pointer
		JNIEnv * operator*() const { return Env; }

		template <typename T>
		ScopedJavaObject<T> MakeJavaObject(const T& InObjRef, bool InNeedDelete = true) {
			return ScopedJavaObject<T>(Env, InObjRef, InNeedDelete);
		}

		Class FindClass(const char* name) const;
		Class GetObjectClass(const Object& object) const;
		jmethodID GetMethodID(const Class& Class, const char* name, const char* sig) const;
		jmethodID GetStaticMethodID(const Class& Class, const char* name, const char* sig) const;
		jfieldID GetFieldID(const Class& Class, const char* name, const char* sig) const;
		jfieldID GetStaticFieldID(const Class& Class, const char* name, const char* sig) const;

		Object GetActivity() const;
		Object GetJavaObject(int Value) const;
		Object GetJavaObject(double Value) const;
		Object GetStringArray(const TArray<FString>& Strings) const;

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

		void SetObjectField(const Object& object, const char* name, const char* sig, const Object& value) const;
		void SetBooleanField(const Object& object, const char* name, bool value) const;
		void SetCharField(const Object& object, const char* name, char value) const;
		void SetShortField(const Object& object, const char* name, short value) const;
		void SetIntField(const Object& object, const char* name, int value) const;
		void SetLongField(const Object& object, const char* name, long value) const;
		void SetFloatField(const Object& object, const char* name, float value) const;
		void SetDoubleField(const Object& object, const char* name, double value) const;
		void SetFStringField(const Object& object, const char* name, const FString& value) const;

		Object GetObjectField(const Object& object, const char* name, const char* sig) const;
		bool GetBooleanField(const Object& object, const char* name) const;
		char GetCharField(const Object& object, const char* name) const;
		short GetShortField(const Object& object, const char* name) const;
		int GetIntField(const Object& object, const char* name) const;
		long GetLongField(const Object& object, const char* name) const;
		float GetFloatField(const Object& object, const char* name) const;
		double GetDoubleField(const Object& object, const char* name) const;
		FString GetFStringField(const Object& object, const char* name) const;

		void SetStaticObjectField(const Class& Class, const char* name, const char* sig, const Object& value) const;
		void SetStaticBooleanField(const Class& Class, const char* name, bool value) const;
		void SetStaticCharField(const Class& Class, const char* name, char value) const;
		void SetStaticShortField(const Class& Class, const char* name, short value) const;
		void SetStaticIntField(const Class& Class, const char* name, int value) const;
		void SetStaticLongField(const Class& Class, const char* name, long value) const;
		void SetStaticFloatField(const Class& Class, const char* name, float value) const;
		void SetStaticDoubleField(const Class& Class, const char* name, double value) const;
		void SetStaticFStringField(const Class& Class, const char* name, const FString& value) const;

		Object GetStaticObjectField(const Class& Class, const char* name, const char* sig) const;
		bool GetStaticBooleanField(const Class& Class, const char* name) const;
		char GetStaticCharField(const Class& Class, const char* name) const;
		short GetStaticShortField(const Class& Class, const char* name) const;
		int GetStaticIntField(const Class& Class, const char* name) const;
		long GetStaticLongField(const Class& Class, const char* name) const;
		float GetStaticFloatField(const Class& Class, const char* name) const;
		double GetStaticDoubleField(const Class& Class, const char* name) const;
		FString GetStaticFStringField(const Class& Class, const char* name) const;

          
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

