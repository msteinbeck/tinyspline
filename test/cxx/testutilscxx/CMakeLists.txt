# ##############################################################################
# Build testutilscxx.
# ##############################################################################
add_library(testutilscxx STATIC "testutilscxx.cxx")
target_include_directories(testutilscxx PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(testutilscxx PUBLIC tinysplinecxx testutils)
