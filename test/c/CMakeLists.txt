# ##############################################################################
# Create unit tests.
# ##############################################################################
file(GLOB_RECURSE TINYSPLINE_TESTS_SOURCE_FILES "*.c")
add_executable(tinyspline_tests ${TINYSPLINE_TESTS_SOURCE_FILES})
target_link_libraries(tinyspline_tests PRIVATE testutils)

if(EMSCRIPTEN)
  add_test(NAME tinyspline_tests COMMAND $ENV{EMSDK_NODE} tinyspline_tests)
else()
  add_test(tinyspline_tests tinyspline_tests)
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set_tests_properties(
    tinyspline_tests
    PROPERTIES ENVIRONMENT "PATH=${TINYSPLINE_OUTPUT_DIRECTORY};$ENV{PATH}"
  )
endif()

# ##############################################################################
# Create code coverage.
# ##############################################################################
if(TINYSPLINE_COVERAGE_AVAILABLE)
  add_executable(
    tinyspline_tests_coverage EXCLUDE_FROM_ALL ${TINYSPLINE_TESTS_SOURCE_FILES}
                                               ${TINYSPLINE_C_SOURCE_FILES}
  )
  target_include_directories(
    tinyspline_tests_coverage PRIVATE ${TINYSPLINE_C_INCLUDE_DIR}
  )
  set_target_properties(
    tinyspline_tests_coverage PROPERTIES COMPILE_FLAGS
                                         ${TINYSPLINE_COVERAGE_C_FLAGS}
  )
  target_link_libraries(
    tinyspline_tests_coverage
    PRIVATE testutils ${TINYSPLINE_COVERAGE_C_LIBRARIES}
            ${TINYSPLINE_C_LINK_LIBRARIES}
  )
  set(TINYSPLINE_COVERAGE_INFO_FILE coverage.info)
  add_custom_target(
    tinyspline_coverage
    COMMAND ${TINYSPLINE_LCOV} --directory . --zerocounters
    COMMAND tinyspline_tests_coverage
    COMMAND ${TINYSPLINE_LCOV} --rc lcov_branch_coverage=1 --directory .
            --capture --output-file ${TINYSPLINE_COVERAGE_INFO_FILE}
    COMMAND
      ${TINYSPLINE_LCOV} --rc lcov_branch_coverage=1 --extract
      ${TINYSPLINE_COVERAGE_INFO_FILE} '*tinyspline.c' --output-file
      ${TINYSPLINE_COVERAGE_INFO_FILE}
    COMMAND
      ${TINYSPLINE_GENHTML} --rc genhtml_branch_coverage=1 -o
      ${TINYSPLINE_COVERAGE_OUTPUT_DIRECTORY}/c ${TINYSPLINE_COVERAGE_INFO_FILE}
    COMMAND ${CMAKE_COMMAND} -E remove ${TINYSPLINE_COVERAGE_INFO_FILE}
    DEPENDS tinyspline_tests_coverage
  )
endif()
