#include <iostream>
#include "tinysplinecxx.h"

int main(int argc, char **argv)
{
	// Create a cubic spline with 7 control points in 2D using
	// a clamped knot vector. This call is equivalent to:
	// tinyspline::BSpline spline(7, 2, 3, TS_CLAMPED);
	tinyspline::BSpline spline(7);

	// Setup control points.
	std::vector<tinyspline::real> ctrlp = spline.controlPoints();
	ctrlp[0]  = (tsReal) -1.75; // x0
	ctrlp[1]  = (tsReal) -1.0;  // y0
	ctrlp[2]  = (tsReal) -1.5;  // x1
	ctrlp[3]  = (tsReal) -0.5;  // y1
	ctrlp[4]  = (tsReal) -1.5;  // x2
	ctrlp[5]  = (tsReal)  0.0;  // y2
	ctrlp[6]  = (tsReal) -1.25; // x3
	ctrlp[7]  = (tsReal)  0.5;  // y3
	ctrlp[8]  = (tsReal) -0.75; // x4
	ctrlp[9]  = (tsReal)  0.75; // y4
	ctrlp[10] = (tsReal)  0.0;  // x5
	ctrlp[11] = (tsReal)  0.5;  // y5
	ctrlp[12] = (tsReal)  0.5;  // x6
	ctrlp[13] = (tsReal)  0.0;  // y6
	spline.setControlPoints(ctrlp);

	// Evaluate `spline` at u = 0.4 using 'eval'.
	std::vector<tinyspline::real> result = spline.eval(0.4f).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	// Derive `spline` and subdivide it into a sequence of Bezier curves.
	tinyspline::BSpline beziers = spline.derive().toBeziers();

	// Evaluate `beziers` at u = 0.3 using '()' instead of 'eval'.
	result = beziers((tsReal) 0.3).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	return 0;
}
