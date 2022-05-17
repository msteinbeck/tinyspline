# ##############################################################################
# C++ extension of `testutils'.
# ##############################################################################
add_subdirectory(testutilscxx)

# ##############################################################################
# Create unit tests.
# ##############################################################################
file(GLOB_RECURSE TINYSPLINE_CXX_TESTS_SOURCE_FILES "*.cxx")
add_executable(tinysplinecxx_tests ${TINYSPLINE_CXX_TESTS_SOURCE_FILES})
target_link_libraries(tinysplinecxx_tests PRIVATE testutilscxx)

if(EMSCRIPTEN)
  add_test(NAME tinysplinecxx_tests COMMAND $ENV{EMSDK_NODE}
                                            tinysplinecxx_tests
  )
else()
  add_test(tinysplinecxx_tests tinysplinecxx_tests)
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set_tests_properties(
    tinysplinecxx_tests
    PROPERTIES ENVIRONMENT "PATH=${TINYSPLINE_OUTPUT_DIRECTORY};$ENV{PATH}"
  )
endif()

# ##############################################################################
# Create code coverage.
# ##############################################################################
if(TINYSPLINE_COVERAGE_AVAILABLE)
  add_executable(
    tinysplinecxx_tests_coverage EXCLUDE_FROM_ALL
    ${TINYSPLINE_CXX_TESTS_SOURCE_FILES} ${TINYSPLINE_CXX_SOURCE_FILES}
  )
  target_include_directories(
    tinysplinecxx_tests_coverage PRIVATE ${TINYSPLINE_CXX_INCLUDE_DIR}
  )
  set_target_properties(
    tinysplinecxx_tests_coverage PROPERTIES COMPILE_FLAGS
                                            ${TINYSPLINE_COVERAGE_CXX_FLAGS}
  )
  target_link_libraries(
    tinysplinecxx_tests_coverage
    PRIVATE testutilscxx ${TINYSPLINE_COVERAGE_CXX_LIBRARIES}
            ${TINYSPLINE_CXX_LINK_LIBRARIES}
  )
  set(TINYSPLINE_CXX_COVERAGE_INFO_FILE coverage.info)
  add_custom_target(
    tinysplinecxx_coverage
    COMMAND ${TINYSPLINE_LCOV} --directory . --zerocounters
    COMMAND tinysplinecxx_tests_coverage
    COMMAND ${TINYSPLINE_LCOV} --rc lcov_branch_coverage=1 --directory .
            --capture --output-file ${TINYSPLINE_CXX_COVERAGE_INFO_FILE}
    COMMAND
      ${TINYSPLINE_LCOV} --rc lcov_branch_coverage=1 --extract
      ${TINYSPLINE_CXX_COVERAGE_INFO_FILE} '*tinysplinecxx.cxx' --output-file
      ${TINYSPLINE_CXX_COVERAGE_INFO_FILE}
    COMMAND
      ${TINYSPLINE_GENHTML} --rc genhtml_branch_coverage=1 -o
      ${TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY}/cxx
      ${TINYSPLINE_CXX_COVERAGE_INFO_FILE}
    COMMAND ${CMAKE_COMMAND} -E remove ${TINYSPLINE_CXX_COVERAGE_INFO_FILE}
    DEPENDS tinysplinecxx_tests_coverage
  )
endif()
