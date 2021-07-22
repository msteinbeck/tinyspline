#include <testutils.h>

void set_knots_custom_interval(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal knots[11], *result = NULL;

	___GIVEN___
	knots[0] = (tsReal) -2.0;
	knots[1] = (tsReal) -1.0;
	knots[2] = (tsReal) -1.0;
	knots[3] = (tsReal) -0.5;
	knots[4] = (tsReal)  1.25;
	knots[5] = (tsReal)  1.5;
	knots[6] = (tsReal)  1.75;
	knots[7] = (tsReal)  2.0;
	knots[8] = (tsReal)  3.0;
	knots[9] = (tsReal)  4.0;
	knots[10] = (tsReal) 7.0;

	C(ts_bspline_new(7, 2, 3, TS_OPENED, &spline, &status))

	___WHEN___
	C(ts_bspline_set_knots( &spline, knots, &status))

	___THEN___
	C(ts_bspline_knots( &spline, &result, &status))

	CuAssertTrue(tc, knots != result);
	CuAssertDblEquals(tc, -2.0,  result[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, -1.0,  result[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, -1.0,  result[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, -0.5,  result[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  1.25, result[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  1.5,  result[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  1.75, result[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  2.0,  result[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  3.0,  result[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  4.0,  result[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  7.0,  result[10], TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(result);
}

void set_knots_decreasing_knot_vector(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal knots[6], *result = NULL;
	tsError err = TS_SUCCESS;

	___GIVEN___
	knots[0] = (tsReal) 1.0;
	knots[1] = (tsReal) 1.0;
	knots[2] = (tsReal) 2.0;
	knots[3] = (tsReal) 2.0;
	knots[4] = (tsReal) 1.0;
	knots[5] = (tsReal) 3.0;

	C(ts_bspline_new(3, 2, 2, TS_CLAMPED, &spline, &status))
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	___WHEN___
	err = ts_bspline_set_knots(&spline, knots, NULL);

	___THEN___
	CuAssertIntEquals(tc, TS_KNOTS_DECR, err);

	/* Check if knots changed. */
	C(ts_bspline_knots(&spline, &result, &status))
	CuAssertTrue(tc, knots != result);
	CuAssertDblEquals(tc,
		TS_DOMAIN_DEFAULT_MIN,
		result[0],
		TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
		TS_DOMAIN_DEFAULT_MIN,
		result[1],
		TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
		TS_DOMAIN_DEFAULT_MIN,
		result[2],
		TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
		TS_DOMAIN_DEFAULT_MAX,
		result[3],
		TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
		TS_DOMAIN_DEFAULT_MAX,
		result[4],
		TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
		TS_DOMAIN_DEFAULT_MAX,
		result[5],
		TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(result);
}

void set_knots_exceeding_multiplicity(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal knots[6], *result = NULL;
	tsError err = TS_SUCCESS;

	___GIVEN___
	knots[0] = (tsReal) 2.0;
	knots[1] = (tsReal) 2.0;
	knots[2] = (tsReal) 2.0;
	knots[3] = (tsReal) 2.0;
	knots[4] = (tsReal) 3.0;
	knots[5] = (tsReal) 3.0;

	C(ts_bspline_new(3, 2, 2, TS_CLAMPED, &spline, &status))
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	___WHEN___
	err = ts_bspline_set_knots(&spline, knots, NULL);

	___THEN___
	CuAssertIntEquals(tc, TS_MULTIPLICITY, err);

	/* Check if knots changed. */
	C(ts_bspline_knots(&spline, &result, &status))
	CuAssertTrue(tc, knots != result);
	CuAssertDblEquals(tc,
			  TS_DOMAIN_DEFAULT_MIN,
			  result[0],
			  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
			  TS_DOMAIN_DEFAULT_MIN,
			  result[1],
			  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
			  TS_DOMAIN_DEFAULT_MIN,
			  result[2],
			  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
			  TS_DOMAIN_DEFAULT_MAX,
			  result[3],
			  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
			  TS_DOMAIN_DEFAULT_MAX,
			  result[4],
			  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
			  TS_DOMAIN_DEFAULT_MAX,
			  result[5],
			  TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(result);
}

void set_knot_at(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal knot, min, max, one = (tsReal) 1;

	___GIVEN___
	C(ts_bspline_new(2, 1, 1, TS_OPENED, &spline, &status))
	ts_bspline_domain(&spline, &min, &max);

	___WHEN___ /* 1 */
	C(ts_bspline_set_knot_at(&spline, 0, min - one, &status))

	___THEN___ /* 1 */
	C(ts_bspline_knot_at(&spline, 0, &knot, &status))
	CuAssertDblEquals(tc, min - one, knot, TS_KNOT_EPSILON);

	___WHEN___ /* 2 */
	C(ts_bspline_set_knot_at(&spline, 3, max + one, &status))

	___THEN___ /* 2 */
	C(ts_bspline_knot_at(&spline, 3, &knot, &status))
	CuAssertDblEquals(tc, max + one, knot, TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void set_knot_at_invalid_index(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal knot, min, max, one = (tsReal) 1.0;
	tsError err = TS_SUCCESS;

	___GIVEN___
	C(ts_bspline_new(1, 2, 0, TS_CLAMPED, &spline, &status))
	ts_bspline_domain(&spline, &min, &max);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	___WHEN___ /* 1 */   /* off-by-one */
	err = ts_bspline_set_knot_at(&spline, 2, max + one, &status);

	___THEN___ /* 1 */
	CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
	C(ts_bspline_knot_at(&spline, 0, &knot, &status))
	CuAssertDblEquals(tc, min, knot, TS_KNOT_EPSILON);
	C(ts_bspline_knot_at(&spline, 1, &knot, &status))
	CuAssertDblEquals(tc, max, knot, TS_KNOT_EPSILON);

	err = TS_SUCCESS; /* restore */

	___WHEN___ /* 2 */   /* off-by-many */
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	err = ts_bspline_set_knot_at(&spline, 10, max + one, &status);

	___THEN___ /* 2 */
	CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
	C(ts_bspline_knot_at(&spline, 0, &knot, &status))
	CuAssertDblEquals(tc, min, knot, TS_KNOT_EPSILON);
	C(ts_bspline_knot_at(&spline, 1, &knot, &status))
	CuAssertDblEquals(tc, max, knot, TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void set_knot_at_decreasing_knot_vector(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal knot, min, max, one = (tsReal) 1.0;
	tsError err = TS_SUCCESS;

	___GIVEN___
	C(ts_bspline_new(1, 2, 0, TS_CLAMPED, &spline, &status))
	ts_bspline_domain(&spline, &min, &max);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	___WHEN___
	err = ts_bspline_set_knot_at( &spline, 1, min - one, &status);

	___THEN___
	CuAssertIntEquals(tc, TS_KNOTS_DECR, err);
	C(ts_bspline_knot_at(&spline, 0, &knot, &status))
	CuAssertDblEquals(tc, min, knot, TS_KNOT_EPSILON);
	C(ts_bspline_knot_at(&spline, 1, &knot, &status))
	CuAssertDblEquals(tc, max, knot, TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void set_knot_at_exceeding_multiplicity(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal knot, min, max;
	tsError err = TS_SUCCESS;

	___GIVEN___
	C(ts_bspline_new(1, 2, 0, TS_CLAMPED, &spline, &status))
	ts_bspline_domain(&spline, &min, &max);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	___WHEN___
	err = ts_bspline_set_knot_at(&spline, 1, min, &status);

	___THEN___
	CuAssertIntEquals(tc, TS_MULTIPLICITY, err);
	C(ts_bspline_knot_at(&spline, 0, &knot, &status))
	CuAssertDblEquals(tc, min, knot, TS_KNOT_EPSILON);
	C(ts_bspline_knot_at(&spline, 1, &knot, &status))
	CuAssertDblEquals(tc, max, knot, TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

CuSuite* get_set_knots_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, set_knots_custom_interval);
	SUITE_ADD_TEST(suite, set_knots_decreasing_knot_vector);
	SUITE_ADD_TEST(suite, set_knots_exceeding_multiplicity);
	SUITE_ADD_TEST(suite, set_knot_at);
	SUITE_ADD_TEST(suite, set_knot_at_invalid_index);
	SUITE_ADD_TEST(suite, set_knot_at_decreasing_knot_vector);
	SUITE_ADD_TEST(suite, set_knot_at_exceeding_multiplicity);
	return suite;
}