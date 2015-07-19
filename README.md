TinySpline
========

TinySpline is a C library for [B-Splines](https://en.wikipedia.org/wiki/B-spline) 
and [NURBS](https://de.wikipedia.org/wiki/Non-Uniform_Rational_B-Spline) 
with a modern C++11 wrapper and bindings for Java and Python (via Swig). 
The goal of this project is to provide a small library with a minimum of 
dependencies which is easy and intuitively to use. Moreover, the integration 
of TinySpline into OpenGL is straight forward.

TinySpline is licensed under [the MIT License](http://opensource.org/licenses/MIT),
so feel free to use it anywhere.

###Some Features of This Library
- TinySpline provides NURBS, B-Splines, Béziers, lines and points within a single struct.
- Create splines of any degree with any dimensions.
- Evaluate splines using [De Boor's algorithm](https://en.wikipedia.org/wiki/De_Boor%27s_algorithm).
- Insert knots and split splines by keeping the splines shape.
- Subdivide B-Splines into Béziers.
- A modern C++11 wrapper.
- Bindings for Java and Python.

###Project Structure
The core of TinySpline is written in ANSI C and consists of the files
[tinyspline.h](https://github.com/retuxx/tinyspline/blob/master/library/tinyspline.h) 
and [tinyspline.c](https://github.com/retuxx/tinyspline/blob/master/library/tinyspline.c).
You can either copy those files into your project or use CMake to create
a static and shared library.

The C++11 wrapper consists of the files [tinysplinecpp.h](https://github.com/retuxx/tinyspline/blob/master/library/tinysplinecpp.h)
and [tinysplinecpp.cpp](https://github.com/retuxx/tinyspline/blob/master/library/tinysplinecpp.cpp).
As for the C library, you can copy those files (alongside tinyspline.h and
tinyspline.c) into your project or use CMake to create a static and shared library.

All bindings of TinySpline work on top of the C++11 wrapper and will be 
generated with [Swig](http://www.swig.org/) (3.0.1 or above). The file
[tinyspline.i](https://github.com/retuxx/tinyspline/blob/master/library/tinyspline.i)
configures all language independent settings, while tinyspline-xyz.i adds
language specific features. Using CMake to create the bindings is recommended.

All these files can be found in the directory 
[library](https://github.com/retuxx/tinyspline/tree/master/library).

The directory 
[example](https://github.com/retuxx/tinyspline/tree/master/example) contains
some examples written in OpenGL.

###API
With a few exceptions, all functions of the C library provide input and output
parameter, where all input parameter are const. Except of the copy functions
(`ts_***_copy`), the pointer of the input may be equal to the pointer of the
output, so you can modify a spline by using it as input and output at once:

```c
tsBSpline spline;
ts_bspline_new(3, 3, 7, TS_CLAMPED, &spline); // create spline
ts_bspline_buckle(&spline, 0.6f, &spline); // modify spline
```

Error handling has been implemented in a single struct (`tsError`). This makes
it easier to reuse the error codes over several functions. Error checking should
be straight forward:

```c
tsBSpline spline;
const tsError err = ts_bspline_new(3, 3, 7, TS_CLAMPED, &spline);
if (err < 0) // or use err != TS_SUCCESS
  // error
else
  // no error
```

The C++11 wrapper uses std::exception instead. All bindings map std::exception
into their own exception types.

###Current Development
- Deriving splines.
- Knot removal.
- Given a point P and a spline S, find the Point Q on S with the 
minimal distance ||P - Q||<sub>2</sub>.
- Wrapper for C#, Rust and Julia.

###Theoretical Backgrounds
[[1]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html)
&nbsp;&nbsp; is a very good entry point for B-Splines.  
[[2]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html)
&nbsp;&nbsp; explains the De Boor's Algorithm and gives some pseudo code.  
[[3]](http://www.codeproject.com/Articles/996281/NURBS-curve-made-easy)
&nbsp;&nbsp; provides a good overview of NURBS with some mathematical background.  
[[4]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/NURBS/NURBS-def.html)
&nbsp;&nbsp; is useful if you want to use NURBS in TinySpline.


###Compiling TinySpline From Source
TinySpline uses the CMake build system. The C library is written in 
ANSI C and should be compilable with nearly every compiler. All
other features of TinySpline are optional and will be disabled
if CMake does not find the required dependencies (such as 
Swig and OpenGL).

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

###Cross Compiling
TinySpline provides toolchain files for mingw and arm. Use the following
command within your build directory for cross compiling
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=<path to root dir of tinypsline>/Toolchain-*.cmake ..
```
