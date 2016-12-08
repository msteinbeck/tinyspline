if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  # disable sprintf warning occurring in CuTest
  add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()

file(GLOB SRC "*.c")

add_executable(tinyspline_tests ${SRC})
target_link_libraries(tinyspline_tests
  LINK_PUBLIC tinyspline_static
  ${TINYSPLINE_LIBRARIES}
)
