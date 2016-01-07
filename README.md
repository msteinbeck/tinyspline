TinySpline
========

TinySpline is a C library for
[NURBS](https://de.wikipedia.org/wiki/Non-Uniform_Rational_B-Spline),
[B-Splines](https://en.wikipedia.org/wiki/B-spline) and [Bézier
curves](https://en.wikipedia.org/wiki/B%C3%A9zier_curve) (even lines and
points) with a modern C++11 wrapper and bindings for C#, Java and Python
(via Swig). The goal of this project is to provide a small library with a
minimum set of dependencies which is easy and intuitively to use. Moreover,
the integration of TinySpline into OpenGL is straightforward.

TinySpline is licensed under [the MIT
License](http://opensource.org/licenses/MIT), hence feel free to use it
anywhere.

###Table of Contents
- [Some Features of This Library](#features)
- [Project Structure](#structure)
- [Bindings](#bindings)
- [API](#api)
  - [Data Structures](#structs)
  - [NURBS, B-Splines, Béziers, Lines and Points](#types)
  - [Functions](#functions)
  - [Memory Management](#memory)
  - [Error Handling](#error)
  - [OpenGL Integration](#opengl)
  - [Spline Evaluation and Discontinuity](#evaldisc)
- [Theoretical Backgrounds](#backgrounds)
- [Installation](#installation)

<a name="features" />
###Some Features of This Library
- TinySpline provides NURBS, B-Splines, Béziers, lines and points within a
single struct.
- Create splines of any degree with any dimensions.
- Perform cubic spline interpolation using [Thomas'
algorithm](https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm).
- Evaluate splines using [De Boor's
algorithm](https://en.wikipedia.org/wiki/De_Boor%27s_algorithm).
- Insert knots and split splines while keeping the splines shape.
- Subdivide B-Splines into Béziers.
- A C++11 wrapper.
- Bindings for C#, Java and Python.

<a name="structure" />
###Project Structure
The core of TinySpline is written in ANSI C and consists of the files
[tinyspline.h](https://github.com/retuxx/tinyspline/blob/master/library/tinysp
line.h) and
[tinyspline.c](https://github.com/retuxx/tinyspline/blob/master/library/tinysp
line.c). You can either copy those files into your project or use CMake to
create a static or shared library.

The C++11 wrapper consists of the files
[tinysplinecpp.h](https://github.com/retuxx/tinyspline/blob/master/library/tin
ysplinecpp.h) and
[tinysplinecpp.cpp](https://github.com/retuxx/tinyspline/blob/master/library/t
inysplinecpp.cpp). As for the C library, you can copy those files
(alongside tinyspline.h and tinyspline.c) into your project or use CMake to
create a static or shared library.

All bindings of TinySpline work on top of the C++11 wrapper and will be
generated with [Swig](http://www.swig.org/) (3.0.1 or above). While
[tinyspline.i](https://github.com/retuxx/tinyspline/blob/master/library/tinysp
line.i) configures all language independent settings, tinysplineXYZ.i adds
language related features. The file
[swig_wrapper.h](https://github.com/retuxx/tinyspline/blob/master/library/swig
_wrapper.h) contains functions which are necessary to provide collections
instead of plain C arrays. Using CMake to create the bindings is recommended.

<a name="bindings" />
###Bindings
Alongside Swig, each binding may have additional dependencies in order to
generate the source code of the target language. The following table gives
an overview:

Language | Dependencies to Generate Source | (Relative) Output Directory
-------- | ------------------------------- | ---------------------------
C#       | -                               | csharp
Java     | JNI headers                     | so/tinyspline
Python   | Python headers                  | python

By design of Swig, each binding generates and compiles its own shared
library which is necessary to use the binding. Depending on your operating
system and the used compiler the actual names of the shared libraries may
vary. The following table shows the names of the shared libraries compiled
with GCC on Linux:

Language | Shared Library
-------- | ----------------------
C#       | libtinysplinecsharp.so
Java     | libtinysplinejava.so
Python   | _tinysplinepython.so

**Note:** In order to use a binding make sure its shared library is available
in the library path. Java does not load its shared libraries automatically.
Thus, you have to do it manually by placing the following code *before* the
first use of TinySpline:

```java
// loads the shared library
System.loadLibrary("tinysplinejava");
```

To simplify the usage of the bindings, the generated source code will be
compiled and/or packaged according to the target language idiom. The
following table gives an overview of the neccessary tools and the resulting
output files.

Language | Necessary Tools                  | Output File
-------- | -------------------------------- | ----------------
C#       | Any of: csc, mcs, dmcs, gmcs     | tinysplinecs.dll
Java     | javac and jar (available in JDK) | tinyspline.jar
Python   | -                                | tinyspline.py*

\* The Python binding is copied from the source code directory into the
build directory and may be renamed for convenience.

<a name="api" />
###API

<a name="structs" />
####Data Structures
The C library of TinySpline consists of two enums and two structs:

Name                   | Description
---------------------- | -----------------------------------------------------
`tsError` (enum)       | Defines some error codes
`tsBSplineType` (enum) | Defines how to setup knots while creating splines
`tsBSpline` (struct)   | The spline itself
`tsDeBoorNet` (struct) | The result of spline evaluation (De Boor's algorithm)

The C++11 wrapper wraps `tsBSpline` and `tsDeBoorNet` into classes (namely
`TsBSpline` and `TsDeBoorNet`) and maps functions into methods. Furthermore,
constructors and destructors are provided.

<a name="types" />
####NURBS, B-Splines, Béziers, Lines and Points
Let `>` be the [superset](https://en.wiktionary.org/wiki/superset) relation
with `A` is superset of `B` iff `A > B`. Then the following equation applies:

`NURBS > B-Splines > Béziers > Lines > Points`

It goes without saying that the struct `tsBSpline` provides all necessary
fields to work with B-Splines. In order to understand how to use this struct
for Bézier curves, let's have a look at them. A Bézier curve `c` of degree
`n` has `n + 1` control points where `c` is tangent to the first control
point `p_0` and tangent to  the last control point `p_n`. The following code
snippet shows how to create a Bézier curve `c` of degree 3 in 2D:

```c
tsBSpline c;
ts_bspline_new(3, 2, 4, TS_CLAMPED, &c);
```

The first parameter (3) is the degree of `c` and the second one (2) the
dimension of `c`. As already mentioned a Bézier curve has `n + 1` control
points. Thus, the third parameter (number of control points) is 4. The forth
parameter (`TS_CLAMPED`) ensures that B-Splines are tangent to the first and
last control point. That's it!

You may ask yourself what `tsBSpline.knots` looks like. Due to the fact that
`c` is a Bézier curve and all Bézier curves are B-Splines, `c` actually
**is** a B-Spline. Furthermore, a B-Spline of degree `n` with `m` control
points has `m + n + 1` knots. Thus, `c` has `3 + 4 + 1 = 8` knots. To ensure
a B-Splines is tangent to the first and last control point the first `n + 1`
and the last `n + 1` knots need to be equal. Since `c` has 8 knots the knot
vector may look like:

`c.knots = [0, 0, 0, 0, 1, 1, 1, 1]`

**Note:** Keep in mind that `u_i <= u_i + 1` must apply for all knot
values `u` and `i = 0...m + n - 1`.

In conclusion all knot values of `c` are either `0` or `1`. Without getting
too much into details here: That's the reason why you don't need to take
care about the knot vector of `c` and Bézier curves in general.

As you might already know, a B-Splines `b` with `m > n+1` (`b` has more
control points than it's degree plus 1) **is** or at last can be described
by sequence of Bézier curves. In the trivial case `b` has `m mod n+1 = 0`
control points and the multiplicity `s` of each knot value u is `s(u) = n+1`.
For example let `b` be a B-Spline of degree 3 with 8 control point `p0, p1,
..., p7` and `8 + 3 + 1 = 12` knots. Furthermore, let the knot vector be:

`b.knots = [0, 0, 0, 0, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1]`

Then you can split `b` into two Bézier curves `c0` and `c1` where `c0` has
control points `p0, p1, p2, p3` and `c1` has control points `p4, p5, p6, p7`.
Although the knot vector of `c0` and `c1` can be ignored (see above) they
actually are:

`c0.knots = [0, 0, 0, 0, 0.5, 0.5, 0.5, 0.5]`  
`c1.knots = [0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1]`

As you can see `c0` and `c1` share the knot value `0.5` and because of `s(u)
= n+1` for all `u`, `c0` and `c1` are tangent to their first and last
control point respectively. If your B-Spline does not fulfill the
requirements of the trivial case, you have to convert it by using the
`ts_bspline_to_beziers` function. TODO, Describe this function in a new
section and create a reference

After looking closely to Bézier curves it should be obvious how to create
lines and points. Lines are Bézier curves of degree 1 with 2 control points:

```c
tsBSpline line;
ts_bspline_new(1, dim, 2, TS_CLAMPED, &line);
```

and points are just a very short lines (generally spoken). Actually a point
is of degree 0 with 1 control point:

```c
tsBSpline point;
ts_bspline_new(0, dim, 1, TS_CLAMPED, &point);
```

**Note:** If you want to create a sequence of connected lines, you just have
to increase the number of control points. A sequence of 8 connected lines,
for example, is of degree 1 with 9 control points.

Finally, we should have a look at NURBS. NURBS are generalizations of
B-Splines and can be expressed by [homogeneous
coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates). If you
want to create a NURBS of degree `n` with `m` control points in 3D, then you
have to create a B-Spline of degree `n` with `m` control points in 4D:

```c
tsBSpline nurbs;
ts_bspline_new(n, 4, m, TS_CLAMPED, &nurbs);
// setup homogeneous coordinates
```

The forth component of the dimension is used to weight the other three
components. You can find  an example of a NURBS in
[nurbs.c](https://github.com/retuxx/tinyspline/blob/master/example/nurbs.c).

**Note:** All functions of TinySpline (e.g. `ts_bspline_evaluate`) are
capable of handling NURBS, B-Splines, Béziers, lines and points.

<a name="functions" />
####Functions
With a few exceptions, all functions of the C library provide input and
output parameter. The input parameter are always const and the pointer of the
input may be equal to the pointer of the output. In order to modify a spline,
use it as input and output at once:

```c
tsBSpline spline;
ts_bspline_new(3, 3, 7, TS_CLAMPED, &spline); // create spline
ts_bspline_buckle(&spline, 0.6f, &spline); // modify spline
...
```

<a name="memory" />
####Memory Management
Due to the fact that TinySpline provides generic splines in size, degree and
dimension, the structs `tsBSpline` and `tsDeBoorNet` contain pointers to
dynamically allocated memory. That means you have to free the memory using
one of the `ts_***_free` functions to prevent memory leaks:

```c
tsBSpline spline; // allocated on stack
ts_bspline_new(3, 3, 7, TS_CLAMPED, &spline); // create spline
// do some cool stuff here
tsDeBoorNet net;
ts_bspline_evaluate(&spline, 0.5f, &net);
// do more cool stuff
ts_deboornet_free(&net); // free dynamically allocated memory
ts_bspline_free(&spline); // free dynamically allocated memory
```

The C++11 wrapper wraps the call of `ts_***_free` into the destrcutor of the
wrapper class. Thus, you don't need to take care of memory management in C++
and the bindings.

<a name="error" />
####Error Handling
The error handling of TinySpline has been implemented with error codes. The
enum `tsError` contains all available errors and should be used to reuse
error codes over several functions. Error checking is straightforward:

```c
tsBSpline spline;
const tsError err = ts_bspline_new(3, 3, 7, TS_CLAMPED, &spline);
if (err < 0) // or use err != TS_SUCCESS
  // error
else
  // no error
```

The C++11 wrapper uses std::runtime_error instead. All bindings map
std::runtime_error into their own exception types.

<a name="opengl" />
####OpenGL Integration
The structs `tsBSpline` and `tsDeBoorNet` provide all necessary fields to
use OpenGL without complex calculations:

```c
tsBSpline spline;
ts_bspline_new(3, 3, 7, TS_CLAMPED, &spline); // create spline
// setup control points

GLUnurbsObj *theNurb = gluNewNurbsRenderer(); // is part of GLU
// setup theNurb, have a look at: gluNurbsProperty, gluNurbsCallback etc.

// draw spline
gluBeginCurve(theNurb);
  gluNurbsCurve(
    theNurb,
    spline.n_knots,
    spline.knots,
    spline.dim,
    spline.ctrlp,
    spline.order, // no need to calc deg + 1
    GL_MAP1_VERTEX_3 // MAP1 = spline, VERTEX_3 = 3 dimensions
  );
gluEndCurve(theNurb);

// draw evaluation at u = 0.5
tsDeBoorNet net;
ts_bspline_evaluate(&spline, 0.5f, &net);
glBegin(GL_POINTS);
  glVertex3fv(net.result);
glEnd();
...
```

If your spline is a Bézier curve, you can use the following code instead:

```c
tsBSpline spline;
ts_bspline_new(3, 2, 4, TS_CLAMPED, &spline); // create bezier curve
// setup control points

// setup evaluator
glMap1f(
  GL_MAP1_VERTEX_3, // MAP1 = curve, VERTEX_3 = 3 dimensions
  0.0, // u_min, usually 0
  1.0, // u_max, usually 1
  spline.dim,
  spline.order, // no need to calc deg + 1
  spline.ctrlp
);
glEnable(GL_MAP1_VERTEX_3);

// draw bezier curve
glBegin(GL_LINE_STRIP);
  for (i = 0; i <= 30; i++)
    glEvalCoord1f((GLfloat) i/30.0);
glEnd();
```

<a name="evaldisc" />
####Spline Evaluation and Discontinuity
Although evaluating a spline at a given knot value is straightforward, there
are some notes you should know about.

1. TinySpline uses floats for control points and knots. If you want to
evaluate a spline `s` at knot value `u`, the function `ts_bspline_evaluate`
tries to find `u` within the knot vector of `s` and count its multiplicity
(as part of [De Boor's
algorithm](https://en.wikipedia.org/wiki/De_Boor%27s_algorithm)). Because of
comparing floats with `==` isn't a smart idea in general, TinySpline
provides its own float comparing function (namely `ts_fequals`) which uses
absolute and relative errors. To keep things valid, the field `tsDeBoorNet.u`
contains the actual used knot value and may only differ from the given `u`
if the multiplicity of `u` is greater than or equals to 1 and `u` is not `==`
to `tsDeBoorNet.u` but `ts_fequals(u, tsDeBoorNet.u) == 1`. If further
calculations depend on the exact value of `u` use `tsDeBoorNet.u` instead.

2. As you perhaps know, increasing the multiplicity of a knot `u` by 1
within a spline `s` decreases the continuity of `s` by 1. The upper bound of
the multiplicity of `u` is equals to the order (degree + 1) of `s` (in this
case `s` is C^-1 continuous aka. discontinuous). Usually you will find this
in clamped splines to ensure a spline is tangent to the first and last
control point. TODO... FINISH THIS.

**Note:** Keep in mind that these are rare cases. Usually you do not need to
take care of this.

<a name="backgrounds" />
###Theoretical Backgrounds
[[1]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html)
&nbsp;&nbsp; is a very good entry point for B-Splines.  
[[2]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html)
&nbsp;&nbsp; explains the De Boor's Algorithm and gives some pseudo code.  
[[3]](http://www.codeproject.com/Articles/996281/NURBS-curve-made-easy)
&nbsp;&nbsp; provides a good overview of NURBS with some mathematical background.  
[[4]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/NURBS/NURBS-def.html)
&nbsp;&nbsp; is useful if you want to use NURBS in TinySpline.

<a name="installation" />
###Installation

####Compiling From Source
TinySpline uses the CMake build system. The C library is written in ANSI C
and should be compilable with nearly every compiler. All other features of
TinySpline are optional and will be disabled if CMake does not find the
required dependencies (such as Swig and OpenGL).

- Checkout the repository
```bash
git clone git@github.com:retuxx/tinyspline.git tinyspline
cd tinyspline
```
- Create a build directory
```bash
mkdir build
cd build
```
- Create the Makefiles and build the library
```bash
cmake ..
make
```

You will find all static and shared libraries, jars etc. in
`tinyspline/build/library`

####Cross Compiling
TinySpline provides toolchain files for mingw and arm. Use the following
command within your build directory for cross compiling
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=<path to root dir of tinypsline>/Toolchain-*.cmake ..
```
