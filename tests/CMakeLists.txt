###############################################################################
### Check if code coverage is availabe. Code coverage is measured and depicted
### using gcov, lcov, and genthml. The following CMake variables may be useful
### for targets creating code coverage.
#
# TINYSPLINE_COVERAGE_AVAILABLE
#   TRUE if tools required to generate coverage are available, FALSE otherwise.
#
# TINYSPLINE_GCOV
#   gcov executable.
#
# TINYSPLINE_LCOV
#   lcov executable.
#
# TINYSPLINE_GENHTML
#   genhtml executable.
#
# TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY
#   Root directory of genhtml coverage output. Each target generating code
#   coverage should store results in a separate subdirectory.
#
# TINYSPLINE_COVERAGE_C_FLAGS
#   C flags for coverage executables.
#
# TINYSPLINE_COVERAGE_C_LIBRARIES
#   C libraries for coverage executables.
###############################################################################
# TINYSPLINE_COVERAGE_AVAILABLE
set(TINYSPLINE_COVERAGE_AVAILABLE FALSE)
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	message(STATUS "Code coverage is only available for \"Debug\" builds")
else()
	if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
			NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		message(STATUS "Code coverage is not available for ${CMAKE_CXX_COMPILER_ID}")
	else()
		if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND
				CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3)
			message(STATUS "Code coverage requires Clang 3.0.0 or later")
		else()
			find_program(TINYSPLINE_GCOV gcov)
			find_program(TINYSPLINE_LCOV lcov)
			find_program(TINYSPLINE_GENHTML genhtml)
			if(NOT TINYSPLINE_GCOV OR NOT TINYSPLINE_LCOV OR NOT TINYSPLINE_GENHTML)
				message(STATUS "Code coverage requires gcov, lcov, and genhtml")
			else()
				set(TINYSPLINE_COVERAGE_AVAILABLE TRUE)
			endif()
		endif()
	endif()
endif()

# TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY
set(TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
	"${TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY}"
)

# TINYSPLINE_COVERAGE_C_FLAGS
set(TINYSPLINE_COVERAGE_C_FLAGS
	"-g -O0 -fprofile-arcs -ftest-coverage"
)

# TINYSPLINE_COVERAGE_C_LIBRARIES
set(TINYSPLINE_COVERAGE_C_LIBRARIES
	"-lgcov"
)

###############################################################################
### Add subdirectories containing the actual unit tests.
###############################################################################
add_subdirectory(c)

###############################################################################
### Add custom targets that are supposed to subsume unit tests and coverage.
###############################################################################
add_custom_target(tests
	COMMAND tinyspline_tests
	DEPENDS tinyspline_tests
)
add_custom_target(coverage
	DEPENDS tinyspline_coverage
)
