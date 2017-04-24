#include <iostream>
#include "tinysplinecpp.h"

int main(int argc, char **argv)
{
	// Create a spline...
	tinyspline::BSpline spline(
		3, // ... of degree 3...
		2, // ... in 2D...
		7, // ... consisting of 7 control points...
		TS_CLAMPED // ... using a clamped knot vector.
	);

	// Setup control points.
	std::vector<tinyspline::real> ctrlp = spline.ctrlp();
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

	// Stores our evaluation results.
	std::vector<tinyspline::real> result;

	// Evaluate `spline` at u = 0.4 using 'evaluate'.
	result = spline.evaluate(0.4f).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	// Derive `spline` and subdivide it into a sequence of Bezier curves.
	tinyspline::BSpline beziers = spline.derive().toBeziers();

	// Evaluate `beziers` at u = 0.3 using '()' instead of 'evaluate'.
	result = beziers(0.3f).result();
	std::cout << "x = " << result[0] << ", y = " << result[1] << std::endl;

	return 0;
}
