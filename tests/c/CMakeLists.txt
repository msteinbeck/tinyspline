###############################################################################
### Setup compiler suite.
###############################################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	# disable sprintf warning occurring in CuTest
	add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()

###############################################################################
### Create unit tests.
###############################################################################
file(GLOB TINYSPLINE_TESTS_SOURCE_FILES "*.c")
add_executable(tinyspline_tests ${TINYSPLINE_TESTS_SOURCE_FILES})
target_link_libraries(tinyspline_tests
	LINK_PUBLIC tinyspline_static
	${TINYSPLINE_LIBRARIES}
)

###############################################################################
### Create code coverage.
###############################################################################
if(TINYSPLINE_COVERAGE_AVAILABLE)
	include_directories(${TINYSPLINE_INCLUDE_DIRS})
	add_executable(tinyspline_tests_coverage
		EXCLUDE_FROM_ALL
		${TINYSPLINE_TESTS_SOURCE_FILES}
		${TINYSPLINE_C_SOURCE_FILES}
	)
	set_target_properties(tinyspline_tests_coverage PROPERTIES
		COMPILE_FLAGS ${TINYSPLINE_COVERAGE_C_FLAGS}
	)
	target_link_libraries(tinyspline_tests_coverage
		${TINYSPLINE_LIBRARIES}
		${TINYSPLINE_COVERAGE_C_LIBRARIES}
	)

	foreach(TINYSPLINE_C_SOURCE_FILE ${TINYSPLINE_C_SOURCE_FILES})
		set(TINYSPLINE_COVERAGE_FILE_PATTERN
			"${TINYSPLINE_COVERAGE_FILE_PATTERN} '${TINYSPLINE_C_SOURCE_FILES}'"
		)
	endforeach()
	string(STRIP "${TINYSPLINE_COVERAGE_FILE_PATTERN}"
		TINYSPLINE_COVERAGE_FILE_PATTERN
	)
	set(TINYSPLINE_COVERAGE_INFO_FILE coverage.info)
	add_custom_target(tinyspline_coverage
		COMMAND ${TINYSPLINE_LCOV} --directory . --zerocounters
		COMMAND tinyspline_tests_coverage
		COMMAND ${TINYSPLINE_LCOV} --rc lcov_branch_coverage=1 --directory . --capture --output-file ${TINYSPLINE_COVERAGE_INFO_FILE}
		COMMAND ${TINYSPLINE_LCOV} --rc lcov_branch_coverage=1 --extract coverage.info ${TINYSPLINE_COVERAGE_FILE_PATTERN} --output-file ${TINYSPLINE_COVERAGE_INFO_FILE}
		COMMAND ${TINYSPLINE_GENHTML} -o ${TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY}/c ${TINYSPLINE_COVERAGE_INFO_FILE}
		COMMAND ${CMAKE_COMMAND} -E remove ${TINYSPLINE_COVERAGE_INFO_FILE}
		DEPENDS tinyspline_tests_coverage
	)
endif()
