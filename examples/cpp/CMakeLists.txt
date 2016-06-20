# create examples
if(TARGET tinysplinecpp_static)
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()

  add_executable(simple simple.cpp)
  target_link_libraries(simple LINK_PUBLIC tinysplinecpp_static)
  set_target_properties(simple PROPERTIES FOLDER "examples/cpp")
endif()
