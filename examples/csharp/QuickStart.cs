using TinySpline;
using System;
using System.Collections.Generic;

class QuickStart
{
	public static void Main (string[] args)
	{
		// Create a cubic spline with 7 control points in 2D using
		// a clamped knot vector. This call is equivalent to:
		// BSpline spline = new BSpline(7, 2, 3, BSplineType.CLAMPED);
		BSpline spline = new BSpline(7);

		// Setup control points.
		IList<float> ctrlp = spline.ctrlp;
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
		spline.ctrlp = ctrlp;

		// Evaluate `spline` at u = 0.4.
		IList<float> result = spline.evaluate(0.4f).result;
		Console.WriteLine("x = {0}, y = {1}", result[0], result[1]));

		// Derive `spline` and subdivide it into a sequence of Bezier curves.
		BSpline beziers = spline.derive().toBeziers();

		// Evaluate `beziers` at u = 0.3.
		result = beziers.evaluate(0.3f).result;
		Console.WriteLine("x = {0}, y = {1}", result[0], result[1]));
	}
}
