add_executable(quickstart_c quickstart.c)
target_link_libraries(quickstart_c PRIVATE tinyspline)
set_target_properties(quickstart_c PROPERTIES FOLDER "examples/c")

add_executable(json_export_c json_export.c)
target_link_libraries(json_export_c PRIVATE tinyspline)
set_target_properties(json_export_c PROPERTIES FOLDER "examples/c")

# ##############################################################################
# GLUT's API supports only floats.
# ##############################################################################
if("${TINYSPLINE_C_DEFINITIONS}" MATCHES "TINYSPLINE_FLOAT_PRECISION"
   AND GLUT_FOUND
   AND OPENGL_FOUND
)

  add_executable(bspline bspline.c)
  target_link_libraries(
    bspline PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(bspline PROPERTIES FOLDER "examples/c")

  add_executable(nurbs nurbs.c)
  target_link_libraries(
    nurbs PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(nurbs PROPERTIES FOLDER "examples/c")

  add_executable(split split.c)
  target_link_libraries(
    split PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(split PROPERTIES FOLDER "examples/c")

  add_executable(beziers beziers.c)
  target_link_libraries(
    beziers PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(beziers PROPERTIES FOLDER "examples/c")

  add_executable(interpolation interpolation.c)
  target_link_libraries(
    interpolation PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(interpolation PROPERTIES FOLDER "examples/c")

  add_executable(derivative derivative.c)
  target_link_libraries(
    derivative PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(derivative PROPERTIES FOLDER "examples/c")

  add_executable(tension tension.c)
  target_link_libraries(
    tension PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(tension PROPERTIES FOLDER "examples/c")

  add_executable(triangle triangle.c)
  target_link_libraries(
    triangle PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(triangle PROPERTIES FOLDER "examples/c")

  add_executable(triangle-nurbs triangle-nurbs.c)
  target_link_libraries(
    triangle-nurbs PRIVATE tinyspline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}
  )
  set_target_properties(triangle-nurbs PROPERTIES FOLDER "examples/c")
endif()
