TinySpline
========

[![Build Status](https://travis-ci.org/msteinbeck/tinyspline.svg?branch=master)](https://travis-ci.org/msteinbeck/tinyspline)
[![Build status](https://ci.appveyor.com/api/projects/status/mu9qlojuff4rvea3/branch/master?svg=true)](https://ci.appveyor.com/project/msteinbeck/tinyspline/branch/master)

TinySpline is a library for NURBS, B-Splines, and Bézier curves, giving you access to several
operations on splines. The library has been implemented in C (C89) and provides a wrapper for
C++ (C++11) along with auto-generated bindings for C#, D, Java, Octave, Lua, PHP, Python, R,
and Ruby. The focus of TinySpline is to be very small in design with a minimum set of
dependencies and an interface developed to be convenient for all levels without lacking
enhanced features.

### License
MIT License - see the LICENSE file in the source distribution.

### Some Features of This Library
- Use a single struct for NURBS, B-Splines, Bézier curves, lines, and points.
- Support for opened and clamped splines.
- Create splines of any degree and dimension.
- Evaluate splines using De Boor's algorithm.
- Interpolate cubic splines using the Thomas algorithm.
- Insert knots and split splines without modifying the shape.
- Derive splines of any degree.
- Subdivide splines into Bézier curves.
- A wrapper for C++ (C++11) and bindings for C#, D, Java, Lua, PHP, Python, and Ruby.
- Easy to use with OpenGL.

Feel free to ask for more features via the [issues](https://github.com/msteinbeck/tinyspline/issues)
or to contribute to TinySpline. :)

### Getting Started
The following listing uses the C++ wrapper to give a short example of TinySpline:

```cpp
#include <iostream>
#include "tinysplinecpp.h"

int main(int argc, char **argv)
{
	// Create a cubic spline with 7 control points in 2D using
	// a clamped knot vector. This call is equivalent to:
	// tinyspline::BSpline spline(7, 2, 3, TS_CLAMPED);
	tinyspline::BSpline spline(7);

	// Setup control points.
	std::vector<tinyspline::real> ctrlp = spline.ctrlp();
	ctrlp[0]  = -1.75f; // x0
	ctrlp[1]  = -1.0f;  // y0
	ctrlp[2]  = -1.5f;  // x1
	ctrlp[3]  = -0.5f;  // y1
	ctrlp[4]  = -1.5f;  // x2
	ctrlp[5]  =  0.0f;  // y2
	ctrlp[6]  = -1.25f; // x3
	ctrlp[7]  =  0.5f;  // y3
	ctrlp[8]  = -0.75f; // x4
	ctrlp[9]  =  0.75f; // y4
	ctrlp[10] =  0.0f;  // x5
	ctrlp[11] =  0.5f;  // y5
	ctrlp[12] =  0.5f;  // x6
	ctrlp[13] =  0.0f;  // y6
	spline.setCtrlp(ctrlp);

	// Stores our evaluation results.
	std::vector<tinyspline::real> result;

	// Evaluate `spline` at u = 0.4 using 'evaluate'.
	result = spline.evaluate(0.4f).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	// Derive `spline` and subdivide it into a sequence of Bezier curves.
	tinyspline::BSpline beziers = spline.derive().toBeziers();

	// Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
	result = beziers(0.3f).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	return 0;
}
```

### Installation

#### Compiling From Source
TinySpline uses the CMake build system to compile and package the interfaces. The C
library has been implemented in C89 and, thus, should be compatible with every modern
compiler. All other features of TinySpline are optional and will be disabled if CMake does
not find the required dependencies; however, CMake and an appropriate C/C++ compiler must
be available, regardless of the interface you want to build. TinySpline is tested using
the following compiler suites: gcc, clang, and msvc. In order to compile the C++ wrapper
or any of the bindings, your compiler suite (e.g. gcc) must support C++11. Additionally,
Swig (in version 3.0.1 or above) is required to generate the bindings for other languages.
Each binding may have further dependencies to generate the source code of the target language.
The following table gives an overview:

Language | Dependencies to Generate Source | (Relative) Output Directory
-------- | ------------------------------- | ---------------------------
C#       |                                 | csharp
D        | -                               | dlang
Golang   | -                               | go
Java     | Java Development Kit            | org/tinyspline
Lua      | Lua headers                     | lua
PHP      | PHP (Zend) headers *            | php
Python   | Python headers                  | python
Ruby     | Ruby headers                    | ruby

* Please note that macOS comes with PHP, but does not have the Zend headers.  It is
recommended to use a package manager (such as Homebrew) to obtain the headers.

To simplify the usage of the bindings, the generated source files will be compiled and/or
packaged if necessary. That is, for instance, the generated Java files will be compiled to
.class files and packaged into a single jar archive. Thus, the following tools are
required if you want to package the corresponding binding:

Language | Required Tool(s)                 | Output File
-------- | -------------------------------- | ----------------
C#       | Any of: csc, mcs, dmcs, gmcs     | TinySpline.dll
Java     | javac and jar (available in JDK) | tinyspline.jar

The remaining bindings do not need to be packaged (usually the interpreted languages).

Now let's start building TinySpline. First of all, checkout the repository and cd into it:

```bash
git clone git@github.com:msteinbeck/tinyspline.git tinyspline
cd tinyspline
```

Afterwards, create a build directory and cd into it:

```bash
mkdir build
cd build
```

Finally, run CMake and build the libraries:

```bash
cmake ..
cmake --build .
```

You will find the resulting libraries, jars, etc. in `tinyspline/build/lib`.

#### Cross Compiling
In order to cross-compile the C and C++ library, use one of the provided toolchain files.
Currently, toolchain files for MinGW, ARM, and AVR are available at the root directory of
the source distribution (e.g. `Toolchain-arm.cmake`). Use the following command within
your build directory to cross compile TinySpline to the desired platform:

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=<path to tinyspline>/tools/toolchain/Toolchain-*.cmake ..
```

#### Python 2 vs. Python 3
Swig needs to distinguish between Python 2 and Python 3 in order to generate source code
that is compatible with the used environment. That is, Swig requires the parameter `-py`
to generate Python 2 compatible code and `-py3` to generate Python 3 compatible code.
Thus, the CMake file used to compile and package the libraries configures Swig according
to the version of the Python instance that was found during initialization. On systems
with multiple versions of Python installed, CMake chooses the more recent one (i.e.
Python 3). If you want to use a specific version of Python, set the environment variable
'TINYSPLINE_PYTHON_VERSION' to '2' or '3'.

The following example shows how to force CMake to use Python 2 rather than Python 3 on
Debian-based systems:

```bash
export TINYSPLINE_PYTHON_VERSION=2
cmake ..
```

#### Disabling Bindings
For one reason or another, you may have the required packages to build a binding, but you
don't want to compile it.  You can pass an additional argument into CMake to prevent the
specified language bindings from compiling:

```bash
cmake -DTINYSPLINE_DISABLE_CSHARP=YES ..
```

#### Install the C and C++ Libraries
The following command installs TinySpline to your system:

```bash
cmake --build . --target install
```

However, there are several binding-related files that CMake does not install with this
command, as some languages use their own approach to install the files to your system.
Python, for instance, uses Distutils/Setuptools to copy the resulting files to Python-specific
installation directories that CMake is not aware of. Thus, TinySpline ships further,
language-related distribution tools that will be explained in the following sections.

### Install the bindings
Depending on your configuration, binding-related distribution files are
generated within the root of your build directory. That is, for instance, the
file `setup.py` will be generated if support for Python has been detected.
Currently, the following build tools are supported: Setuptools (Python), Maven
(Java), and Luarocks (Lua).

### Theoretical Backgrounds
[[1]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html) is a very good starting point for B-Splines.

[[2]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html) explains De Boor's Algorithm and gives some pseudo code.

[[3]](http://www.codeproject.com/Articles/996281/NURBS-curve-made-easy) provides a good overview of NURBS with some mathematical background.

[[4]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/NURBS/NURBS-def.html) is useful if you want to use NURBS in TinySpline.

