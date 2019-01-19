#include "tinyspline.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	tsStatus status;   /**< Used for error handling. */

	tsBSpline spline;  /**< The spline to setup. */
	tsReal *ctrlp;     /**< Pointer to the control points of `spline`. */

	tsBSpline beziers; /**< `spline` as a sequence of bezier curves. */

	tsDeBoorNet net;   /**< Used to evaluate `spline` and `beziers`. */
	tsReal *result;    /**< Pointer to the result of `net`. */

/* ------------------------------------------------------------------------- */
/* TinySpline includes a powerful, system-independent, and thread-safe error
 * handling system in the form of easy-to-use macros. All you need to do is to
 * embed your code into TS_TRY/TS_END_TRY and use TS_CALL when calling a
 * TinySpline function. Likewise, you can use any of the TS_THROW macros to
 * raise an error if an external function (e.g. malloc) failed.
 *
 * Errors can be handled in TS_CATCH. TS_FINALLY contains code that is executed
 * in any case, therefore being perfectly suitable for cleaning up resources.
 * Yet, error handling is entirely optional. You may omit TS_TRY, TS_CALL, and
 * TS_THROW and pass NULL instead of a pointer to a tsStatus object. */

	spline = ts_bspline_init();
	beziers = ts_bspline_init();
	net = ts_deboornet_init();
	ctrlp = result = NULL;
	TS_TRY(try, status.code, &status)
		/* Create a spline... */
		TS_CALL(try, status.code, ts_bspline_new(
			7, /* ... consisting of 7 control points... */
			2, /* ... in 2D... */
			3, /* ... of degree 3... */
			TS_CLAMPED, /* ... using a clamped knot vector. */
			&spline, &status))

		/* Setup control points of `spline`. */
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
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
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))

		/* Evaluate `spline` at u = 0.4. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, 0.4f, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		printf("x = %f, y = %f\n", result[0], result[1]);

		/* Derive `spline` ... */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, &beziers, &status))
		/* ... and subdivide it into a sequence of Bezier curves. */
		TS_CALL(try, status.code, ts_bspline_to_beziers(
			&beziers, &beziers, &status))

		ts_deboornet_free(&net);
		free(result);
		/* Evaluate `beziers` at u = 0.3. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&beziers, 0.3f, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		printf("x = %f, y = %f\n", result[0], result[1]);
	TS_CATCH(status.code)
		puts(status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&beziers);
		ts_deboornet_free(&net);
		if (ctrlp)
			free(ctrlp);
		if (result)
			free(result);
	TS_END_TRY

	return status.code? 1 : 0;
}
