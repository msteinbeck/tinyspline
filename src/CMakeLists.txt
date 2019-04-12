###############################################################################
### General CMake setup.
###############################################################################
cmake_minimum_required(VERSION 3.4)
project(tinyspline)

# Turn on folders.
# https://cmake.org/cmake/help/v3.0/prop_gbl/USE_FOLDERS.html
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add additional CMake modules shipped with TinySpline.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")



###############################################################################
### The following section defines project-related constants that are used to
### configure external tools, such as Setuptools and Doxygen. Accordingly,
### they are set wth 'CACHE INTERNAL'.
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

set(TINYSPLINE_LICENSE "MIT" CACHE INTERNAL "")

set(TINYSPLINE_VERSION "0.3.0" CACHE INTERNAL "")

set(TINYSPLINE_SUMMARY "NURBS Library for a Multitude of Programming Languages" CACHE INTERNAL "")

set(TINYSPLINE_DESCRIPTION "TinySpline is a small, yet powerful library for interpolating, transforming, and querying arbitrary NURBS, B-Splines, and Bézier curves. The library is implemented in ANSI C (C89) and provides a wrapper for C++ along with auto-generated bindings for C#, D, Java, Lua, Octave, PHP, Python, R, and Ruby." CACHE INTERNAL "")



###############################################################################
### The following section defines location-related constants that are used to
### configure external tools, such as Setuptools and Doxygen. Accordingly,
### they are set with 'CACHE INTERNAL'.
#
# TINYSPLINE_BINARY_OUTPUT_DIRECTORY
#   Location of compiled binaries.
#
# TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY
#   Location of the compiled libraries.
#
# TINYSPLINE_***_SOURCE_DIRECTORY
#   The directory where the auto-generated source code of binding *** is
#   located.
#
# TINYSPLINE_***_INTERFACE_FILE
#   Location of the auto-generated interface file of binding *** (if any).
#
# TINYSPLINE_DLANG_INTERMEDIATE_FILE
#   Location of the auto-generated intermediate file of D.
###############################################################################
set(TINYSPLINE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "")
set(TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "")

# First for the generic no-config case (e.g. with MinGW)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TINYSPLINE_BINARY_OUTPUT_DIRECTORY} CACHE INTERNAL "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY} CACHE INTERNAL "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY} CACHE INTERNAL "")
# Second, for multi-config builds (e.g. MSVC)
foreach(TINYSPLINE_CONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${TINYSPLINE_CONFIG} TINYSPLINE_CONFIG)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_BINARY_OUTPUT_DIRECTORY} CACHE INTERNAL "")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY} CACHE INTERNAL "")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY} CACHE INTERNAL "")
endforeach()

set(TINYSPLINE_CSHARP_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/csharp")
set(TINYSPLINE_DLANG_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/dlang")
set(TINYSPLINE_JAVA_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/java")
set(TINYSPLINE_OCTAVE_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/octave")
set(TINYSPLINE_LUA_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lua")
set(TINYSPLINE_PHP_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/php")
set(TINYSPLINE_PYTHON_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/python")
set(TINYSPLINE_R_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/r")
set(TINYSPLINE_RUBY_SOURCE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/ruby")

set(TINYSPLINE_CSHARP_INTERFACE_FILE "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/TinySpline.dll" CACHE INTERNAL "")
set(TINYSPLINE_DLANG_INTERFACE_FILE "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/tinyspline.d" CACHE INTERNAL "")
set(TINYSPLINE_JAVA_INTERFACE_FILE "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/tinyspline.jar" CACHE INTERNAL "")
set(TINYSPLINE_PHP_INTERFACE_FILE "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/tinyspline.php" CACHE INTERNAL "")
set(TINYSPLINE_PYTHON_INTERFACE_FILE "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/tinyspline.py" CACHE INTERNAL "")
set(TINYSPLINE_R_INTERFACE_FILE "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/tinyspline.R" CACHE INTERNAL "")

set(TINYSPLINE_DLANG_INTERMEDIATE_FILE "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/tinyspline_im.d" CACHE INTERNAL "")

# Add additional files and directories to delete with 'cmake --build . --target clean'.
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
	"${TINYSPLINE_CSHARP_SOURCE_DIRECTORY};${TINYSPLINE_CSHARP_INTERFACE_FILE};${TINYSPLINE_DLANG_SOURCE_DIRECTORY};${TINYSPLINE_DLANG_INTERFACE_FILE};${TINYSPLINE_DLANG_INTERMEDIATE_FILE};${TINYSPLINE_JAVA_SOURCE_DIRECTORY};${TINYSPLINE_JAVA_INTERFACE_FILE};${TINYSPLINE_OCTAVE_SOURCE_DIRECTORY};${TINYSPLINE_LUA_SOURCE_DIRECTORY};${TINYSPLINE_PHP_SOURCE_DIRECTORY};${TINYSPLINE_PHP_INTERFACE_FILE};${TINYSPLINE_PYTHON_SOURCE_DIRECTORY};${TINYSPLINE_PYTHON_INTERFACE_FILE};${TINYSPLINE_R_SOURCE_DIRECTORY};${TINYSPLINE_R_INTERFACE_FILE};${TINYSPLINE_RUBY_SOURCE_DIRECTORY}"
)



###############################################################################
### The following section defines CMake-related constants that are used to
### configure external tools, such as Setuptools and Doxygen. Accordingly,
### they are set with 'CACHE INTERNAL'.
#
# TINYSPLINE_***_CMAKE_TARGET
#   The CMake buid target for binding ***.
#
# TINYSPLINE_PLATFORM_NAME
#   Name of the platform in lowercase. Supported values are: 'linux', 'macosx',
#   'windows', and 'generic'.
#
# TINYSPLINE_PLATFORM
#   Platform specific identifier, e. g. 'linux-x86_64'.
###############################################################################
set(TINYSPLINE_CSHARP_CMAKE_TARGET "tinysplinecsharp" CACHE INTERNAL "")
set(TINYSPLINE_DLANG_CMAKE_TARGET "tinysplinedlang" CACHE INTERNAL "")
set(TINYSPLINE_JAVA_CMAKE_TARGET "tinysplinejava" CACHE INTERNAL "")
set(TINYSPLINE_OCTAVE_CMAKE_TARGET "tinysplineoctave" CACHE INTERNAL "")
set(TINYSPLINE_LUA_CMAKE_TARGET "tinysplinelua" CACHE INTERNAL "")
set(TINYSPLINE_PHP_CMAKE_TARGET "tinysplinephp" CACHE INTERNAL "")
set(TINYSPLINE_PYTHON_CMAKE_TARGET "tinysplinepython" CACHE INTERNAL "")
set(TINYSPLINE_R_CMAKE_TARGET "tinyspliner" CACHE INTERNAL "")
set(TINYSPLINE_RUBY_CMAKE_TARGET "tinysplineruby" CACHE INTERNAL "")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(TINYSPLINE_PLATFORM_NAME "linux" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM "linux-${CMAKE_SYSTEM_PROCESSOR}" CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set(TINYSPLINE_PLATFORM_NAME "macosx" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM "macosx-${CMAKE_SYSTEM_PROCESSOR}" CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(TINYSPLINE_PLATFORM_NAME "windows" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM "windows-x86_64" CACHE INTERNAL "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(TINYSPLINE_PLATFORM_NAME "windows" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM "windows-x86" CACHE INTERNAL "")
else()
	set(TINYSPLINE_PLATFORM_NAME "generic" CACHE INTERNAL "")
	set(TINYSPLINE_PLATFORM "generic" CACHE INTERNAL "")
endif()



###############################################################################
### Add different options to this script. The following options are available.
#
# TINYSPLINE_FLOAT_PRECISION - default: OFF
#   Use single (float) instead of double precision.
#
# TINYSPLINE_PYTHON_VERSION - default: ANY
#   Force Python version.
#
# TINYSPLINE_ENABLE_*** - default: TRUE for CXX, FALSE otherwise
#   These options allow to enable certain interfaces.
#
# TINYSPLINE_ENABLE_ALL_INTERFACES
#   If set, all interfaces are enabled automatically.
###############################################################################
# TINYSPLINE_FLOAT_PRECISION
option(TINYSPLINE_FLOAT_PRECISION "Build TinySpline with single (float) precision." OFF)

# TINYSPLINE_PYTHON_VERSION
set(TINYSPLINE_PYTHON_VERSION "ANY" CACHE STRING "Force Python version. Supported values are: '2', '3', and 'ANY' (fallback for unknown values).")

# TINYSPLINE_ENABLE_***
option(TINYSPLINE_ENABLE_CXX "Enable the C++ interface." TRUE)
option(TINYSPLINE_ENABLE_CSHARP "Enable the C# interface." FALSE)
option(TINYSPLINE_ENABLE_DLANG "Enable the D interface." FALSE)
option(TINYSPLINE_ENABLE_GOLANG "Enable the Go interface. (Not supported yet)" FALSE)
option(TINYSPLINE_ENABLE_JAVA "Enable the Java interface." FALSE)
option(TINYSPLINE_ENABLE_LUA "Enable the Lua interface." FALSE)
option(TINYSPLINE_ENABLE_OCTAVE "Enable the Octave interface." FALSE)
option(TINYSPLINE_ENABLE_PHP "Enable the PHP interface." FALSE)
option(TINYSPLINE_ENABLE_PYTHON "Enable the Python interface." FALSE)
option(TINYSPLINE_ENABLE_R "Enable the R interface." FALSE)
option(TINYSPLINE_ENABLE_RUBY "Enable the Ruby interface." FALSE)

option(TINYSPLINE_ENABLE_ALL_INTERFACES "Enable all interfaces." FALSE)
if (TINYSPLINE_ENABLE_ALL_INTERFACES)
	set(TINYSPLINE_ENABLE_CSHARP TRUE)
	set(TINYSPLINE_ENABLE_DLANG TRUE)
	#set(TINYSPLINE_ENABLE_GOLANG TRUE)
	set(TINYSPLINE_ENABLE_JAVA TRUE)
	set(TINYSPLINE_ENABLE_LUA TRUE)
	set(TINYSPLINE_ENABLE_OCTAVE TRUE)
	set(TINYSPLINE_ENABLE_PHP TRUE)
	set(TINYSPLINE_ENABLE_PYTHON TRUE)
	set(TINYSPLINE_ENABLE_R TRUE)
	set(TINYSPLINE_ENABLE_RUBY TRUE)
endif()



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
### Setup compiler suite. The following CMake variables may be useful in the
### remainder of this file. CMake variables labeled with (exported) will be set
### in parent scope and should be used accordingly after including this file.
#
# TINYSPLINE_DEBUG_POSTFIX
#   Postfix of debug builds.
#
# TINYSPLINE_DEFINITIONS (exported)
#   Preprocessor definitions that should be added to parent file with
#   'add_definitions("${TINYSPLINE_DEFINITIONS}")'.
#
# TINYSPLINE_CXX11_COMPLIANT
#   Does the C++ compiler support C++11? (Required by some bindings)
#
# TINYSPLINE_LIBRARIES (exported)
#   Additional libraries to link against.
#
# TINYSPLINE_LIBRARY_C_FLAGS
#   C library specific flags.
#
# TINYSPLINE_LIBRARY_CXX_FLAGS
#   C++ library specific flags.
#
# TINYSPLINE_BINDING_CXX_FLAGS
#   C++ binding specific flags.
###############################################################################
# TINYSPLINE_DEBUG_POSTFIX
# TINYSPLINE_DEFINITIONS
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	# it is common practice for MSVC builds to add a debug postfix
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
# Remove leading and trailing spaces from TINYSPLINE_DEFINITIONS
string(STRIP "${TINYSPLINE_DEFINITIONS}" TINYSPLINE_DEFINITIONS)
# Export TINYSPLINE_DEFINITIONS to parent CMakeLists.txt file
set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS}" PARENT_SCOPE)
add_definitions("${TINYSPLINE_DEFINITIONS}")

# TINYSPLINE_CXX11_COMPLIANT
# TINYSPLINE_LIBRARIES
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

	# CMAKE_C_FLAGS
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

	# TINYSPLINE_LIBRARIES
	set(TINYSPLINE_LIBRARIES "${TINYSPLINE_LIBRARIES} -lm")

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

	# TINYSPLINE_LIBRARIES
	set(TINYSPLINE_LIBRARIES "${TINYSPLINE_LIBRARIES} -lm")

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
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
	# TODO: Find appropriate settings for intel

	# TINYSPLINE_CXX11_COMPLIANT
	set(TINYSPLINE_CXX11_COMPLIANT TRUE)
endif()
# Remove leading and trailing spaces
string(STRIP "${CMAKE_C_FLAGS}" CMAKE_C_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS)
string(STRIP "${TINYSPLINE_LIBRARIES}" TINYSPLINE_LIBRARIES)
string(STRIP "${TINYSPLINE_LIBRARY_C_FLAGS}" TINYSPLINE_LIBRARY_C_FLAGS)
string(STRIP "${TINYSPLINE_LIBRARY_CXX_FLAGS}" TINYSPLINE_LIBRARY_CXX_FLAGS)
string(STRIP "${TINYSPLINE_BINDING_CXX_FLAGS}" TINYSPLINE_BINDING_CXX_FLAGS)
# Export variables to parent file
set(TINYSPLINE_LIBRARIES "${TINYSPLINE_LIBRARIES}" PARENT_SCOPE)



###############################################################################
### Create C/C++ libraries. We explicitly set the prefix of the static
### libraries to 'lib' to prevent naming collisions when compiling with MSVC.
### The following CMake variables may be useful in the remainder of this file.
### CMake variables labeled with (exported) will be set in parent scope and
### should be used accordingly after including this file.
#
# TINYSPLINE_INCLUDE_DIRS (exported)
#   Include directories of the C/C++ library. The listed directories contain
#   all header files as well as all source files (.c/.cpp files).
#
# TINYSPLINE_C_SOURCE_FILES (exported)
#   List of source files (absolute paths) that are required to build the C
#   library. Does not contain header files.
#
# TINYSPLINE_CXX_SOURCE_FILES (exported)
#   List of source files (absolute paths) that are required to build the C++
#   library. Does not contain header files, but all source files listed in
#   TINYSPLINE_C_SOURCE_FILES.
###############################################################################
# TINYSPLINE_INCLUDE_DIRS
set(TINYSPLINE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

# TINYSPLINE_C_SOURCE_FILES
list(APPEND TINYSPLINE_C_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/tinyspline.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/parson.c"
)
set(TINYSPLINE_C_SOURCE_FILES ${TINYSPLINE_C_SOURCE_FILES} PARENT_SCOPE)

# TINYSPLINE_CXX_SOURCE_FILES
list(APPEND TINYSPLINE_CXX_SOURCE_FILES
	${TINYSPLINE_C_SOURCE_FILES}
	"${CMAKE_CURRENT_SOURCE_DIR}/tinysplinecpp.cpp"
)
set(TINYSPLINE_CXX_SOURCE_FILES ${TINYSPLINE_CXX_SOURCE_FILES} PARENT_SCOPE)

# C library
add_library(tinyspline_shared SHARED ${TINYSPLINE_C_SOURCE_FILES})
set_target_properties(tinyspline_shared PROPERTIES
	OUTPUT_NAME "tinyspline"
	DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
	COMPILE_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS}"
)
install(TARGETS tinyspline_shared
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)
add_library(tinyspline_static STATIC ${TINYSPLINE_C_SOURCE_FILES})
set_target_properties(tinyspline_static PROPERTIES
	OUTPUT_NAME "tinyspline"
	DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
	PREFIX "lib"
	COMPILE_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS}"
)
install(TARGETS tinyspline_static
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
install(FILES tinyspline.h
	DESTINATION include
)
target_include_directories(tinyspline_static
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# C++ library
if(TINYSPLINE_ENABLE_CXX)
	add_library(tinysplinecpp_shared SHARED ${TINYSPLINE_CXX_SOURCE_FILES})
	set_target_properties(tinysplinecpp_shared PROPERTIES
		OUTPUT_NAME "tinysplinecpp"
		DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
		COMPILE_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS}"
	)
	install(TARGETS tinysplinecpp_shared
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
	)
	add_library(tinysplinecpp_static STATIC ${TINYSPLINE_CXX_SOURCE_FILES})
	set_target_properties(tinysplinecpp_static PROPERTIES
		OUTPUT_NAME "tinysplinecpp"
		DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
		PREFIX "lib"
		COMPILE_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS}"
	)
	install(TARGETS tinysplinecpp_static
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)
	install(FILES tinysplinecpp.h
		DESTINATION include
	)
	target_include_directories(tinysplinecpp_static
		PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	)
endif()



###############################################################################
### Setup SWIG and create bindings. The following variables may be useful in
### the remainder of this script. Keep in mind that the source code of the C#
### binding will always be available.
#
# TINYSPLINE_BINDINGS_FOLDER_NAME
#   Name of the IDE project folder containing the bindings.
#   ( set_property(GLOBAL PROPERTY USE_FOLDERS ON) )
#
# TINYSPLINE_***_DIR
#   Source output directory of binding ***.
#
# TINYSPLINE_ADDITIONAL_SWIG_DEFINES
#   A list of defines passed to Swig.
#
# TINYSPLINE_CSHARP_COMPILER
#   The used C# compiler. Indicates that the C# DLL of the C# binding is
#   available.
###############################################################################
find_package(SWIG 3.0.1)
if (SWIG_FOUND)
	include(${SWIG_USE_FILE})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})

	# TINYSPLINE_BINDINGS_FOLDER_NAME
	set(TINYSPLINE_BINDINGS_FOLDER_NAME "bindings")

	# Simplifies definition handling with SWIG by reducing the number of
	# required if statements.
	if(TINYSPLINE_FLOAT_PRECISION)
		list(APPEND TINYSPLINE_ADDITIONAL_SWIG_DEFINES -DTINYSPLINE_FLOAT_PRECISION)
	endif()

	# Python
	if(TINYSPLINE_ENABLE_PYTHON)
		if(${TINYSPLINE_PYTHON_VERSION} STREQUAL "2")
			set(Python_ADDITIONAL_VERSIONS 2.7 2.6 2.5 2.4 2.3 2.2 2.1 2.0)
		elseif(${TINYSPLINE_PYTHON_VERSION} STREQUAL "3")
			set(Python_ADDITIONAL_VERSIONS 3.9 3.8 3.7 3.6 3.5 3.4 3.3 3.2 3.1 3.0)
		endif()
		find_package(PythonLibs)
		if(${TINYSPLINE_PYTHON_VERSION} STREQUAL "2" AND NOT ${PYTHONLIBS_VERSION_STRING} MATCHES "^2."
				OR ${TINYSPLINE_PYTHON_VERSION} STREQUAL "3" AND NOT ${PYTHONLIBS_VERSION_STRING} MATCHES "^3.")
			message(FATAL_ERROR "Unable to find requested Python version")
		endif()
		if(PYTHONLIBS_FOUND)
			include_directories(${PYTHON_INCLUDE_DIRS})
			set_source_files_properties("${TINYSPLINE_PYTHON_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
			if(${PYTHONLIBS_VERSION_STRING} MATCHES "^3.")
				set(TINYSPLINE_PYTHON_DEFINES "-py3")
			endif()
			set(CMAKE_SWIG_FLAGS ${TINYSPLINE_PYTHON_DEFINES} -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
			set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_PYTHON_SOURCE_DIRECTORY})
			if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
				swig_add_module(${TINYSPLINE_PYTHON_CMAKE_TARGET} python "${TINYSPLINE_PYTHON_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			else()
				swig_add_library(${TINYSPLINE_PYTHON_CMAKE_TARGET} LANGUAGE python SOURCES "${TINYSPLINE_PYTHON_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			endif()
			swig_link_libraries(${TINYSPLINE_PYTHON_CMAKE_TARGET} ${PYTHON_LIBRARIES})
			# For some reason the make target for python starts with '_'.
			add_custom_command(TARGET _${TINYSPLINE_PYTHON_CMAKE_TARGET} POST_BUILD
				COMMAND
					${CMAKE_COMMAND} -E copy
					"${TINYSPLINE_PYTHON_SOURCE_DIRECTORY}/${TINYSPLINE_PYTHON_CMAKE_TARGET}.py"
					"${TINYSPLINE_PYTHON_INTERFACE_FILE}"
			)
			set_target_properties(_${TINYSPLINE_PYTHON_CMAKE_TARGET} PROPERTIES
				FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
				COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
			)
		endif()
	endif()

	# Java
	if(TINYSPLINE_ENABLE_JAVA)
		find_package(JNI)
		if(JNI_FOUND)
			include_directories(${JNI_INCLUDE_DIRS})
			set_source_files_properties("${TINYSPLINE_JAVA_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
			set(CMAKE_SWIG_FLAGS -package "org.tinyspline" -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
			set(CMAKE_SWIG_OUTDIR "${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/org/tinyspline")
			if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
				swig_add_module(${TINYSPLINE_JAVA_CMAKE_TARGET} java "${TINYSPLINE_JAVA_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			else()
				swig_add_library(${TINYSPLINE_JAVA_CMAKE_TARGET} LANGUAGE java SOURCES "${TINYSPLINE_JAVA_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			endif()
			swig_link_libraries(${TINYSPLINE_JAVA_CMAKE_TARGET} ${JNI_LIBRARIES})

			# Jar archive
			find_package(Java COMPONENTS Development)
			if(Java_Development_FOUND)
				include(UseJava)
				add_custom_command(TARGET ${TINYSPLINE_JAVA_CMAKE_TARGET} POST_BUILD
					COMMAND
						${CMAKE_COMMAND}
						-E make_directory
						"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes"
					COMMAND
						${CMAKE_COMMAND}
						-E copy
						"${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/$<TARGET_FILE_NAME:${TINYSPLINE_JAVA_CMAKE_TARGET}>"
						"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes/${TINYSPLINE_PLATFORM}"
					COMMAND
						"${Java_JAVAC_EXECUTABLE}"
						-d "${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes"
						"${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/org/tinyspline/*.java"
					COMMAND
						"${Java_JAR_EXECUTABLE}"
						-cfM "${TINYSPLINE_JAVA_INTERFACE_FILE}"
						-C "${TINYSPLINE_JAVA_SOURCE_DIRECTORY}/classes" .
				)
			endif()
			set_target_properties(${TINYSPLINE_JAVA_CMAKE_TARGET} PROPERTIES
				FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
				COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
			)
		endif()
	endif()

	# Ruby
	if(TINYSPLINE_ENABLE_RUBY)
		find_package(Ruby)
		if(RUBY_FOUND)
			include_directories(${RUBY_INCLUDE_DIRS})
			set_source_files_properties("${TINYSPLINE_RUBY_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
			set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
			set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_RUBY_SOURCE_DIRECTORY})
			if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
				swig_add_module(${TINYSPLINE_RUBY_CMAKE_TARGET} ruby "${TINYSPLINE_RUBY_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			else()
				swig_add_library(${TINYSPLINE_RUBY_CMAKE_TARGET} LANGUAGE ruby SOURCES "${TINYSPLINE_RUBY_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			endif()
			swig_link_libraries(${TINYSPLINE_RUBY_CMAKE_TARGET} ${RUBY_LIBRARY})
			set_target_properties(${TINYSPLINE_RUBY_CMAKE_TARGET} PROPERTIES
				FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
				COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
			)
		endif()
	endif()

	# C#
	if(TINYSPLINE_ENABLE_CSHARP)
		set_source_files_properties("${TINYSPLINE_CSHARP_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
		set(CMAKE_SWIG_FLAGS -namespace TinySpline -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
		set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_CSHARP_SOURCE_DIRECTORY})
		if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
			swig_add_module(${TINYSPLINE_CSHARP_CMAKE_TARGET} csharp "${TINYSPLINE_CSHARP_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
		else()
			swig_add_library(${TINYSPLINE_CSHARP_CMAKE_TARGET} LANGUAGE csharp SOURCES "${TINYSPLINE_CSHARP_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
		endif()
		set_target_properties(${TINYSPLINE_CSHARP_CMAKE_TARGET} PROPERTIES
			FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
			COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
		)

		# C# DLL
		find_program(TINYSPLINE_CSHARP_COMPILER
			NAMES csc mcs
			PATHS "C:/Windows/Microsoft.NET/Framework/v4.5"
		)
		if(TINYSPLINE_CSHARP_COMPILER)
			if(TINYSPLINE_CSHARP_COMPILER MATCHES "csc$|csc.exe$")
				# csc neither supports the 'sdk' flag nor '/' in 'recursive'
				add_custom_command(TARGET ${TINYSPLINE_CSHARP_CMAKE_TARGET} POST_BUILD
					COMMAND
						${TINYSPLINE_CSHARP_COMPILER}
						/target:library
						/out:"${TINYSPLINE_CSHARP_INTERFACE_FILE}"
						/recurse:"${TINYSPLINE_CSHARP_SOURCE_DIRECTORY}\\*.cs"
				)
			else()
				add_custom_command(TARGET ${TINYSPLINE_CSHARP_CMAKE_TARGET} POST_BUILD
					COMMAND
						${TINYSPLINE_CSHARP_COMPILER}
						-sdk:4.5
						-target:library
						-out:"${TINYSPLINE_CSHARP_INTERFACE_FILE}"
						-recurse:"${TINYSPLINE_CSHARP_SOURCE_DIRECTORY}/*.cs"
				)
			endif()
		endif()
	endif()

	# Go (not fully supported yet)
	if(TINYSPLINE_ENABLE_GOLANG)
		#set_source_files_properties(tinysplinego.i PROPERTIES CPLUSPLUS ON)
		#set(CMAKE_SWIG_FLAGS -intgosize 32 -package tinyspline -O)
		#set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_GO_DIR})
		#swig_add_module(tinysplinego go tinysplinego.i ${TINYSPLINE_CXX_SOURCE_FILES})
		#add_custom_command(TARGET tinysplinego POST_BUILD
		#  COMMAND ${CMAKE_COMMAND} -E copy "${TINYSPLINE_GO_DIR}/tinysplinego.go"
		#  "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_GO_INTERFACE_FILE}"
		#)
	endif()

	# PHP
	if(TINYSPLINE_ENABLE_PHP)
		find_package(PHP COMPONENTS Zend)
		if(PHP_FOUND)
			set(TINYSPLINE_PHP_VERSION "php5")
			if(${PHP_VERSION_NUMBER} MATCHES "^7")
				set(TINYSPLINE_PHP_VERSION "php7")
			endif()
			if(${TINYSPLINE_PHP_VERSION} STREQUAL "php7" AND ${SWIG_VERSION} VERSION_LESS 3.0.11)
				message(STATUS "PHP 7 requires Swig 3.0.11 or later")
				# Clear PHP related variables used in summary.
				set(PHP_FOUND FALSE)
				unset(PHP_VERSION_STRING)
				unset(PHP_INCLUDE_DIRS)
			else()
				include_directories(${PHP_INCLUDE_DIRS})
				set_source_files_properties("${TINYSPLINE_PHP_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
				set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
				set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_PHP_SOURCE_DIRECTORY})
				if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
					swig_add_module(${TINYSPLINE_PHP_CMAKE_TARGET} ${TINYSPLINE_PHP_VERSION} "${TINYSPLINE_PHP_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
				else()
					swig_add_library(${TINYSPLINE_PHP_CMAKE_TARGET} LANGUAGE ${TINYSPLINE_PHP_VERSION} SOURCES "${TINYSPLINE_PHP_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
				endif()
				add_custom_command(TARGET ${TINYSPLINE_PHP_CMAKE_TARGET} POST_BUILD
					COMMAND
						${CMAKE_COMMAND}
						-E copy
						"${TINYSPLINE_PHP_SOURCE_DIRECTORY}/${TINYSPLINE_PHP_CMAKE_TARGET}.php"
						"${TINYSPLINE_PHP_INTERFACE_FILE}"
				)
				set_target_properties(${TINYSPLINE_PHP_CMAKE_TARGET} PROPERTIES
					FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
					COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} ${PHP_INCLUDE_DIRS}"
				)
			endif()
		else()
			if(APPLE)
				message(STATUS "Note: The PHP version that comes with macOS does bundle Zend by default.")
			endif()
		endif()
	endif()

	# Lua
	if(TINYSPLINE_ENABLE_LUA)
		find_package(Lua)
		if(LUA_FOUND)
			include_directories(${LUA_INCLUDE_DIR})
			set_source_files_properties("${TINYSPLINE_LUA_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
			set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
			set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_LUA_SOURCE_DIRECTORY})
			if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
				swig_add_module(${TINYSPLINE_LUA_CMAKE_TARGET} lua "${TINYSPLINE_LUA_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			else()
				swig_add_library(${TINYSPLINE_LUA_CMAKE_TARGET} LANGUAGE lua SOURCES "${TINYSPLINE_LUA_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			endif()
			swig_link_libraries(${TINYSPLINE_LUA_CMAKE_TARGET} ${LUA_LIBRARIES})
			set_target_properties(${TINYSPLINE_LUA_CMAKE_TARGET} PROPERTIES
				FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
				COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
			)
		endif()
	endif()

	# D
	if(TINYSPLINE_ENABLE_DLANG)
		set_source_files_properties("${TINYSPLINE_DLANG_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
		set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES} -d2 -wrapperlibrary ${TINYSPLINE_DLANG_CMAKE_TARGET} -module ${TINYSPLINE_PACKAGE_NAME})
		set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_DLANG_SOURCE_DIRECTORY})
		if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
			swig_add_module(${TINYSPLINE_DLANG_CMAKE_TARGET} d "${TINYSPLINE_DLANG_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
		else()
			swig_add_library(${TINYSPLINE_DLANG_CMAKE_TARGET} LANGUAGE d SOURCES "${TINYSPLINE_DLANG_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
		endif()
		add_custom_command(TARGET ${TINYSPLINE_DLANG_CMAKE_TARGET} POST_BUILD
			COMMAND
				${CMAKE_COMMAND}
				-E copy
				"${TINYSPLINE_DLANG_SOURCE_DIRECTORY}/${TINYSPLINE_PACKAGE_NAME}.d"
				"${TINYSPLINE_DLANG_INTERFACE_FILE}"
			COMMAND
				${CMAKE_COMMAND}
				-E copy
				"${TINYSPLINE_DLANG_SOURCE_DIRECTORY}/${TINYSPLINE_PACKAGE_NAME}_im.d"
				"${TINYSPLINE_DLANG_INTERMEDIATE_FILE}"
		)
		set_target_properties(${TINYSPLINE_DLANG_CMAKE_TARGET} PROPERTIES
			FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
			COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
		)
	endif()

	# Octave
	if(TINYSPLINE_ENABLE_OCTAVE)
		find_package(Octave)
		if(OCTAVE_FOUND)
			if((${OCTAVE_VERSION_STRING} VERSION_GREATER 4.2 OR ${OCTAVE_VERSION_STRING} VERSION_EQUAL 4.2)
					AND ${SWIG_VERSION} VERSION_LESS 3.0.12)
				message(STATUS "Octave 4.2 requires Swig 3.0.12 or later")
				# Clear Octave related variables used in summary.
				set(OCTAVE_FOUND FALSE)
				unset(OCTAVE_VERSION_STRING)
				unset(OCTAVE_INCLUDE_DIRS)
				unset(OCTAVE_LIBRARIES)
			elseif(NOT TINYSPLINE_CXX11_COMPLIANT)
				message(STATUS "Octave requires C++11")
				# Clear Octave related variables used in summary.
				set(OCTAVE_FOUND FALSE)
				unset(OCTAVE_VERSION_STRING)
				unset(OCTAVE_INCLUDE_DIRS)
				unset(OCTAVE_LIBRARIES)
			else()
				include_directories(${OCTAVE_INCLUDE_DIRS})
				set_source_files_properties("${TINYSPLINE_OCTAVE_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
				set(CMAKE_SWIG_FLAGS -module ${TINYSPLINE_PACKAGE_NAME} -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
				set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_OCTAVE_SOURCE_DIRECTORY})
				if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
					swig_add_module(${TINYSPLINE_OCTAVE_CMAKE_TARGET} octave "${TINYSPLINE_OCTAVE_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
				else()
					swig_add_library(${TINYSPLINE_OCTAVE_CMAKE_TARGET} LANGUAGE octave SOURCES "${TINYSPLINE_OCTAVE_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
				endif()
				swig_link_libraries(${TINYSPLINE_OCTAVE_CMAKE_TARGET} ${OCTAVE_LIBRARIES})
				set_target_properties(${TINYSPLINE_OCTAVE_CMAKE_TARGET} PROPERTIES
					FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
					COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
					CXX_STANDARD 11
					CXX_STANDARD_REQUIRED ON
					CXX_EXTENSIONS OFF
					OUTPUT_NAME "${TINYSPLINE_PACKAGE_NAME}"
				)
			endif()
		endif()
	endif()

	# R
	if(TINYSPLINE_ENABLE_R)
		find_package(R COMPONENTS Rcpp)
		if(R_FOUND)
			include_directories(${R_INCLUDE_DIRS})
			set_source_files_properties("${TINYSPLINE_R_CMAKE_TARGET}.i" PROPERTIES CPLUSPLUS ON)
			set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
			set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_R_SOURCE_DIRECTORY})
			if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
				swig_add_module(${TINYSPLINE_R_CMAKE_TARGET} r "${TINYSPLINE_R_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			else()
				swig_add_library(${TINYSPLINE_R_CMAKE_TARGET} LANGUAGE r SOURCES "${TINYSPLINE_R_CMAKE_TARGET}.i" ${TINYSPLINE_CXX_SOURCE_FILES})
			endif()
			swig_link_libraries(${TINYSPLINE_R_CMAKE_TARGET} ${R_LIBRARIES})
			add_custom_command(TARGET ${TINYSPLINE_R_CMAKE_TARGET} POST_BUILD
				COMMAND
					${CMAKE_COMMAND}
					-E copy
					"${TINYSPLINE_R_SOURCE_DIRECTORY}/${TINYSPLINE_R_CMAKE_TARGET}.R"
					"${TINYSPLINE_R_INTERFACE_FILE}"
			)
			set_target_properties(${TINYSPLINE_R_CMAKE_TARGET} PROPERTIES
				FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
				COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
			)
		endif()
	endif()
endif(SWIG_FOUND)



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
tinyspline_set_on_off(TINYSPLINE_WITH_GOLANG FALSE)
tinyspline_set_on_off(TINYSPLINE_WITH_JAVA JNI_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_LUA LUA_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_OCTAVE OCTAVE_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_PHP PHP_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_PYTHON PYTHONLIBS_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_RUBY RUBY_FOUND)
tinyspline_set_on_off(TINYSPLINE_WITH_R R_FOUND)



###############################################################################
### Configure language-specific build systems.
###############################################################################
# Dub
file(REMOVE "${CMAKE_BINARY_DIR}/dub.json")
if(${TINYSPLINE_WITH_DLANG})
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/dub.json.in"
		"${CMAKE_BINARY_DIR}/dub.json"
		@ONLY
	)
endif()

# Luarocks
file(REMOVE "${CMAKE_BINARY_DIR}/tinyspline-${TINYSPLINE_VERSION}-1.rockspec")
if(${TINYSPLINE_WITH_LUA})
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/tinyspline.rockspec.in"
		"${CMAKE_BINARY_DIR}/tinyspline-${TINYSPLINE_VERSION}-1.rockspec"
		@ONLY
	)
endif()

# Nuget
file(REMOVE "${CMAKE_BINARY_DIR}/.nuspec")
if(${TINYSPLINE_WITH_CSHARP})
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/.nuspec.in"
		"${CMAKE_BINARY_DIR}/.nuspec"
		@ONLY
	)
endif()

# Maven
file(REMOVE "${CMAKE_BINARY_DIR}/pom.xml")
if(${TINYSPLINE_WITH_JAVA})
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/pom.xml.in"
		"${CMAKE_BINARY_DIR}/pom.xml"
		@ONLY
	)
endif()

# Distutils
file(REMOVE "${CMAKE_BINARY_DIR}/setup.py")
if(${TINYSPLINE_WITH_PYTHON})
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in"
		"${CMAKE_BINARY_DIR}/setup.py"
		@ONLY
	)
endif()

# Gem
file(REMOVE "${CMAKE_BINARY_DIR}/tinyspline.gemspec")
if(${TINYSPLINE_WITH_RUBY})
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/tinyspline.gemspec.in"
		"${CMAKE_BINARY_DIR}/tinyspline.${TINYSPLINE_PLATFORM}.gemspec"
		@ONLY
	)
endif()



###############################################################################
### Print summary.
###############################################################################
string(STRIP "${CMAKE_C_FLAGS} ${TINYSPLINE_LIBRARY_C_FLAGS}" TINYSPLINE_SUMMARY_C_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS} ${TINYSPLINE_LIBRARY_CXX_FLAGS}" TINYSPLINE_SUMMARY_CXX_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS} ${TINYSPLINE_BINDING_CXX_FLAGS}" TINYSPLINE_SUMMARY_BINDING_FLAGS)

message(STATUS
"
******************* TinySpline Configuration Summary *******************
Interface Configuration:
  With single precision  (default: OFF): ${TINYSPLINE_FLOAT_PRECISION}

Compiler Configuration:
  Compiler:       ${CMAKE_CXX_COMPILER}
  C flags:        ${TINYSPLINE_SUMMARY_C_FLAGS}
  C++ flags:      ${TINYSPLINE_SUMMARY_CXX_FLAGS}
  Binding flags:  ${TINYSPLINE_SUMMARY_BINDING_FLAGS}
  Definitions:    ${TINYSPLINE_DEFINITIONS}
  Toolchain file: ${CMAKE_TOOLCHAIN_FILE}

Available Interfaces:
  C++:          ${TINYSPLINE_WITH_CXX}

  C\#:           ${TINYSPLINE_WITH_CSHARP}
    Compiler: ${TINYSPLINE_CSHARP_COMPILER}

  D:            ${TINYSPLINE_WITH_DLANG}

  Go:           ${TINYSPLINE_WITH_GOLANG}
    (Not supported yet)

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
")
