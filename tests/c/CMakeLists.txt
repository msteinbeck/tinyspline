file(GLOB SRC "*.c")

add_executable(ctests ${SRC})
target_link_libraries(ctests
  LINK_PUBLIC tinyspline_static
  ${TINYSPLINE_LIBRARIES}
)

add_custom_target(run_tests
  COMMAND ctests
  DEPENDS ctests
)
