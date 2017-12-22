# -*-cmake-*-
#
# Nightwave Studios - FindR.cmake
# https://www.nightwave.co
#
# Search for R library files
#
# Once loaded this will define:
#   R_FOUND       - system has R
#   R_INCLUDE_DIR - include directory for R
#   R_LIBRARY_DIR - library directory for R
#   R_LIBRARIES   - libraries you need to link to



# Find headers
find_path(R_INCLUDE_DIR
	NAMES "R.h"
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\R\\Current;BinPath]"
	"$ENV{R_LOCATION}"
	"$ENV{R_LOCATION}/include"
	"$ENV{R_LOCATION}/include/R"
	"$ENV{R_HOME}/include"
	/usr/include/
	/usr/include/R
	/usr/local/include
	/usr/local/include/R
	/opt/local/include/R
)

# Find compiled library file
find_library(R_LIBRARIES
	NAMES libR R
	PATHS 
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\R\\Current;BinPath]"
	"$ENV{R_LOCATION}/R/.libs"
	"$ENV{R_LOCATION}/lib"
	"$ENV{R_HOME}/lib"
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	DOC "R library"
)

# Get path to library
get_filename_component(R_LIBRARY_DIR ${R_LIBRARIES} PATH)

# Set variables as advanced (hide from GUI unless "show advanced checked")
mark_as_advanced(R_LIBRARIES R_LIBRARY_DIR R_INCLUDE_DIR)

# Set R_FOUND and print message
INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	R
	DEFAULT_MSG
	R_LIBRARIES
	R_INCLUDE_DIR
)
