# FindPHP
# ~~~
#
# Finds PHP and the Zend framework (nowadays also known as Laminas).
#
# This module is based on the work of Paul Colby:
# https://github.com/pcolby/cmake-modules/blob/main/FindPHP.cmake
#
# License: ======================================================================
#
# Copyright (c) 2013, Paul Colby
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# *  Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright notice, this
#   list of conditions and the following disclaimer in the documentation and/or
#   other materials provided with the distribution.
#
# *  Neither the name of the pcolby nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# ===============================================================================
#
# The module has been modified to include the option to specify a "Zend"
# component in order to find the Zend headers.  © Nightwave Studios, 2017.
#
# This module defines:
#  PHP_CONFIG_EXECUTABLE      - Full path to the ``php-config`` binary.
#  PHP_EXECUTABLE             - Full path to the ``php`` binary.
#  PHP_EXTENSIONS_DIR         - Directory containing PHP extensions.
#  PHP_EXTENSIONS_INCLUDE_DIR - Directory containing PHP extension headers.
#  PHP_INCLUDE_DIRS           - Include directives for PHP development.
#  PHP_VERSION_STRING         - PHP version string.
#  PHP_VERSION_MAJOR          - PHP major version.
#  PHP_VERSION_MINOR          - PHP minor version.
#  PHP_FOUND                  - Set to TRUE if all of the above has been found.
#
# This file has been modified to include the option to specify a "Zend" component
# which, if set, forces this module to search for the Zend headers and to fail if
# the headers could not be found.  © Nightwave Studios, 2017.
# ~~~

find_program(PHP_CONFIG_EXECUTABLE NAMES php-config)

if(PHP_CONFIG_EXECUTABLE)
  execute_process(
    COMMAND ${PHP_CONFIG_EXECUTABLE} --php-binary
    OUTPUT_VARIABLE PHP_EXECUTABLE
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND ${PHP_CONFIG_EXECUTABLE} --extension-dir
    OUTPUT_VARIABLE PHP_EXTENSIONS_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND ${PHP_CONFIG_EXECUTABLE} --include-dir
    OUTPUT_VARIABLE PHP_EXTENSIONS_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND ${PHP_CONFIG_EXECUTABLE} --includes
    OUTPUT_VARIABLE PHP_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND ${PHP_CONFIG_EXECUTABLE} --version
    OUTPUT_VARIABLE PHP_VERSION_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  string(REPLACE "." ";" PHP_VERSION_MAJOR ${PHP_VERSION_STRING})
  list(GET PHP_VERSION_MAJOR 0 PHP_VERSION_MAJOR)

  string(REPLACE "." ";" PHP_VERSION_MINOR ${PHP_VERSION_STRING})
  list(GET PHP_VERSION_MINOR 1 PHP_VERSION_MINOR)
endif()

mark_as_advanced(
  PHP_CONFIG_EXECUTABLE
  PHP_EXECUTABLE
  PHP_EXTENSIONS_DIR
  PHP_EXTENSIONS_INCLUDE_DIR
  PHP_INCLUDE_DIRS
  PHP_VERSION_STRING
  PHP_VERSION_MAJOR
  PHP_VERSION_MINOR
)

if(PHP_FIND_COMPONENTS)
  foreach(component ${PHP_FIND_COMPONENTS})
    if(component STREQUAL "Zend")
      find_path(
        PHP_ZEND_HEADER
        NAMES "zend.h"
        PATHS "${PHP_EXTENSIONS_INCLUDE_DIR}/*"
      )
      if(PHP_ZEND_HEADER)
        set(PHP_Zend_FOUND TRUE)
      endif()
      unset(PHP_ZEND_HEADER)
    endif()
  endforeach()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  PHP
  FOUND_VAR PHP_FOUND
  REQUIRED_VARS
    PHP_CONFIG_EXECUTABLE
    PHP_EXECUTABLE
    PHP_EXTENSIONS_DIR
    PHP_EXTENSIONS_INCLUDE_DIR
    PHP_INCLUDE_DIRS
    PHP_VERSION_STRING
  VERSION_VAR PHP_VERSION_STRING
  HANDLE_COMPONENTS
)
