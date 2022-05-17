# ##############################################################################
# Set up the compiler suite.
# ##############################################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ##############################################################################
# Build CuTest.
# ##############################################################################
add_library(cutest STATIC "cutest/CuTest.c")
target_include_directories(cutest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/cutest")
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  # Disable warnings regarding sprintf.
  target_compile_definitions(cutest PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# ##############################################################################
# Build testutils.
# ##############################################################################
add_library(testutils STATIC "testutils.c")
target_include_directories(testutils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(testutils PUBLIC cutest tinyspline)

# ##############################################################################
# Check if code coverage is available. Coverage is measured and visualized using
# gcov, lcov, and genthml. If one of these tools is missing, code coverage
# cannot be generated.
#
# TINYSPLINE_COVERAGE_AVAILABLE TRUE if the tools that are required to generate
# code coverage are available. FALSE otherwise.
#
# TINYSPLINE_GCOV gcov executable.
#
# TINYSPLINE_LCOV lcov executable.
#
# TINYSPLINE_GENHTML genhtml executable.
#
# TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY Root directory of genhtml output. Each
# target that generates code coverage should store its results in a separate
# subdirectory.
#
# TINYSPLINE_COVERAGE_C_FLAGS C flags for coverage executables.
#
# TINYSPLINE_COVERAGE_C_LIBRARIES C libraries for coverage executables.
#
# TINYSPLINE_COVERAGE_CXX_FLAGS C++ flags for coverage executables.
#
# TINYSPLINE_COVERAGE_CXX_LIBRARIES C++ libraries for coverage executables.
# ##############################################################################
# TINYSPLINE_COVERAGE_AVAILABLE
set(TINYSPLINE_COVERAGE_AVAILABLE FALSE)
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  message(STATUS "Code coverage needs GCC")
else()
  find_program(TINYSPLINE_GCOV gcov)
  find_program(TINYSPLINE_LCOV lcov)
  find_program(TINYSPLINE_GENHTML genhtml)
  if(NOT TINYSPLINE_GCOV
     OR NOT TINYSPLINE_LCOV
     OR NOT TINYSPLINE_GENHTML
  )
    message(STATUS "Code coverage requires gcov, lcov, and genhtml")
  else()
    set(TINYSPLINE_COVERAGE_AVAILABLE TRUE)
  endif()
endif()

# TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY
set(TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)
set_directory_properties(
  PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
             "${TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY}"
)

# TINYSPLINE_COVERAGE_C_FLAGS TINYSPLINE_COVERAGE_CXX_FLAGS
set(TINYSPLINE_COVERAGE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
set(TINYSPLINE_COVERAGE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

# TINYSPLINE_COVERAGE_C_LIBRARIES TINYSPLINE_COVERAGE_CXX_LIBRARIES
set(TINYSPLINE_COVERAGE_C_LIBRARIES "-lgcov")
set(TINYSPLINE_COVERAGE_CXX_LIBRARIES "-lgcov")

# ##############################################################################
# Add subdirectories containing unit tests.
# ##############################################################################
add_subdirectory(c)
add_subdirectory(cxx)

# ##############################################################################
# Subsume unit tests and code coverage via custom targets.
# ##############################################################################
add_custom_target(
  tests
  DEPENDS tinyspline_tests tinysplinecxx_tests
  COMMAND tinyspline_tests
  COMMAND tinysplinecxx_tests
)
add_custom_target(coverage DEPENDS tinyspline_coverage tinysplinecxx_coverage)

# ##############################################################################
# Add a custom target for checking memory issues with CTest.
# ##############################################################################
add_custom_target(
  memcheck
  DEPENDS tests
  COMMAND ${CMAKE_CTEST_COMMAND} -T memcheck
  COMMAND ${CMAKE_COMMAND} -E cat
          "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.1.log"
  COMMAND ${CMAKE_COMMAND} -E cat
          "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.2.log"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)
