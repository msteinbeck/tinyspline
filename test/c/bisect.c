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

void bisect_compare_with_eval_y_coordinate(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net_eval = ts_deboornet_init();
	tsDeBoorNet net_bisect = ts_deboornet_init();
	tsReal *result_eval = NULL, *result_bisect = NULL;
	tsReal min, max, knot, dist;
	tsStatus status;

	tsReal ctrlp[10] = {
		 0.0, -0.7,
		-3.7,  2.6,
		-1.6,  3.0,
		-0.5,  3.1,
		 2.9,  5.6
	};

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			5, 2, 2, TS_CLAMPED, &spline, &status))
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
				&spline, result_eval[1], 0.f, 0, 1, 1, 50,
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

void bisect_compare_with_eval_z_coordinate(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net_eval = ts_deboornet_init();
	tsDeBoorNet net_bisect = ts_deboornet_init();
	tsReal *result_eval = NULL, *result_bisect = NULL;
	tsReal min, max, knot, dist;
	tsStatus status;

	tsReal ctrlp[9] = {
		1.0, 4.0, -3.0,
		2.0, 2.0, -1.6,
		4.0, 1.0,  2.8,
	};

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			3, 3, 1, TS_CLAMPED, &spline, &status))
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
				&spline, result_eval[2], 0.f, 0, 2, 1, 50,
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

void bisect_less_than_first_control_point(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result = NULL;
	tsReal min, max, knot, dist;
	tsStatus status;

	tsReal ctrlp[10] = {
		100,  200,
		200,  300,
		400,  600,
		800,  450,
		1200, 120
	};

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			5, 2, 2, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		ts_bspline_domain(&spline, &min, &max);

		/* Bisect a point that is less than ctrlp[0] */
		TS_CALL(try, status.code, ts_bspline_bisect(
			&spline, ctrlp[0]-100, 0, 0, 0, 1, 50,
			&net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))

		/* Check knot and result. */
		knot = ts_deboornet_knot(&net);
		CuAssertDblEquals(tc, min, knot, EPSILON);
		dist = ts_distance(ctrlp, result,
			ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
		free(result);
		result = NULL;
	TS_END_TRY
}

void bisect_greater_than_last_control_point(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result = NULL;
	tsReal min, max, knot, dist;
	tsStatus status;

	tsReal ctrlp[10] = {
		100,  200,
		200,  300,
		400,  600,
		800,  450,
		1200, 120
	};

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			5, 2, 3, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		ts_bspline_domain(&spline, &min, &max);

		/* Bisect a point that is greater than ctrlp[8] */
		TS_CALL(try, status.code, ts_bspline_bisect(
			&spline, ctrlp[8]+100, 0, 0, 0, 1, 50,
			&net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))

		/* Check knot and result. */
		knot = ts_deboornet_knot(&net);
		CuAssertDblEquals(tc, max, knot, EPSILON);
		dist = ts_distance(ctrlp+8, result,
			ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
		free(result);
		result = NULL;
	TS_END_TRY
}

void bisect_invalid_index(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsError err;
	tsStatus status, stat;

	TS_TRY(try, status.code, &status)
		/* Create arbitrary spline. */
		TS_CALL(try, status.code, ts_bspline_new(
			16, 3, 3, TS_OPENED, &spline, &status))

		/* Check index off-by-one without status. */
		err = ts_bspline_bisect(&spline, 0.f, 0.f, 0,
			4 /**< index */, 1, 50, &net, NULL);
		CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
		CuAssertPtrEquals(tc, NULL, net.pImpl);

		/* Check index off-by-one with status. */
		stat.code = TS_SUCCESS;
		err = ts_bspline_bisect(&spline, 0.f, 0.f, 0,
			4 /**< index */, 1, 50, &net, &stat);
		CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
		CuAssertPtrEquals(tc, NULL, net.pImpl);
		CuAssertIntEquals(tc, TS_INDEX_ERROR, stat.code);

		/* Check another invalid index without status. */
		err = ts_bspline_bisect(&spline, 0.f, 0.f, 0,
			8 /**< index */, 1, 50, &net, NULL);
		CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
		CuAssertPtrEquals(tc, NULL, net.pImpl);

		/* Check another invalid index with status. */
		stat.code = TS_SUCCESS;
		err = ts_bspline_bisect(&spline, 0.f, 0.f, 0,
			8 /**< index */, 1, 50, &net, &stat);
		CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
		CuAssertPtrEquals(tc, NULL, net.pImpl);
		CuAssertIntEquals(tc, TS_INDEX_ERROR, stat.code);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
	TS_END_TRY
}

void bisect_max_iter_0(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsError err;
	tsStatus status, stat;

	stat.code = TS_SUCCESS;
	TS_TRY(try, status.code, &status)
		/* Create arbitrary spline. */
		TS_CALL(try, status.code, ts_bspline_new(
			12, 2, 6, TS_CLAMPED, &spline, &status))

		/* Check max_iter = 0 without status. */
		err = ts_bspline_bisect(&spline, 0.f, 0.f, 0,  0, 1,
			0 /**< max_iter */, &net, NULL);
		CuAssertIntEquals(tc, TS_NO_RESULT, err);
		CuAssertPtrEquals(tc, NULL, net.pImpl);

		/* Check max_iter = 0 with status. */
		stat.code = TS_SUCCESS;
		err = ts_bspline_bisect(&spline, 0.f, 0.f, 0,  0, 1,
			0 /**< max_iter */, &net, &stat);
		CuAssertIntEquals(tc, TS_NO_RESULT, err);
		CuAssertPtrEquals(tc, NULL, net.pImpl);
		CuAssertIntEquals(tc, TS_NO_RESULT, stat.code);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
	TS_END_TRY
}

CuSuite* get_bisect_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, bisect_compare_with_eval_x_coordinate);
	SUITE_ADD_TEST(suite, bisect_compare_with_eval_y_coordinate);
	SUITE_ADD_TEST(suite, bisect_compare_with_eval_z_coordinate);
	SUITE_ADD_TEST(suite, bisect_less_than_first_control_point);
	SUITE_ADD_TEST(suite, bisect_greater_than_last_control_point);
	SUITE_ADD_TEST(suite, bisect_invalid_index);
	SUITE_ADD_TEST(suite, bisect_max_iter_0);
	return suite;
}