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
	import java.io.FileInputStream;
	import java.io.FileNotFoundException;
	import java.io.FileOutputStream;
	import java.io.IOException;
	import java.io.InputStream;
	import java.io.RandomAccessFile;
	import java.nio.channels.Channels;
	import java.nio.channels.FileChannel;
	import java.nio.channels.FileLock;
	import java.nio.file.Path;
	import java.nio.file.Paths;
	import java.nio.file.Files;
	import java.security.MessageDigest;
	import java.security.NoSuchAlgorithmException;
	import java.util.Arrays;
	import java.util.HashMap;
	import java.util.Iterator;
	import java.util.List;
	import java.util.Map;
	import java.util.Properties;
	import java.util.stream.Collectors;
	import java.util.logging.Logger;

	import static java.security.MessageDigest.getInstance;
%}

%pragma(java) jniclasscode=%{
	private static Logger log = Logger.getLogger(tinysplinejavaJNI.class.getName());
	private static String accumulatedLogMsg = "";

	private static void log(String msg,
	                        Object... args) {
		msg = accumulatedLogMsg += msg;
		log.fine(String.format(msg, args));
		accumulatedLogMsg = "";
	}

	private static void logNN(String msg,
	                          Object... args) {
		accumulatedLogMsg += String.format(msg, args);
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
		String platform = detectPlatform();
		Properties properties = loadProperties(platform);
		String nativeLib = nativeLib(properties);
		File storage = createLibStorage(platform + "/" + nativeLib);

		log("Reading libraries from properties file");
		List<String> libsToCopy = runtimeLibs(properties);
		libsToCopy.add(nativeLib);
		log("Libraries to be loaded: %s", libsToCopy);

		// Copy libraries.
		Map<String, File> libs = new HashMap<>();
		for (String lib : libsToCopy) {
			libs.put(lib,
			         copyResource(platform + "/" + lib,
			                      lib,
			                      storage));
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
					logNN("Trying to load: %s ... ", lib);
					System.load(file.getAbsolutePath());
					log("success");
					it.remove();
					progress = true;
				} catch (UnsatisfiedLinkError e) {
					log("%s", lib, e.getMessage());
				} catch (Exception e) {
					error(e, "Unexpected error while loading: %s", lib);
				}
			}
		} while (progress);

		// Check for missing libraries.
		if (!libs.isEmpty()) {
			error("Could not load libraries: %s",
			      libs.keySet());
		}
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

		log("Detected platform: %s", platform);
		return platform;
	}

	private static File createLibStorage(String keyResource) {
		log("Creating library storage ...");
		InputStream stream = null;
		try {
			stream = loadResource(keyResource);
			String checksum = checksumOf(stream);
			log("... with checksum: %s ...", checksum);
			String home = System.getProperty("user.home");
			if (home == null) error("User home directory not set");
			Path storage = Paths.get(home, ".tinyspline", checksum);
			if (Files.exists(storage)) {
				log("... storage already exists ...");
				if (Files.isDirectory(storage)) {
					log("... and is a directory ...");
				} else {
					log("... but is not a directory ...");
					log("... panic");
					error("%s is not a directory", storage);
				}
			} else { Files.createDirectories(storage); }
			log("... success");
			log("Library storage: %s", storage.toString());
			return storage.toFile();
		} catch (IOException e) {
			error(e, "Could not create directory");
		} finally {
			closeQuietly(stream);
		}
		throw new Error("<Unreachable>"); // for the compiler
	}

	private static String checksumOf(InputStream stream) {
		try {
			MessageDigest algo = getInstance("MD5");
			byte[] buffer = new byte[16384];
			int read = -1;
			do { read = stream.read(buffer);
			     if (read > 0) algo.update(buffer, 0, read);
			} while (read != -1);

			buffer = algo.digest();
			int val;
			StringBuilder checksum = new StringBuilder();
			for (int i = 0; i < buffer.length; i++) {
				val = (buffer[i] & 0xff) + 0x100;
				checksum.append(Integer.toString(val, 16)
				                       .substring(1));
			}
			return checksum.toString();
		} catch (NoSuchAlgorithmException e) {
			error(e, "Could not load algorithm: MD5");
		} catch (IOException e) {
			error(e, "Could not create checksum from stream");
		}
		throw new Error("<Unreachable>"); // for the compiler
	}

	private static String checksumOfFile(RandomAccessFile file) {
		FileChannel channel = file.getChannel();
		InputStream stream = Channels.newInputStream(channel);
		return checksumOf(stream);
	}

	private static String checksumOfResource(String name) {
		InputStream stream = null;
		try { stream = loadResource(name);
		      return checksumOf(stream);
		} finally { closeQuietly(stream); }
	}

	private static FileLock acquireLock(RandomAccessFile file) {
		FileLock lock = null;
		FileChannel channel = file.getChannel();
		for (int i = 1; i <= 5; i++) {
			log("... acquiring lock (%d/5) ...", i);
			try {
				lock = channel.tryLock();
				if (lock != null) break;
				Thread.sleep(500); // milliseconds
			} catch (Exception e) { /* ignored */ }
		}
		if (lock == null) {
			log("... giving up; panic");
			error("Could not acquire lock");
		}
		return lock;
	}

	private static InputStream loadResource(String name) {
		InputStream stream = tinysplinejavaJNI.class
		                     .getResourceAsStream("/" + name);
		if (stream == null) error("Missing resource: %s", name);
		return stream;
	}

	private static void closeQuietly(AutoCloseable closeable) {
		if (closeable != null) {
			try { closeable.close(); }
			catch (Exception e) { /* ignored */ }
		}
	}

	private static File copyResource(String res,
	                                 String name,
	                                 File outDir) {
		FileLock lock = null;
		InputStream in = null;
		RandomAccessFile out = null;
		try {
			log("Copying: %s ... ", name);
			File dest = new File(outDir, name);

			/* Check for existing file. Do not copy resource if
			   file already exists and has a valid checksum. */
			Path destPath = dest.toPath();
			if (Files.exists(destPath)) {
				log("... target already exists ...");
				/* Must be created after the existence check
				   because the initialization of a random
				   access file creates the file if it doesn't
				   already exist---hence, we would always enter
				   this if branch. */
				out = new RandomAccessFile(dest, "rw");
				lock = acquireLock(out);

				if (!Files.isRegularFile(destPath)) {
					log("... but is not a file ...");
					log("... panic");
					error("%s is not a file", destPath);
				}
				log("... and is a file ...");

				String csFile = checksumOfFile(out);
				String csResource = checksumOfResource(res);
				if (csFile.equals(csResource)) {
					log("... with matching checksum ...");
					log("... success");
					return dest; // no need to copy resource
				} else {
					log("... with invalid checksum ...");
					log("... file will be overwritten ...");
					out.seek(0); // restore file pointer
					out.setLength(0); // clear file
				}
			} else {
				/* This creates a new empty file. */
				out = new RandomAccessFile(dest, "rw");
				lock = acquireLock(out);
			}

			/* Copy resource to target file. */
			in = loadResource(res);
			byte[] buffer = new byte[16384];
			int read = -1;
			while((read = in.read(buffer)) != -1)
				out.write(buffer, 0, read);
			log("... success");
			return dest;
		} catch (FileNotFoundException e) {
			error(e, "Error while opening target file");
		} catch (IOException e) {
			error(e, "Error while copying file");
		} finally {
			closeQuietly(lock);
			closeQuietly(in);
			closeQuietly(out);
		}
		throw new Error("<Unreachable>"); // for the compiler
	}

	private static Properties loadProperties(String platform) {
		String file = platform + "/libs.properties";
		InputStream stream = null;
		try {
			logNN("Loading properties file: %s ... ", file);
			Properties properties = new Properties();
			stream = loadResource(file);
			properties.load(stream);
			log("success");
			return properties;
		} catch (IOException e) {
			error(e, "Error while loading properties file");
		} finally {
			closeQuietly(stream);
		}
		throw new Error("<Unreachable>"); // for the compiler
	}

	private static String nativeLib(Properties properties) {
		String key = "native";
		String nat = properties.getProperty(key);
		if (nat == null) error("Missing property: %s", key);
		nat = nat.trim();
		if (nat.isEmpty()) error("Value of %s is empty", key);
		return nat;
	}

	private static List<String> runtimeLibs(Properties properties) {
		String key = "runtime";
		String libs = properties.getProperty(key);
		if (libs == null) error("Missing property: %s", key);
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
