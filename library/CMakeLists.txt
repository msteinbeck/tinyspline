cmake_minimum_required(VERSION 2.8)

# create c libs
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wfloat-equal")
add_library(tinyspline SHARED tinyspline.c)
add_library(tinyspline_static STATIC tinyspline.c)

# create c++ libs
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wfloat-equal -std=c++11")
  add_library(tinysplinecpp SHARED tinyspline.c tinysplinecpp.cpp)
  add_library(tinysplinecpp_static STATIC tinyspline.c tinysplinecpp.cpp)
endif()

# create proxies
find_package(SWIG)
if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  # http://www.swig.org/Doc1.3/Java.html#compiling_dynamic
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")

  find_package(JNI)
  if(JNI_FOUND)
    include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
    set_source_files_properties(tinysplinejava.i PROPERTIES CPLUSPLUS ON)
    set(CMAKE_SWIG_FLAGS -package "so.tinyspline" -O)
    set(TS_GENERATED_JAVA_DIR ${CMAKE_CURRENT_BINARY_DIR}/so/tinyspline)
    set(CMAKE_SWIG_OUTDIR ${TS_GENERATED_JAVA_DIR})
    swig_add_module(tinysplinejava java tinysplinejava.i tinyspline.c tinysplinecpp.cpp)
    swig_link_libraries(tinysplinejava ${PYTHON_LIBRARIES})
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/so")
    
    find_package(Java COMPONENTS Development)
    if(Java_Development_FOUND)
      include(UseJava)
      add_jar(tinysplinejar 
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_p_float.java
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_p_size_t.java
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_p_tsBSpline.java
        ${TS_GENERATED_JAVA_DIR}/SWIGTYPE_p_tsDeBoorNet.java
        ${TS_GENERATED_JAVA_DIR}/tinysplinejava.java
        ${TS_GENERATED_JAVA_DIR}/tinysplinejavaJNI.java
        ${TS_GENERATED_JAVA_DIR}/TsBSpline.java
        ${TS_GENERATED_JAVA_DIR}/tsBSplineType.java
        ${TS_GENERATED_JAVA_DIR}/TsDeBoorNet.java
        ${TS_GENERATED_JAVA_DIR}/tsError.java
        ${TS_GENERATED_JAVA_DIR}/TsFloatList.java
      )
      add_dependencies(tinysplinejar tinysplinejava)
    endif()
  endif()
endif()

# make static lib of tinyspline available for examples
target_include_directories(tinyspline_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
