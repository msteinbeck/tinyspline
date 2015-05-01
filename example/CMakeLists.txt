# find glut
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
# find opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})
add_definitions(${OPENGL_DEFINITIONS})

add_executable(bspline bspline.c)
target_link_libraries(bspline LINK_PUBLIC tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

add_executable(nurbs nurbs.c)
target_link_libraries(nurbs LINK_PUBLIC tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

add_executable(split split.c)
target_link_libraries(split LINK_PUBLIC tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
