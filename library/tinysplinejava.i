%module tinysplinejava

%typemap(jstype) std::vector<float> "List<Float>"

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
  jfloat val; // final resulat
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
%typemap(javaimports) ts::BSpline
%{
import java.util.List;
%}

//********************************************************
//*                                                      *
//* DeBoorNet (Java)                                     *
//*                                                      *
//********************************************************
%ignore ts::DeBoorNet::operator=;
%typemap(javaimports) ts::DeBoorNet
%{
import java.util.List;
%}

//********************************************************
//*                                                      *
//* SWIG base file                                       *
//*                                                      *
//********************************************************
%include "tinyspline.i"
