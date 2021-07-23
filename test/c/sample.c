#include <testutils.h>

void sample_num_1(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal dist, *points = NULL;
	size_t num;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 3, TS_CLAMPED, &spline, &status,
		-1.75, 1.0,
		-1.5, -0.5,
		-1.5,  0.0,
		-1.25, 0.5))

	___WHEN___
	C(ts_bspline_sample(&spline, 1, &points, &num, &status))

	___THEN___
	CuAssertTrue(tc, num == 1);
	dist = ts_distance_varargs(tc, 2, points, -1.75, 1.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(points);
}

void sample_num_2(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal dist, *points = NULL;
	size_t num;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 3, TS_CLAMPED, &spline, &status,
		-1.75,  1.0,
		-1.5,  -0.5,
		-1.5,   0.0,
		 1.25, -0.5))

	___WHEN___
	C(ts_bspline_sample(&spline, 2, &points, &num, &status))

	___THEN___
	CuAssertTrue(tc, num == 2);
	dist = ts_distance_varargs(tc, 2, points, -1.75, 1.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, points + 2, 1.25, -0.5);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(points);
}

void sample_num_3(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal dist, min, max, *middle = NULL, *points = NULL;
	size_t num;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 3, TS_CLAMPED, &spline, &status,
		-1.75,  1.0,
		-1.5,   0.5,
		-1.5,   0.0,
		 1.25, -0.5))

	ts_bspline_domain(&spline, &min, &max);
	C(ts_bspline_eval(&spline, (max - min) / (tsReal) 2.0, &net, &status))
	C(ts_deboornet_result(&net, &middle, &status))

	___WHEN___
	C(ts_bspline_sample(&spline, 3, &points, &num, &status))

	___THEN___
	CuAssertTrue(tc, num == 3);
	dist = ts_distance_varargs(tc, 2, points, -1.75, 1.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance(points + 2, middle, 2);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, points + 4, 1.25, -0.5);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(middle);
	free(points);
}

void sample_compare_with_bisect(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal dist, *points = NULL, *result = NULL;
	tsReal *point; /**< Points to the current point in points. */
	size_t i, num;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		-1.75, -1.0,  /* P1 */
		-1.5,  -0.5,  /* P2 */
		-1.3,   0.0,  /* P3 */
		-1.25,  0.5,  /* P4 */
		-0.75,  0.75, /* P5 */
		 0.0,   0.5,  /* P6 */
		 0.5,   0.0)) /* P7 */

	___WHEN___
	C(ts_bspline_sample(&spline, 100, &points, &num, &status))

	___THEN___
	for (i = 0; i < num; i++) {
		point = points + i * ts_bspline_dimension(&spline);
		C(ts_bspline_bisect(&spline, *point,
			(tsReal) 0.0, 0, 0, 1, 50, &net, &status))
		C(ts_deboornet_result(&net, &result, &status))

		dist = ts_distance(point, result,
			ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

		ts_deboornet_free(&net);
		free(result);
		result = NULL;
	}

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(points);
	free(result);
}

void sample_default_num(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal *points = NULL;
	size_t num = 0;

	___GIVEN___
	C(ts_bspline_new(14, 3, 6, TS_OPENED, &spline, &status))

	___WHEN___
	C(ts_bspline_sample(&spline, 0 /* Use default num. */,
		&points, &num, &status))

	___THEN___
	CuAssertTrue(tc, num > 0);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(points);
}

CuSuite* get_sample_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, sample_num_1);
	SUITE_ADD_TEST(suite, sample_num_2);
	SUITE_ADD_TEST(suite, sample_num_3);
	SUITE_ADD_TEST(suite, sample_compare_with_bisect);
	SUITE_ADD_TEST(suite, sample_default_num);
	return suite;
}