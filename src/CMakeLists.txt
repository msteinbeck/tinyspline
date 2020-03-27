###############################################################################
### General CMake setup.
#
# TINYSPLINE_HAS_PARENT
#   Does this script have a parent?
###############################################################################
cmake_minimum_required(VERSION 3.4)
project(tinyspline)

# Turn on folders.
# https://cmake.org/cmake/help/v3.0/prop_gbl/USE_FOLDERS.html
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add additional CMake modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Does this script have a parent?
get_directory_property(TINYSPLINE_HAS_PARENT PARENT_DIRECTORY)



###############################################################################
### Project-related constants.
#
# TINYSPLINE_PACKAGE_NAME
#   The name of the package.
#
# TINYSPLINE_AUTHOR
#   The author of TinySpline.
#
# TINYSPLINE_MAIL
#   The contact email address.
#
# TINYSPLINE_URL
#   The URL of the repository managing TinySpline.
#
# TINYSPLINE_LICENSE
#   TinySpline's license.
#
# TINYSPLINE_VERSION
#   The current version of TinySpline.
#
# TINYSPLINE_SUMMARY
#   A short describtion.
#
# TINYSPLINE_DESCRIPTION
#   The long describtion.
###############################################################################
set(TINYSPLINE_PACKAGE_NAME "tinyspline" CACHE INTERNAL "")

set(TINYSPLINE_AUTHOR "Marcel Steinbeck" CACHE INTERNAL "")

set(TINYSPLINE_MAIL "tinyspline@retux.de" CACHE INTERNAL "")

set(TINYSPLINE_URL "https://github.com/msteinbeck/tinyspline" CACHE INTERNAL "")

set(TINYSPLINE_SCM "scm:git:git://github.com/msteinbeck/tinyspline.git" CACHE INTERNAL "")

set(TINYSPLINE_SCM_CONNECTION "scm:git:git@github.com:msteinbeck/tinyspline.git" CACHE INTERNAL "")

set(TINYSPLINE_LICENSE "MIT" CACHE INTERNAL "")

set(TINYSPLINE_LICENSE_URL "https://opensource.org/licenses/mit-license" CACHE INTERNAL "")

set(TINYSPLINE_VERSION "0.3.0" CACHE INTERNAL "")

set(TINYSPLINE_SUMMARY "Spline Library for a Multitude of Programming Languages" CACHE INTERNAL "")

set(TINYSPLINE_DESCRIPTION "TinySpline is a small, yet powerful library for interpolating, transforming, and querying arbitrary NURBS, B-Splines, and Bézier curves. The library is implemented in ANSI C (C89) and provides a wrapper for C++ along with auto-generated bindings for C#, D, Go, Java, Lua, Octave, PHP, Python, R, and Ruby." CACHE INTERNAL "")



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
# CMAKE_RUNTIME_OUTPUT_DIRECTORY_***_BACKUP
#   Backup of config specific CMAKE_RUNTIME_OUTPUT_DIRECTORY (with *** being
#   one of the available CMake configuration types).
#
# CMAKE_LIBRARY_OUTPUT_DIRECTORY_***_BACKUP
#   Backup of config specific CMAKE_LIBRARY_OUTPUT_DIRECTORY (with *** being
#   one of the available CMake configuration types).
#
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY_***_BACKUP
#   Backup of config specific CMAKE_ARCHIVE_OUTPUT_DIRECTORY (with *** being
#   one of the available CMake configuration types).
#
# TINYSPLINE_OUTPUT_DIRECTORY
#   Output directory of build artifacts.
#
# TINYSPLINE_***_SOURCE_DIRECTORY
#   Location of the source code of binding ***.
#
# TINYSPLINE_***_INTERFACE_FILE
#   Full path of the interface file of binding *** (if any).
#
# TINYSPLINE_DLANG_INTERMEDIATE_FILE
#   Full path of the D intermediate file.
###############################################################################
set(TINYSPLINE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "")

# First for the generic no-config case (e.g. with MinGW)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_BACKUP ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_BACKUP ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TINYSPLINE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TINYSPLINE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TINYSPLINE_OUTPUT_DIRECTORY})
# Second, for multi-config builds (e.g. MSVC)
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
set(TINYSPLINE_NODEJS_SOURCE_DIRECTORY
	"${CMAKE_CURRENT_BINARY_DIR}/nodejs"
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
	${TINYSPLINE_NODEJS_SOURCE_DIRECTORY}
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
### Platform-related constants.
#
# TINYSPLINE_PLATFORM_NAME
#   Name of target platform in lowercase. Supported values are: 'linux',
#   'macosx', and 'windows'.
#
# TINYSPLINE_PLATFORM_ARCH
#   Architecture of target platform. Supported values are: 'x86' and 'x86_64'.
#
# TINYSPLINE_PLATFORM
#   Platform (target platform) specific identifier, e. g. 'linux-x86_64'.
###############################################################################
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(TINYSPLINE_PLATFORM_NAME "linux" CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set(TINYSPLINE_PLATFORM_NAME "macosx" CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(TINYSPLINE_PLATFORM_NAME "windows" CACHE INTERNAL "")
else()
	message(FATAL_ERROR "Unsupported target platform: ${CMAKE_SYSTEM_NAME}")
endif()
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(TINYSPLINE_PLATFORM_ARCH "x86_64" CACHE INTERNAL "")
else()
	set(TINYSPLINE_PLATFORM_ARCH "x86" CACHE INTERNAL "")
endif()
set(TINYSPLINE_PLATFORM
	"${TINYSPLINE_PLATFORM_NAME}-${TINYSPLINE_PLATFORM_ARCH}"
	CACHE INTERNAL "")



###############################################################################
### CMake-related constants.
#
# TINYSPLINE_***_CMAKE_TARGET
#   CMake build target of binding ***.
###############################################################################
set(TINYSPLINE_CSHARP_CMAKE_TARGET "tinysplinecsharp" CACHE INTERNAL "")
set(TINYSPLINE_DLANG_CMAKE_TARGET "tinysplinedlang" CACHE INTERNAL "")
set(TINYSPLINE_GO_CMAKE_TARGET "tinysplinego" CACHE INTERNAL "")
set(TINYSPLINE_JAVA_CMAKE_TARGET "tinysplinejava" CACHE INTERNAL "")
set(TINYSPLINE_LUA_CMAKE_TARGET "tinysplinelua" CACHE INTERNAL "")
set(TINYSPLINE_NODEJS_CMAKE_TARGET "tinysplinenodejs" CACHE INTERNAL "")
set(TINYSPLINE_OCTAVE_CMAKE_TARGET "tinysplineoctave" CACHE INTERNAL "")
set(TINYSPLINE_PHP_CMAKE_TARGET "tinysplinephp" CACHE INTERNAL "")
set(TINYSPLINE_PYTHON_CMAKE_TARGET "tinysplinepython" CACHE INTERNAL "")
set(TINYSPLINE_R_CMAKE_TARGET "tinyspliner" CACHE INTERNAL "")
set(TINYSPLINE_RUBY_CMAKE_TARGET "tinysplineruby" CACHE INTERNAL "")



###############################################################################
### Add different options to this script.
#
# TINYSPLINE_INSTALL_INCLUDE_DIR
#   Installation directory of header files.
#
# TINYSPLINE_INSTALL_LIBRARY_DIR
#   Installation directory of library files.
#
# TINYSPLINE_INSTALL_PKGCONFIG_DIR
#   Installation directory of pkg-config files.
#
# TINYSPLINE_FLOAT_PRECISION - default: OFF
#   Build with float instead of double precision.
#
# TINYSPLINE_CSHARP_FRAMEWORK_VERSION
#   The .NET framework version that is used to compile the C# DLL.
#
# TINYSPLINE_CSHARP_FRAMEWORK_NAME
#   Name of the directory located in the 'lib' directory of the .nupkg archive.
#
# TINYSPLINE_PYTHON_VERSION - default: ANY
#   Force Python version.
#
# TINYSPLINE_ENABLE_*** - default: TRUE for CXX, FALSE otherwise
#   These options enable certain interfaces.
#
# TINYSPLINE_ENABLE_ALL_INTERFACES
#   If set, all interfaces are enabled automatically.
#
# TINYSPLINE_RUNTIME_LIBRARIES
#   List of compiler specific runtime libraries that mus be included into
#   distribution artifacts.
###############################################################################
# TINYSPLINE_INSTALL_INCLUDE_DIR
set(TINYSPLINE_INSTALL_INCLUDE_DIR "include" CACHE STRING "Installation directory of header files.")

# TINYSPLINE_INSTALL_LIBRARY_DIR
set(TINYSPLINE_INSTALL_LIBRARY_DIR "" CACHE STRING "Installation directory of library files. If empty, the directory is determined automatically.")
if(TINYSPLINE_INSTALL_LIBRARY_DIR STREQUAL "")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(TINYSPLINE_INSTALL_LIBRARY_DIR
            "lib64" CACHE STRING "Autodetected." FORCE)
	else()
        set(TINYSPLINE_INSTALL_LIBRARY_DIR "lib"
            CACHE STRING "Autodetected." FORCE)
	endif()
endif()

# TINYSPLINE_INSTALL_PKGCONFIG_DIR
set(TINYSPLINE_INSTALL_PKGCONFIG_DIR "share/pkgconfig" CACHE STRING "Installation directory of pkg-config (.pc) files.")

# TINYSPLINE_FLOAT_PRECISION
option(TINYSPLINE_FLOAT_PRECISION "Build TinySpline with float precision." OFF)

# TINYSPLINE_CSHARP_FRAMEWORK_VERSION
set(TINYSPLINE_CSHARP_FRAMEWORK_VERSION "" CACHE STRING "Sets the .NET framework version that is used to compile the C# DLL (e.g. 3.5). If empty, the version is determined automatically.")

# TINYSPLINE_CSHARP_FRAMEWORK_NAME
set(TINYSPLINE_CSHARP_FRAMEWORK_NAME "" CACHE STRING "Name of the directory located in the 'lib' directory of the .nupkg archive (e.g. net35). If empty, the name is determined automatically.")

# TINYSPLINE_PYTHON_VERSION
set(TINYSPLINE_PYTHON_VERSION "ANY" CACHE STRING "Force Python version. Supported values are: '2', '3', and 'ANY' (fallback for unknown values).")

# TINYSPLINE_ENABLE_***
option(TINYSPLINE_ENABLE_CXX "Enable the C++ interface." TRUE)
option(TINYSPLINE_ENABLE_CSHARP "Enable the C# interface." FALSE)
option(TINYSPLINE_ENABLE_DLANG "Enable the D interface." FALSE)
option(TINYSPLINE_ENABLE_GO "Enable the Go interface." FALSE)
option(TINYSPLINE_ENABLE_JAVA "Enable the Java interface." FALSE)
option(TINYSPLINE_ENABLE_LUA "Enable the Lua interface." FALSE)
option(TINYSPLINE_ENABLE_NODEJS "Enable the Javascript (Node.js) interface." FALSE)
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
	set(TINYSPLINE_ENABLE_NODEJS TRUE)
	set(TINYSPLINE_ENABLE_OCTAVE TRUE)
	set(TINYSPLINE_ENABLE_PHP TRUE)
	set(TINYSPLINE_ENABLE_PYTHON TRUE)
	set(TINYSPLINE_ENABLE_R TRUE)
	set(TINYSPLINE_ENABLE_RUBY TRUE)
endif()

# TINYSPLINE_RUNTIME_LIBRARIES
set(TINYSPLINE_RUNTIME_LIBRARIES "" CACHE FILEPATH "Compiler specific runtime libraries that must be included into distribution artifacts. If empty, the necessary files are determined automatically.")



###############################################################################
### Map supported environment variables to CMake variables. The following
### variables will be processed and may overwrite previously defined options
### (only if the option's default value has been selected).
#
# TINYSPLINE_FLOAT_PRECISION
#   See corresponding option above.
#
# TINYSPLINE_PYTHON_VERSION
#   See corresponding option above.
#
# CMAKE_TOOLCHAIN_FILE
#   The CMake tool chain file for cross-compiling.
#
# SWIG_EXECUTABLE
#   The SWIG executable.
###############################################################################
# TINYSPLINE_FLOAT_PRECISION
if(NOT TINYSPLINE_FLOAT_PRECISION AND DEFINED ENV{TINYSPLINE_FLOAT_PRECISION})
	message(STATUS "Using environment variable 'TINYSPLINE_FLOAT_PRECISION'")
	set(TINYSPLINE_FLOAT_PRECISION $ENV{TINYSPLINE_FLOAT_PRECISION})
endif()

# TINYSPLINE_PYTHON_VERSION
if(${TINYSPLINE_PYTHON_VERSION} STREQUAL "ANY" AND DEFINED ENV{TINYSPLINE_PYTHON_VERSION})
	message(STATUS "Using environment variable 'TINYSPLINE_PYTHON_VERSION'")
	set(TINYSPLINE_PYTHON_VERSION $ENV{TINYSPLINE_PYTHON_VERSION})
endif()

# CMAKE_TOOLCHAIN_FILE
if(DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
	message(STATUS "Using environment variable 'CMAKE_TOOLCHAIN_FILE'")
	set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# SWIG_EXECUTABLE
if(DEFINED ENV{SWIG_EXECUTABLE})
	message(STATUS "Using environment variable 'SWIG_EXECUTABLE'")
	set(SWIG_EXECUTABLE $ENV{SWIG_EXECUTABLE})
endif()



###############################################################################
### Setup compiler suite.
#
# TINYSPLINE_DEBUG_POSTFIX
#   Postfix of debug builds.
#
# TINYSPLINE_DEFINITIONS (set in parent scope)
#   Preprocessor definitions that must be added to parent file with
#   'add_definitions("${TINYSPLINE_DEFINITIONS}")'.
#
# TINYSPLINE_CXX11_COMPLIANT
#   Does the C++ compiler support C++11? (Required by some bindings)
#
# TINYSPLINE_C_STATIC_LIBRARIES (set in parent scope)
#   Additional libraries to link against when linking TinySpline's C interface
#   statically (a string containing the necessary linker flags).
#
# TINYSPLINE_CXX_STATIC_LIBRARIES (set in parent scope)
#   Additional libraries to link against when linking TinySpline's C++
#   interface statically (a string containing the necessary linker flags).
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
# TINYSPLINE_PKGCONFIG_C_FLAGS
#   Cflags added to the pkg-config file of the C library.
#
# TINYSPLINE_PKGCONFIG_CXX_FLAGS
#   Cflags added to the pkg-config file of the C++ library.
#
# TINYSPLINE_RUNTIME_LIBS (set in parent scope)
#   Stores the names of the files that are copied (and stripped) from
#   TINYSPLINE_RUNTIME_LIBRARIES to TINYSPLINE_OUTPUT_DIRECTORY.
###############################################################################
# TINYSPLINE_DEBUG_POSTFIX
# TINYSPLINE_DEFINITIONS
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	# It is common practice for MSVC builds to add a debug postfix.
	set(TINYSPLINE_DEBUG_POSTFIX "d")
	if(TINYSPLINE_FLOAT_PRECISION)
		set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS} /DTINYSPLINE_FLOAT_PRECISION")
	endif()
else()
	set(TINYSPLINE_DEBUG_POSTFIX "")
	if(TINYSPLINE_FLOAT_PRECISION)
		set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS} -DTINYSPLINE_FLOAT_PRECISION")
	endif()
endif()
# Remove leading and trailing spaces from variables.
string(STRIP "${TINYSPLINE_DEBUG_POSTFIX}" TINYSPLINE_DEBUG_POSTFIX)
string(STRIP "${TINYSPLINE_DEFINITIONS}" TINYSPLINE_DEFINITIONS)
# Export relevant variables.
if(TINYSPLINE_HAS_PARENT)
	set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS}" PARENT_SCOPE)
else()
	set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS}")
endif()
add_definitions("${TINYSPLINE_DEFINITIONS}")

# TINYSPLINE_PKGCONFIG_C_FLAGS (common)
# TINYSPLINE_PKGCONFIG_CXX_FLAGS (common)
set(TINYSPLINE_PKGCONFIG_C_FLAGS "${TINYSPLINE_DEFINITIONS}")
set(TINYSPLINE_PKGCONFIG_CXX_FLAGS "${TINYSPLINE_DEFINITIONS}")

# TINYSPLINE_CXX11_COMPLIANT
# TINYSPLINE_C_STATIC_LIBRARIES
# TINYSPLINE_CXX_STATIC_LIBRARIES
# TINYSPLINE_LIBRARY_C_FLAGS
# TINYSPLINE_LIBRARY_CXX_FLAGS
# TINYSPLINE_BINDING_CXX_FLAGS
# TINYSPLINE_PKGCONFIG_C_FLAGS (specific)
# TINYSPLINE_PKGCONFIG_CXX_FLAGS (specific)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	# TINYSPLINE_CXX11_COMPLIANT
	# http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7 OR
			CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.7)
		set(TINYSPLINE_CXX11_COMPLIANT TRUE)
	else()
		set(TINYSPLINE_CXX11_COMPLIANT FALSE)
	endif()

	# CMAKE_C_FLAGS
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

	# TINYSPLINE_C_STATIC_LIBRARIES
	# TINYSPLINE_CXX_STATIC_LIBRARIES
	set(TINYSPLINE_C_STATIC_LIBRARIES "${TINYSPLINE_C_STATIC_LIBRARIES} -lm")
	set(TINYSPLINE_CXX_STATIC_LIBRARIES "${TINYSPLINE_CXX_STATIC_LIBRARIES} -lm")
	# Link the necessary libraries directly into shared libs.
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lm")

	# TINYSPLINE_LIBRARY_C_FLAGS
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -Werror")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -Wall")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -Wextra")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -Wfloat-equal")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -pedantic")

	# TINYSPLINE_LIBRARY_CXX_FLAGS
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} -Werror")
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} -Wall")
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} -Wextra")
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} -Wfloat-equal")

	# TINYSPLINE_BINDING_CXX_FLAGS
	set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -w")
	set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -DSWIG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	# TINYSPLINE_CXX11_COMPLIANT
	# http://stackoverflow.com/questions/10408849/how-can-i-use-c-11-features-in-clang
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.3 OR
			CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 3.3)
		set(TINYSPLINE_CXX11_COMPLIANT TRUE)
	else()
		set(TINYSPLINE_CXX11_COMPLIANT FALSE)
	endif()

	# CMAKE_C_FLAGS
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

	# TINYSPLINE_C_STATIC_LIBRARIES
	# TINYSPLINE_CXX_STATIC_LIBRARIES
	set(TINYSPLINE_C_STATIC_LIBRARIES "${TINYSPLINE_C_STATIC_LIBRARIES} -lm")
	set(TINYSPLINE_CXX_STATIC_LIBRARIES "${TINYSPLINE_CXX_STATIC_LIBRARIES} -lm")
	# Link the necessary libraries directly into shared libs.
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lm")

	# TINYSPLINE_LIBRARY_C_FLAGS
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -Werror")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -Wall")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -Wextra")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -Wfloat-equal")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} -pedantic")

	# TINYSPLINE_LIBRARY_CXX_FLAGS
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} -Werror")
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} -Wall")
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} -Wextra")
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} -Wfloat-equal")

	# TINYSPLINE_BINDING_CXX_FLAGS
	set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -w")
	set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -DSWIG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	# TINYSPLINE_CXX11_COMPLIANT
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 18 OR
			CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 18)
		set(TINYSPLINE_CXX11_COMPLIANT TRUE)
	else()
		set(TINYSPLINE_CXX11_COMPLIANT FALSE)
	endif()

	# TINYSPLINE_LIBRARY_C_FLAGS
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} /W4")
	set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} /WX")

	# TINYSPLINE_LIBRARY_CXX_FLAGS
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} /W4")
	set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} /WX")

	# TINYSPLINE_BINDING_CXX_FLAGS
	set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /w")
	set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /DSWIG")

	# Used to automatically create a .def file with all the global symbols for
	# a shared library.
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()
# Remove leading and trailing spaces from variables.
string(STRIP "${CMAKE_C_FLAGS}" CMAKE_C_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS)
string(STRIP "${TINYSPLINE_C_STATIC_LIBRARIES}" TINYSPLINE_C_STATIC_LIBRARIES)
string(STRIP "${TINYSPLINE_CXX_STATIC_LIBRARIES}" TINYSPLINE_CXX_STATIC_LIBRARIES)
string(STRIP "${TINYSPLINE_LIBRARY_C_FLAGS}" TINYSPLINE_LIBRARY_C_FLAGS)
string(STRIP "${TINYSPLINE_LIBRARY_CXX_FLAGS}" TINYSPLINE_LIBRARY_CXX_FLAGS)
string(STRIP "${TINYSPLINE_BINDING_CXX_FLAGS}" TINYSPLINE_BINDING_CXX_FLAGS)
string(STRIP "${TINYSPLINE_PKGCONFIG_C_FLAGS}" TINYSPLINE_PKGCONFIG_C_FLAGS)
string(STRIP "${TINYSPLINE_PKGCONFIG_CXX_FLAGS}" TINYSPLINE_PKGCONFIG_CXX_FLAGS)
# Export relevant variables.
if(TINYSPLINE_HAS_PARENT)
	set(TINYSPLINE_C_STATIC_LIBRARIES
			"${TINYSPLINE_C_STATIC_LIBRARIES}"
			PARENT_SCOPE)
	set(TINYSPLINE_CXX_STATIC_LIBRARIES
			"${TINYSPLINE_CXX_STATIC_LIBRARIES}"
			PARENT_SCOPE)
else()
	set(TINYSPLINE_C_STATIC_LIBRARIES "${TINYSPLINE_C_STATIC_LIBRARIES}")
	set(TINYSPLINE_CXX_STATIC_LIBRARIES "${TINYSPLINE_CXX_STATIC_LIBRARIES}")
endif()

# TINYSPLINE_RUNTIME_LIBS
set(TINYSPLINE_RUNTIME_LIBS "")
if(TINYSPLINE_RUNTIME_LIBRARIES STREQUAL "")
	if(${TINYSPLINE_PLATFORM_NAME} STREQUAL "windows")
		# Store determined files in CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS.
		# This allows to conveniently reuse the
		# InstallRequiredSystemLibraries module.
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
			# Determining the used exception model is not quite as
			# simple as one would suspect. Thus, we simple add all
			# existing DLL files that are responsible for exception
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
			# Add the libwinpthread DLL if it exists, indicating
			# that POSIX (instead of win32) threads are enabled.
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
# Export relevant variables.
if(TINYSPLINE_HAS_PARENT)
	set(TINYSPLINE_RUNTIME_LIBS "${TINYSPLINE_RUNTIME_LIBS}" PARENT_SCOPE)
else()
	set(TINYSPLINE_RUNTIME_LIBS "${TINYSPLINE_RUNTIME_LIBS}")
endif()



###############################################################################
### Create the C/C++ library. We explicitly set the prefix of the static
### library to 'lib' to prevent naming collisions when compiling with MSVC.
#
# TINYSPLINE_C_INCLUDE_DIR (set in parent scope)
#   Include directory (absolute path) of the C library.
#
# TINYSPLINE_CXX_INCLUDE_DIR (set in parent scope)
#   Include directory (absolute path) of the C++ library.
#
# TINYSPLINE_C_SOURCE_FILES (set in parent scope)
#   List of source files (absolute paths) that are required to build the C
#   library. Does not contain header files.
#
# TINYSPLINE_CXX_SOURCE_FILES (set in parent scope)
#   List of source files (absolute paths) that are required to build the C++
#   library. Does not contain header files, but all source files listed in
#   TINYSPLINE_C_SOURCE_FILES.
###############################################################################
# TINYSPLINE_C_INCLUDE_DIR
# TINYSPLINE_CXX_INCLUDE_DIR
set(TINYSPLINE_C_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TINYSPLINE_CXX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
if(TINYSPLINE_HAS_PARENT)
	set(TINYSPLINE_C_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)
	set(TINYSPLINE_CXX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)
endif()

# TINYSPLINE_C_SOURCE_FILES
list(APPEND TINYSPLINE_C_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/tinyspline.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/parson.c")
if(TINYSPLINE_HAS_PARENT)
	set(TINYSPLINE_C_SOURCE_FILES ${TINYSPLINE_C_SOURCE_FILES} PARENT_SCOPE)
else()
	set(TINYSPLINE_C_SOURCE_FILES ${TINYSPLINE_C_SOURCE_FILES})
endif()

# TINYSPLINE_CXX_SOURCE_FILES
list(APPEND TINYSPLINE_CXX_SOURCE_FILES
	${TINYSPLINE_C_SOURCE_FILES}
	"${CMAKE_CURRENT_SOURCE_DIR}/tinysplinecpp.cpp")
if(TINYSPLINE_HAS_PARENT)
	set(TINYSPLINE_CXX_SOURCE_FILES ${TINYSPLINE_CXX_SOURCE_FILES} PARENT_SCOPE)
else()
	set(TINYSPLINE_CXX_SOURCE_FILES ${TINYSPLINE_CXX_SOURCE_FILES})
endif()

# C library
add_library(tinyspline_shared SHARED ${TINYSPLINE_C_SOURCE_FILES})
set_target_properties(tinyspline_shared PROPERTIES
	OUTPUT_NAME "tinyspline"
	DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
	COMPILE_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS}")
install(TARGETS tinyspline_shared
	LIBRARY DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
	ARCHIVE DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
	RUNTIME DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR})
target_include_directories(tinyspline_shared
	PUBLIC ${TINYSPLINE_C_INCLUDE_DIR})
add_library(tinyspline_static STATIC ${TINYSPLINE_C_SOURCE_FILES})
set_target_properties(tinyspline_static PROPERTIES
	OUTPUT_NAME "tinyspline"
	DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
	PREFIX "lib"
	COMPILE_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS}")
install(TARGETS tinyspline_static
	LIBRARY DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
	ARCHIVE DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR})
target_include_directories(tinyspline_static
	PUBLIC ${TINYSPLINE_C_INCLUDE_DIR})
install(FILES tinyspline.h
	DESTINATION ${TINYSPLINE_INSTALL_INCLUDE_DIR})

# C++ library
if(TINYSPLINE_ENABLE_CXX)
	add_library(tinysplinecpp_shared SHARED ${TINYSPLINE_CXX_SOURCE_FILES})
	set_target_properties(tinysplinecpp_shared PROPERTIES
		OUTPUT_NAME "tinysplinecpp"
		DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
		COMPILE_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS}")
	install(TARGETS tinysplinecpp_shared
		LIBRARY DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
		ARCHIVE DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
		RUNTIME DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR})
	target_include_directories(tinysplinecpp_shared
			PUBLIC ${TINYSPLINE_CXX_INCLUDE_DIR})
	add_library(tinysplinecpp_static STATIC ${TINYSPLINE_CXX_SOURCE_FILES})
	set_target_properties(tinysplinecpp_static PROPERTIES
		OUTPUT_NAME "tinysplinecpp"
		DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
		PREFIX "lib"
		COMPILE_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS}")
	install(TARGETS tinysplinecpp_static
		LIBRARY DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR}
		ARCHIVE DESTINATION ${TINYSPLINE_INSTALL_LIBRARY_DIR})
	target_include_directories(tinysplinecpp_static
		PUBLIC ${TINYSPLINE_CXX_INCLUDE_DIR})
	install(FILES tinysplinecpp.h
			DESTINATION ${TINYSPLINE_INSTALL_INCLUDE_DIR})
	CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pkg/tinysplinecpp.pc.in"
			"tinysplinecpp.pc" @ONLY)
	install(FILES ${CMAKE_BINARY_DIR}/src/tinysplinecpp.pc
			DESTINATION ${TINYSPLINE_INSTALL_PKGCONFIG_DIR})
endif()



###############################################################################
### Setup SWIG and create bindings.
#
# TINYSPLINE_BINDINGS_FOLDER_NAME
#   Name of the IDE project folder containing the bindings.
#   ( set_property(GLOBAL PROPERTY USE_FOLDERS ON) )
#
# TINYSPLINE_SWIG_DEFINES
#   List of defines passed to Swig.
#
# TINYSPLINE_BINDING_REQUESTED
#   At least one binding should be created.
###############################################################################
# TINYSPLINE_BINDINGS_FOLDER_NAME
set(TINYSPLINE_BINDINGS_FOLDER_NAME "bindings")

# TINYSPLINE_SWIG_DEFINES
if(TINYSPLINE_FLOAT_PRECISION)
	list(APPEND TINYSPLINE_SWIG_DEFINES -DTINYSPLINE_FLOAT_PRECISION)
endif()

# TINYSPLINE_BINDING_REQUESTED
if(${TINYSPLINE_ENABLE_CSHARP}
		OR ${TINYSPLINE_ENABLE_DLANG}
		OR ${TINYSPLINE_ENABLE_GO}
		OR ${TINYSPLINE_ENABLE_JAVA}
		OR ${TINYSPLINE_ENABLE_LUA}
		OR ${TINYSPLINE_ENABLE_NODEJS}
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
		"TARGET;LANG;OUTPUT;LIBS;NAME"
		"SWIG_ARGS;FLAGS"
		${ARGN})
	set_source_files_properties("swig/${ARGS_TARGET}.i"
		PROPERTIES CPLUSPLUS ON)
	set(CMAKE_SWIG_FLAGS ${ARGS_SWIG_ARGS} -O ${TINYSPLINE_SWIG_DEFINES})
	set(CMAKE_SWIG_OUTDIR ${ARGS_OUTPUT})
	if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
		swig_add_module(
			${ARGS_TARGET}
			${ARGS_LANG}
			"swig/${ARGS_TARGET}.i"
			${TINYSPLINE_CXX_SOURCE_FILES})
	else()
		swig_add_library(
			${ARGS_TARGET}
			TYPE MODULE
			LANGUAGE ${ARGS_LANG}
			SOURCES "swig/${ARGS_TARGET}.i"
			${TINYSPLINE_CXX_SOURCE_FILES})
	endif()
	# On Linux, we can (and for the sake of portability should) omit
	# linking libraries.
	if(NOT ${TINYSPLINE_PLATFORM_NAME} STREQUAL "linux")
		swig_link_libraries(${ARGS_TARGET} ${ARGS_LIBS})
	endif()
	# For some reasons Python's make target starts with '_'.
	set(ACTUAL_TARGET ${ARGS_TARGET})
	if(${ARGS_LANG} STREQUAL python)
		set(ACTUAL_TARGET _${ACTUAL_TARGET})
	endif()
	set(TINYSPLINE_SWIG_CMAKE_TARGET ${ACTUAL_TARGET} PARENT_SCOPE)
	set_target_properties(${ACTUAL_TARGET} PROPERTIES
		FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
		COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} ${ARGS_FLAGS}")
	if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
		set_property(TARGET ${ACTUAL_TARGET}
			APPEND PROPERTY TYPE MODULE_LIBRARY)
	endif()
	if(${ARGS_CXX11})
		set_property(TARGET ${ACTUAL_TARGET}
			APPEND PROPERTY CXX_STANDARD 11)
		set_property(TARGET ${ACTUAL_TARGET}
			APPEND PROPERTY CXX_STANDARD_REQUIRED ON)
		set_property(TARGET ${ACTUAL_TARGET}
			APPEND PROPERTY CXX_EXTENSIONS OFF)
	endif()
	if (NOT ${ARGS_NAME} STREQUAL "")
		set_property(TARGET ${ACTUAL_TARGET}
			APPEND PROPERTY OUTPUT_NAME "${ARGS_NAME}")
	endif()
	if (CMAKE_STRIP)
		add_custom_command(
			TARGET ${ACTUAL_TARGET}
			POST_BUILD
			COMMAND
				"${CMAKE_STRIP}" --strip-unneeded
				"$<TARGET_FILE_NAME:${ACTUAL_TARGET}>"
			WORKING_DIRECTORY "${TINYSPLINE_OUTPUT_DIRECTORY}")
	endif()
endfunction()

if(${TINYSPLINE_BINDING_REQUESTED})
	find_package(SWIG 3.0.1 REQUIRED)
	include(${SWIG_USE_FILE})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})

	# C#
	if(${TINYSPLINE_ENABLE_CSHARP})
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_CSHARP_CMAKE_TARGET}
			LANG csharp
			OUTPUT ${TINYSPLINE_CSHARP_SOURCE_DIRECTORY}
			SWIG_ARGS -namespace TinySpline)
		# DLL
		if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
			if(TINYSPLINE_CSHARP_FRAMEWORK_VERSION STREQUAL "")
				set(TINYSPLINE_CSHARP_FRAMEWORK_VERSION
					"3.5" CACHE STRING "Autodetected." FORCE)
			endif()
			if(TINYSPLINE_CSHARP_FRAMEWORK_NAME STREQUAL "")
				set(TINYSPLINE_CSHARP_FRAMEWORK_NAME
					"net35" CACHE STRING "Autodetected." FORCE)
			endif()
			set(TINYSPLINE_CSHARP_HOME
				"C:/Windows/Microsoft.NET/Framework/v")
			set(TINYSPLINE_CSHARP_HOME
				"${TINYSPLINE_CSHARP_HOME}${TINYSPLINE_CSHARP_FRAMEWORK_VERSION}")
			if(NOT EXISTS ${TINYSPLINE_CSHARP_HOME})
				message(FATAL_ERROR "'${TINYSPLINE_CSHARP_HOME}' does not exist")
			endif()
			find_program(TINYSPLINE_CSHARP_COMPILER
				NAMES csc
				PATHS "${TINYSPLINE_CSHARP_HOME}")
		else()
			if(TINYSPLINE_CSHARP_FRAMEWORK_VERSION STREQUAL "")
				set(TINYSPLINE_CSHARP_FRAMEWORK_VERSION
					"2" CACHE STRING "Autodetected." FORCE)
			endif()
			if(TINYSPLINE_CSHARP_FRAMEWORK_NAME STREQUAL "")
				set(TINYSPLINE_CSHARP_FRAMEWORK_NAME
					"net20" CACHE STRING "Autodetected." FORCE)
			endif()
			find_program(TINYSPLINE_CSHARP_COMPILER  NAMES mcs)
		endif()
		if(TINYSPLINE_CSHARP_COMPILER)
			if(${TINYSPLINE_CSHARP_COMPILER} MATCHES "csc.exe$")
				# csc does not support '/' in 'recursive'.
				add_custom_command(
					TARGET ${TINYSPLINE_SWIG_CMAKE_TARGET}
					POST_BUILD
					COMMAND
						${TINYSPLINE_CSHARP_COMPILER}
						/target:library
						/out:"${TINYSPLINE_CSHARP_INTERFACE_FILE}"
						/recurse:"${TINYSPLINE_CSHARP_SOURCE_DIRECTORY}\\*.cs")
			else()
				add_custom_command(
					TARGET ${TINYSPLINE_SWIG_CMAKE_TARGET}
					POST_BUILD
					COMMAND
						${TINYSPLINE_CSHARP_COMPILER}
						-sdk:${TINYSPLINE_CSHARP_FRAMEWORK_VERSION}
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
			TARGET ${TINYSPLINE_SWIG_CMAKE_TARGET}
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
			LANG go
			OUTPUT ${TINYSPLINE_GO_SOURCE_DIRECTORY}
			SWIG_ARGS -cgo -intgosize ${TINYSPLINE_GO_INTGOSIZE} -package tinyspline)
		add_custom_command(
			TARGET ${TINYSPLINE_SWIG_CMAKE_TARGET}
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
		set(TINYSPLINE_JAVA_NATIVE_LIB "${TINYSPLINE_PLATFORM}")
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
				TARGET ${TINYSPLINE_SWIG_CMAKE_TARGET}
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
					"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes/${TINYSPLINE_PLATFORM}/${TINYSPLINE_PLATFORM}"
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

	# Node.js
	if(${TINYSPLINE_ENABLE_NODEJS})
		find_path(TINYSPLINE_NODEJS_NODE_H_PATH
				node.h
				HINTS "/usr/include/node")
		find_path(TINYSPLINE_NODEJS_V8_H_PATH
				v8.h
				HINTS "/usr/include/node")
		if(NOT TINYSPLINE_NODEJS_NODE_H_PATH)
			message(FATAL_ERROR "Missing node.h")
		endif()
		if(NOT TINYSPLINE_NODEJS_V8_H_PATH)
			message(FATAL_ERROR "Missing v8.h")
		endif()
		include_directories(${TINYSPLINE_NODEJS_NODE_H_PATH}
				${TINYSPLINE_NODEJS_V8_H_PATH})
		tinyspline_add_swig_library(
			TARGET ${TINYSPLINE_NODEJS_CMAKE_TARGET}
			LANG javascript
			OUTPUT ${TINYSPLINE_NODEJS_SOURCE_DIRECTORY}
			LIBS ${LUA_LIBRARIES}
			SWIG_ARGS -node
			CXX11)
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
				TARGET ${TINYSPLINE_SWIG_CMAKE_TARGET}
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
			TARGET ${TINYSPLINE_SWIG_CMAKE_TARGET}
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
			TARGET ${TINYSPLINE_SWIG_CMAKE_TARGET}
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

# Dub
file(REMOVE "${CMAKE_BINARY_DIR}/dub.json")
if(${TINYSPLINE_WITH_DLANG})
	set(TINYSPLINE_DUB_COPY_FILES "\"${TINYSPLINE_LIB_DIR}/*.d\"")
	set(TINYSPLINE_DUB_COPY_FILES "${TINYSPLINE_DUB_COPY_FILES}\
, \"${TINYSPLINE_LIB_DIR}/*${TINYSPLINE_DLANG_CMAKE_TARGET}*\"")
	foreach(lib ${TINYSPLINE_RUNTIME_LIBS})
		set(TINYSPLINE_DUB_COPY_FILES "${TINYSPLINE_DUB_COPY_FILES}\
, \"${TINYSPLINE_LIB_DIR}/${lib}\"")
	endforeach()
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/pkg/dub.json.in"
		"${CMAKE_BINARY_DIR}/dub.json"
		@ONLY)
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
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/pkg/tinyspline.rockspec.in"
		"${CMAKE_BINARY_DIR}/${TINYSPLINE_LUAROCKS_SPEC}"
		@ONLY)
endif()

# Nuget
file(REMOVE "${CMAKE_BINARY_DIR}/.nuspec")
if(${TINYSPLINE_WITH_CSHARP})
	get_filename_component(TINYSPLINE_NUGET_INTERFACE_FILE
		${TINYSPLINE_CSHARP_INTERFACE_FILE}
		NAME)
	if(${TINYSPLINE_PLATFORM_NAME} STREQUAL "linux")
		set(TINYSPLINE_NUGET_RID "linux")
	elseif(${TINYSPLINE_PLATFORM_NAME} STREQUAL "macosx")
		set(TINYSPLINE_NUGET_RID "osx")
	elseif(${TINYSPLINE_PLATFORM_NAME} STREQUAL "windows")
		set(TINYSPLINE_NUGET_RID "win")
	else()
		message(FATAL_ERROR "Unable to determine Nuget RID (platform)")
	endif()
	if(${TINYSPLINE_PLATFORM_ARCH} STREQUAL "x86_64")
		set(TINYSPLINE_NUGET_RID "${TINYSPLINE_NUGET_RID}-x64")
	elseif(${TINYSPLINE_PLATFORM_ARCH} STREQUAL "x86")
		set(TINYSPLINE_NUGET_RID "${TINYSPLINE_NUGET_RID}-x86")
	else()
		message(FATAL_ERROR "Unable to determine Nuget RID (arch)")
	endif()
	set(TINYSPLINE_NUGET_FILES "<file \
src=\"${TINYSPLINE_LIB_DIR}/${TINYSPLINE_NUGET_INTERFACE_FILE}\" \
target=\"lib/${TINYSPLINE_CSHARP_FRAMEWORK_NAME}\" />")
	set(TINYSPLINE_NUGET_FILES "${TINYSPLINE_NUGET_FILES}\n\t\t<file \
src=\"${TINYSPLINE_LIB_DIR}/*${TINYSPLINE_CSHARP_CMAKE_TARGET}.*\" \
target=\"runtimes/${TINYSPLINE_NUGET_RID}/lib/native\" />")
	foreach(lib ${TINYSPLINE_RUNTIME_LIBS})
		set(TINYSPLINE_NUGET_FILES
			"${TINYSPLINE_NUGET_FILES}\n\t\t<file \
src=\"${TINYSPLINE_LIB_DIR}/${lib}\" \
target=\"runtimes/${TINYSPLINE_NUGET_RID}/lib/native\" />")
	endforeach()
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/pkg/.nuspec.in"
		"${CMAKE_BINARY_DIR}/.nuspec"
		@ONLY)
endif()

# Maven
file(REMOVE "${CMAKE_BINARY_DIR}/pom.xml")
if(${TINYSPLINE_WITH_JAVA})
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/pkg/pom.xml.in"
		"${CMAKE_BINARY_DIR}/pom.xml"
		@ONLY)
endif()

# Distutils
file(REMOVE "${CMAKE_BINARY_DIR}/setup.py")
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
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/pkg/setup.py.in"
		"${CMAKE_BINARY_DIR}/setup.py"
		@ONLY)
endif()

# Gem
file(REMOVE "${CMAKE_BINARY_DIR}/tinyspline.gemspec")
if(${TINYSPLINE_WITH_RUBY})
	set(TINYSPLINE_RUBY_REQ_VERSION
			"${RUBY_VERSION_MAJOR}.${RUBY_VERSION_MINOR}")
	set(TINYSPLINE_GEM_FILES
"Dir.glob(\"{${TINYSPLINE_LIB_DIR}}/*${TINYSPLINE_RUBY_CMAKE_TARGET}*\")")
	foreach(lib ${TINYSPLINE_RUNTIME_LIBS})
		set(TINYSPLINE_GEM_FILES "${TINYSPLINE_GEM_FILES} \
<< \"${TINYSPLINE_LIB_DIR}/${lib}\"")
	endforeach()
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/pkg/tinyspline.gemspec.in"
		"${CMAKE_BINARY_DIR}/tinyspline.gemspec"
		@ONLY)
endif()



###############################################################################
### Restore output directories.
###############################################################################
# First for the generic no-config case (e.g. with MinGW)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_BACKUP})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_BACKUP})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_BACKUP})
# Second, for multi-config builds (e.g. MSVC)
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
string(STRIP "${CMAKE_C_FLAGS} ${TINYSPLINE_LIBRARY_C_FLAGS}" TINYSPLINE_SUMMARY_C_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS} ${TINYSPLINE_LIBRARY_CXX_FLAGS}" TINYSPLINE_SUMMARY_CXX_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS} ${TINYSPLINE_BINDING_CXX_FLAGS}" TINYSPLINE_SUMMARY_BINDING_FLAGS)

message(STATUS
"
/// TinySpline Configuration Summary //////////////////////////////////////////
Platform:
  Id: ${TINYSPLINE_PLATFORM}

Interface Configuration:
  With single precision  (default: OFF): ${TINYSPLINE_FLOAT_PRECISION}

Compiler Configuration:
  Compiler:       ${CMAKE_CXX_COMPILER}
  C flags:        ${TINYSPLINE_SUMMARY_C_FLAGS}
  C libraries:    ${TINYSPLINE_C_STATIC_LIBRARIES}
  C++ flags:      ${TINYSPLINE_SUMMARY_CXX_FLAGS}
  C++ libraries:  ${TINYSPLINE_CXX_STATIC_LIBRARIES}
  Binding flags:  ${TINYSPLINE_SUMMARY_BINDING_FLAGS}
  Runtime libs:   ${TINYSPLINE_RUNTIME_LIBS}
  Definitions:    ${TINYSPLINE_DEFINITIONS}
  Debug postfix:  ${TINYSPLINE_DEBUG_POSTFIX}
  Toolchain file: ${CMAKE_TOOLCHAIN_FILE}

Available Interfaces:
  C++:          ${TINYSPLINE_WITH_CXX}

  C\#:           ${TINYSPLINE_WITH_CSHARP}
    Compiler: ${TINYSPLINE_CSHARP_COMPILER}

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
