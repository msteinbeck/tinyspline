cmake_minimum_required(VERSION 3.4)
project(tinyspline)

# Turn on folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Detect tinyspline being built as main project
if(CMAKE_SOURCE_DIR STREQUAL "${tinysplice_SOURCE_DIR}")
    set(TINYSPLINE_MAIN_PROJECT ON)
endif()

option(TINYSPLINE_BUILD_EXAMPLES "Build TinySpline tests." ON)
option(TINYSPLINE_BUILD_TESTS "Build TinySpline tests." ON)
option(TINYSPLINE_BUILD_DOCS "Build TinySpline docs." ON)

add_subdirectory(src)
add_definitions("${TINYSPLINE_DEFINITIONS}")
if(TINYSPLINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(TINYSPLINE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
if(TINYSPLINE_BUILD_DOCS)
    add_subdirectory(docs)
endif()
