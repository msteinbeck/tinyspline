import std.stdio;
import tinyspline;

void main()
{
	// Create a cubic spline with 7 control points in 2D using
	// a clamped knot vector. This call is equivalent to:
	// BSpline spline = new BSpline(7, 2, 3, BSplineType.CLAMPED);
	BSpline spline = new BSpline(7);

	// Setup control points.
	Vector ctrlp = spline.getControlPoints();
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
	ctrlp[1]  =  0.0;  // x5
	ctrlp[11] =  0.5;  // y5
	ctrlp[12] =  0.5;  // x6
	ctrlp[13] =  0.0;  // y6
	spline.setControlPoints(ctrlp);

	// Evaluate `spline` at u = 0.4 using 'eval'.
	Vector result = spline.eval(0.4).result();
	writefln("x = %f, y = %f", result[0], result[1]);

	// Derive `spline` and subdivide it into a sequence of Bezier curves.
	BSpline beziers = spline.derive().toBeziers();

	// Evaluate `beziers` at u = 0.3 using '()' instead of 'eval'.
	result = beziers(0.3).result();
	writefln("x = %f, y = %f", result[0], result[1]);
}