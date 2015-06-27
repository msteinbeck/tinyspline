TinySpline
========

TinySpline is a C library for [B-Spines](https://en.wikipedia.org/wiki/B-spline) 
and [NURBS](https://de.wikipedia.org/wiki/Non-Uniform_Rational_B-Spline) 
with wrappers for C++11, Java and Python (using Swig). The goal of this 
project is to provide a small library with a minimum of dependencies 
which is easy and intuitively to use. Moreover, the integration of 
TinySpline into OpenGL is straight forward.

###Some Features of This Library
- Modeling B-Splines and NURBS (even Béziers, lines and points, as 
they are implicit forms) of any degree with any dimensions (2D, 3D, ...).
- Evaluating splines using [De Boor's algorithm](https://en.wikipedia.org/wiki/De_Boor%27s_algorithm).
- Inserting knots and splitting splines.
- Subdividing splines into Béziers.
- Wrapper for C++11, Java and Python.

###Current Development
- Deriving splines.
- Knot removal.
- Given a point P and a spline S, find the Point Q on S with the 
minimal distance ||P - Q||<sub>2</sub>.
- Improving the Python wrapper.
- Wrapper for C#, Rust and Julia.

###Compiling TinySpline From Source
TinySpline uses the CMake build system. All Wrappers, except of C++11, 
require [Swig](http://www.swig.org/). The provided examples require 
OpenGL.

- Checkout the repository
```bash
svn co git@github.com:retuxx/tinyspline.git tinyspline
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
