###############################################################################
### Setup GLUT.
###############################################################################
find_package(GLUT)
if(GLUT_FOUND)
	include_directories(${GLUT_INCLUDE_DIR})
	if(MSVC)
		# to fix a debug issue: http://stackoverflow.com/q/29110985/2741329
		get_filename_component( GLUT_LIBRARY_DIRS ${GLUT_glut_LIBRARY} DIRECTORY)  
	endif()  
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})
endif()



###############################################################################
### Setup OpenGL.
###############################################################################
find_package(OpenGL)
if(OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIRS})
	link_directories(${OPENGL_LIBRARY_DIRS})
	add_definitions(${OPENGL_DEFINITIONS})
endif()



###############################################################################
### Add required compiler/platform specific C flags.
###############################################################################
if(APPLE)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -framework OpenGL -framework GLUT")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Qunused-arguments")
endif()



###############################################################################
### Create examples. Exclude GLUT related examples in double precision mode.
###############################################################################
add_executable(quickstart_c quickstart.c)
target_link_libraries(quickstart_c
		LINK_PUBLIC tinyspline_static
		${TINYSPLINE_LIBRARIES}
		)
set_target_properties(quickstart_c PROPERTIES FOLDER "examples/c")

add_executable(json_export_c json_export.c)
target_link_libraries(json_export_c
		LINK_PUBLIC tinyspline_static
		${TINYSPLINE_LIBRARIES}
		)
set_target_properties(json_export_c PROPERTIES FOLDER "examples/c")

if("${TINYSPLINE_DEFINITIONS}" MATCHES "TINYSPLINE_FLOAT_PRECISION"
		AND GLUT_FOUND AND OPENGL_FOUND)
	add_executable(bspline bspline.c)
	target_link_libraries(bspline
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(bspline PROPERTIES FOLDER "examples/c")

	add_executable(nurbs nurbs.c)
	target_link_libraries(nurbs
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(nurbs PROPERTIES FOLDER "examples/c")

	add_executable(split split.c)
	target_link_libraries(split
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(split PROPERTIES FOLDER "examples/c")

	add_executable(beziers beziers.c)
	target_link_libraries(beziers
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(beziers PROPERTIES FOLDER "examples/c")

	add_executable(interpolation interpolation.c)
	target_link_libraries(interpolation
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(interpolation PROPERTIES FOLDER "examples/c")

	add_executable(derivative derivative.c)
	target_link_libraries(derivative
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(derivative PROPERTIES FOLDER "examples/c")

	add_executable(buckle buckle.c)
	target_link_libraries(buckle
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(buckle PROPERTIES FOLDER "examples/c")

	add_executable(triangle triangle.c)
	target_link_libraries(triangle
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(triangle PROPERTIES FOLDER "examples/c")

	add_executable(triangle-nurbs triangle-nurbs.c)
	target_link_libraries(triangle-nurbs
		LINK_PUBLIC tinyspline_static
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${TINYSPLINE_LIBRARIES}
	)
	set_target_properties(triangle-nurbs PROPERTIES FOLDER "examples/c")
endif()
