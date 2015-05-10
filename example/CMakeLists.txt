cmake_minimum_required(VERSION 2.8)

# find glut
find_package(GLUT)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

# find opengl
find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})
add_definitions(${OPENGL_DEFINITIONS})

if(GLUT_FOUND AND OPENGL_FOUND)
  add_executable(bspline bspline.c)
  target_link_libraries(bspline LINK_PUBLIC tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
  
  add_executable(nurbs nurbs.c)
  target_link_libraries(nurbs LINK_PUBLIC tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
  
  add_executable(split split.c)
  target_link_libraries(split LINK_PUBLIC tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif()
