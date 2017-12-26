# - Find R
#
# This module is based on:
# 	Nightwave Studios - FindR.cmake
# 	https://www.nightwave.co
#
# The following variables are set:
#   R_FOUND        - System has R
#   R_HOME_DIR     - Home directory of R
#   R_INCLUDE_DIRS - Include directory for R
#   R_VERSION      - Version of R
#   R_LIBRARIES    - Libraries you need to link to
#
# The following components are supported:
#   Rcpp           - Seamless R and C++ Integration
#

FIND_PROGRAM(R_RSCRIPT_EXECUTABLE NAMES Rscript)

if(R_RSCRIPT_EXECUTABLE)
	execute_process(
		COMMAND
			Rscript -e "cat(R.home())"
			OUTPUT_VARIABLE R_HOME_DIR
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	execute_process(
		COMMAND
			Rscript -e "cat(R.home('include'))"
			OUTPUT_VARIABLE R_INCLUDE_DIRS
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	execute_process(
		COMMAND
			Rscript -e "cat(R.version.string)"
			OUTPUT_VARIABLE R_VERSION
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	string(REGEX REPLACE "[a-zA-Z]|[(].*|[ ]" "" R_VERSION ${R_VERSION})

	find_library(R_LIBRARIES
		NAMES libR R
		PATHS "${R_HOME_DIR}/*"
	)

	if(R_FIND_COMPONENTS)
		foreach(component ${R_FIND_COMPONENTS})
			if(component STREQUAL "Rcpp")
				execute_process(
					COMMAND
					Rscript -e "library(Rcpp) ; cat(path.package('Rcpp'))"
					OUTPUT_VARIABLE R_RCPP_HOME_DIR
					OUTPUT_STRIP_TRAILING_WHITESPACE
				)
				find_path(R_RCPP_INCLUDE_DIR
					NAMES Rcpp.h
					PATHS "${R_RCPP_HOME_DIR}/*"
				)
				if(R_RCPP_INCLUDE_DIR)
					list(APPEND R_INCLUDE_DIRS
						${R_RCPP_INCLUDE_DIR})
					set(R_Rcpp_FOUND TRUE)
				endif()
				unset(R_RCPP_HOME_DIR)
				unset(R_RCPP_INCLUDE_DIR)
			endif()
		endforeach()
	endif()
endif()

MARK_AS_ADVANCED(
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
