%module tinysplinejava

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

// Automatically load native library.
%pragma(java) jniclasscode=%{
	static {
		final java.util.logging.Logger log = java.util.logging.Logger
			.getLogger(tinysplinejavaJNI.class.getName());

		// Load dependencies, ignore errors.
		try { System.loadLibrary("jvm"); } catch(Throwable e) {}
		try { System.loadLibrary("awt"); } catch(Throwable e) {}

		// Copy libraries.
		final String platform = determinePlatform();
		final java.util.Properties prop = loadProperties(platform);
		final java.util.Map<String, java.io.File> libs =
			new java.util.HashMap<String, java.io.File>();
		for (final String lib : getRuntimeLibs(prop)) {
			libs.put(lib, copyResource(platform + "/" + lib));
		}
		final String nativeLib = getNativeLib(prop);
		libs.put(nativeLib, copyResource(platform + "/" + nativeLib));

		// Load libraries.
		boolean progress;
		do {
			progress = false;
			final java.util.Iterator<String> it =
				libs.keySet().iterator();
			while (it.hasNext()) {
				final String lib = it.next();
				final java.io.File file = libs.get(lib);
				try {
					log.fine(String.format(
						"Trying to load '%s'", lib));
					System.load(file.getAbsolutePath());
					it.remove();
					progress = true;
				} catch (final UnsatisfiedLinkError e) {
				} catch (final Exception e) {
					throw new Error(String.format(
						"Error while loading '%s'",
						lib));
				}
			}
		} while (progress);

		// Check for missing libraries.
		if (!libs.isEmpty()) {
			throw new Error(String.format(
				"Unable to load the following libraries %s",
				libs.keySet().toString()));
		}
	}

	private static String determinePlatform() throws Error {
		final String os = System.getProperty("os.name").toLowerCase();
		if (os == null) {
			throw new Error(
				"Unable to determine operating system");
		}
		final String arch = System.getProperty("sun.arch.data.model");
		if (arch == null) {
			throw new Error(
				"Unable to determine architecture");
		}
		String platform = "";
		if (os.startsWith("linux")) {
			platform = "linux";
		} else if (os.startsWith("mac")) {
			platform = "macosx";
		} else if (os.startsWith("windows")) {
			platform = "windows";
		} else {
			throw new Error(String.format(
				"Unsupported operating system '%s'", os));
		}
		if (arch.equals("64")) {
			platform += "-x86_64";
		} else if (arch.equals("32")) {
			platform += "-x86";
		} else {
			throw new Error(String.format(
				"Unsupported architecture '%s'", arch));
		}
		return platform;
	}

	private static java.io.InputStream loadResource(final String path) {
		if (path == null) {
			throw new Error(
				"'null' is not a valid resource");
		}
		final java.io.InputStream is =
			tinysplinejavaJNI.class
			.getResourceAsStream("/" + path);
		if (is == null) {
			throw new Error(String.format(
				"Missing resource '%s'", path));
		}
		return is;
	}

	private static void closeQuietly(final java.io.Closeable closeable) {
		if (closeable != null) {
			try { closeable.close(); }
			catch (final java.io.IOException e) {}
		}
	}

	private static java.io.File copyResource(final String file) {
		java.io.InputStream in = null;
		java.io.OutputStream out = null;
		try {
			in = loadResource(file);
			final java.io.File tmp = java.io.File
				.createTempFile("tinyspline", null);
			tmp.deleteOnExit();
			out = new java.io.FileOutputStream(tmp);
			final byte[] buffer = new byte[4096];
			int read = -1;
			while((read = in.read(buffer)) != -1) {
				out.write(buffer, 0, read);
			}
			return tmp;
		} catch (final Exception e) {
			throw new Error(String.format(
				"Error while copying resource '%s'", file));
		} finally {
			closeQuietly(in);
			closeQuietly(out);
		}
	}

	private static java.util.Properties loadProperties(final String pltf) {
		if (pltf == null) {
			throw new Error(
				"'null' is not a valid platform");
		} else if (pltf.contains("/")) {
			throw new Error(String.format(
				"Platform id (%s) must not contain '/'",
				pltf));
		}
		final java.util.Properties prop = new java.util.Properties();
		final String file = pltf + "/libs.properties";
		java.io.InputStream is = null;
		try {
			is = loadResource(file);
			prop.load(is);
		} catch (Exception e) {
			throw new Error(String.format(
				"Error while loading properties file '%s'",
				file));
		} finally {
			closeQuietly(is);
		}
		return prop;
	}

	private static String getNativeLib(final java.util.Properties prop) {
		final String key = "native";
		final String nat = prop.getProperty(key);
		if (nat == null) {
			throw new Error(String.format(
				"Missing property '%s'", key));
		} else if (nat.trim().isEmpty()) {
			throw new Error(String.format(
				"Property '%s' is blank", key));
		}
		return nat;
	}

	private static java.util.List<String> getRuntimeLibs(
			final java.util.Properties prop) {
		final String key = "runtime";
		final String libs = prop.getProperty(key);
		if (libs == null) {
			throw new Error(String.format(
				"Missing property '%s'", key));
		}
		final String[] libsArr = libs.split(",");
		final java.util.List<String> libsList =
			new java.util.ArrayList<String>();
		for (int i = 0; i < libsArr.length; i++) {
			libsArr[i] = libsArr[i].trim();
			if (!libsArr[i].isEmpty()) {
				libsList.add(libsArr[i]);
			}
		}
		return libsList;
	}
%}

// Change the signature of the JNI file and the signature of the Java interface
// files from std::vector<tinyspline::real> to List<Float/Double>.
#ifdef TINYSPLINE_FLOAT_PRECISION
	%typemap(jtype) std::vector<tinyspline::real> * "java.util.List<Float>"
	%typemap(jstype) std::vector<tinyspline::real> * "java.util.List<Float>"
#else
	%typemap(jtype) std::vector<tinyspline::real> * "java.util.List<Double>"
	%typemap(jstype) std::vector<tinyspline::real> * "java.util.List<Double>"
#endif

// Let the Java interface files directly return the JNI result.
%typemap(javaout) std::vector<tinyspline::real> * {
	return $jnicall;
}

// Let the Java interface files redirect the input argument to JNI.
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
%typemap(newfree) std::vector<tinyspline::real> * {
	delete $1;
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
%typemap(freearg) std::vector<tinyspline::real> * {
	delete $1;
}

%include "tinyspline.i"
