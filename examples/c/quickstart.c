#include "tinyspline.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	/* Create a spline... */
	tsBSpline spline;
	ts_bspline_new(
		7, /* ... consisting of 7 control points... */
		2, /* ... in 2D... */
		3, /* ... of degree 3... */
		TS_CLAMPED, /* ... using a clamped knot vector. */
		&spline
	);

	/* Setup control points. */
	spline.ctrlp[0]  = -1.75f; /* x0 */
	spline.ctrlp[1]  = -1.0f;  /* y0 */
	spline.ctrlp[2]  = -1.5f;  /* x1 */
	spline.ctrlp[3]  = -0.5f;  /* y1 */
	spline.ctrlp[4]  = -1.5f;  /* x2 */
	spline.ctrlp[5]  =  0.0f;  /* y2 */
	spline.ctrlp[6]  = -1.25f; /* x3 */
	spline.ctrlp[7]  =  0.5f;  /* y3 */
	spline.ctrlp[8]  = -0.75f; /* x4 */
	spline.ctrlp[9]  =  0.75f; /* y4 */
	spline.ctrlp[10] =  0.0f;  /* x5 */
	spline.ctrlp[11] =  0.5f;  /* y5 */
	spline.ctrlp[12] =  0.5f;  /* x6 */
	spline.ctrlp[13] =  0.0f;  /* y6 */

	/* Stores our evaluation results. */
	tsDeBoorNet net;

	/* Evaluate `spline` at u = 0.4. */
	ts_bspline_evaluate(&spline, 0.4f, &net);
	printf("x = %f, y = %f\n", net.result[0], net.result[1]);
	ts_deboornet_free(&net);

	/* Stores our transformed spline. */
	tsBSpline beziers;
	/* Derive `spline` ... */
	ts_bspline_derive(&spline, &beziers);
	/* ... and subdivide it into a sequence of Bezier curves. */
	ts_bspline_to_beziers(&beziers, &beziers);

	/* Evaluate `beziers` at u = 0.3. */
	ts_bspline_evaluate(&beziers, 0.3f, &net);
	printf("x = %f, y = %f\n", net.result[0], net.result[1]);
	ts_deboornet_free(&net);
	
	/* Cleanup */
	ts_bspline_free(&spline);
	ts_bspline_free(&beziers);

	return 0;
}
