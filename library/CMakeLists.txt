###############################################################################
### Add different options to this script. The following options are available.
#
# TINYSPLINE_DOUBLE_PRECISION - default: OFF
#   Use double instead of single (float) precision.
#
# TINYSPLINE_DISABLE_CXX11_FEATURES - default: OFF
#   Disable C++11 specific features in C++ interface.
###############################################################################
# TINYSPLINE_DOUBLE_PRECISION
option(TINYSPLINE_DOUBLE_PRECISION "Build TinySpline with double precision" OFF)

# TINYSPLINE_DISABLE_CXX11_FEATURES
option(TINYSPLINE_DISABLE_CXX11_FEATURES "Build TinySpline without C++11 features" OFF)


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
# CMAKE_TOOLCHAIN_FILE
#   The CMake tool chain file for cross-compiling.
#
# SWIG_EXECUTABLE
#   The SWIG executable.
#
# PYTHON_LIBRARY and PYTHON_INCLUDE_DIR
#   The Python environment. Both variables must be available in order to set a
#   custom environment.
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

# CMAKE_TOOLCHAIN_FILE
if(DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
  message(STATUS "Using environment variable 'SWIG_EXECUTABLE'")
  set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

# SWIG_EXECUTABLE
if(DEFINED ENV{SWIG_EXECUTABLE})
  message(STATUS "Using environment variable 'SWIG_EXECUTABLE'")
  set(SWIG_EXECUTABLE $ENV{SWIG_EXECUTABLE})
endif()

# PYTHON_LIBRARY and PYTHON_INCLUDE_DIR
if(DEFINED ENV{PYTHON_LIBRARY} AND DEFINED ENV{PYTHON_INCLUDE_DIR})
  message(STATUS "Using environment variables 'PYTHON_LIBRARY' and 'PYTHON_INCLUDE_DIR'")
  set(PYTHON_LIBRARY $ENV{PYTHON_LIBRARY})
  set(PYTHON_INCLUDE_DIR $ENV{PYTHON_INCLUDE_DIR})
endif()



###############################################################################
### Set output directory to 'TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY'. Allows
### other build systems like Python's 'setuptools' and Java's 'maven' to make
### assumptions about the location of the resulting files.
#
# TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY
#   The location of the resulting files.
###############################################################################
# First for the generic no-config case (e.g. with MinGW)
set(TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY})
# Second, for multi-config builds (e.g. MSVC)
foreach(TINYSPLINE_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${TINYSPLINE_CONFIG} TINYSPLINE_CONFIG)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY})
endforeach()



###############################################################################
### Setup compiler suite. The following variables may be useful in the
### remainder of this script.
#
# CMAKE_SYSTEM_PROCESSOR
#   The name of the target system.
#
# TARGET_SUPPORTS_SHARED_LIBS
#   Does the target support shared libraries?
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

# TINYSPLINE_CXX_AVAILABLE
set(TINYSPLINE_CXX_AVAILABLE FALSE)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if(TINYSPLINE_DOUBLE_PRECISION)
    add_definitions("-DTINYSPLINE_DOUBLE_PRECISION")
  endif()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wfloat-equal -pedantic")
  # avr uses the 'inline' keyword in some header files which is not supported by c89
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "avr")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")
  endif()

  if(TINYSPLINE_DISABLE_CXX11_FEATURES OR
      # http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake
      CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.7)
    set(TINYSPLINE_CXX_AVAILABLE TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wfloat-equal")
    # fixes some warnings of source files generated by swig
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-qualifiers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
    if(TINYSPLINE_DISABLE_CXX11_FEATURES)
      add_definitions("-DTINYSPLINE_DISABLE_CXX11_FEATURES")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()    
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(TINYSPLINE_DOUBLE_PRECISION)
    add_definitions("-DTINYSPLINE_DOUBLE_PRECISION")
  endif()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wfloat-equal -pedantic -std=c89")

  if(TINYSPLINE_DISABLE_CXX11_FEATURES OR
      # http://stackoverflow.com/questions/10408849/how-can-i-use-c-11-features-in-clang
      CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.3 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 3.3)
    set(TINYSPLINE_CXX_AVAILABLE TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wfloat-equal")
    # fixes some warnings of source files generated by swig
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-qualifiers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
    if(TINYSPLINE_DISABLE_CXX11_FEATURES)
      add_definitions("-DTINYSPLINE_DISABLE_CXX11_FEATURES")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  if(TINYSPLINE_DOUBLE_PRECISION)
    add_definitions("/DTINYSPLINE_DOUBLE_PRECISION")
  endif()
  # used to automatically create a .def file with all the global symbols for a shared library
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall /WX /wd4820 /wd4711")

  if(TINYSPLINE_DISABLE_CXX11_FEATURES OR
      CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 18 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 18)
    set(TINYSPLINE_CXX_AVAILABLE TRUE)
    # using WX causes to many errors in generated source code
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /W4 /wd4702")
    if(TINYSPLINE_DISABLE_CXX11_FEATURES)
      add_definitions("-DTINYSPLINE_DISABLE_CXX11_FEATURES")
    endif()
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # TODO: Find appropriate settings for intel
  if(TINYSPLINE_DOUBLE_PRECISION)
    add_definitions("-DTINYSPLINE_DOUBLE_PRECISION")
  endif()
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 2014 OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 2014)
    set(TINYSPLINE_CXX_AVAILABLE TRUE)
    if(TINYSPLINE_DISABLE_CXX11_FEATURES)
      add_definitions("-DTINYSPLINE_DISABLE_CXX11_FEATURES")
    endif()
  endif()
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES "avr")
  # avr is missing some required headers
  set(TINYSPLINE_CXX_AVAILABLE FALSE)
endif()



###############################################################################
### Create C/C++ libraries. We explicitly set the prefix of the static
### libraries to 'lib' to prevent naming collisions when compiling with MSVC.
###############################################################################
# C library
if(TARGET_SUPPORTS_SHARED_LIBS)
  add_library(tinyspline_shared SHARED tinyspline.c)
  set_target_properties(tinyspline_shared PROPERTIES OUTPUT_NAME "tinyspline")
  install(TARGETS tinyspline_shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
endif()
add_library(tinyspline_static STATIC tinyspline.c)
set_target_properties(tinyspline_static PROPERTIES OUTPUT_NAME "tinyspline" PREFIX "lib")
install(TARGETS tinyspline_static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(FILES tinyspline.h
  DESTINATION include
)
target_include_directories(tinyspline_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# C++ library
if(TINYSPLINE_CXX_AVAILABLE)
  if(TARGET_SUPPORTS_SHARED_LIBS)
    add_library(tinysplinecpp_shared SHARED tinyspline.c tinysplinecpp.cpp)
    set_target_properties(tinysplinecpp_shared PROPERTIES OUTPUT_NAME "tinysplinecpp")
    install(TARGETS tinysplinecpp_shared
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
    )
  endif()
  add_library(tinysplinecpp_static STATIC tinyspline.c tinysplinecpp.cpp)
  set_target_properties(tinysplinecpp_static PROPERTIES OUTPUT_NAME "tinysplinecpp" PREFIX "lib")
  install(TARGETS tinysplinecpp_static
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
  install(FILES tinysplinecpp.h
    DESTINATION include
  )
  target_include_directories(tinysplinecpp_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
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
# PYTHONLIBS_FOUND
#   Python binding is available.
#
# TINYSPLINE_PYTHON_INTERFACE_FILE_NAME
#   Name of Python interface file.
#
# JNI_FOUND
#   Java source of Java binding is available.
#
# Java_Development_FOUND
#   Compiled jar archive of Java binding is available.
#
# TINYSPLINE_JAVA_INTERFACE_FILE_NAME
#   Name of Java interface file (jar archive).
#
# RUBY_FOUND
#   Ruby binding is available.
#
# TINYSPLINE_RUBY_INTERFACE_FILE_NAME
#   Name of Ruby interface file.
#
# TINYSPLINE_CSHARP_COMPILER
#   The used C# compiler. Indicates that the C# DLL of the C# binding is
#   available.
#
# TINYSPLINE_CSHARP_INTERFACE_FILE_NAME
#   Name of C# interface file (dll).
###############################################################################
if(TINYSPLINE_CXX_AVAILABLE AND TARGET_SUPPORTS_SHARED_LIBS)
  # Swig
  find_package(SWIG 3.0.1)
  if (SWIG_FOUND)
    include(${SWIG_USE_FILE})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})

    # TINYSPLINE_BINDINGS_FOLDER_NAME
    set(TINYSPLINE_BINDINGS_FOLDER_NAME "bindings")

    # Output directories of bindings
    set(TINYSPLINE_PYTHON_DIR "${CMAKE_CURRENT_BINARY_DIR}/python")
    set(TINYSPLINE_JAVA_DIR "${CMAKE_CURRENT_BINARY_DIR}/so/tinyspline")
    set(TINYSPLINE_RUBY_DIR "${CMAKE_CURRENT_BINARY_DIR}/ruby")
    set(TINYSPLINE_CSHARP_DIR "${CMAKE_CURRENT_BINARY_DIR}/csharp")
    set(TINYSPLINE_GO_DIR "${CMAKE_CURRENT_BINARY_DIR}/go")
    # Names of interface files
    set(TINYSPLINE_PYTHON_INTERFACE_FILE_NAME "tinyspline.py")
    set(TINYSPLINE_JAVA_INTERFACE_FILE_NAME "tinyspline.jar")
    set(TINYSPLINE_RUBY_INTERFACE_FILE_NAME "tinyspline.rb")
    set(TINYSPLINE_CSHARP_INTERFACE_FILE_NAME "TinySpline.dll")
    # Add additional clean targets
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES 
      "${TINYSPLINE_PYTHON_DIR};${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_PYTHON_INTERFACE_FILE_NAME};${CMAKE_CURRENT_BINARY_DIR}/so;${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_JAVA_INTERFACE_FILE_NAME};${TINYSPLINE_RUBY_DIR};${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_RUBY_INTERFACE_FILE_NAME};${TINYSPLINE_CSHARP_DIR};${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_CSHARP_INTERFACE_FILE_NAME};${TINYSPLINE_GO_DIR}"
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
    find_package(PythonLibs)
    if(PYTHONLIBS_FOUND)
      include_directories(${PYTHON_INCLUDE_DIRS})
      set_source_files_properties(tinysplinepython.i PROPERTIES CPLUSPLUS ON)
      if (${PYTHONLIBS_VERSION_STRING} MATCHES "^3.")
        set(TINYSPLINE_PYTHON_DEFINE "-py3")
      endif()
      set(CMAKE_SWIG_FLAGS ${TINYSPLINE_PYTHON_DEFINE} -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
      set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_PYTHON_DIR})
      swig_add_module(tinysplinepython python tinysplinepython.i tinyspline.c tinysplinecpp.cpp)
      swig_link_libraries(tinysplinepython ${PYTHON_LIBRARIES})
      # for some reason the generated make target for python is '_tinysplinepython'
      add_custom_command(TARGET _tinysplinepython POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${TINYSPLINE_PYTHON_DIR}/tinysplinepython.py"
        "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_PYTHON_INTERFACE_FILE_NAME}"
      )
      set_target_properties(_tinysplinepython PROPERTIES FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME})
    endif(PYTHONLIBS_FOUND)
    
    # Java
    find_package(JNI)
    if(JNI_FOUND)
      include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
      set_source_files_properties(tinysplinejava.i PROPERTIES CPLUSPLUS ON)
      set(CMAKE_SWIG_FLAGS -package "so.tinyspline" -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
      set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_JAVA_DIR})
      swig_add_module(tinysplinejava java tinysplinejava.i tinyspline.c tinysplinecpp.cpp)
      swig_link_libraries(tinysplinejava ${JAVA_LIBRARIES})
      
      # Jar archive
      find_package(Java COMPONENTS Development)
      if(Java_Development_FOUND)
        include(UseJava)
        add_jar(tinysplinejar
          ${TINYSPLINE_JAVA_DIR}/tinysplinejava.java
          ${TINYSPLINE_JAVA_DIR}/tinysplinejavaJNI.java
          OUTPUT_NAME tinyspline
        )
        add_dependencies(tinysplinejar tinysplinejava)
        add_custom_command(TARGET tinysplinejar POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/tinyspline.jar"
          "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_JAVA_INTERFACE_FILE_NAME}"
        )
        set_target_properties(tinysplinejar PROPERTIES FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME})
      endif(Java_Development_FOUND)

      set_target_properties(tinysplinejava PROPERTIES FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME})
    endif(JNI_FOUND)
    
    # Ruby
    find_package(Ruby)
    if(RUBY_FOUND)
      include_directories(${RUBY_INCLUDE_DIRS})
      set_source_files_properties(tinysplineruby.i PROPERTIES CPLUSPLUS ON)
      set(CMAKE_SWIG_FLAGS -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
      set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_RUBY_DIR})
      swig_add_module(tinysplineruby ruby tinysplineruby.i tinyspline.c tinysplinecpp.cpp)
      swig_link_libraries(tinysplineruby ${RUBY_LIBRARY})
      add_custom_command(TARGET tinysplineruby POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/tinyspline.rb"
        "${TINYSPLINE_LIBRARY_OUTPUT_DIRECTORY}/${TINYSPLINE_RUBY_INTERFACE_FILE_NAME}"
      )
      set_target_properties(tinysplineruby PROPERTIES FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME})
    endif(RUBY_FOUND)
    
    # C#
    set_source_files_properties(tinysplinecsharp.i PROPERTIES CPLUSPLUS ON)
    set(CMAKE_SWIG_FLAGS -namespace TinySpline -O ${TINYSPLINE_ADDITIONAL_SWIG_DEFINES})
    set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_CSHARP_DIR})
    swig_add_module(tinysplinecsharp csharp tinysplinecsharp.i tinyspline.c tinysplinecpp.cpp)
    set_target_properties(tinysplinecsharp PROPERTIES FOLDER ${TINYSPLINE_BINDINGS_FOLDER_NAME})
    
    # C# DLL
    find_program(TINYSPLINE_CSHARP_COMPILER
      NAMES csc mcs dmcs gmcs
      PATHS "C:/Windows/Microsoft.NET/Framework/v3.5"
    )
    if(TINYSPLINE_CSHARP_COMPILER)
      if(TINYSPLINE_CSHARP_COMPILER MATCHES "csc.exe$")
        # csc (Windows) neither supports the 'sdk' flag nor '/' in 'recursive'
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
    # set_source_files_properties(tinysplinego.i PROPERTIES CPLUSPLUS ON)
    # set(CMAKE_SWIG_FLAGS -intgosize 32 -package tinyspline -O)
    # set(CMAKE_SWIG_OUTDIR ${TINYSPLINE_GO_DIR})
    # swig_add_module(tinysplinego go tinysplinego.i tinyspline.c tinysplinecpp.cpp)
  endif(SWIG_FOUND)
endif(TINYSPLINE_CXX_AVAILABLE AND TARGET_SUPPORTS_SHARED_LIBS)
