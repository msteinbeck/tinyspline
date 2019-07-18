# - Find MkDocs
#
# The following variables are set:
#   MKDOCS_EXECUTABLE - path to the MkDocs binary
#   MKDOCS_VERSION    - version of MkDocs

find_program(MKDOCS_EXECUTABLE
	NAMES mkdocs
)

if(MKDOCS_EXECUTABLE)
	execute_process(
		COMMAND
			${MKDOCS_EXECUTABLE} --version
			OUTPUT_VARIABLE MKDOCS_VERSION
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	separate_arguments(MKDOCS_VERSION)
	list(GET MKDOCS_VERSION 2 MKDOCS_VERSION)
endif()

mark_as_advanced(
	MKDOCS_EXECUTABLE
	MKDOCS_VERSION
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MkDocs
	REQUIRED_VARS
		MKDOCS_EXECUTABLE
	VERSION_VAR
		MKDOCS_VERSION
)
