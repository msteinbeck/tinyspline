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
