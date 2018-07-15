#include "tinyspline.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	tsBSpline spline;  /**< The spline to setup. */
	tsReal *ctrlp;     /**< Pointer to the control points of `spline`. */

	tsBSpline beziers; /**< `spline` as a sequence of bezier curves. */

	tsDeBoorNet net;   /**< Used to evaluate `spline` and `beziers`. */
	tsReal *result;    /**< Pointer to the result of `net`. */

/* ------------------------------------------------------------------------- */

	/* Create a spline... */
	ts_bspline_new(
		7, /* ... consisting of 7 control points... */
		2, /* ... in 2D... */
		3, /* ... of degree 3... */
		TS_CLAMPED, /* ... using a clamped knot vector. */
		&spline
	);

	/* Setup control points of `spline`. */
	ctrlp = ts_bspline_control_points(&spline);
	ctrlp[0]  = -1.75f; /* x0 */
	ctrlp[1]  = -1.0f;  /* y0 */
	ctrlp[2]  = -1.5f;  /* x1 */
	ctrlp[3]  = -0.5f;  /* y1 */
	ctrlp[4]  = -1.5f;  /* x2 */
	ctrlp[5]  =  0.0f;  /* y2 */
	ctrlp[6]  = -1.25f; /* x3 */
	ctrlp[7]  =  0.5f;  /* y3 */
	ctrlp[8]  = -0.75f; /* x4 */
	ctrlp[9]  =  0.75f; /* y4 */
	ctrlp[10] =  0.0f;  /* x5 */
	ctrlp[11] =  0.5f;  /* y5 */
	ctrlp[12] =  0.5f;  /* x6 */
	ctrlp[13] =  0.0f;  /* y6 */
	ts_bspline_set_control_points(&spline, ctrlp);
	free(ctrlp);

	/* Evaluate `spline` at u = 0.4. */
	ts_bspline_eval(&spline, 0.4f, &net);
	result = ts_deboornet_result(&net);
	printf("x = %f, y = %f\n", result[0], result[1]);
	ts_deboornet_free(&net);
	free(result);

	/* Derive `spline` ... */
	ts_bspline_derive(&spline, &beziers);
	/* ... and subdivide it into a sequence of Bezier curves. */
	ts_bspline_to_beziers(&beziers, &beziers);

	/* Evaluate `beziers` at u = 0.3. */
	ts_bspline_eval(&beziers, 0.3f, &net);
	result = ts_deboornet_result(&net);
	printf("x = %f, y = %f\n", result[0], result[1]);
	ts_deboornet_free(&net);
	free(result);
	
	/* Cleanup `spline` and `beziers`. */
	ts_bspline_free(&spline);
	ts_bspline_free(&beziers);

	return 0;
}
