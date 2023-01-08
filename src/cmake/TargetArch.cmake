# Detects the target architecture.
# ~~~
#
# This module is based on the work of the Petroules Corporation:
# https://github.com/axr/solar-cmake/blob/master/TargetArch.cmake
#
# License: ======================================================================
#
# Copyright (c) 2012 Petroules Corporation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   1. Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# ===============================================================================
#
# See: https://github.com/axr/solar-cmake/blob/master/TargetArch.cmake for more
# details.
# ~~~

set(archdetect_c_code
    "
#if   defined(__aarch64__) \\
   || defined(_M_ARM64)
        #error cmake_ARCH arm64
#elif defined(__arm__) \\
   || defined(__TARGET_ARCH_ARM) \\
   || defined(_M_ARM)
        #error cmake_ARCH arm
#elif defined(__i386) \\
   || defined(__i386__) \\
   || defined(_M_IX86)
        #error cmake_ARCH x86
#elif defined(__x86_64) \\
   || defined(__x86_64__) \\
   || defined(__amd64) \\
   || defined(_M_X64)
        #error cmake_ARCH x86_64
#elif defined(__ia64) \\
   || defined(__ia64__) \\
   || defined(_M_IA64)
        #error cmake_ARCH ia64
#elif defined(__EMSCRIPTEN__)
        #error cmake_ARCH any
#endif

#error cmake_ARCH unknown
"
)

function(target_architecture output_var)
  file(WRITE "${TINYSPLINE_BINARY_DIR}/arch.c" "${archdetect_c_code}")

  enable_language(C)

  # Detect the architecture in a rather creative way...
  #
  # This compiles a small C program which is a series of ifdefs that selects a
  # particular #error preprocessor directive whose message string contains the
  # target architecture. The program will always fail to compile (both because
  # the file is not a valid C program, and obviously because of the presence of
  # the #error preprocessor directives). By exploiting the preprocessor in this
  # way, we can detect the correct target architecture even when
  # cross-compiling, since the program itself never needs to be run (only the
  # compiler/preprocessor).
  try_run(run_result_unused compile_result_unused "${TINYSPLINE_BINARY_DIR}"
          "${TINYSPLINE_BINARY_DIR}/arch.c" COMPILE_OUTPUT_VARIABLE ARCH
  )

  # Parse the architecture name from the compiler output.
  string(REGEX MATCH "cmake_ARCH ([a-zA-Z0-9_]+)" ARCH "${ARCH}")

  # Get rid of the value marker, leaving just the architecture name.
  string(REPLACE "cmake_ARCH " "" ARCH "${ARCH}")

  # Check for macOS universal2.
  list(LENGTH CMAKE_OSX_ARCHITECTURES OSX_ARCH_LENGTH)
  if(OSX_ARCH_LENGTH EQUAL 2)
    list(FIND CMAKE_OSX_ARCHITECTURES "x86_64" OSX_X86_INDEX)
    list(FIND CMAKE_OSX_ARCHITECTURES "arm64" OSX_ARM64_INDEX)
    if((${OSX_X86_INDEX} GREATER -1) AND (${OSX_ARM64_INDEX} GREATER -1))
      set(ARCH "universal2")
    endif()
  endif()

  # If we are compiling for an unknown architecture, the following variable
  # should already be set to "unknown". If the variable is empty (e.g., due to a
  # typo in the code), then explicitly set it to "unknown".
  if(NOT ARCH)
    set(ARCH unknown)
  endif()

  set(${output_var}
      "${ARCH}"
      PARENT_SCOPE
  )
endfunction()
