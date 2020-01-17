#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define EPSILON 0.0001

void bisect_compare_with_eval_x_coordinate(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net_eval = ts_deboornet_init();
	tsDeBoorNet net_bisect = ts_deboornet_init();
	tsReal *result_eval = NULL, *result_bisect = NULL;
	tsReal min, max, knot, dist;
	tsStatus status;

	tsReal ctrlp[18] = {
		1.0,  0.5,  0.3,
		2.0,  1.5, -1.6,
		4.0, -3.0, -2.9,
		4.5, -4.1, -1.0,
		4.9, -5.5,  1.3,
		6.8, -6.3,  2.6
	};

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			6, 3, 3, TS_OPENED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		ts_bspline_domain(&spline, &min, &max);

		/* Compare eval with bisect. */
		for (knot = min; knot < max;
			knot += (max - min) / TS_MAX_NUM_KNOTS) {

			/* Eval the point to bisect. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&spline, knot, &net_eval, &status));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net_eval, &result_eval, &status))

			/* Bisect the corresponding point. */
			TS_CALL(try, status.code, ts_bspline_bisect(
				&spline, result_eval[0], 0.f, 0, 0, 1, 50,
				&net_bisect, &status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net_bisect, &result_bisect, &status))

			/* Compare knots and results. */
			CuAssertDblEquals(tc, ts_deboornet_knot(&net_eval),
				ts_deboornet_knot(&net_bisect), EPSILON);
			dist = ts_distance(result_eval, result_bisect,
				ts_bspline_dimension(&spline));
			CuAssertDblEquals(tc, 0, dist, EPSILON);

			/* Cleanup. */
			ts_deboornet_free(&net_eval);
			ts_deboornet_free(&net_bisect);
			free(result_eval);
			result_eval = NULL;
			free(result_bisect);
			result_bisect = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net_eval);
		ts_deboornet_free(&net_bisect);
		free(result_eval);
		free(result_bisect);
	TS_END_TRY
}

CuSuite* get_bisect_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, bisect_compare_with_eval_x_coordinate);
	return suite;
}