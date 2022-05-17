cmake_minimum_required(VERSION 3.12)
project(tinyspline)

# Turn on folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(TINYSPLINE_BUILD_EXAMPLES "Build TinySpline examples." ON)
option(TINYSPLINE_BUILD_TESTS "Build TinySpline tests." ON)
option(TINYSPLINE_BUILD_DOCS "Build TinySpline documentation." ON)

if(NOT DEFINED TINYSPLINE_OUTPUT_DIRECTORY)
  set(TINYSPLINE_OUTPUT_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}/lib
      CACHE INTERNAL ""
  )
endif()
add_subdirectory(src)
if(TINYSPLINE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
if(TINYSPLINE_BUILD_TESTS)
  include(CTest) # Must be included in top-level CMakeLists.txt.
  add_subdirectory(test)
endif()
if(TINYSPLINE_BUILD_DOCS)
  add_subdirectory(docs)
endif()
