if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  # disable sprintf warning occurring in CuTest
  add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()

file(GLOB TINYSPLINE_TESTS_SOURCE_FILES "*.c")
add_executable(tinyspline_tests ${TINYSPLINE_TESTS_SOURCE_FILES})
target_link_libraries(tinyspline_tests
  LINK_PUBLIC tinyspline_static
  ${TINYSPLINE_LIBRARIES}
)

include_directories(${TINYSPLINE_INCLUDE_DIRS})
list(APPEND TINYSPLINE_COVERAGE_SOURCE_FILES ${TINYSPLINE_TESTS_SOURCE_FILES})
list(APPEND TINYSPLINE_COVERAGE_SOURCE_FILES ${TINYSPLINE_C_SOURCE_FILES})
add_executable(tinyspline_tests_coverage ${TINYSPLINE_COVERAGE_SOURCE_FILES})
target_link_libraries(tinyspline_tests_coverage ${TINYSPLINE_LIBRARIES})
