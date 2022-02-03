TinySpline
========

![CI](https://github.com/msteinbeck/tinyspline/workflows/CI/badge.svg)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/msteinbeck/tinyspline.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/msteinbeck/tinyspline/context:cpp)

TinySpline is a small, yet powerful library for interpolating, transforming,
and querying arbitrary NURBS, B-Splines, and Bézier curves. The core of the
library is written in ANSI C (C89) with a C++ wrapper for an object-oriented
programming model. Based on the C++ wrapper, auto-generated bindings for C#, D,
Go, Java, Javascript, Lua, Octave, PHP, Python, R, and Ruby are provided.

### License
MIT License - see the LICENSE file in the source distribution.

### Installation

#### Pre-built Binaries

Releases can be downloaded from the 
[releases](https://github.com/msteinbeck/tinyspline/releases) page.

In addition, the following package manager are supported:

Conan (C/C++):  
https://conan.io/center/tinyspline

Luarocks (Lua; currently only Linux and macOS):
```bash
luarocks install --server=https://msteinbeck.github.io/tinyspline/luarocks tinyspline
```

Maven (Java):
```xml
<dependency>
   <groupId>org.tinyspline</groupId>
   <artifactId>tinyspline</artifactId>
   <version>0.3.0-2</version>
</dependency>
```

PyPI (Python):
```bash
python -m pip install tinyspline
```

#### Compiling From Source

See [BUILD.md](BUILD.md).

### Getting Started

Examples for each interface (target language) can be found in the
[examples](examples) subdirectory.

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
 * handling system. Embed your code into a TS_TRY/TS_END_TRY block and use
 * TS_CALL when calling a TinySpline function. Likewise, you can use any of
 * the TS_THROW macros to raise an error if an external function (e.g.,
 * malloc) failed.
 *
 * Errors can be handled with TS_CATCH. TS_FINALLY contains code that is
 * executed in any case, therefore being perfectly suitable for cleaning up
 * resources. That said, error handling is entirely optional. You may omit
 * TS_TRY/TS_END_TRY, TS_CALL, and TS_THROW and pass NULL instead of a pointer
 * to a tsStatus object. */

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
			&spline, 1, TS_POINT_EPSILON,
			&beziers, &status))
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
#include "tinysplinecxx.h"

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

### Theoretical Backgrounds
[[1]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html) is a very good starting point for B-Splines.

[[2]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html) explains De Boor's Algorithm and gives some pseudo code.

[[3]](http://www.codeproject.com/Articles/996281/NURBS-curve-made-easy) provides a good overview of NURBS with some mathematical background.

[[4]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/NURBS/NURBS-def.html) is useful if you want to use NURBS in TinySpline.

