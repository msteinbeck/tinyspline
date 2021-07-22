#include <testutils.h>

void insert_knot_once(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline result = ts_bspline_init();
	size_t k;
	tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		-1.75, -1.0,  /* P1 */
		-1.5,  -0.5,  /* P2 */
		-1.5,   0.0,  /* P3 */
		-1.25,  0.5,  /* P4 */
		-0.75,  0.75, /* P5 */
		 0.0,   0.5,  /* P6 */
		 0.5,   0.0)) /* P7 */

	/* Check initial knots. */
	CuAssertIntEquals(tc, 11, (int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[10], TS_KNOT_EPSILON);
	free(knots);
	knots = NULL;

	___WHEN___
	C(ts_bspline_insert_knot(&spline,
		(tsReal) 0.3, 1, &result, &k, &status))

	___THEN___
	CuAssertIntEquals(tc, 12, (int)ts_bspline_num_knots(&result));
	C(ts_bspline_knots(&result, &knots, &status))
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.3,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[10], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[11], TS_KNOT_EPSILON);
	CuAssertIntEquals(tc, 5, (int) k);

	assert_equal_shape(tc, &spline, &result);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&result);
	free(knots);
}

void insert_knot_twice(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline result = ts_bspline_init();
	size_t k;
	tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		-1.75, -1.0,  /* P1 */
		-1.5,  -0.5,  /* P2 */
		-1.5,   0.0,  /* P3 */
		-1.25,  0.5,  /* P4 */
		-0.75,  0.75, /* P5 */
		 0.0,   0.5,  /* P6 */
		 0.5,   0.0)) /* P7 */

	/* Check initial knots. */
	CuAssertIntEquals(tc, 11, (int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[10], TS_KNOT_EPSILON);
	free(knots);
	knots = NULL;

	___WHEN___
	C(ts_bspline_insert_knot(&spline,
		(tsReal) 0.6, 2, &result, &k, &status))

	___THEN___
	CuAssertIntEquals(tc, 13, (int)ts_bspline_num_knots(&result));
	C(ts_bspline_knots(&result, &knots, &status))
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[10],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[11], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[12], TS_KNOT_EPSILON);
	CuAssertIntEquals(tc, 7, (int) k);

	assert_equal_shape(tc, &spline, &result);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&result);
	free(knots);
}

void insert_knot_three_times(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline result = ts_bspline_init();
	size_t k;
	tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		-1.75, -1.0,  /* P1 */
		-1.5,  -0.5,  /* P2 */
		-1.5,   0.0,  /* P3 */
		-1.25,  0.5,  /* P4 */
		-0.75,  0.75, /* P5 */
		 0.0,   0.5,  /* P6 */
		 0.5,   0.0)) /* P7 */

	/* Check initial knots. */
	CuAssertIntEquals(tc, 11, (int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[10], TS_KNOT_EPSILON);
	free(knots);
	knots = NULL;

	___WHEN___
	C(ts_bspline_insert_knot(&spline,
		(tsReal) 0.8, 3, &result, &k, &status))

	___THEN___
	CuAssertIntEquals(tc, 14, (int)ts_bspline_num_knots(&result));
	C(ts_bspline_knots(&result, &knots, &status))
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.8,  knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.8,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.8,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[10],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[11],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[12], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[13], TS_KNOT_EPSILON);
	CuAssertIntEquals(tc, 9, (int) k);

	assert_equal_shape(tc, &spline, &result);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&result);
	free(knots);
}

void insert_knot_too_many(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline result = ts_bspline_init();
	tsError err;
	size_t k;

	___GIVEN___
	C(ts_bspline_new(7, 2, 3, TS_CLAMPED, &spline, &status))

	___WHEN___
	err = ts_bspline_insert_knot(&spline,
		(tsReal) 0.25, ts_bspline_degree(&spline) + 1,
		&result, &k, NULL);

	___THEN___
	CuAssertIntEquals(tc, TS_MULTIPLICITY, err);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&result);
}

void insert_knot_way_too_many(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline result = ts_bspline_init();
	tsError err;
	size_t k;

	___GIVEN___
	C(ts_bspline_new(7, 2, 3, TS_CLAMPED, &spline, &status))

	___WHEN___
	err = ts_bspline_insert_knot(&spline,
		(tsReal) 0.75, ts_bspline_degree(&spline) + 1 + 5,
		&result, &k, NULL);

	___THEN___
	CuAssertIntEquals(tc, TS_MULTIPLICITY, err);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&result);
}

CuSuite* get_insert_knot_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, insert_knot_once);
	SUITE_ADD_TEST(suite, insert_knot_twice);
	SUITE_ADD_TEST(suite, insert_knot_three_times);
	SUITE_ADD_TEST(suite, insert_knot_too_many);
	SUITE_ADD_TEST(suite, insert_knot_way_too_many);
	return suite;
}
