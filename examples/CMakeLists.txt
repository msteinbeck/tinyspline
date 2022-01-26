###############################################################################
### Setup GLUT.
###############################################################################
find_package(GLUT)
if(GLUT_FOUND)
  include_directories(${GLUT_INCLUDE_DIR})
  if(MSVC)
    # Fix a debug issue: http://stackoverflow.com/q/29110985/2741329
    get_filename_component(GLUT_LIBRARY_DIRS ${GLUT_glut_LIBRARY} DIRECTORY)
  endif()
  link_directories(${GLUT_LIBRARY_DIRS})
  add_definitions(${GLUT_DEFINITIONS})
endif()



###############################################################################
### Setup OpenGL.
###############################################################################
# https://cmake.org/cmake/help/latest/module/FindOpenGL.html#linux-specific
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL)
if(OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIRS})
  link_directories(${OPENGL_LIBRARY_DIRS})
  add_definitions(${OPENGL_DEFINITIONS})
endif()



###############################################################################
### Add required compiler/platform specific C/C++ flags.
###############################################################################
if(APPLE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -framework OpenGL -framework GLUT")
  add_definitions(-DGL_SILENCE_DEPRECATION)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Qunused-arguments")
endif()



###############################################################################
### Add the subdirectories containing the examples.
###############################################################################
add_subdirectory(c)
if(TARGET tinysplinecxx)
  add_subdirectory(cxx)
endif()
