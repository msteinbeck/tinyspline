TinySpline
========

TinySpline is library for NURBS, B-Splines, and Bézier curves giving you access to several
operations on splines. The library has been implemented in C (C89) and provides a wrapper
for C++ (C++11) along with bindings for C#, Java, Lua, PHP, Python, and Ruby. The focus of
TinySpline is to be very small by design with a minimum set of dependencies. Nonetheless,
the interface has been developed to be convenient for non-experts without lacking enhanced
features.

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
- A wrapper for C++ (C++11) and bindings for C#, Java, Lua, PHP, Python, and
  Ruby.
- Easy to use with OpenGL.

Feel free to ask for special features or to contribute to TinySpline :).

### Project Structure
The source distribution of TinySpline consists of two sub projects. The first one is the
library itself that is located in the `library` directory. It contains all files that
are required to build TinySpline. The second sub project provides some basic examples and
is located in the `examples` directory.

The core of TinySpline has been implemented in C and consists of the files `tinyspline.h`
and `tinyspline.c`. In order to use the C interface of TinySpline you can either copy
those files into your project or use CMake to create a static or shared library.

The C++ wrapper consists of the files `tinysplinecpp.h` and `tinysplinecpp.cpp`. As for
the C interface, you can copy those files (along with `tinyspline.h` and `tinyspline.c`)
into your project or use CMake to create a static or shared library.

All bindings of TinySpline work on top of the C++ wrapper and are generated with
[Swig](http://www.swig.org/). The file `tinyspline.i` is used to configure language
independent settings. The file `tinysplineXYZ` adds language related features (e.g.
properties for Python). Using CMake to create the bindings is recommended.

Note: Use the file `debugging.h` to add some debugging features to the C interface.

### Getting Started
The following listing uses the C++ wrapper to give a short example of TinySpline:

```c
// Create a clamped spline of degree 3 in 2D consisting of 7 control points.
ts::BSpline spline(3, 2, 7, TS_CLAMPED);

// Setup the control points.
std::vector<float> ctrlp = spline.ctrlp();
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

// Evaluate `spline` at u = 0.4
std::vector<float> result = spline.evaluate(0.4f).result();
std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

// Derive `spline` and subdivide it into a sequence of Bezier curves.
ts::BSpline beziers = spline.derive().toBeziers();

// Evaluate `beziers` at u = 0.3
result = beziers(0.3f).result(); // you can use '()' instead of 'evaluate'
std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;
```

### Installation

#### Compiling From Source
TinySpline uses the CMake build system to compile and package the interfaces. The C
library has been implemented in C89 and, thus, should be compilable with nearly every
compiler. All other features of TinySpline are optional and will be disabled if CMake does
not find the required dependencies. However, CMake and an appropriate C/C++ compiler must
be available, regardless of the interface you want to build. TinySpline supports any of
the following compiler suites: gcc, clang, and msvc. In order to compile the C++ wrapper
or any of the bindings, your compiler suite (e.g. gcc) must support C++11. Additionally,
Swig (in version 3.0.1 or above) is required to generate the bindings. Along Swig, each
binding may have further dependencies to generate the source code of the target language.
The following table gives an overview:

Language | Dependencies to Generate Source | (Relative) Output Directory
-------- | ------------------------------- | ---------------------------
C#       | -                               | csharp
Java     | JNI headers                     | so/tinyspline
Lua      | Lua headers                     | lua
PHP      | Zend headers                    | php
Python   | Python headers                  | python
Ruby     | Ruby headers                    | ruby

To simplify the usage of the bindings, the generated source files will be compiled and/or
packaged if necessary. That is, for instance, the generated Java files will be compiled to
.class files and packaged into a single jar archive. Thus, the following tools are
required if you want to package the corresponding binding:

Language | Required Tool(s)                 | Output File
-------- | -------------------------------- | ----------------
C#       | Any of: csc, mcs, dmcs, gmcs     | TinySpline.dll
Java     | javac and jar (available in JDK) | tinyspline.jar

The remaining bindings do not need to be packaged (usually the script languages).

Now let's start building TinySpline. First of all, check out the repository and change
into the root directory:

```bash
git clone git@github.com:retuxx/tinyspline.git tinyspline
cd tinyspline
```

Afterwards, create a build directory and change into it:

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
In order to cross compile the C and C++ library, use one of the provided toolchain files.
Currently, toolchain files for MinGW, ARM, and AVR are available at the root directory of
the source distribution (e.g. `Toolchain-arm.cmake`). Use the following command within
your build directory to cross compile TinySpline to the desired platform:

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=<path to root dir of tinypsline>/Toolchain-*.cmake ..
```

#### Python 2 vs. Python 3
Swig needs to distinguish between Python 2 and Python 3 in order to generate source code
that is compatible with the used environment. That is, Swig requires the parameter `-py`
to generate Python 2 compatible code and `-py3` to generate Python 3 compatible code.
Thus, the CMake file used to compile and package the libraries, configures Swig according
to the version of the Python instance that was found during initialization. On systems
with multiple versions of Python installed, CMake aims to use the most recent one. If you,
on the other hand, want to use a specific version of Python, set the environment variable
'TINYSPLINE_PYTHON_VERSION' to '2' or '3'.

The following example shows how to force CMake to use Python 2 rather than Python 3 on
Ubuntu:

```bash
export TINYSPLINE_PYTHON_VERSION=2
cmake ..
```

#### Install the C and C++ Libraries
The following command installs TinySpline:

```
cmake --build . --target install
```

However, there are several binding related files that CMake does not install with this
command because some languages use their own approach to install files to your system.
Python, for instance, uses Distutils/Setuptools to copy the resulting files to Python
specific installation directories CMake is not aware of. Thus, TinySpline ships further,
language related distribution tools that will be explained in the following sections.

#### Install the Python Binding
The root directory of TinySpline contains the Python script `setup.py` using Setuptools
to wrap the CMake build process. Additionally, it copies the resulting files to the
appropriate Python installation directory. Use the following command to build and
install the Python binding of TinySpline:

```bash
python setup.py install
```

Note that you may need root privileges to copy the files to the desired installation
directory.

#### Install the Java Binding
There are several tools to manage the build process of software implemented in Java.
TinySpline uses Maven to create and install the Java binding as Maven is used in many
other projects and is well supported by various integrated development environments. You
will find the `pom.xml` file that is used to configure Maven in the root directory of the
source distribution. This file follows the usual mantra of wrapping the CMake build
process to create the binding. Additionally, the shared library that is required to use
the binding gets packaged into the jar archive. Use the following command to create and
install the archive into your local maven repository:

```bash
mvn install
```

If you run the above command on systems that do not support Unix makefiles by default (for
instance the Windows operating system), you may get the following error message: "CMake
Error: Could not create named generator Unix makefiles". This error results from the
[cmake-maven-project](https://github.com/cmake-maven-project/cmake-maven-project) plug-in
that requires an actual CMake generator to properly wrap the build process. That is, if
you skip the generator configuration of `cmake-maven-project`, it fails with: "The
parameters 'generator' for goal
com.googlecode.cmake-maven-project:cmake-maven-plugin:3.4.1-b1:generate are missing or
invalid". Thus, Maven has been configured to use the Unix makefiles generator by default.
However, open the file `pom.xml` with you preferred text editor and replace the line:

```xml
<generator>Unix Makefiles</generator>
```

with one of the generators listed at:
<http://www.cmake.org/cmake/help/v2.8.10/cmake.html#section_Generators>. Afterwards, build
TinySpline with your new generator configuration:

```bash
maven clean install
```

#### Install the C# and Ruby Bindings
Currently, TinySpline does not provide tools to install the bindings for C# and Ruby.
However, adding such tools is planned for the future. If you have experience with, for
instance, Ruby gems and Rake, feel free to create a pull request :).

### Theoretical Backgrounds
[[1]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html)
&nbsp;&nbsp; is a very good starting point for B-Splines.  
[[2]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html)
&nbsp;&nbsp; explains De Boor's Algorithm and gives some pseudo code.  
[[3]](http://www.codeproject.com/Articles/996281/NURBS-curve-made-easy)
&nbsp;&nbsp; provides a good overview of NURBS with some mathematical background.  
[[4]](http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/NURBS/NURBS-def.html)
&nbsp;&nbsp; is useful if you want to use NURBS in TinySpline.
