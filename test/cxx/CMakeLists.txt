###############################################################################
### Create unit tests.
###############################################################################
file(GLOB_RECURSE TINYSPLINE_CXX_TESTS_SOURCE_FILES "*.cpp")
add_executable(tinysplinecxx_tests ${TINYSPLINE_CXX_TESTS_SOURCE_FILES})
target_link_libraries(tinysplinecxx_tests PRIVATE testutils tinysplinecxx)

if(EMSCRIPTEN)
	add_test(NAME tinysplinecxx_tests
		COMMAND $ENV{EMSDK_NODE} tinysplinecxx_tests)
else()
	add_test(tinysplinecxx_tests tinysplinecxx_tests)
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set_tests_properties(tinysplinecxx_tests PROPERTIES
		ENVIRONMENT "PATH=${TINYSPLINE_OUTPUT_DIRECTORY};$ENV{PATH}")
endif()
