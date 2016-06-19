# create examples
if(TS_CPP_AVAILABLE)
  add_executable(simple simple.cpp)
  target_link_libraries(simple LINK_PUBLIC tinysplinecpp_static)
  set_target_properties(simple PROPERTIES FOLDER "examples/cpp")
endif()
