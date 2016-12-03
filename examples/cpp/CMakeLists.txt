###############################################################################
### Add required compiler/platform specific CXX flags.
###############################################################################
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  if(NOT TINYSPLINE_DISABLE_CXX11_FEATURES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
endif()

###############################################################################
### Create examples.
###############################################################################
if(TARGET tinysplinecpp_static)
  add_executable(quickstart quickstart.cpp)
  target_link_libraries(quickstart LINK_PUBLIC tinysplinecpp_static)
  set_target_properties(quickstart PROPERTIES FOLDER "examples/cpp")
endif()
