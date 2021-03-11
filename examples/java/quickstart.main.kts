@file:DependsOn("org.tinyspline:tinyspline:0.3.0-2")

import org.tinyspline.BSpline
import org.tinyspline.BSplineType

// Create a cubic spline with 7 control points in 2D using
// a clamped knot vector. This call is equivalent to:
// val spline: BSpline = BSpline(7, 2, 3, BSplineType.CLAMPED)
val spline: BSpline = BSpline(7)

// Setup control points.
val ctrlp: MutableList<Double> = spline.controlPoints
ctrlp[0]  = -1.75 // x0
ctrlp[1]  = -1.0  // y0
ctrlp[2]  = -1.5  // x1
ctrlp[3]  = -0.5  // y1
ctrlp[4]  = -1.5  // x2
ctrlp[5]  = 0.0   // y2
ctrlp[6]  = -1.25 // x3
ctrlp[7]  = 0.5   // y3
ctrlp[8]  = -0.75 // x4
ctrlp[9]  = 0.75  // y4
ctrlp[10] = 0.0   // x5
ctrlp[11] = 0.5   // y5
ctrlp[12] = 0.5   // x6
ctrlp[13] = 0.0   // y6
spline.controlPoints = ctrlp

// Evaluate `spline` at u = 0.4.
var result: List<Double> = spline.eval(0.4).result
System.out.printf("x = %f, y = %f%n", result[0], result[1])

// Derive `spline` and subdivide it into a sequence of Bezier curves.
val beziers: BSpline = spline.derive().toBeziers()

// Evaluate `beziers` at u = 0.3.
result = beziers.eval(0.3).result
System.out.printf("x = %f, y = %f%n", result[0], result[1])
