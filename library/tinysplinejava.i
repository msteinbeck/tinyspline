%module tinysplinejava

// change the signature of the jni file from std::vector<ts::rational> to
// List<Float/Double>
%typemap(jtype) std::vector<float> * "java.util.List<Float>"
%typemap(jtype) std::vector<double> * "java.util.List<Double>"

// change the signature of the java interface files from
// std::vector<ts:rational> to List<Float/Double>
%typemap(jstype) std::vector<float> * "java.util.List<Float>"
%typemap(jstype) std::vector<double> * "java.util.List<Double>"

// let java interface files directly return jni result
%typemap(javaout) std::vector<ts::rational> * {
  return $jnicall;
}

// let java interface files redirect the input argument to jni
%typemap(javain) std::vector<ts::rational> * "$javainput"

// std::vector<float> to List<Float>
%typemap(out) std::vector<float> * {
  const jclass listClass = jenv->FindClass("java/util/ArrayList");
  const jmethodID listCtor = jenv->GetMethodID(listClass, "<init>", "()V");
  const jmethodID listAdd = jenv->GetMethodID(
      listClass, "add", "(Ljava/lang/Object;)Z");
  const jobject list = jenv->NewObject(listClass, listCtor);

  const jclass floatClass = jenv->FindClass("java/lang/Float");
  const jmethodID floatCtor = jenv->GetMethodID(floatClass, "<init>", "(F)V");

  jobject value;
  for (std::vector<float>::iterator it = $1->begin();
      it != $1->end(); it++) {
    value = jenv->NewObject(floatClass, floatCtor, *it);
    jenv->CallVoidMethod(list, listAdd, value);
    jenv->DeleteLocalRef(value);
  }
  *(jobject*)&$result = list;
}

// std::vector<double> to List<Double>
%typemap(out) std::vector<double> * {
  const jclass listClass = jenv->FindClass("java/util/ArrayList");
  const jmethodID listCtor = jenv->GetMethodID(listClass, "<init>", "()V");
  const jmethodID listAdd = jenv->GetMethodID(
      listClass, "add", "(Ljava/lang/Object;)Z");
  const jobject list = jenv->NewObject(listClass, listCtor);

  const jclass doubleClass = jenv->FindClass("java/lang/Double");
  const jmethodID doubleCtor = jenv->GetMethodID(doubleClass, "<init>", "(D)V");

  jobject value;
  for (std::vector<double>::iterator it = $1->begin();
      it != $1->end(); it++) {
    value = jenv->NewObject(doubleClass, doubleCtor, *it);
    jenv->CallVoidMethod(list, listAdd, value);
    jenv->DeleteLocalRef(value);
  }
  *(jobject*)&$result = list;
}

// List<Float> to std::vector<float>
%typemap(in) std::vector<float> * {
  $1 = new std::vector<float>();
  const jobject list = *(jobject*)&$input;
  
  const jclass listClass = jenv->FindClass("java/util/ArrayList");
  const jmethodID listSize = jenv->GetMethodID(listClass, "size", "()I");
  const jmethodID listGet = jenv->GetMethodID(
      listClass, "get", "(I)Ljava/lang/Object;");
  
  const jclass floatClass = jenv->FindClass("java/lang/Float");
  const jmethodID floatFloatValue = jenv->GetMethodID(
      floatClass, "floatValue", "()F");
  
  const jint size = jenv->CallIntMethod(list, listSize);
  jobject tmp;
  jfloat value;
  for (jint i = 0; i < size; i++) {
    tmp = jenv->CallObjectMethod(list, listGet, i);
    value = jenv->CallFloatMethod(tmp, floatFloatValue);
    jenv->DeleteLocalRef(tmp);
    $1->push_back(value);
  }
}

// List<Double> to std::vector<double>
%typemap(in) std::vector<double> * {
  $1 = new std::vector<double>();
  const jobject list = *(jobject*)&$input;

  const jclass listClass = jenv->FindClass("java/util/ArrayList");
  const jmethodID listSize = jenv->GetMethodID(listClass, "size", "()I");
  const jmethodID listGet = jenv->GetMethodID(
      listClass, "get", "(I)Ljava/lang/Object;");

  const jclass doubleClass = jenv->FindClass("java/lang/Double");
  const jmethodID doubleDoubleValue = jenv->GetMethodID(
      doubleClass, "doubleValue", "()D");

  const jint size = jenv->CallIntMethod(list, listSize);
  jobject tmp;
  jdouble value;
  for (jint i = 0; i < size; i++) {
    tmp = jenv->CallObjectMethod(list, listGet, i);
    value = jenv->CallDoubleMethod(tmp, doubleDoubleValue);
    jenv->DeleteLocalRef(tmp);
    $1->push_back(value);
  }
}

// cleanup memory allocated while applying a typemap
%typemap(freearg) std::vector<float> * {
  delete $1;
}
%typemap(freearg) std::vector<double> * {
  delete $1;
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
