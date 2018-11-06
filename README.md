TinySpline
========

[![Build Status](https://travis-ci.org/msteinbeck/tinyspline.svg?branch=master)](https://travis-ci.org/msteinbeck/tinyspline)
[![Build status](https://ci.appveyor.com/api/projects/status/mu9qlojuff4rvea3/branch/master?svg=true)](https://ci.appveyor.com/project/msteinbeck/tinyspline/branch/master)
[![CircleCI](https://circleci.com/gh/msteinbeck/tinyspline.svg?style=svg)](https://circleci.com/gh/msteinbeck/tinyspline)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/msteinbeck/tinyspline.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/msteinbeck/tinyspline/context:cpp)

TinySpline is a small, yet powerful library for interpolating, transforming,
and querying arbitrary NURBS, B-Splines, and Bézier curves. It is implemented
in ANSI C (C89) and provides a wrapper for C++ along with auto-generated
bindings C#, D, Java, Lua, Octave, PHP, Python, R, and Ruby.

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
- A wrapper for C++ as well as bindings for C#, D, Java, Lua, PHP, Python, and Ruby.
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
	std::vector<tinyspline::real> ctrlp = spline.controlPoints();
	ctrlp[0]  = -1.75; // x0
	ctrlp[1]  = -1.0;  // y0
	ctrlp[2]  = -1.5;  // x1
	ctrlp[3]  = -0.5;  // y1
	ctrlp[4]  = -1.5;  // x2
	ctrlp[5]  =  0.0;  // y2
	ctrlp[6]  = -1.25; // x3
	ctrlp[7]  =  0.5;  // y3
	ctrlp[8]  = -0.75; // x4
	ctrlp[9]  =  0.75; // y4
	ctrlp[10] =  0.0;  // x5
	ctrlp[11] =  0.5;  // y5
	ctrlp[12] =  0.5;  // x6
	ctrlp[13] =  0.0;  // y6
	spline.setControlPoints(ctrlp);

	// Stores our evaluation results.
	std::vector<tinyspline::real> result;

	// Evaluate `spline` at u = 0.4 using 'evaluate'.
	result = spline.eval(0.4).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	// Derive `spline` and subdivide it into a sequence of Bezier curves.
	tinyspline::BSpline beziers = spline.derive().toBeziers();

	// Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
	result = beziers(0.3).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	return 0;
}
```

### Installation

#### Prebuilt Binaries

Snapshot binaries are available at:

- macOS: https://github.com/msteinbeck/tinyspline/tree/build_osx_64
- Linux (Ubuntu 16.04): https://github.com/msteinbeck/tinyspline/tree/build_linux_64

#### Compiling From Source
TinySpline uses the CMake build system to compile and package its interfaces.
The C interface is implemented in ANSI C (C89) and, thus, should be compatible
with almost every compiler. All other features of TinySpline are optional and
will be disabled if CMake does not find the corresponding dependencies;
however, CMake and an appropriate C/C++ compiler must be available, regardless
of the interface you want to build. The following compiler suites are tested:
GCC, Clang, and MSVC. In order to create the bindings, Swig (3.0.1 or above)
must be available. Each binding may have further dependencies to generate the
source code of the target language. The following table gives an overview:

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

* Please note that macOS comes with PHP, but does not provide the Zend headers.
It is recommended to use a package manager (such as Homebrew) to obtain the
headers.

To simplify the usage of the bindings, the generated source files are compiled
and/or packaged if necessary. That is, for instance, the generated Java files
are compiled to .class files and packaged into a jar archive. Accordingly, the
following tools are required if you want to package the corresponding binding:

Language | Required Tool(s)                 | Output File
-------- | -------------------------------- | ----------------
C#       | Any of: csc, mcs, dmcs, gmcs     | TinySpline.dll
Java     | javac and jar (available in JDK) | tinyspline.jar

Now let's start building TinySpline. First of all, checkout the repository and
cd into it:

```bash
git clone git@github.com:msteinbeck/tinyspline.git tinyspline
cd tinyspline
```

Afterwards, create a build directory and cd into it:

```bash
mkdir build
cd build
```

Finally, run CMake and build the project:

```bash
cmake ..
cmake --build .
```

You will find the resulting libraries and packages in `tinyspline/build/lib`.

#### Python 2 vs. Python 3
While generating the Python binding, Swig needs to distinguish between Python 2
and Python 3. That is, Swig uses the command line parameter `-py` to generate
Python 2 compatible code and `-py3` to generate Python 3 compatible code.
Accordingly, Swig is configured depending on the Python version found by CMake
during initialization. On systems with multiple versions of Python installed,
CMake usually chooses the more recent one. If you want to use a specific
version of Python instead, set the environment variable
'TINYSPLINE_PYTHON_VERSION' to '2' or '3'.

The following example shows how to force CMake to use Python 2 rather than
Python 3:

```bash
TINYSPLINE_PYTHON_VERSION=2 cmake ..
```

#### Disabling Bindings
For one reason or another, you may have the required packages to build a
binding, but you don't want to compile it. You can pass additional arguments to
prevent particular bindings from being compiled and packaged:

```bash
cmake -DTINYSPLINE_DISABLE_CSHARP=YES ..
```

#### Install the C and C++ Libraries
The following command installs TinySpline to your system:

```bash
cmake --build . --target install
```

However, there are several binding-related files that CMake does not install
with this command, as some languages use custom tools to install files.
Python, for instance, uses Distutils/Setuptools to install files to
Python-specific directories that CMake is not aware of. Thus, TinySpline
ships further, language-related distribution tools.

###3 Install the Bindings
Depending on your configuration, binding-related distribution files are
generated within the root of your build directory. That is, for instance, the
file `setup.py` is generated if support for Python was detected. Currently, the
following build tools are supported: Setuptools (Python), Maven (Java), and
Luarocks (Lua).

### Theoretical Backgrounds
[[1]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html) is a very good starting point for B-Splines.

[[2]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html) explains De Boor's Algorithm and gives some pseudo code.

[[3]](http://www.codeproject.com/Articles/996281/NURBS-curve-made-easy) provides a good overview of NURBS with some mathematical background.

[[4]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/NURBS/NURBS-def.html) is useful if you want to use NURBS in TinySpline.

