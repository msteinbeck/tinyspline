%module tinysplinejava

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::BSpline::operator=;
%ignore tinyspline::DeBoorNet::operator=;
%ignore tinyspline::Domain::operator=;

%javamethodmodifiers tinyspline::BSpline::toString "@Override public";
%javamethodmodifiers tinyspline::DeBoorNet::toString "@Override public";
%javamethodmodifiers tinyspline::Domain::toString "@Override public";

// Automatically load native library.
%pragma(java) jniclasscode=%{
	private static final java.util.logging.Logger log = java.util.logging.Logger
		.getLogger(tinysplinejavaJNI.class.getName());

	private static void log(final String msg, final Object... args) {
		log.fine(String.format(msg, args));
	}

	private static void error(final String msg, final Object... args) {
		throw new Error(String.format(msg, args));
	}

	private static void error(final Throwable t, final String msg, final Object... args) {
		throw new Error(String.format(msg, args), t);
	}

	static {
		log("Creating temporary directory...");
		final java.io.File tmpDir = createTmpDir();
		log("... done");

		// Determine the libraries that need to be loaded.
		log("Detecting platform");
		final String platform = detectPlatform();
		log("Detected platform '%s'", platform);
		log("Loading platform specific properties file");
		final java.util.Properties prop = loadProperties(platform);
		log("Reading libraries from properties file");
		final java.util.List<String> libsToCopy = getRuntimeLibs(prop);
		libsToCopy.add(getNativeLib(prop));
		log("Libraries to load: %s", libsToCopy.toString());

		// Copy libraries.
		final java.util.Map<String, java.io.File> libs =
			new java.util.HashMap<String, java.io.File>();
		for (final String lib : libsToCopy) {
			log("Copying '%s' into '%s'", lib, tmpDir);
			libs.put(lib, copyResource(platform + "/" + lib, lib, tmpDir));
			log("Copied '%s' to '%s'", lib, libs.get(lib).getAbsolutePath());
		}

		// Load libraries.
		boolean progress;
		do {
			progress = false;
			final java.util.Iterator<String> it = libs.keySet().iterator();
			while (it.hasNext()) {
				final String lib = it.next();
				final java.io.File file = libs.get(lib);
				try {
					log("Trying to load '%s'", lib);
					System.load(file.getAbsolutePath());
					log("Successfully loaded '%s'", lib);
					it.remove();
					progress = true;
				} catch (final UnsatisfiedLinkError e) {
					log("'%s' could not be loaded yet (%s)",
						lib, e.getMessage());
				} catch (final Exception e) {
					error(e, "Unexpected error while loading '%s'", lib);
				}
			}
		} while (progress);

		// Check for missing libraries.
		if (!libs.isEmpty()) {
			error("Could not load libraries: %s", libs.keySet().toString());
		}
	}

	private static java.io.File createTmpDir() {
		final java.io.File tmpDir;
		try {
			log("Creating a temporary file to obtain path");
			tmpDir = java.io.File.createTempFile("tinyspline","");
		} catch (final java.io.IOException e) {
			error(e, "Could not create file");
			// Just for the compiler.
			throw new Error("Unreachable");
		}
		log.fine(String.format("Deleteing file '%s'", tmpDir.getPath()));
		if (!tmpDir.delete()) {
			error("Could not delete file '%s'", tmpDir.getPath());
		}
		log("Creating directory from file path");
		if(!tmpDir.mkdir()) {
			error("Could not create directory '%s'", tmpDir.getPath());
		}
		log("Setting permissions");
		tmpDir.setReadable(true, true);
		tmpDir.setWritable(true, true);
		tmpDir.setExecutable(false, false);
		log("Adding shutdown hook");
		Runtime.getRuntime().addShutdownHook(new Thread(
			new Runnable() {
				@Override
				public void run() {
					try { delete(tmpDir); }
					catch (final Exception e) {}
				}

				private void delete(final java.io.File file) {
					if (file.isDirectory()) {
						for (final java.io.File f : file.listFiles()) {
							delete(f);
						}
					}
					file.delete();
				}
			}));
		return tmpDir;
	}

	private static String detectPlatform() {
		final String os = System.getProperty("os.name").toLowerCase();
		if (os == null) {
			error("Could not detect operating system");
		}
		final String arch = System.getProperty("sun.arch.data.model");
		if (arch == null) {
			error("Could not detect architecture");
		}
		String platform = "";
		if (os.startsWith("linux")) {
			platform = "linux";
		} else if (os.startsWith("mac")) {
			platform = "macosx";
		} else if (os.startsWith("windows")) {
			platform = "windows";
		} else {
			error("Unsupported operating system '%s'", os);
		}
		if (arch.equals("64")) {
			platform += "-x86_64";
		} else if (arch.equals("32")) {
			platform += "-x86";
		} else {
			error("Unsupported architecture '%s'", arch);
		}
		return platform;
	}

	private static java.io.InputStream loadResource(final String path) {
		if (path == null) {
			error("Internal error: path to resource is 'null'");
		}
		final java.io.InputStream is =tinysplinejavaJNI.class
			.getResourceAsStream("/" + path);
		if (is == null) {
			error("Missing resource '%s'", path);
		}
		return is;
	}

	private static void closeQuietly(final java.io.Closeable closeable) {
		if (closeable != null) {
			try { closeable.close(); }
			catch (final java.io.IOException e) {}
		}
	}

	private static java.io.File copyResource(final String res, final String name,
			final java.io.File outDir) {
		if (res == null) {
			error("Internal error: path to resource is 'null'");
		}
		if (name == null) {
			error("Internal error: name of resource is 'null'");
		}
		if (outDir == null) {
			error("Internal error: output directory is 'null'");
		}
		java.io.InputStream in = null;
		java.io.OutputStream out = null;
		try {
			in = loadResource(res);
			final java.io.File dest = new java.io.File(outDir, name);
			out = new java.io.FileOutputStream(dest);
			final byte[] buffer = new byte[16384];
			int read = -1;
			while((read = in.read(buffer)) != -1) {
				out.write(buffer, 0, read);
			}
			return dest;
		} catch (final Exception e) {
			error(e, "Could not copy resource '%s'", res);
			// Just for the compiler.
			throw new Error("Unreachable");
		} finally {
			closeQuietly(in);
			closeQuietly(out);
		}
	}

	private static java.util.Properties loadProperties(final String platform) {
		if (platform == null) {
			error("Internal error: platform is 'null'");
		} else if (platform.contains("/")) {
			error("Internal error: platform id (%s) must not contain '/'", platform);
		}
		final java.util.Properties prop = new java.util.Properties();
		final String file = platform + "/libs.properties";
		java.io.InputStream is = null;
		try {
			is = loadResource(file);
			prop.load(is);
		} catch (Exception e) {
			error(e, "Could not load properties file '%s'", file);
		} finally {
			closeQuietly(is);
		}
		return prop;
	}

	private static String getNativeLib(final java.util.Properties prop) {
		if (prop == null) {
			error("Internal error: properties is 'null'");
		}
		final String key = "native";
		final String nat = prop.getProperty(key);
		if (nat == null) {
			error("Missing property '%s'", key);
		} else if (nat.trim().isEmpty()) {
			error("Property '%s' is blank", key);
		}
		return nat;
	}

	private static java.util.List<String> getRuntimeLibs(final java.util.Properties prop) {
		if (prop == null) {
			error("Internal error: properties is 'null'");
		}
		final String key = "runtime";
		final String libs = prop.getProperty(key);
		if (libs == null) {
			error("Missing property '%s'", key);
		}
		final String[] libsArr = libs.split(",");
		final java.util.List<String> libsList = new java.util.ArrayList<String>();
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
