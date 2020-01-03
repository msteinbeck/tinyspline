set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(MkDocs)
if(MKDOCS_FOUND)
	file(GLOB_RECURSE TINYSPLINE_MKDOCS_FILES
		RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
		"${CMAKE_CURRENT_SOURCE_DIR}/mkdocs/*")
	foreach(FROM ${TINYSPLINE_MKDOCS_FILES})
		# Ignore vim backup and swap files.
		if(${FROM} MATCHES "~$" OR ${FROM} MATCHES ".swp$")
			continue()
		endif()
		# Configure '.in' files.
		if(${FROM} MATCHES ".in$")
			string(REGEX REPLACE ".in$" "" TO "${FROM}")
			configure_file(
				"${CMAKE_CURRENT_SOURCE_DIR}/${FROM}"
				"${CMAKE_CURRENT_BINARY_DIR}/${TO}"
				@ONLY
			)
		# Copy all other files.
		else()
			configure_file(
				"${CMAKE_CURRENT_SOURCE_DIR}/${FROM}"
				"${CMAKE_CURRENT_BINARY_DIR}/${FROM}"
				COPYONLY
			)
		endif()
	endforeach()

	set(TINYSPLINE_WEBSITE_TARGET
		"${CMAKE_CURRENT_BINARY_DIR}/mkdocs/site")
	add_custom_command(OUTPUT "${TINYSPLINE_WEBSITE_TARGET}"
		COMMAND ${MKDOCS_EXECUTABLE} build --config-file
			"${CMAKE_CURRENT_BINARY_DIR}/mkdocs/mkdocs.yml"
		DEPENDS ${TINYSPLINE_MKDOCS_FILES}
		COMMENT "Generating website")
	add_custom_target(website DEPENDS ${TINYSPLINE_WEBSITE_TARGET})
endif()
