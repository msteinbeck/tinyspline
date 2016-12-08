add_subdirectory(c)

add_custom_target(run_tests
  COMMAND tinyspline_tests
  DEPENDS tinyspline_tests
)
