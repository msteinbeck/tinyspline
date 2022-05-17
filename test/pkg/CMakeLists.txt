cmake_minimum_required(VERSION 3.12)
project(test)

find_package(tinyspline REQUIRED)
find_package(tinysplinecxx REQUIRED)

# Checks for each path listed in ${list_of_dirs} whether it exists and is a
# directory. Errors are reported with message(SEND_ERROR ...).
function(check_directories list_of_dirs)
  foreach(dir ${list_of_dirs})
    if(NOT EXISTS ${dir})
      message(SEND_ERROR "'${dir}' does not exist")
      return()
    endif()
    if(NOT IS_DIRECTORY ${dir})
      message(SEND_ERROR "'${dir}' is not a directory")
    endif()
  endforeach()
endfunction()

# Check the CMake targets/variables exported by the C interface. Note that
# TINYSPLINE_DEFINITIONS and TINYSPLINE_BINARY_DIRS may be empty.
if(NOT TARGET tinyspline::tinyspline)
  message(SEND_ERROR "tinyspline::tinyspline is not defined")
endif()

if(NOT DEFINED TINYSPLINE_INCLUDE_DIRS)
  message(SEND_ERROR "TINYSPLINE_INCLUDE_DIRS is not defined")
endif()
if(TINYSPLINE_INCLUDE_DIRS STREQUAL "")
  message(SEND_ERROR "TINYSPLINE_INCLUDE_DIRS is empty")
endif()
check_directories(${TINYSPLINE_INCLUDE_DIRS})

if(NOT DEFINED TINYSPLINE_LIBRARY_DIRS)
  message(SEND_ERROR "TINYSPLINE_LIBRARY_DIRS is not defined")
endif()
if(TINYSPLINE_LIBRARY_DIRS STREQUAL "")
  message(SEND_ERROR "TINYSPLINE_LIBRARY_DIRS is empty")
endif()
check_directories(${TINYSPLINE_LIBRARY_DIRS})

if(NOT DEFINED TINYSPLINE_BINARY_DIRS) # may be empty
  message(SEND_ERROR "TINYSPLINE_BINARY_DIRS is not defined")
endif()
# check_directories(${TINYSPLINE_BINARY_DIRS}) # may not exist

if(NOT DEFINED TINYSPLINE_VERSION)
  message(SEND_ERROR "TINYSPLINE_VERSION is not defined")
endif()
if(TINYSPLINE_VERSION STREQUAL "")
  message(SEND_ERROR "TINYSPLINE_VERSION is empty")
endif()

if(NOT DEFINED TINYSPLINE_DEFINITIONS) # may be empty
  message(SEND_ERROR "TINYSPLINE_DEFINITIONS is not defined")
endif()

if(NOT DEFINED TINYSPLINE_LIBRARIES)
  message(SEND_ERROR "TINYSPLINE_LIBRARIES is not defined")
endif()
if(TINYSPLINE_LIBRARIES STREQUAL "")
  message(SEND_ERROR "TINYSPLINE_LIBRARIES is empty")
endif()

# Check the CMake targets/variables exported by the C++ interface. Note that
# TINYSPLINECXX_DEFINITIONS TINYSPLINECXX_BINARY_DIRS may be empty.
if(NOT TARGET tinysplinecxx::tinysplinecxx)
  message(SEND_ERROR "tinysplinecxx::tinysplinecxx is not defined")
endif()

if(NOT DEFINED TINYSPLINECXX_INCLUDE_DIRS)
  message(SEND_ERROR "TINYSPLINECXX_INCLUDE_DIRS is not defined")
endif()
if(TINYSPLINECXX_INCLUDE_DIRS STREQUAL "")
  message(SEND_ERROR "TINYSPLINECXX_INCLUDE_DIRS is empty")
endif()
check_directories(${TINYSPLINECXX_INCLUDE_DIRS})

if(NOT DEFINED TINYSPLINECXX_LIBRARY_DIRS)
  message(SEND_ERROR "TINYSPLINECXX_LIBRARY_DIRS is not defined")
endif()
if(TINYSPLINECXX_LIBRARY_DIRS STREQUAL "")
  message(SEND_ERROR "TINYSPLINECXX_LIBRARY_DIRS is empty")
endif()
check_directories(${TINYSPLINECXX_LIBRARY_DIRS})

if(NOT DEFINED TINYSPLINECXX_BINARY_DIRS) # may be empty
  message(SEND_ERROR "TINYSPLINECXX_BINARY_DIRS is not defined")
endif()
# check_directories(${TINYSPLINECXX_BINARY_DIRS}) # may not exist

if(NOT DEFINED TINYSPLINECXX_VERSION)
  message(SEND_ERROR "TINYSPLINECXX_VERSION is not defined")
endif()
if(TINYSPLINECXX_VERSION STREQUAL "")
  message(SEND_ERROR "TINYSPLINECXX_VERSION is empty")
endif()

if(NOT DEFINED TINYSPLINECXX_DEFINITIONS) # may be empty
  message(SEND_ERROR "TINYSPLINECXX_DEFINITIONS is not defined")
endif()

if(NOT DEFINED TINYSPLINECXX_LIBRARIES)
  message(SEND_ERROR "TINYSPLINECXX_LIBRARIES is not defined")
endif()
if(TINYSPLINECXX_LIBRARIES STREQUAL "")
  message(SEND_ERROR "TINYSPLINECXX_LIBRARIES is empty")
endif()

# Check if libraries can be linked using CMake targets.
add_executable(testc_target test.c)
target_link_libraries(testc_target tinyspline::tinyspline)

add_executable(testcxx_target test.cxx)
target_link_libraries(testcxx_target tinysplinecxx::tinysplinecxx)

# Check if libraries can be linked using CMake variables.
add_executable(testc_variable test.c)
target_link_libraries(testc_variable ${TINYSPLINE_LIBRARIES})

add_executable(testcxx_variable test.cxx)
target_link_libraries(testcxx_variable ${TINYSPLINECXX_LIBRARIES})

# Add executables as tests.
enable_testing()
add_test(testc_target testc_target)
add_test(testcxx_target testcxx_target)
add_test(testc_variable testc_variable)
add_test(testcxx_variable testcxx_variable)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set_tests_properties(
    testc_target PROPERTIES ENVIRONMENT
                            "PATH=${TINYSPLINE_BINARY_DIRS};$ENV{PATH}"
  )
  set_tests_properties(
    testcxx_target PROPERTIES ENVIRONMENT
                              "PATH=${TINYSPLINECXX_BINARY_DIRS};$ENV{PATH}"
  )
  set_tests_properties(
    testc_variable PROPERTIES ENVIRONMENT
                              "PATH=${TINYSPLINE_BINARY_DIRS};$ENV{PATH}"
  )
  set_tests_properties(
    testcxx_variable PROPERTIES ENVIRONMENT
                                "PATH=${TINYSPLINECXX_BINARY_DIRS};$ENV{PATH}"
  )
endif()
