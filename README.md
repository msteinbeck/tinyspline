TinySpline
========

TinySpline is a [B-Spine](https://en.wikipedia.org/wiki/B-spline) and 
[NURBS](https://de.wikipedia.org/wiki/Non-Uniform_Rational_B-Spline) 
library written in pure C with wrappers for C++11 and Java (using Swig). 
The goal of this project is to provide a small library with a minimum of 
dependencies which is easy and intuitively to use. Moreover, the 
interface was designed for easy integration in OpenGL.

###Some Features of This Library
- Modeling B-Splines and NURBS (even Béziers, lines and points, as 
they are implicit forms) of any degree with any dimensions (2D, 3D, ...).
- Evaluating splines using [De Boor's algorithm](https://en.wikipedia.org/wiki/De_Boor%27s_algorithm).
- Inserting knots and splitting splines.
- Subdividing splines into Béziers.
- Wrapper for C++11 and Java.

###Current Development
- Deriving splines.
- Knot removal.
- Given a point P and a spline S, find the Point Q on S with the 
minimal distance ||P - Q||<sub>2</sub>.
- Wrapper for Python, C#, Rust and Julia.

###Compiling TinySpline From Source
TinySpline uses CMake for the C/C++11 library and Swig for the Java 
proxy. The provided examples require OpenGL. All components except the 
C library are optional.

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
