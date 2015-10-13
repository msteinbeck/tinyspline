cmake_minimum_required(VERSION 2.8)

# create c libs
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wfloat-equal -ansi")
add_library(tinyspline_shared SHARED tinyspline.c)
set_target_properties(tinyspline_shared PROPERTIES OUTPUT_NAME "tinyspline")
add_library(tinyspline_static STATIC tinyspline.c)
set_target_properties(tinyspline_static PROPERTIES OUTPUT_NAME "tinyspline")
install(TARGETS tinyspline_shared tinyspline_static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(FILES tinyspline.h
  DESTINATION include
)

# create c++ libs
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wfloat-equal -std=c++11")
  add_library(tinysplinecpp_shared SHARED tinyspline.c tinysplinecpp.cpp)
  set_target_properties(tinysplinecpp_shared PROPERTIES OUTPUT_NAME "tinysplinecpp")
  add_library(tinysplinecpp_static STATIC tinyspline.c tinysplinecpp.cpp)
  set_target_properties(tinysplinecpp_static PROPERTIES OUTPUT_NAME "tinysplinecpp")
  install(TARGETS tinysplinecpp_shared tinysplinecpp_static
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
  install(FILES tinysplinecpp.h
    DESTINATION include
  )
endif()

# create proxies
find_package(SWIG 3.0.1)
if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  # http://www.swig.org/Doc1.3/Java.html#compiling_dynamic
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")
  # define the output dirs
  set(TS_GENERATED_PYTHON_DIR ${CMAKE_CURRENT_BINARY_DIR}/python)
  set(TS_GENERATED_JAVA_DIR ${CMAKE_CURRENT_BINARY_DIR}/so/tinyspline)
  set(TS_GENERATED_CSHARP_DIR ${CMAKE_CURRENT_BINARY_DIR}/csharp)

  # python
  find_package(PythonLibs)
  if (PYTHONLIBS_FOUND)
    include_directories(${PYTHON_INCLUDE_PATH})
    set_source_files_properties(tinysplinepython.i PROPERTIES CPLUSPLUS ON)
    if (${PYTHONLIBS_VERSION_STRING} MATCHES "^3.")
      message(STATUS "Using Python 3 mode.")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
      set(CMAKE_SWIG_FLAGS -py3 -O)
    else()
      message(STATUS "Using Python 2 mode.")
      set(CMAKE_SWIG_FLAGS -O)
    endif()
    set(CMAKE_SWIG_OUTDIR ${TS_GENERATED_PYTHON_DIR})
    swig_add_module(tinysplinepython python tinysplinepython.i tinyspline.c tinysplinecpp.cpp)
    swig_link_libraries(tinysplinepython ${PYTHON_LIBRARIES})
    # for some reason the generated make target for python is _tinysplinepython
    add_custom_command(TARGET _tinysplinepython POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${TS_GENERATED_PYTHON_DIR}/tinysplinepython.py
      ${CMAKE_CURRENT_BINARY_DIR}/tinyspline.py
    )
    install(TARGETS _tinysplinepython
      LIBRARY DESTINATION lib
    )
  endif()

  # java
  find_package(JNI)
  if(JNI_FOUND)
    include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
    set_source_files_properties(tinysplinejava.i PROPERTIES CPLUSPLUS ON)
    set(CMAKE_SWIG_FLAGS -package "so.tinyspline" -O)
    set(CMAKE_SWIG_OUTDIR ${TS_GENERATED_JAVA_DIR})
    swig_add_module(tinysplinejava java tinysplinejava.i tinyspline.c tinysplinecpp.cpp)
    swig_link_libraries(tinysplinejava ${JAVA_LIBRARIES})
    install(TARGETS tinysplinejava
      LIBRARY DESTINATION lib
    )
    
    find_package(Java COMPONENTS Development)
    if(Java_Development_FOUND)
      include(UseJava)
      add_jar(tinysplinejar 
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_p_float.java
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_p_size_t.java
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_p_tsBSpline.java
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_p_tsDeBoorNet.java
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_tsError.java
        ${TS_GENERATED_JAVA_DIR}/tinysplinejava.java
        ${TS_GENERATED_JAVA_DIR}/tinysplinejavaJNI.java
        ${TS_GENERATED_JAVA_DIR}/TsBSpline.java
        ${TS_GENERATED_JAVA_DIR}/tsBSplineType.java
        ${TS_GENERATED_JAVA_DIR}/TsDeBoorNet.java
        ${TS_GENERATED_JAVA_DIR}/TsFloatList.java
        OUTPUT_NAME tinyspline
      )
      add_dependencies(tinysplinejar tinysplinejava)
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tinyspline.jar
        DESTINATION share/java
      )
    endif()
  endif()

  # csharp
  set_source_files_properties(tinysplinecsharp.i PROPERTIES CPLUSPLUS ON)
  set(CMAKE_SWIG_FLAGS -O)
  set(CMAKE_SWIG_OUTDIR ${TS_GENERATED_CSHARP_DIR})
  swig_add_module(tinysplinecsharp csharp tinysplinecsharp.i tinyspline.c tinysplinecpp.cpp)
  install(TARGETS tinysplinecsharp
    LIBRARY DESTINATION lib
  )
  find_program(CSHARP_COMPILER
    NAMES csc mcs dmcs gmcs
    PATHS "C:/Windows/Microsoft.NET/Framework/v3.5/bin"
  )
  if(NOT CSHARP_COMPILER)
    message(STATUS "Could NOT find any suitable C# compiler.")
  else()
    message(STATUS "Using ${CSHARP_COMPILER} to compile C# sources.")
    # append 'cs' to assembly name to ensure it doesn't clash
    # with the c library.
    add_custom_command(TARGET tinysplinecsharp POST_BUILD
      COMMAND ${CSHARP_COMPILER} /t:library /sdk:2 /recurse:"${TS_GENERATED_CSHARP_DIR}/*.cs" /out:tinysplinecs.dll
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tinysplinecs.dll
      DESTINATION lib/mono/2.0
    )
  endif()

  # add additional clean targets
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES 
    "${TS_GENERATED_PYTHON_DIR};${CMAKE_CURRENT_BINARY_DIR}/tinyspline.py;${CMAKE_CURRENT_BINARY_DIR}/so;${TS_GENERATED_CSHARP_DIR};${CMAKE_CURRENT_BINARY_DIR}/tinysplinecs.dll"
  )
endif()

# make static lib of tinyspline available for examples
target_include_directories(tinyspline_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
