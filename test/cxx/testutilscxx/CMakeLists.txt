###############################################################################
### Set up the compiler suite.
###############################################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



###############################################################################
### Build testutilscxx.
###############################################################################
add_library(testutilscxx STATIC "testutilscxx.cxx")
target_include_directories(testutilscxx PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(testutilscxx PUBLIC tinysplinecxx testutils)
