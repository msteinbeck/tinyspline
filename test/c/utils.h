#ifndef TINYSPLINE_UTILS_H
#define TINYSPLINE_UTILS_H

#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define CTRLP_EPSILON 0.00001

/**
 * Asserts that \p s1 and \p s2 have equal shape by evaluating them at
 * different knots and comparing the distance of the evaluated points with
 * ::CTRLP_EPSILON. The given splines may have different dimensions. In this
 * case, the distance is computed with the lower dimension. If the assertion
 * fails, \p s1 and \p s2 are freed.
 *
 * @param tc
 * 	The active test container.
 * @param s1
 * 	First spline.
 * @param s2
 * 	Second spline.
 */
void assert_equal_shape(CuTest *tc, tsBSpline *s1, tsBSpline *s2)
{
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *s1val = NULL, *s2val = NULL, dist;
	size_t k, dim;
	tsStatus status;

	/* Find minimum dimension. */
	dim = ts_bspline_dimension(s1);
	if (ts_bspline_dimension(s2) < dim)
		dim = ts_bspline_dimension(s2);

	TS_TRY(try, status.code, &status)
		for (k = 0; k < TS_MAX_NUM_KNOTS; k++) {
			/* Eval s1. */
			TS_CALL(try, status.code, ts_bspline_eval(
				s1, (tsReal)k / TS_MAX_NUM_KNOTS, &net,
				&status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &s1val, &status))
			ts_deboornet_free(&net);

			/* Eval s2. */
			TS_CALL(try, status.code, ts_bspline_eval(
				s2, (tsReal)k / TS_MAX_NUM_KNOTS, &net,
				&status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &s2val, &status))
			ts_deboornet_free(&net);

			/* Compare results. */
			dist = ts_distance(s1val, s2val, dim);
			CuAssertDblEquals(tc, 0, dist, CTRLP_EPSILON);

			/* Clean up. */
			free(s1val);
			free(s2val);
			s1val = s2val = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
		ts_bspline_free(s1);
		ts_bspline_free(s2);
	TS_FINALLY
		ts_deboornet_free(&net);
		free(s1val);
		free(s2val);
	TS_END_TRY
}

#endif //TINYSPLINE_UTILS_H
