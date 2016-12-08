if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  # disable sprintf warning occurring in CuTest
  add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()

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
