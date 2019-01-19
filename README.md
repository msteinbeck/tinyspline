TinySpline
========

[![Build Status](https://travis-ci.org/msteinbeck/tinyspline.svg?branch=master)](https://travis-ci.org/msteinbeck/tinyspline)
[![Build status](https://ci.appveyor.com/api/projects/status/mu9qlojuff4rvea3/branch/master?svg=true)](https://ci.appveyor.com/project/msteinbeck/tinyspline/branch/master)
[![CircleCI](https://circleci.com/gh/msteinbeck/tinyspline.svg?style=svg)](https://circleci.com/gh/msteinbeck/tinyspline)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/msteinbeck/tinyspline.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/msteinbeck/tinyspline/context:cpp)

TinySpline is a small, yet powerful library for interpolating, transforming,
and querying arbitrary NURBS, B-Splines, and Bézier curves. The library is
implemented in ANSI C (C89) and provides a wrapper for C++ along with
auto-generated bindings for C#, D, Java, Lua, Octave, PHP, Python, R, and Ruby.

### License
MIT License - see the LICENSE file in the source distribution.

### Getting Started
The following listing uses the ANSI C interface:

```c
#include "tinyspline.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	tsStatus status;   /**< Used for error handling. */

	tsBSpline spline;  /**< The spline to setup. */
	tsReal *ctrlp;     /**< Pointer to the control points of `spline`. */

	tsBSpline beziers; /**< `spline` as a sequence of bezier curves. */

	tsDeBoorNet net;   /**< Used to evaluate `spline` and `beziers`. */
	tsReal *result;    /**< Pointer to the result of `net`. */

/* ------------------------------------------------------------------------- */
/* TinySpline includes a powerful, system-independent, and thread-safe error
 * handling system in the form of easy-to-use macros. All you need to do is to
 * embed your code into TS_TRY/TS_END_TRY and use TS_CALL when calling a
 * TinySpline function. Likewise, you can use any of the TS_THROW macros to
 * raise an error if an external function (e.g. malloc) failed.
 *
 * Errors can be handled in TS_CATCH. TS_FINALLY contains code that is executed
 * in any case, therefore being perfectly suitable for cleaning up resources.
 * Yet, error handling is entirely optional. You may omit TS_TRY, TS_CALL, and
 * TS_THROW and pass NULL instead of a pointer to a tsStatus object. */

	spline = ts_bspline_init();
	beziers = ts_bspline_init();
	net = ts_deboornet_init();
	ctrlp = result = NULL;
	TS_TRY(try, status.code, &status)
		/* Create a spline... */
		TS_CALL(try, status.code, ts_bspline_new(
			7, /* ... consisting of 7 control points... */
			2, /* ... in 2D... */
			3, /* ... of degree 3... */
			TS_CLAMPED, /* ... using a clamped knot vector. */
			&spline, &status))

		/* Setup control points of `spline`. */
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		ctrlp[0]  = -1.75f; /* x0 */
		ctrlp[1]  = -1.0f;  /* y0 */
		ctrlp[2]  = -1.5f;  /* x1 */
		ctrlp[3]  = -0.5f;  /* y1 */
		ctrlp[4]  = -1.5f;  /* x2 */
		ctrlp[5]  =  0.0f;  /* y2 */
		ctrlp[6]  = -1.25f; /* x3 */
		ctrlp[7]  =  0.5f;  /* y3 */
		ctrlp[8]  = -0.75f; /* x4 */
		ctrlp[9]  =  0.75f; /* y4 */
		ctrlp[10] =  0.0f;  /* x5 */
		ctrlp[11] =  0.5f;  /* y5 */
		ctrlp[12] =  0.5f;  /* x6 */
		ctrlp[13] =  0.0f;  /* y6 */
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))

		/* Evaluate `spline` at u = 0.4. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, 0.4f, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		printf("x = %f, y = %f\n", result[0], result[1]);

		/* Derive `spline` ... */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, &beziers, &status))
		/* ... and subdivide it into a sequence of Bezier curves. */
		TS_CALL(try, status.code, ts_bspline_to_beziers(
			&beziers, &beziers, &status))

		ts_deboornet_free(&net);
		free(result);
		/* Evaluate `beziers` at u = 0.3. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&beziers, 0.3f, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		printf("x = %f, y = %f\n", result[0], result[1]);
	TS_CATCH(status.code)
		puts(status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&beziers);
		ts_deboornet_free(&net);
		if (ctrlp)
			free(ctrlp);
		if (result)
			free(result);
	TS_END_TRY

	return status.code? 1 : 0;
}
```
The same example using the C++ interface:

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

	// Evaluate `spline` at u = 0.4 using 'eval'.
	std::vector<tinyspline::real> result = spline.eval(0.4).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	// Derive `spline` and subdivide it into a sequence of Bezier curves.
	tinyspline::BSpline beziers = spline.derive().toBeziers();

	// Evaluate `beziers` at u = 0.3 using '()' instead of 'eval'.
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
The following compiler suites are tested: GCC, Clang, and MSVC. In order to
create the bindings, Swig (3.0.1 or later) must be available. Each binding may
have further dependencies to generate the source code of the target language.
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

If you want to build a specific binding, use `-DTINYSPLINE_ENABLE_LANGUAGE`
(where `LANGUAGE` is interface you want to build) when setting up cmake. For
example:

```bash
cmake -DTINYSPLINE_ENABLE_PYTHON=True ..
build --build .
```

To enable all interfaces, use `-DTINYSPLINE_ENABLE_ALL_INTERFACES`:

```bash
cmake -DTINYSPLINE_ENABLE_ALL_INTERFACES=True ..
build --build .
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

#### Install the Bindings
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

