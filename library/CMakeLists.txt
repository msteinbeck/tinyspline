###############################################################################
### Add different options to this script. The following options are available.
#
# TINYSPLINE_DOUBLE_PRECISION - default: OFF
#   Use double instead of single (float) precision.
#
# TINYSPLINE_DISABLE_CXX11_FEATURES - default: OFF
#   Disable C++11 specific features in the C++ interface.
#
# TINYSPLINE_PYTHON_VERSION - default: ANY
#   Force Python version.
###############################################################################
# TINYSPLINE_DOUBLE_PRECISION
option(TINYSPLINE_DOUBLE_PRECISION "Build TinySpline with double precision." OFF)

# TINYSPLINE_DISABLE_CXX11_FEATURES
option(TINYSPLINE_DISABLE_CXX11_FEATURES "Build TinySpline without C++11 features." OFF)

# TINYSPLINE_PYTHON_VERSION
set(TINYSPLINE_PYTHON_VERSION "ANY" CACHE STRING "Force Python version. Supported values are: '2', '3', and 'ANY' (fallback for unknown values).")



###############################################################################
### Map supported environment variables to CMake variables. The following
### variables will be processed and may overwrite previously defined options
### (only if the option's default value has been selected).
#
# TINYSPLINE_DOUBLE_PRECISION
#   See corresponding option above.
#
# TINYSPLINE_DISABLE_CXX11_FEATURES
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
# TINYSPLINE_DOUBLE_PRECISION
if(NOT TINYSPLINE_DOUBLE_PRECISION AND DEFINED ENV{TINYSPLINE_DOUBLE_PRECISION})
  message(STATUS "Using environment variable 'TINYSPLINE_DOUBLE_PRECISION'")
  set(TINYSPLINE_DOUBLE_PRECISION $ENV{TINYSPLINE_DOUBLE_PRECISION})
endif()

# TINYSPLINE_DISABLE_CXX11_FEATURES
if(NOT TINYSPLINE_DISABLE_CXX11_FEATURES AND DEFINED ENV{TINYSPLINE_DISABLE_CXX11_FEATURES})
  message(STATUS "Using environment variable 'TINYSPLINE_DISABLE_CXX11_FEATURES'")
  set(TINYSPLINE_DISABLE_CXX11_FEATURES $ENV{TINYSPLINE_DISABLE_CXX11_FEATURES})
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
### Set output directory for binaries to 'TINYSPLINE_BINARY_OUTPUT_DIRECTORY'
### and for libraries to 'TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY'. Allows other
### build systems like Python's 'setuptools' and Java's 'maven' to make
### assumptions about the location of the resulting files.
#
# TINYSPLINE_BINARY_OUTPUT_DIRECTORY
#   Location of the resulting binaries.
#
# TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY
#   Location of the resulting libraries.
###############################################################################
set(TINYSPLINE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

# First for the generic no-config case (e.g. with MinGW)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TINYSPLINE_BINARY_OUTPUT_DIRECTORY} CACHE INTERNAL "" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY} CACHE INTERNAL "" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY} CACHE INTERNAL "" FORCE)
# Second, for multi-config builds (e.g. MSVC)
foreach(TINYSPLINE_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${TINYSPLINE_CONFIG} TINYSPLINE_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_BINARY_OUTPUT_DIRECTORY} CACHE INTERNAL "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY} CACHE INTERNAL "" FORCE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY} CACHE INTERNAL "" FORCE)
endforeach()



###############################################################################
### Setup compiler suite. The following CMake variables may be useful in the
### remainder of this file. CMake variables labeled with (exported) will be set
### in parent scope and should be used accordingly after including this file.
#
# CMAKE_SYSTEM_PROCESSOR
#   The name of the target system.
#
# TARGET_SUPPORTS_SHARED_LIBS
#   Does the target support shared libraries?
#
# TINYSPLINE_DEBUG_POSTFIX
#   Postfix of debug builds.
#
# TINYSPLINE_DEFINITIONS (exported)
#   Preprocessor definitions that should be added to parent file with
#   'add_definitions("${TINYSPLINE_DEFINITIONS}")'.
#
# CMAKE_C_FLAGS (exported)
#   Common C flags.
#
# CMAKE_CXX_FLAGS (exported)
#   Common C++ flags.
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
#
# TINYSPLINE_CXX_AVAILABLE
#   Is an appropriate C++ compiler available?
###############################################################################
# TARGET_SUPPORTS_SHARED_LIBS
if(NOT TARGET_SUPPORTS_SHARED_LIBS)
  set(TARGET_SUPPORTS_SHARED_LIBS TRUE)
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES "avr")
  set(TARGET_SUPPORTS_SHARED_LIBS FALSE)
endif()

# TINYSPLINE_DEBUG_POSTFIX
# TINYSPLINE_DEFINITIONS
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  # it is common practice for MSVC builds to add a debug postfix
  set(TINYSPLINE_DEBUG_POSTFIX "d")
  if(TINYSPLINE_DOUBLE_PRECISION)
    set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS} /DTINYSPLINE_DOUBLE_PRECISION")
  endif()
  if(TINYSPLINE_DISABLE_CXX11_FEATURES)
    set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS} /DTINYSPLINE_DISABLE_CXX11_FEATURES")
  endif()
else()
  set(TINYSPLINE_DEBUG_POSTFIX "")
  if(TINYSPLINE_DOUBLE_PRECISION)
    set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS} -DTINYSPLINE_DOUBLE_PRECISION")
  endif()
  if(TINYSPLINE_DISABLE_CXX11_FEATURES)
    set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS} -DTINYSPLINE_DISABLE_CXX11_FEATURES")
  endif()
endif()
# Remove leading and trailing spaces from TINYSPLINE_DEFINITIONS
string(STRIP "${TINYSPLINE_DEFINITIONS}" TINYSPLINE_DEFINITIONS)
# Export TINYSPLINE_DEFINITIONS to parent CMakeLists.txt file
set(TINYSPLINE_DEFINITIONS "${TINYSPLINE_DEFINITIONS}" PARENT_SCOPE)
add_definitions("${TINYSPLINE_DEFINITIONS}")

# CMAKE_C_FLAGS
# CMAKE_CXX_FLAGS
# TINYSPLINE_LIBRARY_C_FLAGS
# TINYSPLINE_LIBRARY_CXX_FLAGS
# TINYSPLINE_BINDING_CXX_FLAGS
# TINYSPLINE_CXX_AVAILABLE
set(TINYSPLINE_CXX_AVAILABLE FALSE)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # CMAKE_C_FLAGS
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "avr")
    # avr uses the 'inline' keyword in some header files which is not
    # supported by c89
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")
  endif()

  # CMAKE_CXX_FLAGS
  if(NOT TINYSPLINE_DISABLE_CXX11_FEATURES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
  
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
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Werror")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wall")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wextra")
  # fixes some warnings of source files generated by swig
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -fno-strict-aliasing")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-missing-field-initializers")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-parameter")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-but-set-variable")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-ignored-qualifiers")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-function")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-sign-compare")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-label")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-variable")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-type-limits")
  
  # TINYSPLINE_CXX_AVAILABLE
  if(TINYSPLINE_DISABLE_CXX11_FEATURES OR
      # http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
      CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7 OR
      CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.7)
    set(TINYSPLINE_CXX_AVAILABLE TRUE)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # CMAKE_C_FLAGS
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

  # CMAKE_CXX_FLAGS
  if(NOT TINYSPLINE_DISABLE_CXX11_FEATURES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
  
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
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Werror")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wall")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wextra")
  # fixes some warnings of source files generated by swig
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-missing-field-initializers")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-parameter")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-ignored-qualifiers")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-function")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-sign-compare")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-deprecated-register")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-label")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-unused-variable")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} -Wno-tautological-compare")

  # TINYSPLINE_CXX_AVAILABLE
  if(TINYSPLINE_DISABLE_CXX11_FEATURES OR
      # http://stackoverflow.com/questions/10408849/how-can-i-use-c-11-features-in-clang
      CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.3 OR
      CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 3.3)
    set(TINYSPLINE_CXX_AVAILABLE TRUE)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  # TINYSPLINE_LIBRARY_C_FLAGS
  set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} /Wall")
  set(TINYSPLINE_LIBRARY_C_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS} /WX")

  # TINYSPLINE_LIBRARY_CXX_FLAGS
  set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} /Wall")
  set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} /WX")
  set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} /wd4514")
  set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} /wd4710")
  set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} /wd4350")
  set(TINYSPLINE_LIBRARY_CXX_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS} /wd4820")

  # TINYSPLINE_BINDING_CXX_FLAGS
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /Wall")
  # '/WX' causes to many errors in generated source code 
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /W4")
  # fixes some warnings of source files generated by swig
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /wd4820")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /wd4711")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /wd4702")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /wd4127")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /wd4244")
  set(TINYSPLINE_BINDING_CXX_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} /wd4189")

  # TINYSPLINE_CXX_AVAILABLE
  if(TINYSPLINE_DISABLE_CXX11_FEATURES OR
      CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 18 OR
      CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 18)
    set(TINYSPLINE_CXX_AVAILABLE TRUE)
  endif()

  # used to automatically create a .def file with all the global symbols for a
  # shared library
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # TODO: Find appropriate settings for intel

  # TINYSPLINE_CXX_AVAILABLE
  if(TINYSPLINE_DISABLE_CXX11_FEATURES OR
      CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 2014 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 2014)
    set(TINYSPLINE_CXX_AVAILABLE TRUE)
  endif()
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES "avr")
  # avr is missing some required headers
  set(TINYSPLINE_CXX_AVAILABLE FALSE)
endif()
# Remove leading and trailing spaces
string(STRIP "${CMAKE_C_FLAGS}" CMAKE_C_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS)
string(STRIP "${TINYSPLINE_LIBRARIES}" TINYSPLINE_LIBRARIES)
string(STRIP "${TINYSPLINE_LIBRARY_C_FLAGS}" TINYSPLINE_LIBRARY_C_FLAGS)
string(STRIP "${TINYSPLINE_LIBRARY_CXX_FLAGS}" TINYSPLINE_LIBRARY_CXX_FLAGS)
string(STRIP "${TINYSPLINE_BINDING_CXX_FLAGS}" TINYSPLINE_BINDING_CXX_FLAGS)
# Export variables to parent file
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" PARENT_SCOPE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
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
#   library. Does not contain header files but all source files listed in
#   TINYSPLINE_C_SOURCE_FILES.
###############################################################################
# TINYSPLINE_INCLUDE_DIRS
set(TINYSPLINE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

# TINYSPLINE_C_SOURCE_FILES
list(APPEND TINYSPLINE_C_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/tinyspline.c"
)
set(TINYSPLINE_C_SOURCE_FILES ${TINYSPLINE_C_SOURCE_FILES} PARENT_SCOPE)

# TINYSPLINE_CXX_SOURCE_FILES
list(APPEND TINYSPLINE_CXX_SOURCE_FILES
  ${TINYSPLINE_C_SOURCE_FILES}
  "${CMAKE_CURRENT_SOURCE_DIR}/tinysplinecpp.cpp"
)
set(TINYSPLINE_CXX_SOURCE_FILES ${TINYSPLINE_CXX_SOURCE_FILES} PARENT_SCOPE)

# C library
if(TARGET_SUPPORTS_SHARED_LIBS)
  add_library(tinyspline_shared SHARED ${TINYSPLINE_C_SOURCE_FILES})
  set_target_properties(tinyspline_shared PROPERTIES
    OUTPUT_NAME "tinyspline"
    DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
    COMPILE_FLAGS "${TINYSPLINE_LIBRARY_C_FLAGS}"
  )
  install(TARGETS tinyspline_shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
endif()
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
if(TINYSPLINE_CXX_AVAILABLE)
  if(TARGET_SUPPORTS_SHARED_LIBS)
    add_library(tinysplinecpp_shared SHARED ${TINYSPLINE_CXX_SOURCE_FILES})
    set_target_properties(tinysplinecpp_shared PROPERTIES
      OUTPUT_NAME "tinysplinecpp"
      DEBUG_POSTFIX "${TINYSPLINE_DEBUG_POSTFIX}"
      COMPILE_FLAGS "${TINYSPLINE_LIBRARY_CXX_FLAGS}"
    )
    install(TARGETS tinysplinecpp_shared
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
    )
  endif()
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
# TINYSPLINE_***_INTERFACE_FILE_NAME
#   Name of the interface file for binding *** (if any).
#
# TINYSPLINE_ADDITIONAL_SWIG_DEFINES
#   A list of defines passed to Swig.
#
# TINYSPLINE_CSHARP_COMPILER
#   The used C# compiler. Indicates that the C# DLL of the C# binding is
#   available.
###############################################################################
if(TINYSPLINE_CXX_AVAILABLE AND TARGET_SUPPORTS_SHARED_LIBS)
  # Swig
  find_package(SWIG 3.0.1)
  if (SWIG_FOUND)
    include(${SWIG_USE_FILE})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    # Use additional CMake modules to find dependencies.
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

    # TINYSPLINE_BINDINGS_FOLDER_NAME
    set(TINYSPLINE_BINDINGS_FOLDER_NAME "bindings")

    # Output directories of bindings
    set(TINYSPLINE_PYTHON_DIR "${CMAKE_CURRENT_BINARY_DIR}/python")
    set(TINYSPLINE_JAVA_DIR "${CMAKE_CURRENT_BINARY_DIR}/org/tinyspline")
    set(TINYSPLINE_RUBY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ruby")
    set(TINYSPLINE_CSHARP_DIR "${CMAKE_CURRENT_BINARY_DIR}/csharp")
    set(TINYSPLINE_GO_DIR "${CMAKE_CURRENT_BINARY_DIR}/go")
    set(TINYSPLINE_PHP_DIR "${CMAKE_CURRENT_BINARY_DIR}/php")
    set(TINYSPLINE_LUA_DIR "${CMAKE_CURRENT_BINARY_DIR}/lua")
    set(TINYSPLINE_DLANG_DIR "${CMAKE_CURRENT_BINARY_DIR}/dlang")
    # Names of interface files
    set(TINYSPLINE_PYTHON_INTERFACE_FILE_NAME "tinyspline.py")
    set(TINYSPLINE_JAVA_INTERFACE_FILE_NAME "tinyspline.jar")
    set(TINYSPLINE_CSHARP_INTERFACE_FILE_NAME "TinySpline.dll")
    set(TINYSPLINE_GO_INTERFACE_FILE_NAME "tinyspline.go")
    set(TINYSPLINE_PHP_INTERFACE_FILE_NAME "tinyspline.php")
    set(TINYSPLINE_DLANG_INTERFACE_FILE_NAME "tinyspline.d")
    set(TINYSPLINE_DLANG_INTERMEDIATE_FILE_NAME "tinyspline_im.d")
    # Add additional clean targets
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES 
      "${TINYSPLINE_PYTHON_DIR};${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_PYTHON_INTERFACE_FILE_NAME};${CMAKE_CURRENT_BINARY_DIR}/org;${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_JAVA_INTERFACE_FILE_NAME};${TINYSPLINE_RUBY_DIR};${TINYSPLINE_CSHARP_DIR};${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_CSHARP_INTERFACE_FILE_NAME};${TINYSPLINE_GO_DIR};${TINYSPLINE_PHP_DIR};${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_PHP_INTERFACE_FILE_NAME};${TINYSPLINE_LUA_DIR};${TINYSPLINE_DLANG_DIR};${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_DLANG_INTERFACE_FILE_NAME};${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_DLANG_INTERMEDIATE_FILE_NAME}"
    )

    # Simplifies definition handling with SWIG by reducing the number of
    # required if statements.
    if(TINYSPLINE_DOUBLE_PRECISION)
      list(APPEND TINYSPLINE_ADDITIONAL_SWIG_DEFINES -DTINYSPLINE_DOUBLE_PRECISION)
    endif()
    if(TINYSPLINE_DISABLE_CXX11_FEATURES)
      list(APPEND TINYSPLINE_ADDITIONAL_SWIG_DEFINES -DTINYSPLINE_DISABLE_CXX11_FEATURES)
    endif()

    # Python
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
      set_source_files_properties(tinysplinepython.i PROPERTIES CPLUSPLUS ON)
      if(${PYTHONLIBS_VERSION_STRING} MATCHES "^3.")
        set(TINYSPLINE_PYTHON_DEFINES "-py3")
      endif()
      set(CMAKE_SWIG_FLAGS ${TINYSPLINE_PYTHON_DEFINES} -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
      set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_PYTHON_DIR})
      swig_add_module(tinysplinepython python tinysplinepython.i ${TINYSPLINE_CXX_SOURCE_FILES})
      swig_link_libraries(tinysplinepython ${PYTHON_LIBRARIES})
      # for some reason the generated make target for python is '_tinysplinepython'
      add_custom_command(TARGET _tinysplinepython POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${TINYSPLINE_PYTHON_DIR}/tinysplinepython.py"
        "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_PYTHON_INTERFACE_FILE_NAME}"
      )
      set_target_properties(_tinysplinepython PROPERTIES
        FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
        COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
      )
    endif(PYTHONLIBS_FOUND)
    
    # Java
    find_package(JNI)
    if(JNI_FOUND)
      include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
      set_source_files_properties(tinysplinejava.i PROPERTIES CPLUSPLUS ON)
      set(CMAKE_SWIG_FLAGS -package "org.tinyspline" -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
      set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_JAVA_DIR})
      swig_add_module(tinysplinejava java tinysplinejava.i ${TINYSPLINE_CXX_SOURCE_FILES})
      swig_link_libraries(tinysplinejava ${JAVA_LIBRARIES})
      
      # Jar archive
      find_package(Java COMPONENTS Development)
      if(Java_Development_FOUND)
        include(UseJava)
        add_jar(tinysplinejar
          ${TINYSPLINE_JAVA_DIR}/tinysplinejava.java
          ${TINYSPLINE_JAVA_DIR}/tinysplinejavaJNI.java
          ${TINYSPLINE_JAVA_DIR}/Utils.java
          OUTPUT_NAME tinyspline
        )
        add_dependencies(tinysplinejar tinysplinejava)
        add_custom_command(TARGET tinysplinejar POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/tinyspline.jar"
          "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_JAVA_INTERFACE_FILE_NAME}"
        )
        set_target_properties(tinysplinejar PROPERTIES
          FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
        )
      endif(Java_Development_FOUND)

      set_target_properties(tinysplinejava PROPERTIES
        FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
        COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
      )
    endif(JNI_FOUND)
    
    # Ruby
    find_package(Ruby)
    if(RUBY_FOUND)
      include_directories(${RUBY_INCLUDE_DIRS})
      set_source_files_properties(tinysplineruby.i PROPERTIES CPLUSPLUS ON)
      set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
      set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_RUBY_DIR})
      swig_add_module(tinysplineruby ruby tinysplineruby.i ${TINYSPLINE_CXX_SOURCE_FILES})
      swig_link_libraries(tinysplineruby ${RUBY_LIBRARY})
      set_target_properties(tinysplineruby PROPERTIES
        FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
        COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
      )
    endif(RUBY_FOUND)
    
    # C#
    set_source_files_properties(tinysplinecsharp.i PROPERTIES CPLUSPLUS ON)
    set(CMAKE_SWIG_FLAGS -namespace TinySpline -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
    set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_CSHARP_DIR})
    swig_add_module(tinysplinecsharp csharp tinysplinecsharp.i ${TINYSPLINE_CXX_SOURCE_FILES})
    set_target_properties(tinysplinecsharp PROPERTIES
      FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
      COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
    )
    
    # C# DLL
    find_program(TINYSPLINE_CSHARP_COMPILER
      NAMES csc mcs dmcs gmcs
      PATHS "C:/Windows/Microsoft.NET/Framework/v3.5"
    )
    if(TINYSPLINE_CSHARP_COMPILER)
      if(TINYSPLINE_CSHARP_COMPILER MATCHES "csc$|csc.exe$")
        # csc neither supports the 'sdk' flag nor '/' in 'recursive'
        add_custom_command(TARGET tinysplinecsharp POST_BUILD
          COMMAND ${TINYSPLINE_CSHARP_COMPILER} /target:library /out:"${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_CSHARP_INTERFACE_FILE_NAME}" /recurse:"${TINYSPLINE_CSHARP_DIR}\\*.cs"
        )
      else()
        add_custom_command(TARGET tinysplinecsharp POST_BUILD
          COMMAND ${TINYSPLINE_CSHARP_COMPILER} -sdk:2 -target:library -out:"${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_CSHARP_INTERFACE_FILE_NAME}" -recurse:"${TINYSPLINE_CSHARP_DIR}/*.cs"
        )
      endif()
    endif()
    
    # Go (not fully supported yet)
    set_source_files_properties(tinysplinego.i PROPERTIES CPLUSPLUS ON)
    set(CMAKE_SWIG_FLAGS -intgosize 32 -package tinyspline -O)
    set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_GO_DIR})
    #swig_add_module(tinysplinego go tinysplinego.i ${TINYSPLINE_CXX_SOURCE_FILES})
    #add_custom_command(TARGET tinysplinego POST_BUILD
    #  COMMAND ${CMAKE_COMMAND} -E copy "${TINYSPLINE_GO_DIR}/tinysplinego.go"
    #  "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_GO_INTERFACE_FILE_NAME}"
    #)

    # PHP
    find_package(PHP)
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
        set_source_files_properties(tinysplinephp.i PROPERTIES CPLUSPLUS ON)
        set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
        set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_PHP_DIR})
        swig_add_module(tinysplinephp ${TINYSPLINE_PHP_VERSION} tinysplinephp.i ${TINYSPLINE_CXX_SOURCE_FILES})
        add_custom_command(TARGET tinysplinephp POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy "${TINYSPLINE_PHP_DIR}/tinysplinephp.php"
          "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_PHP_INTERFACE_FILE_NAME}"
        )
        set_target_properties(tinysplinephp PROPERTIES
          FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
          COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS} ${PHP_INCLUDE_DIRS}"
        )
      endif()
    endif(PHP_FOUND)

    # Lua
    find_package(Lua)
    if(LUA_FOUND)
      include_directories(${LUA_INCLUDE_DIR})
      set_source_files_properties(tinysplinelua.i PROPERTIES CPLUSPLUS ON)
      set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
      set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_LUA_DIR})
      swig_add_module(tinysplinelua lua tinysplinelua.i ${TINYSPLINE_CXX_SOURCE_FILES})
      swig_link_libraries(tinysplinelua ${LUA_LIBRARIES})
      set_target_properties(tinysplinelua PROPERTIES
        FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
        COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
      )
    endif(LUA_FOUND)

    # D
    set_source_files_properties(tinysplinedlang.i PROPERTIES CPLUSPLUS ON)
    set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES} -d2 -wrapperlibrary tinysplinedlang -module tinyspline)
    set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_DLANG_DIR})
    swig_add_module(tinysplinedlang d tinysplinedlang.i ${TINYSPLINE_CXX_SOURCE_FILES})
    add_custom_command(TARGET tinysplinedlang POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "${TINYSPLINE_DLANG_DIR}/${TINYSPLINE_DLANG_INTERFACE_FILE_NAME}"
      "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_DLANG_INTERFACE_FILE_NAME}"
      COMMAND ${CMAKE_COMMAND} -E copy "${TINYSPLINE_DLANG_DIR}/${TINYSPLINE_DLANG_INTERMEDIATE_FILE_NAME}"
      "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_DLANG_INTERMEDIATE_FILE_NAME}"
    )
    set_target_properties(tinysplinedlang PROPERTIES
      FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME}
      COMPILE_FLAGS "${TINYSPLINE_BINDING_CXX_FLAGS}"
    )
  endif(SWIG_FOUND)
endif(TINYSPLINE_CXX_AVAILABLE AND TARGET_SUPPORTS_SHARED_LIBS)



###############################################################################
### Print summary
###############################################################################
string(STRIP "${CMAKE_C_FLAGS} ${TINYSPLINE_LIBRARY_C_FLAGS}" TINYSPLINE_SUMMARY_C_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS} ${TINYSPLINE_LIBRARY_CXX_FLAGS}" TINYSPLINE_SUMMARY_CXX_FLAGS)
string(STRIP "${CMAKE_CXX_FLAGS} ${TINYSPLINE_BINDING_CXX_FLAGS}" TINYSPLINE_SUMMARY_BINDING_FLAGS)

message(STATUS
"
******************* TinySpline Configuration Summary *******************
Interface Configuration:
  With double precision  (default: OFF): ${TINYSPLINE_DOUBLE_PRECISION}
  Without C++11 features (default: OFF): ${TINYSPLINE_DISABLE_CXX11_FEATURES}

Compiler Configuration:
  Compiler:       ${CMAKE_CXX_COMPILER}
  C flags:        ${TINYSPLINE_SUMMARY_C_FLAGS}
  C++ flags:      ${TINYSPLINE_SUMMARY_CXX_FLAGS}
  Binding flags:  ${TINYSPLINE_SUMMARY_BINDING_FLAGS}
  Definitions:    ${TINYSPLINE_DEFINITIONS}
  Toolchain file: ${CMAKE_TOOLCHAIN_FILE}

Available Interfaces:
  C++:          ${TINYSPLINE_CXX_AVAILABLE}

  C\#:           TRUE
    Compiler: ${TINYSPLINE_CSHARP_COMPILER}

  D:            TRUE

  Java:		${JNI_FOUND}
    Version:  ${Java_VERSION_STRING}
    Compiler: ${Java_JAVAC_EXECUTABLE}
    Archive:  ${Java_JAR_EXECUTABLE}

  Lua:          ${LUA_FOUND}
    Version:  ${LUA_VERSION_STRING}
    Include:  ${LUA_INCLUDE_DIR}
    Library:  ${LUA_LIBRARIES}

  PHP:          ${PHP_FOUND}
    Version:  ${PHP_VERSION_STRING}
    Include:  ${PHP_INCLUDE_DIRS}

  Python:       ${PYTHONLIBS_FOUND}
    Version:  ${PYTHONLIBS_VERSION_STRING}
    Include:  ${PYTHON_INCLUDE_DIRS}
    Library:  ${PYTHON_LIBRARIES}

  Ruby:         ${RUBY_FOUND}
    Version:  ${RUBY_VERSION}
    Include:  ${RUBY_INCLUDE_DIRS}
    Library:  ${RUBY_LIBRARY}

Installation:
  Prefix: ${CMAKE_INSTALL_PREFIX}
")
