cmake_minimum_required(VERSION 2.8)
project(tinyspline)

# set options
option( TS_CPP_WITH_MOVE_SEMANTICS "Build C++ wrapper with move semantics (C++11 compiler required)" OFF )
if( TS_CPP_WITH_MOVE_SEMANTICS )
    add_definitions(    -DWITH_MOVE_SEMANTICS )
endif()
option( TS_BINDING_PYTHON "Build Python binding" ON )
option( TS_BINDING_RUBY "Build Ruby binding" ON )
option( TS_BINDING_JAVA "Build Java binding" ON )
option( TS_BINDING_CSHARP "Build CSHARP binding" ON )
option( TS_BUILD_EXAMPLES "Build the examples" ON )

# Turn on using solution folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(library)
if( TS_BUILD_EXAMPLES )
  add_subdirectory(example)
endif()
