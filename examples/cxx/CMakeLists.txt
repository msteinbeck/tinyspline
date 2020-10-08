###############################################################################
### Create examples.
###############################################################################
add_executable(quickstart quickstart.cpp)
target_link_libraries(quickstart
        LINK_PUBLIC tinysplinecxx
        ${TINYSPLINE_CXX_STATIC_LIBRARIES})
set_target_properties(quickstart PROPERTIES FOLDER "examples/cpp")

add_executable(time_series time_series.cpp)
target_link_libraries(time_series
        LINK_PUBLIC tinysplinecxx
        ${TINYSPLINE_CXX_STATIC_LIBRARIES})
set_target_properties(time_series PROPERTIES FOLDER "examples/cpp")
