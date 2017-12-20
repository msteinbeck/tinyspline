###############################################################################
### Create examples.
###############################################################################
if(TARGET tinysplinecpp_static)
	add_executable(quickstart quickstart.cpp)
	target_link_libraries(quickstart LINK_PUBLIC tinysplinecpp_static)
	set_target_properties(quickstart PROPERTIES FOLDER "examples/cpp")
endif()
