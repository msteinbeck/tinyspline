###############################################################################
### Check if code coverage is available. Coverage is measured and visualized
### using gcov, lcov, and genthml.
#
# TINYSPLINE_COVERAGE_AVAILABLE
#   TRUE if the tools that are required to generate code coverage are
#   available. FALSE otherwise.
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
#   Root directory of genhtml output. Each target that generates code coverage
#   should store its results in a separate subdirectory.
#
# TINYSPLINE_COVERAGE_C_FLAGS
#   C flags for coverage executables.
#
# TINYSPLINE_COVERAGE_C_LIBRARIES
#   C libraries for coverage executables.
###############################################################################
# TINYSPLINE_COVERAGE_AVAILABLE
set(TINYSPLINE_COVERAGE_AVAILABLE FALSE)
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	message(STATUS "Code coverage requires GCC")
else()
	find_program(TINYSPLINE_GCOV gcov)
	find_program(TINYSPLINE_LCOV lcov)
	find_program(TINYSPLINE_GENHTML genhtml)
	if(NOT TINYSPLINE_GCOV
			OR NOT TINYSPLINE_LCOV
			OR NOT TINYSPLINE_GENHTML)
		message(STATUS "Code coverage requires gcov, lcov, and genhtml")
	else()
		set(TINYSPLINE_COVERAGE_AVAILABLE TRUE)
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
