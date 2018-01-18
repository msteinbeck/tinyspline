cmake_minimum_required(VERSION 3.4)
project(tinyspline)

# Turn on folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Exports TINYSPLINE_DEFINITIONS containing several preprocessor defines that
# are required by the remaining modules
add_subdirectory(library)
add_definitions("${TINYSPLINE_DEFINITIONS}")

add_subdirectory(examples)
add_subdirectory(tests)

find_package(Doxygen)
IF(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen/doxygen.conf.in ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf @ONLY)
	add_custom_target(docs ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doxygen/)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen/ COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/library ${CMAKE_CURRENT_SOURCE_DIR}/README.md ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/* VERBATIM) # COMMENT "Generating API documentation with Doxygen"
ENDIF(DOXYGEN_FOUND)

file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
if(${TINYSPLINE_WITH_PYTHON})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/toolchain/setup.py.in" "${CMAKE_CURRENT_BINARY_DIR}/setup.py" @ONLY)
endif()

file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/pom.xml")
if(${TINYSPLINE_WITH_JAVA})
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/toolchain/pom.xml.in" "${CMAKE_CURRENT_BINARY_DIR}/pom.xml" @ONLY)
endif()
