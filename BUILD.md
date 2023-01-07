TinySpline uses the CMake build system to compile and package its interfaces.
The following compiler suites are tested: GCC, Clang, and MSVC. In order to
create the bindings, Swig (4.1.0 or later) must be installed. Each binding may
have additional dependencies that are necessary to compile the native library
of the target language. The following table gives an overview:

Language | Dependencies to Generate Source | (Relative) Output Directory
-------- | ------------------------------- | ---------------------------
C#       | -                               | csharp
D        | -                               | dlang
Golang   | -                               | go
Java     | Java Development Kit            | java/org/tinyspline
Lua      | Lua headers                     | lua
Octave   | Octave headers                  | octave
PHP      | PHP (Zend) headers *            | php
Python   | Python headers                  | python
R        | R headers and RCPP              | r
Ruby     | Ruby headers                    | ruby

* Please note that macOS comes with PHP, but does not provide the Zend headers.
It is recommended to use a package manager (such as Homebrew) to obtain the
required headers.

In addition, the following tools are required to compile the generated
interface files (.java files etc.) and create the binary package of the target
language using CMake:

Language | Required Tool(s)                 | Output File
-------- | -------------------------------- | ----------------
C#       | Any of: csc, mcs, dmcs, gmcs     | TinySpline.dll
Java     | javac and jar (available in JDK) | tinyspline.jar

Checkout the repository and cd into it:

```bash
git clone git@github.com:msteinbeck/tinyspline.git tinyspline
cd tinyspline
```

Create a build directory and cd into it:

```bash
mkdir build
cd build
```

Run CMake and build the project:

```bash
cmake ..
cmake --build .
```

To build a specific interface, use `-DTINYSPLINE_ENABLE_<LANGUAGE>` when
setting up CMake (`<LANGUAGE>` is interface you want to build). For example:

```bash
cmake -DTINYSPLINE_ENABLE_PYTHON=True ..
cmake --build . tinysplinepython
```

Note that the C interface is always enabled and cannot be disabled. By default,
the C++ interface is enabled but can be disabled by setting the corresponding
CMake option to `False`, `Off` etc.

To enable all interfaces at once, use `-DTINYSPLINE_ENABLE_ALL_INTERFACES`:

```bash
cmake -DTINYSPLINE_ENABLE_ALL_INTERFACES=True ..
cmake --build .
```

You will find the libraries and packages in `tinyspline/build/lib`.

### Python 2 vs. Python 3
While generating the Python binding, Swig needs to distinguish between Python 2
and Python 3. That is, Swig uses the command line parameter `-py` to generate
Python 2 compatible code and `-py3` to generate Python 3 compatible code.
Accordingly, Swig is configured depending on the Python version found by CMake
during initialization. On systems with multiple versions of Python installed,
CMake usually chooses the more recent one. If you want to use a specific
version of Python instead, set the CMake option `TINYSPLINE_PYTHON_VERSION` to
`2` or `3`.

The following example shows how to force CMake to use Python 2 rather than
Python 3:

```bash
cmake -DTINYSPLINE_PYTHON_VERSION=2 ..
```

### Install the C and C++ Interfaces
The following command installs the C and C++ (if enabled) interface to your
system:

```bash
cmake --build . --target install
```

This command also installs a set of CMake config scripts and pkg-config files
(for the C and C++ interface respectively). The CMake config script of the C
interface exports the following variables:

- TINYSPLINE_INCLUDE_DIRS: Contain the header files.
- TINYSPLINE_LIBRARY_DIRS: Contain the libraries.
- TINYSPLINE_LIBRARIES: Shared libraries to link against.
- TINYSPLINE_DEFINITIONS: Definitions to add with `add_definitions`.
- TINYSPLINE_VERSION: The version string.

The CMake config script of the C++ interface exports the same variables except
that they have prefix `TINYSPLINECXX`, e.g., `TINYSPLINECXX_INCLUDE_DIRS`.

Use the CMake commands `find_package(tinyspline)` (C) and
`find_package(tinysplinecxx)` (C++) to include TinySpline into your project.

### Install the Bindings
Depending on your build configuration, binding-related distribution files are
generated within the root of your build directory. For example, the file
`setup.py` is generated if Python is enabled. Currently, the following build
tools are supported: NuGet (C#), Dub (D), Maven (Java), and Luarocks (Lua),
Setuptools (Python), RubyGems (Ruby).
