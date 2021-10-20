###############################################################################
### General CMake setup.
###############################################################################
cmake_minimum_required(VERSION 3.13)
project(tinyspline)

# Turn on folders.
# https://cmake.org/cmake/help/v3.0/prop_gbl/USE_FOLDERS.html
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add additional CMake modules.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")



###############################################################################
### Project-related constants.
#
# TINYSPLINE_PACKAGE_NAME
#   Name of the package.
#
# TINYSPLINE_AUTHOR
#   Author's full name.
#
# TINYSPLINE_MAIL
#   Contact email address.
#
# TINYSPLINE_URL
#   URL of homepage.
#
# TINYSPLINE_ISSUE_TRACKER
#   URL of issue tracker.
#
# TINYSPLINE_REPOSITORY
#   URL of repository.
#
# TINYSPLINE_LICENSE
#   TinySpline's license.
#
# TINYSPLINE_VERSION
#   Current version.
#
# TINYSPLINE_SUMMARY
#   Short describtion.
#
# TINYSPLINE_DESCRIPTION
#   Long describtion.
###############################################################################
set(TINYSPLINE_PACKAGE_NAME
	"tinyspline"
	CACHE INTERNAL "")

set(TINYSPLINE_AUTHOR
	"Marcel Steinbeck"
	CACHE INTERNAL "")

set(TINYSPLINE_MAIL
	"tinyspline@retux.de"
	CACHE INTERNAL "")

set(TINYSPLINE_URL
	"https://github.com/msteinbeck/tinyspline"
	CACHE INTERNAL "")

set(TINYSPLINE_ISSUE_TRACKER
	"https://github.com/msteinbeck/tinyspline/issues"
	CACHE INTERNAL "")

set(TINYSPLINE_REPOSITORY
	"https://github.com/msteinbeck/tinyspline.git"
	CACHE INTERNAL "")

set(TINYSPLINE_SCM
	"scm:git:git://github.com/msteinbeck/tinyspline.git"
	CACHE INTERNAL "")

set(TINYSPLINE_SCM_CONNECTION
	"scm:git:git@github.com:msteinbeck/tinyspline.git"
	CACHE INTERNAL "")

set(TINYSPLINE_LICENSE
	"MIT"
	CACHE INTERNAL "")

set(TINYSPLINE_LICENSE_URL
	"https://opensource.org/licenses/mit-license"
	CACHE INTERNAL "")

set(TINYSPLINE_VERSION
	"0.4.0"
	CACHE INTERNAL "")

set(TINYSPLINE_SUMMARY
	"Spline Library for a Multitude of Programming Languages"
	CACHE INTERNAL "")

set(TINYSPLINE_DESCRIPTION
	"TinySpline is a small, yet powerful library for interpolating, transforming, and querying arbitrary NURBS, B-Splines, and Bézier curves. The core of the library is written in ANSI C (C89) with a C++ wrapper for an object-oriented programming model. Based on the C++ wrapper, auto-generated bindings for C#, D, Go, Java, Javascript, Lua, Octave, PHP, Python, R, and Ruby are provided."
	CACHE INTERNAL "")



###############################################################################
### Location-related constants. The default output directories will be restored
### at the end of this script.
#
# CMAKE_RUNTIME_OUTPUT_DIRECTORY_BACKUP
#   Backup of CMAKE_RUNTIME_OUTPUT_DIRECTORY.
#
# CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP
#   Backup of CMAKE_LIBRARY_OUTPUT_DIRECTORY.
#
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY_BACKUP
#   Backup of CMAKE_ARCHIVE_OUTPUT_DIRECTORY.
#
# CMAKE_RUNTIME_OUTPUT_DIRECTORY_<CONFIG>_BACKUP
#   Backup of config specific CMAKE_RUNTIME_OUTPUT_DIRECTORY.
#
# CMAKE_LIBRARY_OUTPUT_DIRECTORY_<CONFIG>_BACKUP
#   Backup of config specific CMAKE_LIBRARY_OUTPUT_DIRECTORY.
#
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY_<CONFIG>_BACKUP
#   Backup of config specific CMAKE_ARCHIVE_OUTPUT_DIRECTORY.
#
# TINYSPLINE_OUTPUT_DIRECTORY
#   Output directory of build artifacts.
#
# TINYSPLINE_BINARY_DIR
#   Parent directory of TINYSPLINE_OUTPUT_DIRECTORY.
#
# TINYSPLINE_<LANG>_SOURCE_DIRECTORY
#   Location of the source code of binding <LANG>.
#
# TINYSPLINE_<LANG>_INTERFACE_FILE
#   Full path of the interface file of binding <LANG> (if any).
#
# TINYSPLINE_DLANG_INTERMEDIATE_FILE
#   Full path of D intermediate file.
###############################################################################
if(NOT DEFINED TINYSPLINE_OUTPUT_DIRECTORY)
	set(TINYSPLINE_OUTPUT_DIRECTORY
		"${CMAKE_CURRENT_BINARY_DIR}/lib" CACHE INTERNAL "")
endif()

get_filename_component(TINYSPLINE_BINARY_DIR
	${TINYSPLINE_OUTPUT_DIRECTORY} DIRECTORY)

# Generic no-config case (e.g. MinGW)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_BACKUP ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_BACKUP ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TINYSPLINE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TINYSPLINE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TINYSPLINE_OUTPUT_DIRECTORY})
# Multi-config builds (e.g. MSVC)
foreach(TINYSPLINE_CONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${TINYSPLINE_CONFIG} TINYSPLINE_CONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}_BACKUP
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}_BACKUP
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}_BACKUP
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}
		${TINYSPLINE_OUTPUT_DIRECTORY})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}
		${TINYSPLINE_OUTPUT_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}
		${TINYSPLINE_OUTPUT_DIRECTORY})
endforeach()

set(TINYSPLINE_CSHARP_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/csharp"
	CACHE INTERNAL "")
set(TINYSPLINE_DLANG_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/dlang"
	CACHE INTERNAL "")
set(TINYSPLINE_GO_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/go"
	CACHE INTERNAL "")
set(TINYSPLINE_JAVA_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/java"
	CACHE INTERNAL "")
set(TINYSPLINE_LUA_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/lua"
	CACHE INTERNAL "")
set(TINYSPLINE_OCTAVE_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/octave"
	CACHE INTERNAL "")
set(TINYSPLINE_PHP_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/php"
	CACHE INTERNAL "")
set(TINYSPLINE_PYTHON_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/python"
	CACHE INTERNAL "")
set(TINYSPLINE_R_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/r"
	CACHE INTERNAL "")
set(TINYSPLINE_RUBY_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/ruby"
	CACHE INTERNAL "")

set(TINYSPLINE_CSHARP_INTERFACE_FILE
	"${TINYSPLINE_OUTPUT_DIRECTORY}/TinySpline.dll"
	CACHE INTERNAL "")
set(TINYSPLINE_DLANG_INTERFACE_FILE
	"${TINYSPLINE_OUTPUT_DIRECTORY}/tinyspline.d"
	CACHE INTERNAL "")
set(TINYSPLINE_GO_INTERFACE_FILE
	"${TINYSPLINE_OUTPUT_DIRECTORY}/tinyspline.go"
	CACHE INTERNAL "")
set(TINYSPLINE_JAVA_INTERFACE_FILE
	"${TINYSPLINE_OUTPUT_DIRECTORY}/tinyspline.jar"
	CACHE INTERNAL "")
set(TINYSPLINE_PHP_INTERFACE_FILE
	"${TINYSPLINE_OUTPUT_DIRECTORY}/tinyspline.php"
	CACHE INTERNAL "")
set(TINYSPLINE_PYTHON_INTERFACE_FILE
	"${TINYSPLINE_OUTPUT_DIRECTORY}/tinyspline.py"
	CACHE INTERNAL "")
set(TINYSPLINE_R_INTERFACE_FILE
	"${TINYSPLINE_OUTPUT_DIRECTORY}/tinyspline.R"
	CACHE INTERNAL "")

set(TINYSPLINE_DLANG_INTERMEDIATE_FILE
	"${TINYSPLINE_OUTPUT_DIRECTORY}/tinyspline_im.d"
	CACHE INTERNAL "")

# Add additional files and directories to delete with
# 'cmake --build . --target clean'.
list(APPEND TINYSPLINE_ADDITIONAL_MAKE_CLEAN_FILES
	${TINYSPLINE_CSHARP_SOURCE_DIRECTORY}
	${TINYSPLINE_CSHARP_INTERFACE_FILE}
	${TINYSPLINE_DLANG_SOURCE_DIRECTORY}
	${TINYSPLINE_DLANG_INTERFACE_FILE}
	${TINYSPLINE_DLANG_INTERMEDIATE_FILE}
	${TINYSPLINE_GO_SOURCE_DIRECTORY}
	${TINYSPLINE_GO_INTERFACE_FILE}
	${TINYSPLINE_JAVA_SOURCE_DIRECTORY}
	${TINYSPLINE_JAVA_INTERFACE_FILE}
	${TINYSPLINE_LUA_SOURCE_DIRECTORY}
	${TINYSPLINE_OCTAVE_SOURCE_DIRECTORY}
	${TINYSPLINE_PHP_SOURCE_DIRECTORY}
	${TINYSPLINE_PHP_INTERFACE_FILE}
	${TINYSPLINE_PYTHON_SOURCE_DIRECTORY}
	${TINYSPLINE_PYTHON_INTERFACE_FILE}
	${TINYSPLINE_R_SOURCE_DIRECTORY}
	${TINYSPLINE_R_INTERFACE_FILE}
	${TINYSPLINE_RUBY_SOURCE_DIRECTORY}
)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
	"${TINYSPLINE_ADDITIONAL_MAKE_CLEAN_FILES}")



###############################################################################
### Host- and platform-related constants.
#
# TINYSPLINE_HOST_IS_WINDOWS
#   True if host is Windows. False otherwise.
#
# TINYSPLINE_PLATFORM_NAME
#   Name of target platform in lowercase. Supported values are: 'linux',
#   'macosx', 'windows', 'uwp' (Universal Windows Platform), 'wasm'
#   (webassembly), and 'undefined' (fallback).
#
# TINYSPLINE_PLATFORM_IS_WINDOWS
#   True if target platform is Windows. False otherwise.
#
# TINYSPLINE_PLATFORM_ARCH
#   Architecture of target platform. Supported values are: 'arm', 'arm64',
#   'x86', 'x86_64', 'ia64', 'any' (usually only with 'wasm'), and 'unknown'
#   (fallback).
#
# TINYSPLINE_PLATFORM
#   Platform (target) specific identifier, e.g. 'linux-x86_64'.
###############################################################################
# Host
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(TINYSPLINE_HOST_IS_WINDOWS True CACHE INTERNAL "")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "WindowsStore")
	set(TINYSPLINE_HOST_IS_WINDOWS True CACHE INTERNAL "")
else()
	set(TINYSPLINE_HOST_IS_WINDOWS False CACHE INTERNAL "")
endif()
# Platform
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(TINYSPLINE_PLATFORM_NAME "linux" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM_IS_WINDOWS False CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set(TINYSPLINE_PLATFORM_NAME "macosx" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM_IS_WINDOWS False CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(TINYSPLINE_PLATFORM_NAME "windows" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM_IS_WINDOWS True CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
	set(TINYSPLINE_PLATFORM_NAME "uwp" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM_IS_WINDOWS True CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
	set(TINYSPLINE_PLATFORM_NAME "wasm" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM_IS_WINDOWS False CACHE INTERNAL "")
else()
	message(WARNING "Undefined platform name: ${CMAKE_SYSTEM_NAME}"
		" -- Builds might be broken")
	set(TINYSPLINE_PLATFORM_NAME "undefined" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM_IS_WINDOWS False CACHE INTERNAL "")
endif()

include(TargetArch)
target_architecture(TINYSPLINE_PLATFORM_ARCH)
if(${TINYSPLINE_PLATFORM_ARCH} STREQUAL "unknown")
	message(WARNING "Unknown platform architecture"
		" -- Builds might be broken")
endif()

set(TINYSPLINE_PLATFORM
	"${TINYSPLINE_PLATFORM_NAME}-${TINYSPLINE_PLATFORM_ARCH}"
	CACHE INTERNAL "")



###############################################################################
### CMake-related constants.
#
# TINYSPLINE_C_LIBRARY_OUTPUT_NAME
#   Output name of the C library without prefix/postfix.
#
# TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME
#   Output name of the C++ library without prefix/postfix.
#
# TINYSPLINE_JS_LIBRARY_OUTPUT_NAME
#   Output name of the JavaScript library without prefix/postfix.
#
# TINYSPLINE_<LANG>_CMAKE_TARGET
#   CMake build target of binding <LANG>.
###############################################################################
set(TINYSPLINE_C_LIBRARY_OUTPUT_NAME "tinyspline")
set(TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME "tinysplinecxx")
set(TINYSPLINE_JS_LIBRARY_OUTPUT_NAME "tinyspline")
set(TINYSPLINE_CSHARP_CMAKE_TARGET "tinysplinecsharp" CACHE INTERNAL "")
set(TINYSPLINE_DLANG_CMAKE_TARGET "tinysplinedlang" CACHE INTERNAL "")
set(TINYSPLINE_GO_CMAKE_TARGET "tinysplinego" CACHE INTERNAL "")
set(TINYSPLINE_JAVA_CMAKE_TARGET "tinysplinejava" CACHE INTERNAL "")
set(TINYSPLINE_JS_CMAKE_TARGET "tinysplinejs" CACHE INTERNAL "")
set(TINYSPLINE_LUA_CMAKE_TARGET "tinysplinelua" CACHE INTERNAL "")
set(TINYSPLINE_OCTAVE_CMAKE_TARGET "tinysplineoctave" CACHE INTERNAL "")
set(TINYSPLINE_PHP_CMAKE_TARGET "tinysplinephp" CACHE INTERNAL "")
set(TINYSPLINE_PYTHON_CMAKE_TARGET "tinysplinepython" CACHE INTERNAL "")
set(TINYSPLINE_R_CMAKE_TARGET "tinyspliner" CACHE INTERNAL "")
set(TINYSPLINE_RUBY_CMAKE_TARGET "tinysplineruby" CACHE INTERNAL "")



###############################################################################
### Add different options to this script.
#
# BUILD_SHARED_LIBS
#   Build the C/C++ interface as shared library.
#
# TINYSPLINE_INSTALL_INCLUDE_DIR
#   Installation directory of header files.
#
# TINYSPLINE_INSTALL_LIBRARY_DIR
#   Installation directory of library files.
#
# TINYSPLINE_INSTALL_BINARY_DIR
#   Installation directory of binary files.
#
# TINYSPLINE_INSTALL_CMAKE_CONFIG_DIR
#   Installation directory of CMake config files.
#
# TINYSPLINE_INSTALL_PKGCONFIG_DIR
#   Installation directory of pkg-config files.
#
# TINYSPLINE_FLOAT_PRECISION - default: OFF
#   Build with float instead of double precision.
#
# TINYSPLINE_WARNINGS_AS_ERRORS - default: ON
#   Treat compiler warnings as errors by adding /WX or -Werror to the compiler
#   flags.
#
# TINYSPLINE_PYTHON_VERSION - default: ANY
#   Force Python version.
#
# TINYSPLINE_ENABLE_<LANG> - default: TRUE for CXX, FALSE otherwise
#   Enables interface <LANG>.
#
# TINYSPLINE_ENABLE_ALL_INTERFACES
#   If set, all interfaces are enabled automatically.
#
# TINYSPLINE_RUNTIME_LIBRARIES
#   List of compiler specific runtime libraries that mus be included into
#   distribution artifacts.
###############################################################################
option(BUILD_SHARED_LIBS "Build the C/C++ interface as shared library." FALSE)

set(TINYSPLINE_INSTALL_INCLUDE_DIR "include" CACHE STRING
	"Installation directory of header files (relative to CMAKE_INSTALL_PREFIX).")

set(TINYSPLINE_INSTALL_LIBRARY_DIR "" CACHE STRING
	"Installation directory of library files (relative to CMAKE_INSTALL_PREFIX). If empty, the directory is determined automatically.")
if(TINYSPLINE_INSTALL_LIBRARY_DIR STREQUAL "")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(TINYSPLINE_INSTALL_LIBRARY_DIR "lib64"
			CACHE STRING "Autodetected." FORCE)
	else()
		set(TINYSPLINE_INSTALL_LIBRARY_DIR "lib"
			CACHE STRING "Autodetected." FORCE)
	endif()
endif()

set(TINYSPLINE_INSTALL_BINARY_DIR "bin" CACHE STRING
	"Installation directory of binary files (relative to CMAKE_INSTALL_PREFIX).")

set(TINYSPLINE_INSTALL_CMAKE_CONFIG_DIR "cmake" CACHE STRING
	"Installation directory of CMake config files (relative to TINYSPLINE_INSTALL_LIBRARY_DIR).")

set(TINYSPLINE_INSTALL_PKGCONFIG_DIR "pkgconfig" CACHE STRING
	"Installation directory of pkg-config files (relative to TINYSPLINE_INSTALL_LIBRARY_DIR).")

option(TINYSPLINE_FLOAT_PRECISION "Build TinySpline with float precision." OFF)

option(TINYSPLINE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

set(TINYSPLINE_PYTHON_VERSION "ANY" CACHE STRING
	"Force Python version. Supported values are: '2', '3', and 'ANY' (fallback for unknown values).")

option(TINYSPLINE_ENABLE_CXX "Enable the C++ interface." TRUE)
option(TINYSPLINE_ENABLE_CSHARP "Enable the C# interface." FALSE)
option(TINYSPLINE_ENABLE_DLANG "Enable the D interface." FALSE)
option(TINYSPLINE_ENABLE_GO "Enable the Go interface." FALSE)
option(TINYSPLINE_ENABLE_JAVA "Enable the Java interface." FALSE)
option(TINYSPLINE_ENABLE_LUA "Enable the Lua interface." FALSE)
option(TINYSPLINE_ENABLE_OCTAVE "Enable the Octave interface." FALSE)
option(TINYSPLINE_ENABLE_PHP "Enable the PHP interface." FALSE)
option(TINYSPLINE_ENABLE_PYTHON "Enable the Python interface." FALSE)
option(TINYSPLINE_ENABLE_R "Enable the R interface." FALSE)
option(TINYSPLINE_ENABLE_RUBY "Enable the Ruby interface." FALSE)

option(TINYSPLINE_ENABLE_ALL_INTERFACES "Enable all interfaces." FALSE)
if (TINYSPLINE_ENABLE_ALL_INTERFACES)
	set(TINYSPLINE_ENABLE_CXX TRUE)
	set(TINYSPLINE_ENABLE_CSHARP TRUE)
	set(TINYSPLINE_ENABLE_DLANG TRUE)
	set(TINYSPLINE_ENABLE_GO TRUE)
	set(TINYSPLINE_ENABLE_JAVA TRUE)
	set(TINYSPLINE_ENABLE_LUA TRUE)
	set(TINYSPLINE_ENABLE_OCTAVE TRUE)
	set(TINYSPLINE_ENABLE_PHP TRUE)
	set(TINYSPLINE_ENABLE_PYTHON TRUE)
	set(TINYSPLINE_ENABLE_R TRUE)
	set(TINYSPLINE_ENABLE_RUBY TRUE)
endif()

set(TINYSPLINE_RUNTIME_LIBRARIES "" CACHE FILEPATH
	"Compiler specific runtime libraries that must be included into distribution artifacts. If empty, the necessary files are determined automatically.")



###############################################################################
### Map supported environment variables to CMake variables.
#
# BUILD_SHARED_LIBS
#   See corresponding option above.
#
# TINYSPLINE_FLOAT_PRECISION
#   See corresponding option above.
#
# TINYSPLINE_PYTHON_VERSION
#   See corresponding option above.
###############################################################################
if(DEFINED ENV{BUILD_SHARED_LIBS})
	message(STATUS
		"Using environment variable 'BUILD_SHARED_LIBS'")
	set(BUILD_SHARED_LIBS $ENV{BUILD_SHARED_LIBS})
endif()

if(DEFINED ENV{TINYSPLINE_FLOAT_PRECISION})
	message(STATUS
		"Using environment variable 'TINYSPLINE_FLOAT_PRECISION'")
	set(TINYSPLINE_FLOAT_PRECISION $ENV{TINYSPLINE_FLOAT_PRECISION})
endif()

if(DEFINED ENV{TINYSPLINE_PYTHON_VERSION})
	message(STATUS
		"Using environment variable 'TINYSPLINE_PYTHON_VERSION'")
	set(TINYSPLINE_PYTHON_VERSION $ENV{TINYSPLINE_PYTHON_VERSION})
endif()



###############################################################################
### Setup compiler suite.
#
# TINYSPLINE_C_DEFINITIONS
#   Preprocessor definitions for the C interface (as list).
#
# TINYSPLINE_CXX_DEFINITIONS
#   Preprocessor definitions for the C++ interface (as list).
#
# TINYSPLINE_PKGCONFIG_C_FLAGS
#   Cflags added to the pkg-config file of the C library.
#
# TINYSPLINE_PKGCONFIG_CXX_FLAGS
#   Cflags added to the pkg-config file of the C++ library.
#
# TINYSPLINE_CXX11_COMPLIANT
#   Does the C++ compiler support C++11? (Required by some bindings)
#
# TINYSPLINE_C_LINK_LIBRARIES
#   Additional libraries to link against when building TinySpline's C interface
#   from source or when linking against it statically. (as list).
#
# TINYSPLINE_CXX_LINK_LIBRARIES
#   Additional libraries to link against when building TinySpline's C++
#   interface from source or when linking against it statically. (as list).
#
# TINYSPLINE_LIBRARY_C_FLAGS
#   C library specific flags.
#
# TINYSPLINE_LIBRARY_CXX_FLAGS
#   C++ library specific flags.
#
# TINYSPLINE_BINDING_CXX_FLAGS
#   C++ binding specific flags.
#
# TINYSPLINE_RUNTIME_LIBS
#   Stores the names of the files that are copied (and stripped) from
#   TINYSPLINE_RUNTIME_LIBRARIES to TINYSPLINE_OUTPUT_DIRECTORY.
###############################################################################
# TINYSPLINE_C_DEFINITIONS
# TINYSPLINE_CXX_DEFINITIONS
if(BUILD_SHARED_LIBS)
	if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		# Enables the export/import of the symbols of the shared C and
		# C++ library. The export of symbols (which is required to
		# build the shared libraries) is enabled by additionally
		# supplying TINYSPLINE_EXPORT. If missing, symbols are
		# automatically imported (which is required to link against the
		# shared libraries). By default, TINYSPLINE_SHARED also
		# specifies the behavior of the C++ interface. However, it can
		# be configures independently by defining TINYSPLINECXX_API as
		# __declspec(dllexport) (export) or __declspec(dllimport)
		# (import). Note that TINYSPLINE_EXPORT should not be added to
		# the following lists. Otherwise, it will be added to install
		# targets (CMake, pkg-config etc.) so that targets linking
		# against the C/C++ library forcibly reuse this definition
		# (which means that symbols are supposed to be exported
		# although they must be imported). Instead, TINYSPLINE_EXPORT
		# is added to the compile flags of the corresponding CMake
		# targets (see below).
		list(APPEND TINYSPLINE_C_DEFINITIONS "TINYSPLINE_SHARED")
		list(APPEND TINYSPLINE_CXX_DEFINITIONS "TINYSPLINE_SHARED")
	endif()
endif()
if(TINYSPLINE_FLOAT_PRECISION)
	list(APPEND TINYSPLINE_C_DEFINITIONS "TINYSPLINE_FLOAT_PRECISION")
	list(APPEND TINYSPLINE_CXX_DEFINITIONS "TINYSPLINE_FLOAT_PRECISION")
endif()

# TINYSPLINE_PKGCONFIG_C_FLAGS
foreach(def ${TINYSPLINE_C_DEFINITIONS})
	set(TINYSPLINE_PKGCONFIG_C_FLAGS
		"${TINYSPLINE_PKGCONFIG_C_FLAGS} -D${def}")
endforeach()
string(STRIP "${TINYSPLINE_PKGCONFIG_C_FLAGS}"
	TINYSPLINE_PKGCONFIG_C_FLAGS)
# TINYSPLINE_PKGCONFIG_CXX_FLAGS
foreach(def ${TINYSPLINE_CXX_DEFINITIONS})
	set(TINYSPLINE_PKGCONFIG_CXX_FLAGS
		"${TINYSPLINE_PKGCONFIG_CXX_FLAGS} -D${def}")
endforeach()
string(STRIP "${TINYSPLINE_PKGCONFIG_CXX_FLAGS}"
	TINYSPLINE_PKGCONFIG_CXX_FLAGS)

# TINYSPLINE_CXX11_COMPLIANT
# TINYSPLINE_C_LINK_LIBRARIES
# TINYSPLINE_CXX_LINK_LIBRARIES
# TINYSPLINE_LIBRARY_C_FLAGS
# TINYSPLINE_LIBRARY_CXX_FLAGS
# TINYSPLINE_BINDING_CXX_FLAGS
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	# TINYSPLINE_CXX11_COMPLIANT
	# http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7 OR
			CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.7)
		set(TINYSPLINE_CXX11_COMPLIANT TRUE)
	else()
		set(TINYSPLINE_CXX11_COMPLIANT FALSE)
	endif()

	# TINYSPLINE_C_LINK_LIBRARIES
	# TINYSPLINE_CXX_LINK_LIBRARIES
	set(TINYSPLINE_C_LINK_LIBRARIES
		"${TINYSPLINE_C_LINK_LIBRARIES} m")
	set(TINYSPLINE_CXX_LINK_LIBRARIES
		"${TINYSPLINE_CXX_LINK_LIBRARIES} m")

	# TINYSPLINE_LIBRARY_C_FLAGS
	list(APPEND TINYSPLINE_LIBRARY_C_FLAGS
		"-std=c89"
		"-pedantic"
		"-Wall"
		"-Wextra"
		"-Wfloat-equal")
	if(TINYSPLINE_WARNINGS_AS_ERRORS)
		list(APPEND TINYSPLINE_LIBRARY_C_FLAGS "-Werror")
	endif()
	list(JOIN TINYSPLINE_LIBRARY_C_FLAGS " "
		TINYSPLINE_LIBRARY_C_FLAGS)

	# TINYSPLINE_LIBRARY_CXX_FLAGS
	list(APPEND TINYSPLINE_LIBRARY_CXX_FLAGS
		"-Wall"
		"-Wextra"
		"-Wfloat-equal")
	if(TINYSPLINE_WARNINGS_AS_ERRORS)
		list(APPEND TINYSPLINE_LIBRARY_CXX_FLAGS "-Werror")
	endif()
	list(JOIN TINYSPLINE_LIBRARY_CXX_FLAGS " "
		TINYSPLINE_LIBRARY_CXX_FLAGS)

	# TINYSPLINE_BINDING_CXX_FLAGS
	set(TINYSPLINE_BINDING_CXX_FLAGS
		"${TINYSPLINE_BINDING_CXX_FLAGS} -w -DSWIG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# TINYSPLINE_CXX11_COMPLIANT
	# http://stackoverflow.com/questions/10408849/how-can-i-use-c-11-features-in-clang
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.3 OR
			CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 3.3)
		set(TINYSPLINE_CXX11_COMPLIANT TRUE)
	else()
		set(TINYSPLINE_CXX11_COMPLIANT FALSE)
	endif()

	# TINYSPLINE_C_LINK_LIBRARIES
	# TINYSPLINE_CXX_LINK_LIBRARIES
	set(TINYSPLINE_C_LINK_LIBRARIES
		"${TINYSPLINE_C_LINK_LIBRARIES} m")
	set(TINYSPLINE_CXX_LINK_LIBRARIES
		"${TINYSPLINE_CXX_LINK_LIBRARIES} m")

	# TINYSPLINE_LIBRARY_C_FLAGS
	if(EMSCRIPTEN)
		# error: 'long long' is an extension when C99 mode is not
		# enabled [-Werror,-Wlong-long] (from Parson)
		list(APPEND TINYSPLINE_LIBRARY_C_FLAGS "-std=c99")
	else()
		list(APPEND TINYSPLINE_LIBRARY_C_FLAGS "-std=c89")
	endif()
	list(APPEND TINYSPLINE_LIBRARY_C_FLAGS
		"-pedantic"
		"-Wall"
		"-Wextra"
		"-Wfloat-equal")
	if(TINYSPLINE_WARNINGS_AS_ERRORS)
		list(APPEND TINYSPLINE_LIBRARY_C_FLAGS "-Werror")
	endif()
	list(JOIN TINYSPLINE_LIBRARY_C_FLAGS " "
		TINYSPLINE_LIBRARY_C_FLAGS)

	# TINYSPLINE_LIBRARY_CXX_FLAGS
	list(APPEND TINYSPLINE_LIBRARY_CXX_FLAGS
		"-Wall"
		"-Wextra"
		"-Wfloat-equal")
	if(TINYSPLINE_WARNINGS_AS_ERRORS)
		list(APPEND TINYSPLINE_LIBRARY_CXX_FLAGS "-Werror")
	endif()
	list(JOIN TINYSPLINE_LIBRARY_CXX_FLAGS " "
		TINYSPLINE_LIBRARY_CXX_FLAGS)

	# TINYSPLINE_BINDING_CXX_FLAGS
	set(TINYSPLINE_BINDING_CXX_FLAGS
		"${TINYSPLINE_BINDING_CXX_FLAGS} -w -DSWIG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	# TINYSPLINE_CXX11_COMPLIANT
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 18 OR
			CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 18)
		set(TINYSPLINE_CXX11_COMPLIANT TRUE)
	else()
		set(TINYSPLINE_CXX11_COMPLIANT FALSE)
	endif()

	# TINYSPLINE_LIBRARY_C_FLAGS
	list(APPEND TINYSPLINE_LIBRARY_C_FLAGS "/W4")
	if(TINYSPLINE_WARNINGS_AS_ERRORS)
		list(APPEND TINYSPLINE_LIBRARY_C_FLAGS "/WX")
	endif()
	if("${TINYSPLINE_C_DEFINITIONS}" MATCHES "TINYSPLINE_SHARED")
		list(APPEND TINYSPLINE_LIBRARY_C_FLAGS
			"/DTINYSPLINE_EXPORT")
	endif()
	list(JOIN TINYSPLINE_LIBRARY_C_FLAGS " "
		TINYSPLINE_LIBRARY_C_FLAGS)

	# TINYSPLINE_LIBRARY_CXX_FLAGS
	list(APPEND TINYSPLINE_LIBRARY_CXX_FLAGS "/W4")
	if(TINYSPLINE_WARNINGS_AS_ERRORS)
		list(APPEND TINYSPLINE_LIBRARY_CXX_FLAGS "/WX")
	endif()
	if("${TINYSPLINE_C_DEFINITIONS}" MATCHES "TINYSPLINE_SHARED")
		list(APPEND TINYSPLINE_LIBRARY_CXX_FLAGS
			"/DTINYSPLINE_EXPORT")
	endif()
	list(JOIN TINYSPLINE_LIBRARY_CXX_FLAGS " "
		TINYSPLINE_LIBRARY_CXX_FLAGS)

	# TINYSPLINE_BINDING_CXX_FLAGS
	set(TINYSPLINE_BINDING_CXX_FLAGS
		"${TINYSPLINE_BINDING_CXX_FLAGS} /w /DSWIG")

	# Automatically create a module definition (.def) file with all global
	# symbols found in the input .obj files for a shared library.
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()
string(STRIP "${TINYSPLINE_C_LINK_LIBRARIES}" TINYSPLINE_C_LINK_LIBRARIES)
string(STRIP "${TINYSPLINE_CXX_LINK_LIBRARIES}" TINYSPLINE_CXX_LINK_LIBRARIES)
string(STRIP "${TINYSPLINE_LIBRARY_C_FLAGS}" TINYSPLINE_LIBRARY_C_FLAGS)
string(STRIP "${TINYSPLINE_LIBRARY_CXX_FLAGS}" TINYSPLINE_LIBRARY_CXX_FLAGS)
string(STRIP "${TINYSPLINE_BINDING_CXX_FLAGS}" TINYSPLINE_BINDING_CXX_FLAGS)

# TINYSPLINE_RUNTIME_LIBS
set(TINYSPLINE_RUNTIME_LIBS "")
if(TINYSPLINE_RUNTIME_LIBRARIES STREQUAL "")
	if(${TINYSPLINE_PLATFORM_NAME} STREQUAL "windows")
		# Store relevant files in CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS.
		# This allows to reuse CMake's InstallRequiredSystemLibraries
		# module.
		set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS "")
		if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
			set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
			include(InstallRequiredSystemLibraries)
		elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
			get_filename_component(basedir
				${CMAKE_CXX_COMPILER}
				DIRECTORY)
			list(APPEND
				CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
				"${basedir}/libstdc++-6.dll")
			# Detecting the used exception model is not quite as
			# easy as one would suspect. Thus, we simply add all
			# existing DLL files that may be used for exception
			# handling.
			list(APPEND
				TINYSPLINE_EXCEPTION_RUNTIME_LIBRARIES
				"${basedir}/libgcc_s_seh-1.dll"
				"${basedir}/libgcc_s_dw2-1.dll"
				"${basedir}/libgcc_s_sjlj-1.dll")
			foreach(file ${TINYSPLINE_EXCEPTION_RUNTIME_LIBRARIES})
				if(EXISTS ${file})
					list(APPEND
					CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
					${file})
				endif()
			endforeach()
			# Add libwinpthread if it exists, indicating that
			# libstdc++ is linked against POSIX threads.
			if(EXISTS "${basedir}/libwinpthread-1.dll")
				list(APPEND
					CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
					"${basedir}/libwinpthread-1.dll")
			endif()
		endif()
		# Override cached variable.
		set(TINYSPLINE_RUNTIME_LIBRARIES
			${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
			CACHE FILEPATH "Autodetected." FORCE)
	endif()
endif()
foreach(file ${TINYSPLINE_RUNTIME_LIBRARIES})
	if(NOT EXISTS ${file})
		message(FATAL_ERROR "'${file}' does not exist")
	endif()
	file(COPY ${file} DESTINATION ${TINYSPLINE_OUTPUT_DIRECTORY})
	get_filename_component(name ${file} NAME)
	list(APPEND TINYSPLINE_RUNTIME_LIBS ${name})
	if (CMAKE_STRIP)
		execute_process(
			COMMAND "${CMAKE_STRIP}" --strip-unneeded ${name}
			WORKING_DIRECTORY "${TINYSPLINE_OUTPUT_DIRECTORY}")
	endif()
endforeach()



###############################################################################
### Create the C/C++ libraries.
#
# TINYSPLINE_C_INCLUDE_DIR
#   Include directory (absolute path) of the C library.
#
# TINYSPLINE_CXX_INCLUDE_DIR
#   Include directory (absolute path) of the C++ library.
#
# TINYSPLINE_C_SOURCE_FILES
#   List of source files (absolute paths) that are required to build the C
#   library. Does not contain header files.
#
# TINYSPLINE_CXX_SOURCE_FILES
#   List of source files (absolute paths) that are required to build the C++
#   library. Does not contain header files, but all source files listed in
#   TINYSPLINE_C_SOURCE_FILES.
###############################################################################
# TINYSPLINE_C_INCLUDE_DIR
# TINYSPLINE_CXX_INCLUDE_DIR
set(TINYSPLINE_C_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TINYSPLINE_CXX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# TINYSPLINE_C_SOURCE_FILES
list(APPEND TINYSPLINE_C_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/tinyspline.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/parson.c")

# TINYSPLINE_CXX_SOURCE_FILES
list(APPEND TINYSPLINE_CXX_SOURCE_FILES
	${TINYSPLINE_C_SOURCE_FILES}
	"${CMAKE_CURRENT_SOURCE_DIR}/tinysplinecxx.cxx")

include(CMakePackageConfigHelpers)
list(APPEND TINYSPLINE_INSTALL_PKGCONFIG_PATH
	"${TINYSPLINE_INSTALL_LIBRARY_DIR}"
	"${TINYSPLINE_INSTALL_PKGCONFIG_DIR}")
list(JOIN TINYSPLINE_INSTALL_PKGCONFIG_PATH "/"
	TINYSPLINE_INSTALL_PKGCONFIG_PATH)

# C library
list(APPEND TINYSPLINE_C_INSTALL_CMAKE_CONFIG_DIR
	"${TINYSPLINE_INSTALL_LIBRARY_DIR}"
	"${TINYSPLINE_INSTALL_CMAKE_CONFIG_DIR}"
	"${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}")
list(JOIN TINYSPLINE_C_INSTALL_CMAKE_CONFIG_DIR "/"
	TINYSPLINE_C_INSTALL_CMAKE_CONFIG_DIR)

add_library(tinyspline ${TINYSPLINE_C_SOURCE_FILES})
target_compile_definitions(tinyspline
	PUBLIC ${TINYSPLINE_C_DEFINITIONS})
set_target_properties(tinyspline PROPERTIES
	OUTPUT_NAME "${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}"
	COMPILE_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS}"
	PUBLIC_HEADER "tinyspline.h")
target_link_libraries(tinyspline
	PRIVATE ${TINYSPLINE_C_LINK_LIBRARIES})

target_include_directories(tinyspline PUBLIC
	$<BUILD_INTERFACE:${TINYSPLINE_C_INCLUDE_DIR}>
	$<INSTALL_INTERFACE:${TINYSPLINE_INSTALL_INCLUDE_DIR}>)

install(TARGETS tinyspline EXPORT tinyspline
	RUNTIME DESTINATION ${TINYSPLINE_INSTALL_BINARY_DIR}
	LIBRARY DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
	ARCHIVE DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
	PUBLIC_HEADER DESTINATION ${TINYSPLINE_INSTALL_INCLUDE_DIR})
install(EXPORT tinyspline
	DESTINATION "${TINYSPLINE_C_INSTALL_CMAKE_CONFIG_DIR}"
	NAMESPACE "tinyspline::"
	FILE "${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}-targets.cmake")
configure_package_config_file(
	"pkg/tinyspline-config.cmake.in"
	"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}-config.cmake"
	INSTALL_DESTINATION "${TINYSPLINE_C_INSTALL_CMAKE_CONFIG_DIR}"
	PATH_VARS
		TINYSPLINE_INSTALL_INCLUDE_DIR
		TINYSPLINE_INSTALL_LIBRARY_DIR
		TINYSPLINE_INSTALL_BINARY_DIR)
write_basic_package_version_file(
	"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}-config-version.cmake"
	VERSION ${TINYSPLINE_VERSION}
	COMPATIBILITY ExactVersion)
install(FILES
		"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}-config.cmake"
		"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}-config-version.cmake"
	DESTINATION "${TINYSPLINE_C_INSTALL_CMAKE_CONFIG_DIR}")
list(APPEND TINYSPLINE_PKGCONFIG_C_LINK_LIBRARIES
	"${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}")
if(NOT BUILD_SHARED_LIBS)
	list(APPEND TINYSPLINE_PKGCONFIG_C_LINK_LIBRARIES
		"${TINYSPLINE_C_LINK_LIBRARIES}")
endif()
list(JOIN TINYSPLINE_PKGCONFIG_C_LINK_LIBRARIES " -l"
	TINYSPLINE_PKGCONFIG_C_LINK_LIBRARIES)
set(TINYSPLINE_PKGCONFIG_C_LINK_LIBRARIES
	"-l${TINYSPLINE_PKGCONFIG_C_LINK_LIBRARIES}")
configure_file("pkg/tinyspline.pc.in"
	"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}.pc" @ONLY)
install(FILES
		"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_C_LIBRARY_OUTPUT_NAME}.pc"
	DESTINATION "${TINYSPLINE_INSTALL_PKGCONFIG_PATH}")

# C++ library
if(TINYSPLINE_ENABLE_CXX)
	list(APPEND TINYSPLINE_CXX_INSTALL_CMAKE_CONFIG_DIR
		"${TINYSPLINE_INSTALL_LIBRARY_DIR}"
		"${TINYSPLINE_INSTALL_CMAKE_CONFIG_DIR}"
		"${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}")
	list(JOIN TINYSPLINE_CXX_INSTALL_CMAKE_CONFIG_DIR "/"
		TINYSPLINE_CXX_INSTALL_CMAKE_CONFIG_DIR)

	function (tinyspline_add_cxx_library)
		cmake_parse_arguments(ARGS "EMSCRIPTEN" "" "" ${ARGN})
		set(target "tinysplinecxx")
		set(name "${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}")
		set(flags "${TINYSPLINE_LIBRARY_CXX_FLAGS}")
		if(${ARGS_EMSCRIPTEN})
			set(target "${TINYSPLINE_JS_CMAKE_TARGET}")
			set(name "${TINYSPLINE_JS_CMAKE_TARGET}")
			set(flags "${flags} -DTINYSPLINE_EMSCRIPTEN")
		endif()
		add_library(${target} ${TINYSPLINE_CXX_SOURCE_FILES})
		target_compile_definitions(${target}
			PUBLIC ${TINYSPLINE_CXX_DEFINITIONS})
		set_target_properties(${target} PROPERTIES
			OUTPUT_NAME ${name}
			COMPILE_FLAGS ${flags})
		if(NOT ${ARGS_EMSCRIPTEN})
			set_property(TARGET ${target} APPEND PROPERTY
				PUBLIC_HEADER "tinysplinecxx.h")
		endif()
		target_link_libraries(${target}
			PRIVATE ${TINYSPLINE_CXX_LINK_LIBRARIES})
	endfunction()

	tinyspline_add_cxx_library()
	if(EMSCRIPTEN)
		tinyspline_add_cxx_library(EMSCRIPTEN)
		add_custom_command(TARGET ${TINYSPLINE_JS_CMAKE_TARGET}
			POST_BUILD
			COMMENT "Generating Javascript Interface (C++)"
			COMMAND "${CMAKE_C_COMPILER}"
				--bind
				-o ${TINYSPLINE_JS_LIBRARY_OUTPUT_NAME}.js
				-s NO_DISABLE_EXCEPTION_CATCHING
				-s ALLOW_MEMORY_GROWTH=1
				-s ABORTING_MALLOC=0
				-Wl,--whole-archive $<TARGET_FILE_NAME:${TINYSPLINE_JS_CMAKE_TARGET}>
				-Wl,--no-whole-archive
			WORKING_DIRECTORY "${TINYSPLINE_OUTPUT_DIRECTORY}"
			BYPRODUCTS
				"${TINYSPLINE_OUTPUT_DIRECTORY}/${TINYSPLINE_JS_LIBRARY_OUTPUT_NAME}.js"
				"${TINYSPLINE_OUTPUT_DIRECTORY}/${TINYSPLINE_JS_LIBRARY_OUTPUT_NAME}.wasm")
	endif()

	target_include_directories(tinysplinecxx PUBLIC
		$<BUILD_INTERFACE:${TINYSPLINE_CXX_INCLUDE_DIR}>
		$<INSTALL_INTERFACE:${TINYSPLINE_INSTALL_INCLUDE_DIR}>)

	install(TARGETS tinysplinecxx EXPORT tinysplinecxx
		RUNTIME DESTINATION ${TINYSPLINE_INSTALL_BINARY_DIR}
		LIBRARY DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
		ARCHIVE DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
		PUBLIC_HEADER DESTINATION ${TINYSPLINE_INSTALL_INCLUDE_DIR})
	install(EXPORT tinysplinecxx
		DESTINATION "${TINYSPLINE_CXX_INSTALL_CMAKE_CONFIG_DIR}"
		NAMESPACE "tinysplinecxx::"
		FILE "${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}-targets.cmake")
	configure_package_config_file(
		"pkg/tinysplinecxx-config.cmake.in"
		"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}-config.cmake"
		INSTALL_DESTINATION "${TINYSPLINE_CXX_INSTALL_CMAKE_CONFIG_DIR}"
		PATH_VARS
			TINYSPLINE_INSTALL_INCLUDE_DIR
			TINYSPLINE_INSTALL_LIBRARY_DIR
			TINYSPLINE_INSTALL_BINARY_DIR)
	write_basic_package_version_file(
		"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}-config-version.cmake"
		VERSION ${TINYSPLINE_VERSION}
		COMPATIBILITY ExactVersion)
	install(FILES
			"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}-config.cmake"
			"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}-config-version.cmake"
		DESTINATION "${TINYSPLINE_CXX_INSTALL_CMAKE_CONFIG_DIR}")
	list(APPEND TINYSPLINE_PKGCONFIG_CXX_LINK_LIBRARIES
		"${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}")
	if(NOT BUILD_SHARED_LIBS)
		list(APPEND TINYSPLINE_PKGCONFIG_CXX_LINK_LIBRARIES
			"${TINYSPLINE_CXX_LINK_LIBRARIES}")
	endif()
	list(JOIN TINYSPLINE_PKGCONFIG_CXX_LINK_LIBRARIES " -l"
		TINYSPLINE_PKGCONFIG_CXX_LINK_LIBRARIES)
	set(TINYSPLINE_PKGCONFIG_CXX_LINK_LIBRARIES
		"-l${TINYSPLINE_PKGCONFIG_CXX_LINK_LIBRARIES}")
	configure_file("pkg/tinysplinecxx.pc.in"
		"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}.pc" @ONLY)
	install(FILES
			"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_CXX_LIBRARY_OUTPUT_NAME}.pc"
		DESTINATION "${TINYSPLINE_INSTALL_PKGCONFIG_PATH}")
endif()



###############################################################################
### Setup SWIG and create bindings.
#
# TINYSPLINE_BINDINGS_FOLDER_NAME
#   Name of the IDE project folder containing the bindings.
#   ( set_property(GLOBAL PROPERTY USE_FOLDERS ON) )
#
# TINYSPLINE_BINDING_REQUESTED
#   At least one binding should be created.
###############################################################################
# Pass ``-module`` option to ``SWIG`` compiler if ``SWIG_MODULE_NAME`` is
# specified. Although this script passes the ``-module`` option explicitly (see
# ``SWIG_ARGS`` in ``tinyspline_add_swig_library``), we enable the new behavior
# as the old one will be removed in future releases of CMake.
cmake_policy(SET CMP0086 NEW)

# Generate standard names for SWIG targets, i.e., the actual name of a SWIG
# target corresponds to the name passed to ``swig_add_library``.
cmake_policy(SET CMP0078 NEW)

# TINYSPLINE_BINDINGS_FOLDER_NAME
set(TINYSPLINE_BINDINGS_FOLDER_NAME "bindings")

# TINYSPLINE_BINDING_REQUESTED
if(${TINYSPLINE_ENABLE_CSHARP}
		OR ${TINYSPLINE_ENABLE_DLANG}
		OR ${TINYSPLINE_ENABLE_GO}
		OR ${TINYSPLINE_ENABLE_JAVA}
		OR ${TINYSPLINE_ENABLE_LUA}
		OR ${TINYSPLINE_ENABLE_OCTAVE}
		OR ${TINYSPLINE_ENABLE_PHP}
		OR ${TINYSPLINE_ENABLE_PYTHON}
		OR ${TINYSPLINE_ENABLE_R}
		OR ${TINYSPLINE_ENABLE_RUBY})
	set(TINYSPLINE_BINDING_REQUESTED TRUE)
else()
	set(TINYSPLINE_BINDING_REQUESTED FALSE)
endif()

# Utility function for creating bindings.
function (tinyspline_add_swig_library)
	cmake_parse_arguments(ARGS
		"CXX11"
		"TARGET;TYPE;LANG;OUTPUT;LIBS;NAME"
		"SWIG_ARGS;FLAGS"
		${ARGN})
	set_source_files_properties("swig/${ARGS_TARGET}.i"
		PROPERTIES CPLUSPLUS ON)
	if (NOT DEFINED ARGS_TYPE)
		set(ARGS_TYPE MODULE)
	endif()
	list(APPEND CMAKE_SWIG_FLAGS ${ARGS_SWIG_ARGS})
	list(APPEND CMAKE_SWIG_FLAGS -O) # enable optimization
	if(TINYSPLINE_FLOAT_PRECISION)
		list(APPEND CMAKE_SWIG_FLAGS -DTINYSPLINE_FLOAT_PRECISION)
	endif()
	swig_add_library(
		${ARGS_TARGET}
		TYPE ${ARGS_TYPE}
		LANGUAGE ${ARGS_LANG}
		OUTPUT_DIR ${ARGS_OUTPUT}
		SOURCES "swig/${ARGS_TARGET}.i"
			${TINYSPLINE_CXX_SOURCE_FILES})
	# On Linux, we can (and for the sake of portability should) omit
	# linking libraries.
	if(NOT ${TINYSPLINE_PLATFORM_NAME} STREQUAL "linux")
		swig_link_libraries(${ARGS_TARGET} ${ARGS_LIBS})
	endif()
	set_target_properties(${ARGS_TARGET} PROPERTIES
		FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
		COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} ${ARGS_FLAGS}")
	target_compile_definitions(${ARGS_TARGET}
		PUBLIC ${TINYSPLINE_CXX_DEFINITIONS})
	# Fix library prefix for C#.
	if(${ARGS_LANG} STREQUAL "csharp")
		set_property(TARGET ${ARGS_TARGET}
			APPEND PROPERTY PREFIX "lib")
	endif()
	# Fix library prefix for Go.
	if(${ARGS_LANG} STREQUAL "go")
		set_property(TARGET ${ARGS_TARGET}
			APPEND PROPERTY PREFIX "lib")
	endif()
	# Fix library prefix for non-windows platforms.
	if(NOT ${TINYSPLINE_PLATFORM_IS_WINDOWS})
		if(${ARGS_LANG} STREQUAL "d")
			set_property(TARGET ${ARGS_TARGET}
				APPEND PROPERTY PREFIX "lib")
		endif()
	endif()
	if(${ARGS_CXX11})
		set_property(TARGET ${ARGS_TARGET}
			APPEND PROPERTY CXX_STANDARD 11)
		set_property(TARGET ${ARGS_TARGET}
			APPEND PROPERTY CXX_STANDARD_REQUIRED ON)
		set_property(TARGET ${ARGS_TARGET}
			APPEND PROPERTY CXX_EXTENSIONS OFF)
	endif()
	if (NOT ${ARGS_NAME} STREQUAL "")
		set_property(TARGET ${ARGS_TARGET}
			APPEND PROPERTY OUTPUT_NAME "${ARGS_NAME}")
	endif()
	if (CMAKE_STRIP)
		add_custom_command(
			TARGET ${ARGS_TARGET}
			POST_BUILD
			COMMAND
				"${CMAKE_STRIP}" --strip-unneeded
				"$<TARGET_FILE_NAME:${ARGS_TARGET}>"
			WORKING_DIRECTORY "${TINYSPLINE_OUTPUT_DIRECTORY}")
	endif()
	if(EXISTS "${TINYSPLINE_MACOSX_RELINK_SCRIPT}")
		add_custom_command(
			TARGET ${ARGS_TARGET}
			POST_BUILD
			COMMAND
				${CMAKE_COMMAND} -E echo
				"Relinking $<TARGET_FILE_NAME:${ARGS_TARGET}>"
			COMMAND
				${TINYSPLINE_MACOSX_RELINK_SCRIPT}
				"$<TARGET_FILE:${ARGS_TARGET}>")
	endif()
endfunction()

if(${TINYSPLINE_BINDING_REQUESTED})
	if(${TINYSPLINE_PLATFORM_NAME} STREQUAL "undefined")
		message(FATAL_ERROR
			"Cannot build bindings for undefined platform name")
	endif()
	if(${TINYSPLINE_PLATFORM_ARCH} STREQUAL "unknown")
		message(FATAL_ERROR
			"Cannot build bindings for unknown platform architecture")
	endif()

	find_package(SWIG 3.0.1 REQUIRED)
	include(${SWIG_USE_FILE})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})

	if(CMAKE_INSTALL_NAME_TOOL)
		if(NOT CMAKE_OTOOL)
			message(STATUS "Found install_name_tool without otool")
			message(STATUS "Trying to find corresponding otool")
			get_filename_component(CMAKE_OTOOL
				${CMAKE_INSTALL_NAME_TOOL} NAME)
			string(REPLACE "-" ";" CMAKE_OTOOL ${CMAKE_OTOOL})
			list(REMOVE_AT CMAKE_OTOOL -1)
			list(APPEND CMAKE_OTOOL "otool")
			list(JOIN CMAKE_OTOOL "-" CMAKE_OTOOL)
			get_filename_component(CMAKE_OTOOL_HINTS
				${CMAKE_INSTALL_NAME_TOOL} DIRECTORY)
			find_program(CMAKE_OTOOL "${CMAKE_OTOOL}"
				HINTS CMAKE_OTOOL_HINTS)
			if(NOT CMAKE_OTOOL)
				message(FATAL_ERROR "Cannot find otool")
			else()
				set(CMAKE_OTOOL ${CMAKE_OTOOL}
					CACHE FILEPATH "otool")
			endif()
		endif()
		set(TINYSPLINE_MACOSX_RELINK_SCRIPT
			"${TINYSPLINE_BINARY_DIR}/macosx-relink.sh")
		configure_file("pkg/macosx-relink.sh.in"
			${TINYSPLINE_MACOSX_RELINK_SCRIPT} @ONLY)
	endif()

	# C#
	if(${TINYSPLINE_ENABLE_CSHARP})
		if(${SWIG_VERSION} VERSION_LESS 4.0.1)
			message(FATAL_ERROR "C# requires Swig 4.0.1 or later")
		endif()
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_CSHARP_CMAKE_TARGET}
			LANG csharp
			OUTPUT ${TINYSPLINE_CSHARP_SOURCE_DIRECTORY}
			SWIG_ARGS -namespace TinySpline)
		# DLL
		find_package(CSharp)
		if(CSHARP_FOUND)
			if(${CSHARP_TYPE} STREQUAL ".NET")
				string(SUBSTRING ${CSHARP_DOTNET_VERSION} 1 3
					TINYSPLINE_CSHARP_FRAMEWORK_VERSION)
				# csc does not support '/' in 'recursive'.
				add_custom_command(
					TARGET ${TINYSPLINE_CSHARP_CMAKE_TARGET}
					POST_BUILD
					COMMAND
						${CSHARP_COMPILER}
						/target:library
						/out:"${TINYSPLINE_CSHARP_INTERFACE_FILE}"
						/recurse:"${TINYSPLINE_CSHARP_SOURCE_DIRECTORY}\\*.cs")
			else()
				set(TINYSPLINE_CSHARP_FRAMEWORK_VERSION "4.5")
				add_custom_command(
					TARGET ${TINYSPLINE_CSHARP_CMAKE_TARGET}
					POST_BUILD
					COMMAND
						${CSHARP_COMPILER}
						-sdk:4.5
						-target:library
						-out:"${TINYSPLINE_CSHARP_INTERFACE_FILE}"
						-recurse:"${TINYSPLINE_CSHARP_SOURCE_DIRECTORY}/*.cs")
			endif()
		endif()
	endif()

	# D
	if(${TINYSPLINE_ENABLE_DLANG})
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_DLANG_CMAKE_TARGET}
			LANG d
			OUTPUT ${TINYSPLINE_DLANG_SOURCE_DIRECTORY}
			SWIG_ARGS -d2 -wrapperlibrary ${TINYSPLINE_DLANG_CMAKE_TARGET}
				-module ${TINYSPLINE_PACKAGE_NAME})
		add_custom_command(
			TARGET ${TINYSPLINE_DLANG_CMAKE_TARGET}
			POST_BUILD
			COMMAND
				${CMAKE_COMMAND}
				-E copy
				"${TINYSPLINE_DLANG_SOURCE_DIRECTORY}/${TINYSPLINE_PACKAGE_NAME}.d"
				"${TINYSPLINE_DLANG_INTERFACE_FILE}"
			COMMAND
				${CMAKE_COMMAND}
				-E copy
				"${TINYSPLINE_DLANG_SOURCE_DIRECTORY}/${TINYSPLINE_PACKAGE_NAME}_im.d"
				"${TINYSPLINE_DLANG_INTERMEDIATE_FILE}")
	endif()

	# Go
	if(${TINYSPLINE_ENABLE_GO})
		math(EXPR TINYSPLINE_GO_INTGOSIZE "${CMAKE_SIZEOF_VOID_P} * 8")
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_GO_CMAKE_TARGET}
			TYPE STATIC
			LANG go
			OUTPUT ${TINYSPLINE_GO_SOURCE_DIRECTORY}
			SWIG_ARGS -cgo -intgosize ${TINYSPLINE_GO_INTGOSIZE}
				-package tinyspline)
		add_custom_command(
			TARGET ${TINYSPLINE_GO_CMAKE_TARGET}
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND} -E copy
			"${TINYSPLINE_GO_SOURCE_DIRECTORY}/${TINYSPLINE_GO_CMAKE_TARGET}.go"
			"${TINYSPLINE_GO_INTERFACE_FILE}")
	endif()

	# Java
	if(${TINYSPLINE_ENABLE_JAVA})
		find_package(JNI REQUIRED)
		include_directories(${JNI_INCLUDE_DIRS})
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_JAVA_CMAKE_TARGET}
			LANG java
			OUTPUT "${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/org/tinyspline"
			LIBS ${JNI_LIBRARIES}
			SWIG_ARGS -package "org.tinyspline")
		# Copy resources (libs, properties file, etc.)
		set(TINYSPLINE_JAVA_RES_DIR
			"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/${TINYSPLINE_PLATFORM}")
		if(${TINYSPLINE_PLATFORM_NAME} STREQUAL "linux")
			set(TINYSPLINE_JAVA_NATIVE_LIB
				"${TINYSPLINE_PLATFORM}.so")
		elseif(${TINYSPLINE_PLATFORM_NAME} STREQUAL "macosx")
			set(TINYSPLINE_JAVA_NATIVE_LIB
				"${TINYSPLINE_PLATFORM}.jnilib")
		else()
			if (NOT ${TINYSPLINE_PLATFORM_NAME} STREQUAL "windows")
				message(FATAL_ERROR "Assertion failed: windows")
			endif()
			set(TINYSPLINE_JAVA_NATIVE_LIB
				"${TINYSPLINE_PLATFORM}.dll")
		endif()
		string(REPLACE ";" "," TINYSPLINE_JAVA_RUNTIME_LIBS
			"${TINYSPLINE_RUNTIME_LIBS}")
		set(TINYSPLINE_JAVA_PROPERTIES_FILE "libs.properties")
		set(TINYSPLINE_JAVA_PROPERTIES_FILE_PATH
			"${TINYSPLINE_JAVA_RES_DIR}/${TINYSPLINE_JAVA_PROPERTIES_FILE}")
		file(WRITE
			${TINYSPLINE_JAVA_PROPERTIES_FILE_PATH}
			"native=${TINYSPLINE_JAVA_NATIVE_LIB}\nruntime=${TINYSPLINE_JAVA_RUNTIME_LIBS}\n")
		foreach(file ${TINYSPLINE_RUNTIME_LIBS})
			file(COPY "${TINYSPLINE_OUTPUT_DIRECTORY}/${file}"
				DESTINATION ${TINYSPLINE_JAVA_RES_DIR})
		endforeach()
		# Jar archive
		find_package(Java COMPONENTS Development)
		if(${Java_Development_FOUND})
			include(UseJava)
			add_custom_command(
				TARGET ${TINYSPLINE_JAVA_CMAKE_TARGET}
				POST_BUILD
				COMMAND
					${CMAKE_COMMAND}
					-E make_directory
					"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes"
				COMMAND
					${CMAKE_COMMAND}
					-E copy_directory
					"${TINYSPLINE_JAVA_RES_DIR}"
					"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes/${TINYSPLINE_PLATFORM}"
				COMMAND
					${CMAKE_COMMAND}
					-E copy
					"${TINYSPLINE_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${TINYSPLINE_JAVA_CMAKE_TARGET}>"
					"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes/${TINYSPLINE_PLATFORM}/${TINYSPLINE_JAVA_NATIVE_LIB}"
				COMMAND
					"${Java_JAVAC_EXECUTABLE}"
					-d "${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes"
					"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/org/tinyspline/*.java"
				COMMAND
					"${Java_JAR_EXECUTABLE}"
					-cfM "${TINYSPLINE_JAVA_INTERFACE_FILE}"
					-C "${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes" .)
		endif()
	endif()

	# Lua
	if(${TINYSPLINE_ENABLE_LUA})
		find_package(Lua REQUIRED)
		include_directories(${LUA_INCLUDE_DIR})
		set(TINYSPLINE_LUA_LIB_NAME
			"${TINYSPLINE_LUA_CMAKE_TARGET}${LUA_VERSION_MAJOR}${LUA_VERSION_MINOR}")
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_LUA_CMAKE_TARGET}
			LANG lua
			OUTPUT ${TINYSPLINE_LUA_SOURCE_DIRECTORY}
			LIBS ${LUA_LIBRARIES}
			NAME ${TINYSPLINE_LUA_LIB_NAME}
			SWIG_ARGS -module ${TINYSPLINE_LUA_LIB_NAME})
	endif()

	# Octave
	if(${TINYSPLINE_ENABLE_OCTAVE})
		find_package(Octave REQUIRED)
		if((${OCTAVE_VERSION_STRING} VERSION_GREATER 4.2
					OR ${OCTAVE_VERSION_STRING} VERSION_EQUAL 4.2)
				AND ${SWIG_VERSION} VERSION_LESS 3.0.12)
			message(FATAL_ERROR "Octave 4.2 requires Swig 3.0.12 or later")
		elseif(NOT ${TINYSPLINE_CXX11_COMPLIANT})
			message(FATAL_ERROR "Octave requires C++11")
		else()
			include_directories(${OCTAVE_INCLUDE_DIRS})
			tinyspline_add_swig_library(
				TARGET ${TINYSPLINE_OCTAVE_CMAKE_TARGET}
				LANG octave
				OUTPUT ${TINYSPLINE_OCTAVE_SOURCE_DIRECTORY}
				LIBS ${OCTAVE_LIBRARIES}
				NAME ${TINYSPLINE_PACKAGE_NAME}
				SWIG_ARGS -module ${TINYSPLINE_PACKAGE_NAME}
				CXX11)
		endif()
	endif()

	# PHP
	if(${TINYSPLINE_ENABLE_PHP})
		find_package(PHP COMPONENTS Zend REQUIRED)
		set(TINYSPLINE_PHP_VERSION "php5")
		if(${PHP_VERSION_NUMBER} MATCHES "^7")
			set(TINYSPLINE_PHP_VERSION "php7")
		endif()
		if(${TINYSPLINE_PHP_VERSION} STREQUAL "php7"
				AND ${SWIG_VERSION} VERSION_LESS 3.0.11)
			message(FATAL_ERROR "PHP 7 requires Swig 3.0.11 or later")
		elseif(${TINYSPLINE_PHP_VERSION} STREQUAL "php5"
				AND (${SWIG_VERSION} VERSION_GREATER 4.0.0
					OR ${SWIG_VERSION} VERSION_EQUAL 4.0.0))
			message(FATAL_ERROR "Swig 4 does not support PHP 5 anymore")
		else()
			include_directories(${PHP_INCLUDE_DIRS})
			tinyspline_add_swig_library(
				TARGET ${TINYSPLINE_PHP_CMAKE_TARGET}
				LANG ${TINYSPLINE_PHP_VERSION}
				OUTPUT ${TINYSPLINE_PHP_SOURCE_DIRECTORY}
				FLAGS ${PHP_INCLUDE_DIRS})
			add_custom_command(
				TARGET ${TINYSPLINE_PHP_CMAKE_TARGET}
				POST_BUILD
				COMMAND
					${CMAKE_COMMAND}
					-E copy
					"${TINYSPLINE_PHP_SOURCE_DIRECTORY}/${TINYSPLINE_PHP_CMAKE_TARGET}.php"
					"${TINYSPLINE_PHP_INTERFACE_FILE}")
		endif()
	endif()

	# Python
	if(${TINYSPLINE_ENABLE_PYTHON})
		if(${TINYSPLINE_PYTHON_VERSION} STREQUAL "2")
			set(Python_ADDITIONAL_VERSIONS
				2.7 2.6 2.5 2.4 2.3 2.2 2.1 2.0)
		elseif(${TINYSPLINE_PYTHON_VERSION} STREQUAL "3")
			set(Python_ADDITIONAL_VERSIONS
				3.9 3.8 3.7 3.6 3.5 3.4 3.3 3.2 3.1 3.0)
		endif()
		find_package(PythonLibs REQUIRED)
		if(${TINYSPLINE_PYTHON_VERSION} STREQUAL "2"
					AND NOT ${PYTHONLIBS_VERSION_STRING} MATCHES "^2."
				OR ${TINYSPLINE_PYTHON_VERSION} STREQUAL "3"
					AND NOT ${PYTHONLIBS_VERSION_STRING} MATCHES "^3.")
			message(FATAL_ERROR "Unable to find requested Python version")
		endif()
		include_directories(${PYTHON_INCLUDE_DIRS})
		if(${PYTHONLIBS_VERSION_STRING} MATCHES "^3.")
			set(TINYSPLINE_PYTHON_DEFINES "-py3")
		endif()
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_PYTHON_CMAKE_TARGET}
			LANG python
			OUTPUT ${TINYSPLINE_PYTHON_SOURCE_DIRECTORY}
			LIBS ${PYTHON_LIBRARIES}
			SWIG_ARGS ${TINYSPLINE_PYTHON_DEFINES})
		add_custom_command(
			TARGET ${TINYSPLINE_PYTHON_CMAKE_TARGET}
			POST_BUILD
			COMMAND
				${CMAKE_COMMAND} -E copy
				"${TINYSPLINE_PYTHON_SOURCE_DIRECTORY}/${TINYSPLINE_PYTHON_CMAKE_TARGET}.py"
				"${TINYSPLINE_PYTHON_INTERFACE_FILE}")
	endif()

	# R
	if(${TINYSPLINE_ENABLE_R})
		find_package(R COMPONENTS Rcpp REQUIRED)
		include_directories(${R_INCLUDE_DIRS})
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_R_CMAKE_TARGET}
			LANG r
			OUTPUT ${TINYSPLINE_R_SOURCE_DIRECTORY}
			LIBS ${R_LIBRARIES})
		add_custom_command(
			TARGET ${TINYSPLINE_R_CMAKE_TARGET}
			POST_BUILD
			COMMAND
				${CMAKE_COMMAND}
				-E copy
				"${TINYSPLINE_R_SOURCE_DIRECTORY}/${TINYSPLINE_R_CMAKE_TARGET}.R"
				"${TINYSPLINE_R_INTERFACE_FILE}")
	endif()

	# Ruby
	if(${TINYSPLINE_ENABLE_RUBY})
		find_package(Ruby REQUIRED)
		include_directories(${RUBY_INCLUDE_DIRS})
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_RUBY_CMAKE_TARGET}
			LANG ruby
			OUTPUT ${TINYSPLINE_RUBY_SOURCE_DIRECTORY}
			LIBS ${RUBY_LIBRARY})
	endif()
endif()



###############################################################################
### Setup constants that are used to determine whether a certain interface is
### available, that is, the corresponding TINYSPLINE_ENABLE_*** option is true
### and all requirements to build interface *** are met.
#
# TINYSPLINE_WITH_***
#   Is 'ON' if interface *** is available, 'OFF' otherwise.
###############################################################################
function (tinyspline_set_on_off varName varValue)
	if(${varValue})
		set(${varName} ON PARENT_SCOPE)
	else()
		set(${varName} OFF PARENT_SCOPE)
	endif()
endfunction()
function (tinyspline_set_on_off_swig varName varValue)
	if(SWIG_FOUND AND ${varValue})
		set(${varName} ON PARENT_SCOPE)
	else()
		set(${varName} OFF PARENT_SCOPE)
	endif()
endfunction()
tinyspline_set_on_off(TINYSPLINE_WITH_CXX TINYSPLINE_ENABLE_CXX)
tinyspline_set_on_off_swig(TINYSPLINE_WITH_CSHARP TINYSPLINE_ENABLE_CSHARP)
tinyspline_set_on_off_swig(TINYSPLINE_WITH_DLANG TINYSPLINE_ENABLE_DLANG)
tinyspline_set_on_off_swig(TINYSPLINE_WITH_GOLANG TINYSPLINE_ENABLE_GO)
tinyspline_set_on_off(TINYSPLINE_WITH_JAVA JNI_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_LUA LUA_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_OCTAVE OCTAVE_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_PHP PHP_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_PYTHON PYTHONLIBS_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_RUBY RUBY_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_R R_FOUND)



###############################################################################
### Configure language-specific build systems.
#
# TINYSPLINE_LIB_DIR
#   Name of TINYSPLINE_OUTPUT_DIRECTORY.
###############################################################################
get_filename_component(TINYSPLINE_LIB_DIR ${TINYSPLINE_OUTPUT_DIRECTORY} NAME)

# CPack
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${TINYSPLINE_AUTHOR})
set(CPACK_PACKAGE_VERSION ${TINYSPLINE_VERSION})
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)

# Dub
file(REMOVE "${TINYSPLINE_BINARY_DIR}/dub.json")
if(${TINYSPLINE_WITH_DLANG})
	set(TINYSPLINE_DUB_COPY_FILES "\"${TINYSPLINE_LIB_DIR}/*.d\"")
	set(TINYSPLINE_DUB_COPY_FILES "${TINYSPLINE_DUB_COPY_FILES}\
, \"${TINYSPLINE_LIB_DIR}/*${TINYSPLINE_DLANG_CMAKE_TARGET}*\"")
	foreach(lib ${TINYSPLINE_RUNTIME_LIBS})
		set(TINYSPLINE_DUB_COPY_FILES "${TINYSPLINE_DUB_COPY_FILES}\
, \"${TINYSPLINE_LIB_DIR}/${lib}\"")
	endforeach()
	configure_file("pkg/dub.json.in"
		"${TINYSPLINE_BINARY_DIR}/dub.json" @ONLY)
endif()

# Luarocks
if(${TINYSPLINE_WITH_LUA})
	set(TINYSPLINE_LUAROCKS_INSTALL_LIB
		"${TINYSPLINE_LIB_DIR}/${TINYSPLINE_LUA_LIB_NAME}")
	if(${TINYSPLINE_PLATFORM_NAME} STREQUAL "windows")
		set(TINYSPLINE_LUAROCKS_INSTALL_LIB
			"${TINYSPLINE_LUAROCKS_INSTALL_LIB}.dll")
	else()
		set(TINYSPLINE_LUAROCKS_INSTALL_LIB
			"${TINYSPLINE_LUAROCKS_INSTALL_LIB}.so")
	endif()
	set(TINYSPLINE_LUAROCKS_INSTALL_LIB
		"\"${TINYSPLINE_LUAROCKS_INSTALL_LIB}\"")
	foreach(lib ${TINYSPLINE_RUNTIME_LIBS})
		set(TINYSPLINE_LUAROCKS_INSTALL_LIB
			"${TINYSPLINE_LUAROCKS_INSTALL_LIB}\
, \"${TINYSPLINE_LIB_DIR}/${lib}\"")
	endforeach()
	set(TINYSPLINE_LUAROCKS_SPEC
		"${LUA_VERSION_MAJOR}${LUA_VERSION_MINOR}")
	set(TINYSPLINE_LUAROCKS_SPEC
		"${TINYSPLINE_LUAROCKS_SPEC}-${TINYSPLINE_VERSION}-1")
	set(TINYSPLINE_LUAROCKS_SPEC
		"tinyspline${TINYSPLINE_LUAROCKS_SPEC}.rockspec")
	configure_file("pkg/tinyspline.rockspec.in"
		"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_LUAROCKS_SPEC}" @ONLY)
endif()

# NuGet/DotNet
set(TINYSPLINE_NUSPEC_FILE "${TINYSPLINE_BINARY_DIR}/.nuspec")
file(REMOVE ${TINYSPLINE_NUSPEC_FILE})
get_filename_component(TINYSPLINE_CSPROJ_FILE
	${TINYSPLINE_CSHARP_INTERFACE_FILE}
	NAME)
string(REPLACE ".dll" ".csproj" TINYSPLINE_CSPROJ_FILE
	${TINYSPLINE_CSPROJ_FILE})
set(TINYSPLINE_CSPROJ_FILE
	"${TINYSPLINE_BINARY_DIR}/${TINYSPLINE_CSPROJ_FILE}")
file(REMOVE ${TINYSPLINE_CSPROJ_FILE})
if(DEFINED TINYSPLINE_CSHARP_FRAMEWORK_VERSION)
	get_filename_component(TINYSPLINE_NUGET_INTERFACE_FILE
		${TINYSPLINE_CSHARP_INTERFACE_FILE}
		NAME)
	find_program(TINYSPLINE_DOTNET_EXECUTABLE dotnet)
	if(${TINYSPLINE_PLATFORM_NAME} STREQUAL "linux")
		set(TINYSPLINE_NUGET_RID "linux")
	elseif(${TINYSPLINE_PLATFORM_NAME} STREQUAL "macosx")
		set(TINYSPLINE_NUGET_RID "osx")
	elseif(${TINYSPLINE_PLATFORM_NAME} STREQUAL "windows")
		set(TINYSPLINE_NUGET_RID "win")
	elseif(${TINYSPLINE_PLATFORM_NAME} STREQUAL "uwp")
		set(TINYSPLINE_NUGET_RID "win10")
	else()
		message(FATAL_ERROR "Unable to determine NuGet RID (platform)")
	endif()
	if(${TINYSPLINE_PLATFORM_ARCH} STREQUAL "arm")
		set(TINYSPLINE_NUGET_RID "${TINYSPLINE_NUGET_RID}-arm")
	elseif(${TINYSPLINE_PLATFORM_ARCH} STREQUAL "arm64")
		set(TINYSPLINE_NUGET_RID "${TINYSPLINE_NUGET_RID}-arm64")
	elseif(${TINYSPLINE_PLATFORM_ARCH} STREQUAL "x86_64")
		set(TINYSPLINE_NUGET_RID "${TINYSPLINE_NUGET_RID}-x64")
	elseif(${TINYSPLINE_PLATFORM_ARCH} STREQUAL "x86")
		set(TINYSPLINE_NUGET_RID "${TINYSPLINE_NUGET_RID}-x86")
	else()
		message(FATAL_ERROR "Unable to determine NuGet RID (arch)")
	endif()
	string(REPLACE "." "" TINYSPLINE_CSHARP_FRAMEWORK_NAME
		${TINYSPLINE_CSHARP_FRAMEWORK_VERSION})
	# Currently, only .NET Framework is supported.
	set(TINYSPLINE_CSHARP_FRAMEWORK_NAME
		"net${TINYSPLINE_CSHARP_FRAMEWORK_NAME}")
	set(TINYSPLINE_NUGET_FILES "<file \
src=\"${TINYSPLINE_LIB_DIR}/${TINYSPLINE_NUGET_INTERFACE_FILE}\" \
target=\"lib/${TINYSPLINE_CSHARP_FRAMEWORK_NAME}\" />")
	if(TINYSPLINE_DOTNET_EXECUTABLE)
		set(TINYSPLINE_NUGET_FILES "${TINYSPLINE_NUGET_FILES}\n\t\t<file \
src=\"netstandard2.0/${TINYSPLINE_NUGET_INTERFACE_FILE}\" \
target=\"lib/netstandard2.0\" />")
	endif()
	if(${TINYSPLINE_HOST_IS_WINDOWS})
		set(TINYSPLINE_NUGET_FILES "${TINYSPLINE_NUGET_FILES}\n\t\t<file \
src=\"${TINYSPLINE_LIB_DIR}\\*${TINYSPLINE_CSHARP_CMAKE_TARGET}*dll\" \
target=\"runtimes/${TINYSPLINE_NUGET_RID}/native\" />")
	else()
		set(TINYSPLINE_NUGET_FILES "${TINYSPLINE_NUGET_FILES}\n\t\t<file \
src=\"${TINYSPLINE_LIB_DIR}/*${TINYSPLINE_CSHARP_CMAKE_TARGET}.*\" \
target=\"runtimes/${TINYSPLINE_NUGET_RID}/native\" />")
	endif()
	foreach(lib ${TINYSPLINE_RUNTIME_LIBS})
		set(TINYSPLINE_NUGET_FILES
			"${TINYSPLINE_NUGET_FILES}\n\t\t<file \
src=\"${TINYSPLINE_LIB_DIR}/${lib}\" \
target=\"runtimes/${TINYSPLINE_NUGET_RID}/lib/native\" />")
	endforeach()
	configure_file("pkg/.nuspec.in"
		${TINYSPLINE_NUSPEC_FILE} @ONLY)
	if(TINYSPLINE_DOTNET_EXECUTABLE)
		configure_file("pkg/TinySpline.csproj.in"
			${TINYSPLINE_CSPROJ_FILE} @ONLY)
	endif()
endif()

# Maven
file(REMOVE "${TINYSPLINE_BINARY_DIR}/pom.xml")
if(${TINYSPLINE_WITH_JAVA})
	configure_file("pkg/pom.xml.in"
		"${TINYSPLINE_BINARY_DIR}/pom.xml" @ONLY)
endif()

# Distutils
file(REMOVE "${TINYSPLINE_BINARY_DIR}/setup.py")
if(${TINYSPLINE_WITH_PYTHON})
	get_filename_component(TINYSPLINE_SETUP_INTERFACE_FILE
		${TINYSPLINE_PYTHON_INTERFACE_FILE}
		NAME)
	set(TINYSPLINE_SETUP_FILE_ARRAY
"\"${TINYSPLINE_LIB_DIR}/${TINYSPLINE_SETUP_INTERFACE_FILE}\"")
	set(TINYSPLINE_SETUP_FILE_ARRAY "${TINYSPLINE_SETUP_FILE_ARRAY}\
, \"${TINYSPLINE_LIB_DIR}/*${TINYSPLINE_PYTHON_CMAKE_TARGET}.*\"")
	foreach(lib ${TINYSPLINE_RUNTIME_LIBS})
		set(TINYSPLINE_SETUP_FILE_ARRAY "${TINYSPLINE_SETUP_FILE_ARRAY}\
, \"${TINYSPLINE_LIB_DIR}/${lib}\"")
	endforeach()
	set(TINYSPLINE_SETUP_FILE_ARRAY "[${TINYSPLINE_SETUP_FILE_ARRAY}]")
	configure_file("pkg/setup.py.in"
		"${TINYSPLINE_BINARY_DIR}/setup.py" @ONLY)
endif()

# Gem
file(REMOVE "${TINYSPLINE_BINARY_DIR}/tinyspline.gemspec")
if(${TINYSPLINE_WITH_RUBY})
	set(TINYSPLINE_RUBY_REQ_VERSION
			"${RUBY_VERSION_MAJOR}.${RUBY_VERSION_MINOR}")
	set(TINYSPLINE_GEM_FILES
"Dir.glob(\"{${TINYSPLINE_LIB_DIR}}/*${TINYSPLINE_RUBY_CMAKE_TARGET}*\")")
	foreach(lib ${TINYSPLINE_RUNTIME_LIBS})
		set(TINYSPLINE_GEM_FILES "${TINYSPLINE_GEM_FILES} \
<< \"${TINYSPLINE_LIB_DIR}/${lib}\"")
	endforeach()
	configure_file("pkg/tinyspline.gemspec.in"
		"${TINYSPLINE_BINARY_DIR}/tinyspline.gemspec" @ONLY)
endif()

# NPM
file(REMOVE "${TINYSPLINE_BINARY_DIR}/package.json")
if(${EMSCRIPTEN})
	set(TINYSPLINE_NPM_FILES
"\"${TINYSPLINE_LIB_DIR}/${TINYSPLINE_JS_LIBRARY_OUTPUT_NAME}.js\",\
 \"${TINYSPLINE_LIB_DIR}/${TINYSPLINE_JS_LIBRARY_OUTPUT_NAME}.wasm\"")
	configure_file("pkg/package.json.in"
		"${TINYSPLINE_BINARY_DIR}/package.json" @ONLY)
endif()



###############################################################################
### Restore output directories.
###############################################################################
# Multi-config builds (e.g. MSVC)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_BACKUP})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_BACKUP})
# Generic no-config case (e.g. MinGW)
foreach(TINYSPLINE_CONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${TINYSPLINE_CONFIG} TINYSPLINE_CONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}_BACKUP})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}_BACKUP})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}
		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG}_BACKUP})
endforeach()



###############################################################################
### Print summary.
###############################################################################
string(STRIP "${CMAKE_C_FLAGS} ${TINYSPLINE_LIBRARY_C_FLAGS}"
	TINYSPLINE_SUMMARY_C_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS} ${TINYSPLINE_LIBRARY_CXX_FLAGS}"
	TINYSPLINE_SUMMARY_CXX_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS} ${TINYSPLINE_BINDING_CXX_FLAGS}"
	TINYSPLINE_SUMMARY_BINDING_FLAGS)

message(STATUS
"
/// TinySpline Configuration Summary //////////////////////////////////////////
Platform:
  Id: ${TINYSPLINE_PLATFORM}

Interface Configuration:
  [C/C++] Shared libraries (default: OFF): ${BUILD_SHARED_LIBS}
  With single precision  (default: OFF): ${TINYSPLINE_FLOAT_PRECISION}

Compiler Configuration:
  Compiler:        ${CMAKE_CXX_COMPILER}
  C flags:         ${TINYSPLINE_SUMMARY_C_FLAGS}
  C libraries:     ${TINYSPLINE_C_LINK_LIBRARIES}
  C Definitions:   ${TINYSPLINE_C_DEFINITIONS}
  C++ flags:       ${TINYSPLINE_SUMMARY_CXX_FLAGS}
  C++ libraries:   ${TINYSPLINE_CXX_LINK_LIBRARIES}
  C++ Definitions: ${TINYSPLINE_CXX_DEFINITIONS}
  Binding flags:   ${TINYSPLINE_SUMMARY_BINDING_FLAGS}
  Runtime libs:    ${TINYSPLINE_RUNTIME_LIBS}
  Toolchain file:  ${CMAKE_TOOLCHAIN_FILE}

Available Interfaces:
  C++:          ${TINYSPLINE_WITH_CXX}

  C\#:           ${TINYSPLINE_WITH_CSHARP}
    Compiler:  ${CSHARP_COMPILER}
    Framework: ${TINYSPLINE_CSHARP_FRAMEWORK_VERSION}

  D:            ${TINYSPLINE_WITH_DLANG}

  Go:           ${TINYSPLINE_WITH_GOLANG}

  Java:         ${TINYSPLINE_WITH_JAVA}
    Version:  ${Java_VERSION_STRING}
    Include:  ${JNI_INCLUDE_DIRS}
    Library:  ${JNI_LIBRARIES}
    Compiler: ${Java_JAVAC_EXECUTABLE}
    Archiver: ${Java_JAR_EXECUTABLE}

  Lua:          ${TINYSPLINE_WITH_LUA}
    Version:  ${LUA_VERSION_STRING}
    Include:  ${LUA_INCLUDE_DIR}
    Library:  ${LUA_LIBRARIES}

  Octave:       ${TINYSPLINE_WITH_OCTAVE}
    Version:  ${OCTAVE_VERSION_STRING}
    Include:  ${OCTAVE_INCLUDE_DIRS}
    Library:  ${OCTAVE_LIBRARIES}

  PHP:          ${TINYSPLINE_WITH_PHP}
    Version:  ${PHP_VERSION_STRING}
    Include:  ${PHP_INCLUDE_DIRS}

  Python:       ${TINYSPLINE_WITH_PYTHON}
    Version:  ${PYTHONLIBS_VERSION_STRING}
    Include:  ${PYTHON_INCLUDE_DIRS}
    Library:  ${PYTHON_LIBRARIES}

  R:            ${TINYSPLINE_WITH_R}
    Version:  ${R_VERSION}
    Include:  ${R_INCLUDE_DIRS}
    Library:  ${R_LIBRARIES}

  Ruby:         ${TINYSPLINE_WITH_RUBY}
    Version:  ${RUBY_VERSION}
    Include:  ${RUBY_INCLUDE_DIRS}
    Library:  ${RUBY_LIBRARY}

Installation:
  Prefix: ${CMAKE_INSTALL_PREFIX}
///////////////////////////////////////////////////////////////////////////////
")



###############################################################################
### Export relevant variables.
###############################################################################
get_directory_property(TINYSPLINE_HAS_PARENT PARENT_DIRECTORY)
if(TINYSPLINE_HAS_PARENT)
	set(TINYSPLINE_OUTPUT_DIRECTORY ${TINYSPLINE_OUTPUT_DIRECTORY} PARENT_SCOPE)
	set(TINYSPLINE_C_DEFINITIONS ${TINYSPLINE_C_DEFINITIONS} PARENT_SCOPE)
	set(TINYSPLINE_CXX_DEFINITIONS ${TINYSPLINE_CXX_DEFINITIONS} PARENT_SCOPE)
	set(TINYSPLINE_C_LINK_LIBRARIES ${TINYSPLINE_C_LINK_LIBRARIES} PARENT_SCOPE)
	set(TINYSPLINE_CXX_LINK_LIBRARIES ${TINYSPLINE_CXX_LINK_LIBRARIES} PARENT_SCOPE)
	set(TINYSPLINE_RUNTIME_LIBS ${TINYSPLINE_RUNTIME_LIBS} PARENT_SCOPE)
	set(TINYSPLINE_C_INCLUDE_DIR ${TINYSPLINE_C_INCLUDE_DIR} PARENT_SCOPE)
	set(TINYSPLINE_CXX_INCLUDE_DIR ${TINYSPLINE_CXX_INCLUDE_DIR} PARENT_SCOPE)
	set(TINYSPLINE_C_SOURCE_FILES ${TINYSPLINE_C_SOURCE_FILES} PARENT_SCOPE)
	set(TINYSPLINE_CXX_SOURCE_FILES ${TINYSPLINE_CXX_SOURCE_FILES} PARENT_SCOPE)
else()
	set(TINYSPLINE_OUTPUT_DIRECTORY ${TINYSPLINE_OUTPUT_DIRECTORY})
	set(TINYSPLINE_C_DEFINITIONS ${TINYSPLINE_C_DEFINITIONS})
	set(TINYSPLINE_CXX_DEFINITIONS ${TINYSPLINE_CXX_DEFINITIONS})
	set(TINYSPLINE_C_LINK_LIBRARIES ${TINYSPLINE_C_LINK_LIBRARIES})
	set(TINYSPLINE_CXX_LINK_LIBRARIES ${TINYSPLINE_CXX_LINK_LIBRARIES})
	set(TINYSPLINE_RUNTIME_LIBS ${TINYSPLINE_RUNTIME_LIBS})
	set(TINYSPLINE_C_INCLUDE_DIR ${TINYSPLINE_C_INCLUDE_DIR})
	set(TINYSPLINE_CXX_INCLUDE_DIR ${TINYSPLINE_CXX_INCLUDE_DIR})
	set(TINYSPLINE_C_SOURCE_FILES ${TINYSPLINE_C_SOURCE_FILES})
	set(TINYSPLINE_CXX_SOURCE_FILES ${TINYSPLINE_CXX_SOURCE_FILES})
endif()
