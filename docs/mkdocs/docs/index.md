<img src="img/logo.png" style="display: block; margin: 0 auto; width: 350px;
max-width: 350px;">

Welcome to the official website of the TinySpline project—a comprehensive
programming library for arbitrary splines with support for a multitude of
programming languages. TinySpline is being developed at
[Github](https://github.com/msteinbeck/tinyspline/) and released under the
terms of the [MIT](https://opensource.org/licenses/MIT) license.

## Introduction

TinySpline is library for NURBS, B-Splines, and Bézier curves allowing you to
handle splines with ease. The library has been implemented in ANSI C (C89) and
provides a wrapper for C++ (C++11) along with auto-generated bindings for C#,
D, Java, Lua, Octave, PHP, Python, R, and Ruby. TinySpline strives to be very
small by design with a minimum set of dependencies and an interface developed
to be convenient without lacking enhanced features.

I started this project as part of my bachelor's thesis where I struggled to
find an easy-to-use (Java) library that allows me to subdivide B-Splines into
sequences of Bézier curves in order to render them using the [QT
Jambi](https://github.com/qtjambi/qtjambi) framework. In fact, most widget
toolkits support only quadratic and cubic Bézier curves. One of the few
exceptions I am aware of is the [GLU NURBS
interface](http://www.glprogramming.com/red/chapter12.html) that is capable of
rendering arbitrary NURBS curves and surfaces. However, drawing splines using
modern widget toolkits seems to be quite challenging due to the lack of proper
support.

## Why Another Spline Library?

Indeed, there are already several spline libraries available in the web and
some of them have become quite sophisticated. Unfortunately, however, the
majority of these libraries are available only for C/C++ or Python, support
only quadratic and cubic splines, or have been designed with fixed
dimension—usually 2D.TinySpline, on the other hand, has been developed with
compatibility and genericness in mind. Compatibility is expressed in the fact
that TinySpline's core is implemented in ANSI C (a C standard supported by most
compiler suits and runtime environments) as well as the fact that TinySpline is
available for various different programming languages, amongst others: D, Java,
Lua, PHP, Python, R, and Ruby (further languages will be added in future).
Genericness is expressed in the fact that TinySpline supports splines of any
degree and dimension as well as supporting single and double precision (using
the data types float and double, respectively). A more comprehensive list of
features is given below:

- Use a single struct (C API) or class (high-level languages) for NURBS,
  B-Splines, Bézier curves, lines, and points.
- Configure TinySpline with single and double precision at compile time.
- Create splines of any degree and dimension.
- Evaluate splines using De Boor's algorithm.
- Interpolate cubic splines using the Thomas algorithm.
- Insert knots and split splines without modifying their shape.
- A convenient wrapper for C++ (C++11) and auto-generated bindings for C#, D,
  Java, Lua, PHP, Python, R, and Ruby.
- Subdivide NURBS and B-Splines into Bézier curves.
- Derive splines of any degree.

Still, this is not a complete list of features for the simple reason that
TinySpline is under active development and new features are added constantly.

## References

if TinySpline does not meet your requirements, feel free to create an
[issue](https://github.com/msteinbeck/tinyspline/issues) on Github or have a
look at one of the libraries referenced below:

- [http://libnurbs.sourceforge.net](http://libnurbs.sourceforge.net) — A C++
  non-uniform rational B-Splines library.

- [https://github.com/bgrimstad/splinter](https://github.com/bgrimstad/splinter)
  — SPLINTER (SPLine INTERpolation) is a library for multivariate function
  approximation with splines. The library is implemented in C++ and provides
  bindings for Python and Matlab.

- [https://www.gnu.org/software/gsl/manual/html_node/Basis-Splines.html](https://www.gnu.org/software/gsl/manual/html_node/Basis-Splines.html)
  — A software library for numerical computations in applied mathematics and
  science.

- [https://github.com/ejmahler/SplineLibrary](https://github.com/ejmahler/SplineLibrary)
  — A C++ library created to provide open-source reference implementations of
  many spline functions, e.g., Natural Splines and Catmull-Rom Splines.

- [https://github.com/thibauts/b-spline](https://github.com/thibauts/b-spline)
  — A JavaScript library for B-Spline interpolation of control points of any
  dimensionality using de Boor's algorithm.

## Theoretical Backgrounds

[[1]](http://www.cs.mtu.edu/%7Eshene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html)
is a very good starting point for B-Splines.  
[[2]](http://www.cs.mtu.edu/%7Eshene/COURSES/cs3621/NOTES/spline/B-spline/de-Boor.html)
explains De Boor's Algorithm and gives some pseudo code.  
[[3]](http://www.codeproject.com/Articles/996281/NURBS-curve-made-easy)
provides a good overview of NURBS with some mathematical background.  
[[4]](http://www.cs.mtu.edu/%7Eshene/COURSES/cs3621/NOTES/spline/NURBS/NURBS-def.html)
is useful if you want to use NURBS in TinySpline.
