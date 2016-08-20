###############################################################################
### Map supported environment variables to CMake variables. The following
### variables will be processed.
#
# TINYSPLINE_DOUBLE_PRECISION
#   Use double instead of single (float) precision.
###############################################################################
if(DEFINED ENV{TINYSPLINE_DOUBLE_PRECISION})
  message(STATUS "Using environment variable 'TINYSPLINE_DOUBLE_PRECISION'")
  set(TINYSPLINE_DOUBLE_PRECISION $ENV{TINYSPLINE_DOUBLE_PRECISION})
endif()



###############################################################################
### Set output directory to 'TINYSPLINE_BINARY_OUTPUT_DIRECTORY'.
#
# TINYSPLINE_BINARY_OUTPUT_DIRECTORY
#   The location of the resulting files.
###############################################################################
# First for the generic no-config case (e.g. with MinGW)
set(TINYSPLINE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TINYSPLINE_BINARY_OUTPUT_DIRECTORY})
# Second, for multi-config builds (e.g. MSVC)
foreach(TINYSPLINE_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${TINYSPLINE_CONFIG} TINYSPLINE_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${TINYSPLINE_CONFIG} ${TINYSPLINE_BINARY_OUTPUT_DIRECTORY})
endforeach()



###############################################################################
### Setup compiler suite.
###############################################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  if(TINYSPLINE_DOUBLE_PRECISION)
    add_definitions("/DTINYSPLINE_DOUBLE_PRECISION")
  endif()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  if(TINYSPLINE_DOUBLE_PRECISION)
    add_definitions("-DTINYSPLINE_DOUBLE_PRECISION")
  endif()
endif()

add_subdirectory(c)
add_subdirectory(cpp)
