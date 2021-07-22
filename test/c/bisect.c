#include <testutils.h>

void
assert_bisect_eval_equal(CuTest *tc, tsBSpline *spline, size_t idx, int asc)
{
	tsDeBoorNet net_eval = ts_deboornet_init();
	tsDeBoorNet net_bisect = ts_deboornet_init();
	tsReal *result_eval = NULL, *result_bisect = NULL;
	tsReal min, max, knot, dist;
	size_t k;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		ts_bspline_domain(spline, &min, &max);
		for (k = 0; k < TS_MAX_NUM_KNOTS; k++) {
			knot = (tsReal)k / TS_MAX_NUM_KNOTS;
			knot = ( (max - min) * knot ) + min;

			/* Eval the point to bisect. */
			TS_CALL(try, status.code, ts_bspline_eval(
				spline, knot, &net_eval, &status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net_eval, &result_eval, &status))

			/* Bisect the corresponding point. */
			TS_CALL(try, status.code, ts_bspline_bisect(
				spline, result_eval[idx], (tsReal) 0.0, 0,
				idx, asc, 50, &net_bisect, &status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net_bisect, &result_bisect, &status))

			/* Compare knots and results. */
			CuAssertDblEquals(tc, ts_deboornet_knot(&net_eval),
				ts_deboornet_knot(&net_bisect),
				TS_KNOT_EPSILON);
			dist = ts_distance(result_eval, result_bisect,
				ts_bspline_dimension(spline));
			CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

			/* Cleanup. */
			ts_deboornet_free(&net_eval);
			ts_deboornet_free(&net_bisect);
			free(result_eval);
			free(result_bisect);
			result_eval = result_bisect = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(spline);
		ts_deboornet_free(&net_eval);
		ts_deboornet_free(&net_bisect);
		free(result_eval);
		free(result_bisect);
	TS_END_TRY
}

void bisect_compare_with_eval_x_coordinate(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		6, 3, 3, TS_OPENED, &spline, &status,
		1.0,  0.5,  0.3,  /* P1 */
		2.0,  1.5, -1.6,  /* P2 */
		4.0, -3.0, -2.9,  /* P3 */
		4.5, -4.1, -1.0,  /* P4 */
		4.9, -5.5,  1.3,  /* P5 */
		6.8, -6.3,  2.6)) /* P6 */

	___WHEN___

	___THEN___
	assert_bisect_eval_equal(tc, &spline, 0, 1);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void bisect_compare_with_eval_y_coordinate(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		5, 2, 2, TS_CLAMPED, &spline, &status,
		 0.0, -0.7,  /* P1 */
		-3.7,  2.6,  /* P2 */
		-1.6,  3.0,  /* P3 */
		-0.5,  3.1,  /* P4 */
		 2.9,  5.6)) /* P5 */

	___WHEN___

	___THEN___
	assert_bisect_eval_equal(tc, &spline, 1, 1);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void bisect_compare_with_eval_z_coordinate(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		3, 3, 1, TS_CLAMPED, &spline, &status,
		1.0, 4.0, -3.0,  /* P1 */
		2.0, 2.0, -1.6,  /* P2 */
		4.0, 1.0,  2.8)) /* P3 */

	___WHEN___

	___THEN___
	assert_bisect_eval_equal(tc, &spline, 2, 1);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void bisect_less_than_first_control_point(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal first[2] = {100.0, 200.0}, *result = NULL;
	tsReal min, max, knot, dist;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		5, 2, 2, TS_CLAMPED, &spline, &status,
		first[0],  first[1], /* P1 */
		200.0,  300.0,       /* P2 */
		400.0,  600.0,       /* P3 */
		800.0,  450.0,       /* P4 */
		1200.0, 120.0))      /* P5 */
	ts_bspline_domain(&spline, &min, &max);

	___WHEN___
	C(ts_bspline_bisect(&spline, first[0] - 200,
		0, 0, 0, 1, 50, &net, &status))

	___THEN___
	knot = ts_deboornet_knot(&net);
	CuAssertDblEquals(tc, min, knot, TS_KNOT_EPSILON);

	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance(first, result, ts_bspline_dimension(&spline));
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
	result = NULL;
}

void bisect_greater_than_last_control_point(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal last[2] = {1200.0, 120.0}, *result = NULL;
	tsReal min, max, knot, dist;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		5, 2, 3, TS_CLAMPED, &spline, &status,
		100.0, 200.0,      /* P1 */
		200.0, 300.0,      /* P2 */
		400.0, 600.0,      /* P3 */
		800.0, 450.0,      /* P4 */
		last[0], last[1])) /* P5 */
	ts_bspline_domain(&spline, &min, &max);

	___WHEN___
	C(ts_bspline_bisect(&spline, last[0] + 100,
		0, 0, 0, 1, 50, &net, &status))

	___THEN___
	knot = ts_deboornet_knot(&net);
	CuAssertDblEquals(tc, max, knot, TS_KNOT_EPSILON);

	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance(last, result, ts_bspline_dimension(&spline));
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
	result = NULL;
}

void bisect_invalid_index(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsError err;
	tsStatus stat;

	___GIVEN___
	C(ts_bspline_new(16, 3, 3, TS_OPENED, &spline, &status))

	___WHEN___ /* 1 */
	/* Check index off-by-one without status. */
	err = ts_bspline_bisect(&spline, (tsReal) 0.0, (tsReal) 0.0, 0,
		4 /**< index */, 1, 50, &net, NULL);

	___THEN___ /* 1 */
	CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
	CuAssertPtrEquals(tc, NULL, net.pImpl);

	___WHEN___ /* 2 */
	/* Check index off-by-one with status. */
	stat.code = TS_SUCCESS;
	err = ts_bspline_bisect(&spline, (tsReal) 0.0, (tsReal) 0.0, 0,
		4 /**< index */, 1, 50, &net, &stat);

	___THEN___ /* 2 */
	CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
	CuAssertPtrEquals(tc, NULL, net.pImpl);
	CuAssertIntEquals(tc, TS_INDEX_ERROR, stat.code);

	___WHEN___ /* 3 */
	/* Check another invalid index without status. */
	err = ts_bspline_bisect(&spline, (tsReal) 0.0, (tsReal) 0.0, 0,
		8 /**< index */, 1, 50, &net, NULL);

	___THEN___ /* 3 */
		CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
		CuAssertPtrEquals(tc, NULL, net.pImpl);

	___WHEN___ /* 4 */
	/* Check another invalid index with status. */
	stat.code = TS_SUCCESS;
	err = ts_bspline_bisect(&spline, (tsReal) 0.0, (tsReal) 0.0, 0,
		8 /**< index */, 1, 50, &net, &stat);

	___THEN___ /* 4 */
	CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
	CuAssertPtrEquals(tc, NULL, net.pImpl);
	CuAssertIntEquals(tc, TS_INDEX_ERROR, stat.code);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
}

void bisect_max_iter_0(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsError err;
	tsStatus stat;

	___GIVEN___
	stat.code = TS_SUCCESS;
	C(ts_bspline_new(12, 2, 6, TS_CLAMPED, &spline, &status))

	___WHEN___ /* 1 */
	/* Check max_iter = 0 without status. */
	err = ts_bspline_bisect(&spline, (tsReal) 0.0, (tsReal) 0.0, 0, 0, 1,
		0 /**< max_iter */, &net, NULL);

	___THEN___ /* 1 */
	CuAssertIntEquals(tc, TS_NO_RESULT, err);
	CuAssertPtrEquals(tc, NULL, net.pImpl);

	___WHEN___ /* 2 */
	/* Check max_iter = 0 with status. */
	stat.code = TS_SUCCESS;
	err = ts_bspline_bisect(&spline, (tsReal) 0.0, (tsReal) 0.0, 0, 0, 1,
		0 /**< max_iter */, &net, &stat);

	___THEN___ /* 2 */
	CuAssertIntEquals(tc, TS_NO_RESULT, err);
	CuAssertPtrEquals(tc, NULL, net.pImpl);
	CuAssertIntEquals(tc, TS_NO_RESULT, stat.code);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
}

void bisect_descending_compare_with_eval(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		6, 3, 3, TS_OPENED, &spline, &status,
		1.0,  9.0,  0.3,  /* P1 */
		2.0,  8.5, -1.6,  /* P2 */
		4.0,  5.4, -2.9,  /* P3 */
		4.5,  0.0, -1.0,  /* P4 */
		4.9, -3.6,  1.3,  /* P5 */
		6.8, -6.3,  2.6)) /* P6 */

	___WHEN___

	___THEN___
	assert_bisect_eval_equal(tc, &spline, 1, 0);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void bisect_persnickety(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsError err;
	tsStatus stat;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		6, 1, 4, TS_OPENED, &spline, &status,
		1.0,  /* P1 */
		1.5,  /* P2 */
		2.0,  /* P3 */
		3.0,  /* P4 */
		6.7,  /* P5 */
		7.0)) /* P6 */

	___WHEN___ /* 1 */
	/* Check persnickety without status. */
	err = ts_bspline_bisect(&spline, 6.0, (tsReal) 0.0,
		1 /**< persnickety */, 0, 1, 2, &net, NULL);

	___THEN___ /* 1 */
	CuAssertIntEquals(tc, TS_NO_RESULT, err);
	CuAssertPtrEquals(tc, NULL, net.pImpl);

	___WHEN___ /* 2 */
	/* Check persnickety with status. */
	stat.code = TS_SUCCESS;
	err = ts_bspline_bisect(&spline, 6.0, (tsReal) 0.0,
		1 /**< persnickety */, 0, 1, 2, &net, &stat);

	___THEN___ /* 2 */
	CuAssertIntEquals(tc, TS_NO_RESULT, err);
	CuAssertPtrEquals(tc, NULL, net.pImpl);
	CuAssertIntEquals(tc, TS_NO_RESULT, stat.code);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
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
	SUITE_ADD_TEST(suite, bisect_descending_compare_with_eval);
	SUITE_ADD_TEST(suite, bisect_persnickety);
	return suite;
}