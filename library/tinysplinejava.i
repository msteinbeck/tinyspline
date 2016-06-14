%module tinysplinejava

// let jni file return List<Float>
%typemap(jtype) std::vector<float> "java.util.List<Float>"

// let java interface files return List<Float>
%typemap(jstype) std::vector<float> "java.util.List<Float>"

// simply return jni result
%typemap(javaout) std::vector<float> {
  return $jnicall;
}

// simply redirect the parameter to jni
%typemap(javain) std::vector<float> "$javainput"

// std:vector<float> to List<Float>
%typemap(out) std::vector<float> {
  const jclass listClass = jenv->FindClass("java/util/ArrayList");
  const jmethodID listCtor = jenv->GetMethodID(listClass, "<init>", "()V");
  const jmethodID listAdd = jenv->GetMethodID(listClass, "add", "(Ljava/lang/Object;)V");
  const jobject list = jenv->NewObject(listClass, listCtor);
  
  for (std::vector<float>::iterator it = $1.begin(); it != $1.end(); it++)
  {
    jenv->CallVoidMethod(list, listAdd, *it);
  }
  *(jobject*)&$result = list;
}

// List<Float> to std::vector<float>
%typemap(in) std::vector<float> {
  $1 = std::vector<float>();
  const jobject list = *(jobject*)&$input;
  
  const jclass listClass = jenv->FindClass("java/util/ArrayList");
  const jmethodID listSize = jenv->GetMethodID(listClass, "size", "()I");
  const jmethodID listGet = jenv->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
  
  const jclass floatClass = jenv->FindClass("java/lang/Float");
  const jmethodID floatFloatValue = jenv->GetMethodID(floatClass, "floatValue", "()I");
  
  const jint size = jenv->CallIntMethod(list, listSize);
  jobject value; // intermediate result
  jfloat val; // final result
  for (jint i = 0; i < size; i++) {
    value = jenv->CallObjectMethod(list, listGet, i);
    val = jenv->CallFloatMethod(value, floatFloatValue);
    jenv->DeleteLocalRef(value);
    $1.push_back(val);
  }
}

//********************************************************
//*                                                      *
//* BSpline (Java)                                       *
//*                                                      *
//********************************************************
%ignore ts::BSpline::operator();
%ignore ts::BSpline::operator=;

//********************************************************
//*                                                      *
//* DeBoorNet (Java)                                     *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
