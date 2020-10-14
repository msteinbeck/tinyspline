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
# https://cmake.org/cmake/help/latest/module/FindOpenGL.html#linux-specific
set(OpenGL_GL_PREFERENCE "GLVND")
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
	add_definitions(-DGL_SILENCE_DEPRECATION)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Qunused-arguments")
endif()



###############################################################################
### Create examples. Exclude GLUT related examples in double precision mode.
###############################################################################
add_executable(quickstart_c quickstart.c)
target_link_libraries(quickstart_c
		PRIVATE tinyspline)
set_target_properties(quickstart_c PROPERTIES FOLDER "examples/c")

add_executable(json_export_c json_export.c)
target_link_libraries(json_export_c
		PRIVATE tinyspline)
set_target_properties(json_export_c PROPERTIES FOLDER "examples/c")

if("${TINYSPLINE_C_DEFINITIONS}" MATCHES "TINYSPLINE_FLOAT_PRECISION"
		AND GLUT_FOUND AND OPENGL_FOUND)
	add_executable(bspline bspline.c)
	target_link_libraries(bspline
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(bspline PROPERTIES FOLDER "examples/c")

	add_executable(nurbs nurbs.c)
	target_link_libraries(nurbs
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(nurbs PROPERTIES FOLDER "examples/c")

	add_executable(split split.c)
	target_link_libraries(split
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(split PROPERTIES FOLDER "examples/c")

	add_executable(beziers beziers.c)
	target_link_libraries(beziers
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(beziers PROPERTIES FOLDER "examples/c")

	add_executable(interpolation interpolation.c)
	target_link_libraries(interpolation
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(interpolation PROPERTIES FOLDER "examples/c")

	add_executable(derivative derivative.c)
	target_link_libraries(derivative
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(derivative PROPERTIES FOLDER "examples/c")

	add_executable(tension tension.c)
	target_link_libraries(tension
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(tension PROPERTIES FOLDER "examples/c")

	add_executable(triangle triangle.c)
	target_link_libraries(triangle
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(triangle PROPERTIES FOLDER "examples/c")

	add_executable(triangle-nurbs triangle-nurbs.c)
	target_link_libraries(triangle-nurbs
		PRIVATE tinyspline
		${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
	set_target_properties(triangle-nurbs PROPERTIES FOLDER "examples/c")
endif()
