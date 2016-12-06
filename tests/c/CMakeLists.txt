file(GLOB SRC "*.c")

add_executable(ctests EXCLUDE_FROM_ALL ${SRC})
target_link_libraries(ctests LINK_PUBLIC tinyspline_static)

add_custom_command(TARGET ctests
  POST_BUILD
  COMMAND ctests
)
