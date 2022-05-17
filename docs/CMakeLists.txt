set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(TINYSPLINE_DOXYGEN_DIR
      "doxygen"
      CACHE INTERNAL ""
  )
  set(TINYSPLINE_DOXYGEN_CONF_FILE
      "doxygen.conf"
      CACHE INTERNAL ""
  )

  set(TINYSPLINE_DOXYGEN_INPUT_DIR
      "${CMAKE_CURRENT_SOURCE_DIR}/${TINYSPLINE_DOXYGEN_DIR}"
      CACHE INTERNAL ""
  )
  set(TINYSPLINE_DOXYGEN_OUTPUT_DIR
      "${CMAKE_CURRENT_BINARY_DIR}/${TINYSPLINE_DOXYGEN_DIR}"
      CACHE INTERNAL ""
  )
  set(TINYSPLINE_DOXYGEN_CONF_FILE_IN
      "${TINYSPLINE_DOXYGEN_INPUT_DIR}/${TINYSPLINE_DOXYGEN_CONF_FILE}.in"
      CACHE INTERNAL ""
  )
  set(TINYSPLINE_DOXYGEN_INPUT_FILE
      "${CMAKE_CURRENT_BINARY_DIR}/${TINYSPLINE_DOXYGEN_CONF_FILE}"
      CACHE INTERNAL ""
  )

  configure_file(
    "${TINYSPLINE_DOXYGEN_CONF_FILE_IN}" "${TINYSPLINE_DOXYGEN_INPUT_FILE}"
    @ONLY
  )
  add_custom_command(
    OUTPUT "${TINYSPLINE_DOXYGEN_OUTPUT_DIR}"
    COMMAND "${DOXYGEN_EXECUTABLE}" "${TINYSPLINE_DOXYGEN_INPUT_FILE}"
    DEPENDS "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/README.md"
            "${TINYSPLINE_DOXYGEN_INPUT_FILE}"
    VERBATIM
  ) # COMMENT "Generating API documentation with Doxygen"
  add_custom_target(docs DEPENDS "${TINYSPLINE_DOXYGEN_OUTPUT_DIR}")
endif()

find_package(MkDocs)
if(MKDOCS_FOUND)
  set(TINYSPLINE_MKDOCS_SOURCE_DIR "mkdocs")
  file(
    GLOB_RECURSE TINYSPLINE_MKDOCS_FILES
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/${MKDOCS_SOURCE_DIR}/*"
  )
  foreach(FROM ${TINYSPLINE_MKDOCS_FILES})
    # Ignore vim backup files.
    if(${FROM} MATCHES "~$"
       # Ignore Vim swap files.
       OR ${FROM} MATCHES ".swp$" # Ignore mkdocs output dir.
       OR ${FROM} MATCHES "^${MKDOCS_DIR}/site"
    )
      continue()
    endif()
    # Configure .in files.
    if(${FROM} MATCHES ".in$")
      string(REGEX REPLACE ".in$" "" TO "${FROM}")
      configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/${FROM}"
        "${CMAKE_CURRENT_BINARY_DIR}/${TO}" @ONLY
      )
      # Copy all other files.
    else()
      configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/${FROM}"
        "${CMAKE_CURRENT_BINARY_DIR}/${FROM}" COPYONLY
      )
    endif()
  endforeach()
endif()
