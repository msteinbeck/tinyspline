add_executable(quickstart quickstart.cpp)
target_link_libraries(quickstart PRIVATE tinysplinecxx)
set_target_properties(quickstart PROPERTIES FOLDER "examples/cpp")

add_executable(time_series time_series.cpp)
target_link_libraries(time_series PRIVATE tinysplinecxx)
set_target_properties(time_series PROPERTIES FOLDER "examples/cpp")

add_subdirectory(fltk)
