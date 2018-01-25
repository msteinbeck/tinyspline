TinySpline - Basics
========

The following chapter describes some basics about splines and how to use them in
TinySpline. At first, we start with a mathematical examination of NURBS, B-Splines, Bézier
curves, lines, and points. The given code examples are using the C interface of
TinySpline, though you don't need to be a C expert to follow this tutorial.

###About Splines
Let `>` be the [superset](https://en.wiktionary.org/wiki/superset) relation so that we can
write `A > B` in case of two sets `A`, `B` with `A` being the superset of `B`.
Furthermore, we define the following sets of splines:

* `#NURBS#` - The set of all NURBS curves.
* `#BSPLINES#` - The set of all B-Spline curves.
* `#BEZIERS#` - The set of all Bézier curves.
* `#LINES#` - The set of all lines.
* `#POINTS#` - The set of all points.

Then, the following equation holds:

`#NURBS# > #BSPLINES# > #BEZIERS# > #LINES# > #POINTS#`

To put it differently, we can say that every point is a line, ever line is a Bézier curve,
every Bézier curve is a B-Spline, and ever B-Spline is a NURBS. In order to understand
this relation, we will start with a (simple) definition of B-Splines that is more
code- than maths-related.

   A B-Spline `b` consists of four attributes: (1) The `degree` of `b` describing the
   (general) 'smoothness' of `b`, (2) the `control points` of `b` used to shape `b`, (3)
   the `knot vector` of `b` (simply `knots`) used to define the continuity of `b` at
   certain points (not the control points!), and (4) the `dimension` of `b` defining how
   many components each control point has (for instance (x, y) in 2D).

   A B-Spline `b` of degree `p` with `m` control points has `n = m+p+1` knots. The knot
   vector `knots` must be in ascending order (monotonically increasing). That is, `u_i
   <= u_{i+1}` for all knot values `u` in `knots` and `0 <= i < n-1`. The domain of `b` is
   given by `u_p` and `u_{n-p}`. That means, you can retrieve points laying on `b` by
   evaluating `b` at any knot value `u` with `u_p <= u <= u_{n-p}`. The evaluation of `b`
   at `u` is denoted by `b(u)`.

The following example illustrates the domain of B-Splines. Let's say `b` is a B-Spline of
degree 3 (cubic B-Spline) with the following knot vector:

`knots = [4, 5, 6, 10, 11, 11, 18, 19, 20]`

This implies that `p` is 3, `n` is 9, and `m = n-p-1 = 9-3-1` is 5. The domain of `b` is
`[6, 18]` so that you can evaluate `b` at any knot value `u` between `6` and `18`, for
instance, `6`, `6,1316`, `9`, `10`, `11`, `12`, `13,333`, `13,6`, `17,999999`, `18` and so
on, but not at values less than 6 or greater than 18. As you may have noticed, the knot
value `11` occurs twice in `knots`. Repeating knot values is permitted in principle but,
however, is limited to the degree of a B-Spline (as we will see in the following
definition).

   Given a B-Spline `b` of degree `p` with knot vector `knots`, the multiplicity `s(u)` of
   any `u` in `knots` is limited by the order (`p+1`) of `b`. That is, `s(u) <= p+1` for
   all knot values `u` in `knots`. Furthermore, `b` is `C^{p-s(u)}` continuous at `u` so
   that increasing `s(u)` by 1 decreases `b`'s continuity at `u` by 1.

That said, you will notice that B-Splines are discontinuous (`C^{-1}` continuous) at
certain points if the corresponding knot values have multiplicity `p+1`. Evaluating a
B-Spline `b` at `u` with `s(u) = p+1` *almost always* returns two points `q`, `r` rather
than one, though `q` and `r` may be the 'same' (`q = r`) and, thus, can be seen as a
single point. As if all this wasn't complicated enough, there is a special case in which
evaluating `b` always returns a single point regardless of `b`'s continuity at this point.
It occurs when the evaluated point is the very first or rather very last point of a
B-Spline (you can think of a B-Spline's 'ending' points).

