# - Find R
#
# This module is based on:
# 	Nightwave Studios - FindR.cmake
# 	https://www.nightwave.co
#
# The following variables are set:
#   R_RSCRIPT_EXECUTABLE - full path to the Rscript binary
#   R_HOME_DIR           - home directory of R
#   R_INCLUDE_DIRS       - include directories of R
#   R_VERSION            - version of R
#   R_LIBRARIES          - libraries you need to link to
#   R_FOUND              - set to TRUE if all of the above has been found.
#
# The following components are supported:
#   Rcpp                 - seamless R and C++ Integration
#

find_program(R_RSCRIPT_EXECUTABLE
	NAMES Rscript
)

if(R_RSCRIPT_EXECUTABLE)
	execute_process(
		COMMAND
			${R_RSCRIPT_EXECUTABLE} -e "cat(R.home())"
			OUTPUT_VARIABLE R_HOME_DIR
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	execute_process(
		COMMAND
			${R_RSCRIPT_EXECUTABLE} -e "cat(R.home('include'))"
			OUTPUT_VARIABLE R_INCLUDE_DIRS
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	execute_process(
		COMMAND
			${R_RSCRIPT_EXECUTABLE} -e "cat(R.version.string)"
			OUTPUT_VARIABLE R_VERSION
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	string(
		REGEX REPLACE
			"[a-zA-Z]|[(].*|[ ]" ""
			R_VERSION
			"${R_VERSION}"
	)

	set(R_LIBRARY_DIR "${R_HOME_DIR}/bin/x64")
	if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		set(R_LIBRARY_DIR "${R_HOME_DIR}/bin/i386")
	endif()
	find_library(R_LIBRARIES
		NAMES libR R
		PATHS ${R_LIBRARY_DIR} "${R_HOME_DIR}/*"
	)
	unset(R_LIBRARY_DIR)

	if(R_FIND_COMPONENTS)
		foreach(component ${R_FIND_COMPONENTS})
			if(component STREQUAL "Rcpp")
				execute_process(
					COMMAND
						${R_RSCRIPT_EXECUTABLE} -e "library(Rcpp) ; cat(path.package('Rcpp'))"
						OUTPUT_VARIABLE R_RCPP_HOME_DIR
						OUTPUT_STRIP_TRAILING_WHITESPACE
				)
				find_path(R_RCPP_INCLUDE_DIR
					NAMES Rcpp.h
					PATHS "${R_RCPP_HOME_DIR}/*"
				)
				if(R_RCPP_INCLUDE_DIR)
					list(
						APPEND
							R_INCLUDE_DIRS
							${R_RCPP_INCLUDE_DIR}
					)
					set(R_Rcpp_FOUND TRUE)
				endif()
				unset(R_RCPP_HOME_DIR)
				unset(R_RCPP_INCLUDE_DIR)
			endif()
		endforeach()
	endif()
endif()

MARK_AS_ADVANCED(
	R_RSCRIPT_EXECUTABLE
	R_HOME
	R_INCLUDE_DIRS
	R_VERSION
	R_LIBRARIES
)

INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(R
	FOUND_VAR
		R_FOUND
	REQUIRED_VARS
		R_RSCRIPT_EXECUTABLE
		R_HOME_DIR
		R_INCLUDE_DIRS
		R_VERSION
		R_LIBRARIES
	VERSION_VAR
		R_VERSION
	HANDLE_COMPONENTS
)
