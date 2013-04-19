%module DetectorModule

%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("detector");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      System.exit(1);
    }
  }
%}


%typemap(throws) SWIGTYPE, SWIGTYPE &, SWIGTYPE *, SWIGTYPE [ANY] %{
  std::string msg = "C++ $1_type exception thrown. Reason: ";
  msg.append($1.what());
  SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, msg.c_str());
  return $null;
%}

%typemap(jni) std::map<std::string, std::string> "jobject"
%typemap(jtype) std::map<std::string, std::string> "Object"
%typemap(jstype) std::map<std::string, std::string> "java.util.Map<String, String>"

%typemap(in) std::map<std::string, std::string> (std::map<std::string, std::string> sMap) {
        if (!$input) {
                SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null map");
                return $null;
        }
        jclass mapClazz = jenv->FindClass("java/util/Map");
        jclass iteratorClazz = jenv->FindClass("java/util/Iterator");
        jclass setClazz = jenv->FindClass("java/util/Set");
        jmethodID keySetMethodId = jenv->GetMethodID(mapClazz, "keySet", "()Ljava/util/Set;");
        jmethodID getMethodId = jenv->GetMethodID(mapClazz, "get", " (Ljava/lang/Object;)Ljava/lang/Object;");
        jobject keySet = jenv->CallObjectMethod($input, keySetMethodId);
        if (!keySet) return $null;
        jmethodID iteratorMethodId = jenv->GetMethodID(setClazz, "iterator", "()Ljava/util/Iterator;");
        jobject iterator = jenv->CallObjectMethod(keySet, iteratorMethodId);
        if (!iterator) return $null;
        jmethodID hasNextMethodId = jenv->GetMethodID(iteratorClass, "hasNext", "()Z;");
        jmethodID nextMethodId = jenv->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
        sMap.clear();
        while(jenv->CallBooleanMethod(iterator, hasNextMethodId)) {
                jstring key = (jstring)jenv->CallObjectMethod(iterator, nextMethodId);
                jstring value = (jstring)jenv->CallObjectMethod(iterator, getMethodId, key);

                const char * keyString = jenv->GetStringUTFChars(jenv, key, 0);
                const char * cKey = malloc((strlen(keyString)+1)*sizeof(char));
                strcpy(cKey, keyString);
                jenv->ReleaseStringUTFChars(jenv, key, keyString);

                const char * valueString = jenv->GetStringUTFChars(jenv, value, 0);
                const char * cValue = malloc((strlen(valueString)+1)*sizeof(char));
                strcpy(cValue, valueString);
                jenv->ReleaseStringUTFChars(jenv, value, valueString);

                sMap.insert(make_pair(cKey, cValue));
        }
        $1 = &sMap;
}

%typemap(out) std::map<std::string, std::string> {
        jclass hashMapClazz = jenv->FindClass("java/util/HashMap");
        jmethodID initMethodId = jenv->GetMethodID(hashMapClazz, "<init>", "()V");
        jmethodID putMethodId = jenv->GetMethodID(hashMapClazz, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        $result = jenv->NewObject(hashMapClazz, initMethodId);
        for (std::map<std::string, std::string>::iterator it = $1.begin(); it != $1.end(); it++)
        {
                jenv->CallObjectMethod($result, putMethodId, jenv->NewStringUTF(it->first.c_str()), jenv->NewStringUTF(it->second.c_str()));
        }
}

%typemap(javain) std::map<std::string, std::string> "$javainput"
%typemap(javaout) std::map<std::string, std::string> { return (java.util.Map<String, String>)$jnicall; }

%apply std::map<std::string, std::string> { std::map<std::string, std::string> const & };

%{
#include "detector.h"
%}

%include "detector.h"
