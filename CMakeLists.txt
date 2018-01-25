cmake_minimum_required(VERSION 3.4)
project(tinyspline)

# Turn on folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Exports TINYSPLINE_DEFINITIONS containing several preprocessor defines that
# are required by the remaining modules
add_subdirectory(src)
add_definitions("${TINYSPLINE_DEFINITIONS}")

add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(docs)


file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
if(${TINYSPLINE_WITH_PYTHON})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/toolchain/setup.py.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.py" @ONLY)
endif()

file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/pom.xml")
if(${TINYSPLINE_WITH_JAVA})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/toolchain/pom.xml.in" "${CMAKE_CURRENT_BINARY_DIR}/pom.xml" @ONLY)
endif()

file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/tinyspline-${TINYSPLINE_VERSION}-1.rockspec")
if(${TINYSPLINE_WITH_LUA})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/toolchain/tinyspline.rockspec.in" "${CMAKE_CURRENT_BINARY_DIR}/tinyspline-${TINYSPLINE_VERSION}-1.rockspec" @ONLY)
endif()
