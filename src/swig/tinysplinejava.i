%module tinysplinejava

// Proper enums.
%include "enums.swg"

%ignore tinyspline::BSpline::operator();
%ignore tinyspline::Morphism::operator();
%ignore tinyspline::Vec2::operator+;
%ignore tinyspline::Vec2::operator-;
%ignore tinyspline::Vec2::operator*;
%ignore tinyspline::Vec3::operator+;
%ignore tinyspline::Vec3::operator-;
%ignore tinyspline::Vec3::operator*;
%ignore tinyspline::Vec4::operator+;
%ignore tinyspline::Vec4::operator-;
%ignore tinyspline::Vec4::operator*;

%javamethodmodifiers tinyspline::Vec2::norm "@Deprecated public";
%javamethodmodifiers tinyspline::Vec3::norm "@Deprecated public";
%javamethodmodifiers tinyspline::Vec4::norm "@Deprecated public";

%javamethodmodifiers tinyspline::BSpline::toString "@Override public";
%javamethodmodifiers tinyspline::ChordLengths::toString "@Override public";
%javamethodmodifiers tinyspline::DeBoorNet::toString "@Override public";
%javamethodmodifiers tinyspline::Domain::toString "@Override public";
%javamethodmodifiers tinyspline::Frame::toString "@Override public";
%javamethodmodifiers tinyspline::FrameSeq::toString "@Override public";
%javamethodmodifiers tinyspline::Morphism::toString "@Override public";
%javamethodmodifiers tinyspline::Vec2::toString "@Override public";
%javamethodmodifiers tinyspline::Vec3::toString "@Override public";
%javamethodmodifiers tinyspline::Vec4::toString "@Override public";

// Automatically load native library.
%pragma(java) jniclassimports=%{
	import java.io.Closeable;
	import java.io.File;
	import java.io.FileOutputStream;
	import java.io.IOException;
	import java.io.InputStream;
	import java.io.OutputStream;
	import java.nio.file.Path;
	import java.nio.file.Paths;
	import java.nio.file.Files;
	import java.nio.file.attribute.PosixFilePermission;
	import java.util.ArrayList;
	import java.util.Arrays;
	import java.util.HashMap;
	import java.util.HashSet;
	import java.util.Iterator;
	import java.util.List;
	import java.util.Map;
	import java.util.Properties;
	import java.util.Set;
	import java.util.stream.Collectors;
	import java.util.logging.Logger;
%}

%pragma(java) jniclasscode=%{
	private static Logger log = Logger.getLogger(tinysplinejavaJNI.class.getName());

	private static void log(String msg,
	                        Object... args) {
		log.fine(String.format(msg, args));
	}

	private static void error(String msg,
	                          Object... args) {
		throw new Error(String.format(msg, args));
	}

	private static void error(Throwable t,
	                          String msg,
	                          Object... args) {
		throw new Error(String.format(msg, args), t);
	}

	static {
		File tmpDir = createTmpDir();
		log("Temporary directory is: %s", tmpDir);

		// Determine the libraries to be loaded.
		String platform = detectPlatform();
		log("Detected platform: %s", platform);

		log("Loading platform specific properties file");
		Properties prop = loadProperties(platform);

		log("Reading libraries from properties file");
		List<String> libsToCopy = runtimeLibs(prop);
		String nativeLib = nativeLib(prop);
		libsToCopy.add(nativeLib);
		log("Libraries to be loaded: %s", libsToCopy);

		// Copy libraries.
		Map<String, File> libs = new HashMap<>();
		for (String lib : libsToCopy) {
			log("Copying: %s ...", lib);
			libs.put(lib,
			         copyResource(platform + "/" + lib,
			                      lib,
			                      tmpDir));
			log("... done");
		}

		// Load libraries.
		boolean progress;
		do {
			progress = false;
			Iterator<String> it = libs.keySet().iterator();
			while (it.hasNext()) {
				String lib = it.next();
				File file = libs.get(lib);
				try {
					log("Trying to load: %s ...", lib);
					System.load(file.getAbsolutePath());
					log("... successfully loaded: %s", lib);
					it.remove();
					progress = true;
				} catch (UnsatisfiedLinkError e) {
					log("... %s could not be loaded yet. Message: %s",
					    lib,
					    e.getMessage());
				} catch (Exception e) {
					error(e, "Unexpected error while loading: %s", lib);
				}
			}
		} while (progress);

		// Check for missing libraries.
		if (!libs.isEmpty())
			error("Could not load libraries: %s", libs.keySet());
	}

	private static File createTmpDir() {
		log("Creating temporary directory ...");

		final Path tmpDir;
		try {
			log("... using prefix: tinyspline ...");
			tmpDir = Files.createTempDirectory("tinyspline");
		} catch (Exception e) {
			error(e, "Could not create directory");
			// Just for the compiler.
			throw new Error("<Unreachable>");
		}

		log("... setting permissions ...");
		Set<PosixFilePermission> perms = new HashSet<>();
		perms.add(PosixFilePermission.OWNER_READ);
		perms.add(PosixFilePermission.OWNER_WRITE);
		perms.add(PosixFilePermission.OWNER_EXECUTE);
		try {
			Files.setPosixFilePermissions(tmpDir, perms);
		} catch (UnsupportedOperationException e) {
			log("... posix permissions are not supported ...");
			log("... falling back to default permission system ...");
			tmpDir.toFile().setReadable(true, true);
			tmpDir.toFile().setWritable(true, true);
			tmpDir.toFile().setExecutable(true, true);
		} catch (Exception e) {
			error(e, "Could not set permissions");
		}

		log("... adding shutdown hook ...");
		Runtime.getRuntime().addShutdownHook(new Thread(
			new Runnable() {
				@Override
				public void run() {
					try { delete(tmpDir.toFile()); }
					catch (Exception e) {}
				}

				private void delete(File file) {
					if (file.isDirectory()) {
						Arrays.stream(file.listFiles())
						      .forEach(this::delete);
					}
					try { Files.delete(file.toPath()); }
					catch (IOException e) { /* ignored */ }
				}
			}));

		log("... done");
		return tmpDir.toFile();
	}

	private static String detectPlatform() {
		String os = System.getProperty("os.name");
		String arch = System.getProperty("os.arch");

		if (os == null) error("Could not detect operating system");
		if (arch == null) error("Could not detect architecture");
		os = os.toLowerCase();
		if (arch.equals("aarch32")) arch = "arm32";
		if (arch.equals("aarch64")) arch = "arm64";
		if (arch.equals("x86_64"))  arch = "amd64";
		if (arch.equals("x86-64"))  arch = "amd64";

		String platform = "";
		if (os.startsWith("linux"))        platform = "linux";
		else if (os.startsWith("mac"))     platform = "macosx";
		else if (os.startsWith("windows")) platform = "windows";
		else error("Unsupported operating system: %s", os);

		if (arch.equals("x86"))        platform += "-x86";
		else if (arch.equals("amd64")) platform += "-x86_64";
		else if (arch.equals("arm32")) platform += "-arm32";
		else if (arch.equals("arm64")) platform += "-arm64";
		else {
			log("Unknown architecture: %s ...", arch);
			log("... assuming x86_64");
			platform += "-x86_64";
		}

		return platform;
	}

	private static InputStream loadResource(String path) {
		InputStream is = tinysplinejavaJNI.class
		                 .getResourceAsStream("/" + path);
		if (is == null) error("Missing resource: %s", path);
		return is;
	}

	private static void closeQuietly(Closeable closeable) {
		if (closeable != null) {
			try { closeable.close(); }
			catch (IOException e) { /* ignored */ }
		}
	}

	private static File copyResource(String res,
	                                 String name,
	                                 File outDir) {
		InputStream in = null;
		OutputStream out = null;
		try {
			in = loadResource(res);
			File dest = new File(outDir, name);
			out = new FileOutputStream(dest);
			byte[] buffer = new byte[16384];
			int read = -1;
			while((read = in.read(buffer)) != -1)
				out.write(buffer, 0, read);
			return dest;
		} catch (Exception e) {
			error(e, "Could not copy resource: %s", res);
			// Just for the compiler.
			throw new Error("<Unreachable>");
		} finally {
			closeQuietly(in);
			closeQuietly(out);
		}
	}

	private static Properties loadProperties(String platform) {
		Properties prop = new Properties();
		String file = platform + "/libs.properties";
		InputStream is = null;
		try {
			is = loadResource(file);
			prop.load(is);
		} catch (Exception e) {
			error(e,
			      "Could not load properties file: %s",
			      file);
		} finally {
			closeQuietly(is);
		}
		return prop;
	}

	private static String nativeLib(Properties prop) {
		String key = "native";
		String nat = prop.getProperty(key);
		if (nat == null)
			error("Missing property: %s", key);
		else if (nat.trim().isEmpty())
			error("Blank property: %s", key);
		return nat;
	}

	private static List<String> runtimeLibs(Properties prop) {
		String key = "runtime";
		String libs = prop.getProperty(key);
		if (libs == null)
			error("Missing property: %s", key);
		return Arrays.stream(libs.split(","))
			.map(String::trim)
			.filter(l -> !l.isEmpty())
			.collect(Collectors.toList());
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

	const jclass listClass = jenv->FindClass("java/util/List");
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
