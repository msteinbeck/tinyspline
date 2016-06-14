cmake_minimum_required(VERSION 2.8)

# find glut
find_package(GLUT)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

# find opengl
find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})
add_definitions(${OPENGL_DEFINITIONS})

if(APPLE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -framework OpenGL -framework GLUT")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm -Qunused-arguments")
endif()

# create examples
if(GLUT_FOUND AND OPENGL_FOUND)
  add_executable(bspline bspline.c)
  target_link_libraries(bspline LINK_PUBLIC tinyspline_static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  set_target_properties(bspline PROPERTIES FOLDER "examples" DEBUG_POSTFIX d)
  
  add_executable(nurbs nurbs.c)
  target_link_libraries(nurbs LINK_PUBLIC tinyspline_static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  set_target_properties(nurbs PROPERTIES FOLDER "examples" DEBUG_POSTFIX d)
  
  add_executable(split split.c)
  target_link_libraries(split LINK_PUBLIC tinyspline_static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  set_target_properties(split PROPERTIES FOLDER "examples" DEBUG_POSTFIX d)

  add_executable(beziers beziers.c)
  target_link_libraries(beziers LINK_PUBLIC tinyspline_static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  set_target_properties(beziers PROPERTIES FOLDER "examples" DEBUG_POSTFIX d)

  add_executable(interpolation interpolation.c)
  target_link_libraries(interpolation LINK_PUBLIC tinyspline_static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  set_target_properties(interpolation PROPERTIES FOLDER "examples" DEBUG_POSTFIX d)

  add_executable(derivative derivative.c)
  target_link_libraries(derivative LINK_PUBLIC tinyspline_static ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  set_target_properties(derivative PROPERTIES FOLDER "examples" DEBUG_POSTFIX d)
endif()
