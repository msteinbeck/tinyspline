%module tinysplinejava

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;

// Automatically load native library.
%pragma(java) jniclasscode=%{
	static {
		// Determine platform.
		final String os = System.getProperty("os.name").toLowerCase();
		final String arch = System.getProperty("sun.arch.data.model");
		final String library;
		if (os.startsWith("mac")) {
			library = "libtinysplinejava.jnilib";
		} else if (os.startsWith("linux") && arch.equals("64")) {
			library = "libtinysplinejava_64.so";
		} else if (os.startsWith("linux") && arch.equals("32")) {
			library = "libtinysplinejava_32.so";
		} else if (os.startsWith("windows") && arch.equals("64")) {
			library = "tinysplinejava_64.dll";
		} else if (os.startsWith("windows") && arch.equals("32")) {
			library = "tinysplinejava_32.dll";
		} else {
			library = "libtinysplinejava_generic.so";
		}

		// Copy native library to a temporary file.
		java.io.InputStream in = null;
		java.io.FileOutputStream out = null;
		java.io.File tmp;
		try {
			in = tinysplinejavaJNI.class.getResourceAsStream("/" + library);
			tmp = java.io.File.createTempFile("tinyspline", null);
			tmp.deleteOnExit();
			out = new java.io.FileOutputStream(tmp);

			final byte[] buffer = new byte[1024];
			int read = -1;
			while((read = in.read(buffer)) != -1) {
				out.write(buffer, 0, read);
			}
		} catch (final Throwable e) {
			throw new Error("Error while copying native library", e);
		} finally {
			try {
				if (in != null) {
					in.close();
				}
				if (out != null) {
					out.close();
				}
			} catch (final java.io.IOException e) { /* ignored */ }
		}

		// Load native library.
		System.load(tmp.getAbsolutePath());
	}
%}

// Change the signature of the JNI file and signature of the Java interface files from
// std::vector<tinyspline::real> to List<Float/Double>.
#ifdef TINYSPLINE_FLOAT_PRECISION
	%typemap(jtype) std::vector<tinyspline::real> * "java.util.List<Float>"
	%typemap(jstype) std::vector<tinyspline::real> * "java.util.List<Float>"
#else
	%typemap(jtype) std::vector<tinyspline::real> * "java.util.List<Double>"
	%typemap(jstype) std::vector<tinyspline::real> * "java.util.List<Double>"
#endif

// Let Java interface files directly return JNI result.
%typemap(javaout) std::vector<tinyspline::real> * {
	return $jnicall;
}

// Let Java interface files redirect the input argument to JNI.
%typemap(javain) std::vector<tinyspline::real> * "$javainput"

// Map std::vector<tinyspline::real> to List<Float/Double>.
%typemap(out) std::vector<tinyspline::real> * {
	const jclass listClass = jenv->FindClass("java/util/ArrayList");
	const jmethodID listCtor = jenv->GetMethodID(listClass, "<init>", "()V");
	const jmethodID listAdd = jenv->GetMethodID(listClass, "add", "(Ljava/lang/Object;)Z");
	const jobject list = jenv->NewObject(listClass, listCtor);
#ifdef TINYSPLINE_FLOAT_PRECISION
	const jclass realClass = jenv->FindClass("java/lang/Float");
	const jmethodID realCtor = jenv->GetMethodID(realClass, "<init>", "(F)V");
#else
	const jclass realClass = jenv->FindClass("java/lang/Double");
	const jmethodID realCtor = jenv->GetMethodID(realClass, "<init>", "(D)V");
#endif
	jobject value;
	for (std::vector<tinyspline::real>::iterator it = $1->begin(); it != $1->end(); it++) {
		value = jenv->NewObject(realClass, realCtor, *it);
		jenv->CallVoidMethod(list, listAdd, value);
		jenv->DeleteLocalRef(value);
	}
	*(jobject*)&$result = list;
}

// Map List<Float/Double> to std::vector<tinyspline::real>.
%typemap(in) std::vector<tinyspline::real> * {
	$1 = new std::vector<tinyspline::real>();
	const jobject list = *(jobject*)&$input;
  
	const jclass listClass = jenv->FindClass("java/util/ArrayList");
	const jmethodID listSize = jenv->GetMethodID(listClass, "size", "()I");
	const jmethodID listGet = jenv->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
#ifdef TINYSPLINE_FLOAT_PRECISION
	const jclass realClass = jenv->FindClass("java/lang/Float");
	const jmethodID realValue = jenv->GetMethodID(realClass, "floatValue", "()F");
	jfloat value;
#else
	const jclass realClass = jenv->FindClass("java/lang/Double");
	const jmethodID realValue = jenv->GetMethodID(realClass, "doubleValue", "()D");
	jdouble value;
#endif
	const jint size = jenv->CallIntMethod(list, listSize);
	jobject tmp;
	for (jint i = 0; i < size; i++) {
		tmp = jenv->CallObjectMethod(list, listGet, i);
#ifdef TINYSPLINE_FLOAT_PRECISION
		value = jenv->CallFloatMethod(tmp, realValue);
#else
		value = jenv->CallDoubleMethod(tmp, realValue);
#endif
		jenv->DeleteLocalRef(tmp);
		$1->push_back(value);
	}
}

// Cleanup memory allocated by typemaps.
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
