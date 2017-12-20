# based on: https://github.com/mkleemann/cmake-avr

# the name of the target operating system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

# here is the target environment located
if(DEFINED ENV{AVR_FIND_ROOT_PATH})
	set(CMAKE_FIND_ROOT_PATH $ENV{AVR_FIND_ROOT_PATH})
else()
	if(EXISTS "/opt/local/avr")
		set(CMAKE_FIND_ROOT_PATH "/opt/local/avr")
	elseif(EXISTS "/usr/avr")
		set(CMAKE_FIND_ROOT_PATH "/usr/avr")
	elseif(EXISTS "/usr/lib/avr")
		set(CMAKE_FIND_ROOT_PATH "/usr/lib/avr")
	else()
		message(FATAL_ERROR "Please set AVR_FIND_ROOT_PATH in your environment.")
	endif()
endif()
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
