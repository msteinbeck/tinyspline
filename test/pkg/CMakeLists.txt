cmake_minimum_required(VERSION 3.4)
project(test) 

find_package(tinyspline)
find_package(tinysplinecxx)



# Check CMake variables exported by the C interface.
# Note that TINYSPLINE_DEFINITIONS may be empty.
if(NOT DEFINED TINYSPLINE_INCLUDE_DIRS)
	message(FATAL_ERROR "TINYSPLINE_INCLUDE_DIRS is not defined")
endif()
if(TINYSPLINE_INCLUDE_DIRS STREQUAL "")
	message(FATAL_ERROR "TINYSPLINE_INCLUDE_DIRS is empty")
endif()

if(NOT DEFINED TINYSPLINE_LIBRARY_DIRS)
	message(FATAL_ERROR "TINYSPLINE_LIBRARY_DIRS is not defined")
endif()
if(TINYSPLINE_LIBRARY_DIRS STREQUAL "")
	message(FATAL_ERROR "TINYSPLINE_LIBRARY_DIRS is empty")
endif()

if(NOT DEFINED TINYSPLINE_VERSION)
	message(FATAL_ERROR "TINYSPLINE_VERSION is not defined")
endif()
if(TINYSPLINE_VERSION STREQUAL "")
	message(FATAL_ERROR "TINYSPLINE_VERSION is empty")
endif()

if(NOT DEFINED TINYSPLINE_DEFINITIONS) # may be empty
	message(FATAL_ERROR "TINYSPLINE_DEFINITIONS is not defined")
endif()

if(NOT DEFINED TINYSPLINE_LIBRARIES)
	message(FATAL_ERROR "TINYSPLINE_LIBRARIES is not defined")
endif()
if(TINYSPLINE_LIBRARIES STREQUAL "")
	message(FATAL_ERROR "TINYSPLINE_LIBRARIES is empty")
endif()



# Check CMake variables exported by the C++ interface.
# Note that TINYSPLINECXX_DEFINITIONS may be empty.
if(NOT DEFINED TINYSPLINECXX_INCLUDE_DIRS)
	message(FATAL_ERROR "TINYSPLINECXX_INCLUDE_DIRS is not defined")
endif()
if(TINYSPLINECXX_INCLUDE_DIRS STREQUAL "")
	message(FATAL_ERROR "TINYSPLINECXX_INCLUDE_DIRS is empty")
endif()

if(NOT DEFINED TINYSPLINECXX_LIBRARY_DIRS)
	message(FATAL_ERROR "TINYSPLINECXX_LIBRARY_DIRS is not defined")
endif()
if(TINYSPLINECXX_LIBRARY_DIRS STREQUAL "")
	message(FATAL_ERROR "TINYSPLINECXX_LIBRARY_DIRS is empty")
endif()

if(NOT DEFINED TINYSPLINECXX_VERSION)
	message(FATAL_ERROR "TINYSPLINECXX_VERSION is not defined")
endif()
if(TINYSPLINECXX_VERSION STREQUAL "")
	message(FATAL_ERROR "TINYSPLINECXX_VERSION is empty")
endif()

if(NOT DEFINED TINYSPLINECXX_DEFINITIONS) # may be empty
	message(FATAL_ERROR "TINYSPLINECXX_DEFINITIONS is not defined")
endif()

if(NOT DEFINED TINYSPLINECXX_LIBRARIES)
	message(FATAL_ERROR "TINYSPLINECXX_LIBRARIES is not defined")
endif()
if(TINYSPLINECXX_LIBRARIES STREQUAL "")
	message(FATAL_ERROR "TINYSPLINECXX_LIBRARIES is empty")
endif()



# Check if libraries can be linked using CMake targets.
add_executable (testc_target test.c)
target_link_libraries(testc_target tinyspline::tinyspline)

add_executable (testcxx_target test.cxx)
target_link_libraries(testcxx_target tinysplinecxx::tinysplinecxx)



# Check if libraries can be linked using CMake variables.
add_executable (testc_variable test.c)
target_link_libraries(testc_variable ${TINYSPLINE_LIBRARIES})

add_executable (testcxx_variable test.cxx)
target_link_libraries(testcxx_variable ${TINYSPLINECXX_LIBRARIES})



# Add executables as tests.
enable_testing()
add_test(testc_target testc_target)
add_test(testcxx_target testcxx_target)
add_test(testc_variable testc_variable)
add_test(testcxx_variable testcxx_variable)
