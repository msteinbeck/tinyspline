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

# create wrapper libs
find_package(SWIG)
if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR})

  find_package(JNI)
  if(JNI_FOUND)
    include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
    set_source_files_properties(tinysplinejava.i PROPERTIES CPLUSPLUS ON)
    set(CMAKE_SWIG_FLAGS -package "so.tinyspline")
    swig_add_module(tinysplinejava java tinysplinejava.i tinyspline.c tinysplinecpp.cpp)
  endif()
endif()

# make static lib of tinyspline available for examples
target_include_directories(tinyspline_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
