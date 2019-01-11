file(REMOVE "${CMAKE_BINARY_DIR}/setup.py")
if(${TINYSPLINE_WITH_PYTHON})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pkg/setup.py.in" "${CMAKE_BINARY_DIR}/setup.py" @ONLY)
endif()

file(REMOVE "${CMAKE_BINARY_DIR}/pom.xml")
if(${TINYSPLINE_WITH_JAVA})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pkg/pom.xml.in" "${CMAKE_BINARY_DIR}/pom.xml" @ONLY)
endif()

file(REMOVE "${CMAKE_BINARY_DIR}/tinyspline-${TINYSPLINE_VERSION}-1.rockspec")
if(${TINYSPLINE_WITH_LUA})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pkg/tinyspline.rockspec.in" "${CMAKE_BINARY_DIR}/tinyspline-${TINYSPLINE_VERSION}-1.rockspec" @ONLY)
endif()

file(REMOVE "${CMAKE_BINARY_DIR}/tinyspline.gemspec")
if(${TINYSPLINE_WITH_RUBY})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pkg/tinyspline.gemspec.in" "${CMAKE_BINARY_DIR}/tinyspline.${TINYSPLINE_PLATFORM}.gemspec" @ONLY)
endif()
