set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross compiler.
set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc-win32)
set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++-win32)

# Location of the target environment.
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Search for programs in the build host directories.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for headers and libraries in the target directories.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Specify the runtime libraries to be distributed.
set(TINYSPLINE_RUNTIME_LIBRARIES
    "/usr/lib/gcc/x86_64-w64-mingw32/6.3-win32/libstdc++-6.dll;/usr/lib/gcc/x86_64-w64-mingw32/6.3-win32/libgcc_s_seh-1.dll"
    CACHE FILEPATH "Set from toolchain file." FORCE
)

# Specify Java related variables.
set(JAVA_HOME "/usr/x86_64-w64-mingw32/java")
