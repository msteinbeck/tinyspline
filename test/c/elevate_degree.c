#include <testutils.h>

void elevate_degree_point(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline elevated = ts_bspline_init();
	size_t i;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		1, 2, 0, TS_CLAMPED, &spline, &status,
		1024.0, 768.0)) /* P1 */

	for (i = 1; i < 10; i++) {
		___WHEN___
		C(ts_bspline_elevate_degree(&spline, i, POINT_EPSILON,
			&elevated, &status))

		___THEN___
		/* Elevated by `i'. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_degree(&spline) + i),
			(int) ts_bspline_degree(&elevated));
		/* Each elevation inserts a control point. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_num_control_points(&spline) + i),
			(int) ts_bspline_num_control_points(&elevated));
		/* Check internal state. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_num_control_points(&elevated)
				+ ts_bspline_order(&elevated)),
			(int) ts_bspline_num_knots(&elevated));
		assert_equal_shape(tc, &spline, &elevated);

		ts_bspline_free(&elevated);
	}

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&elevated);
}

void elevate_degree_line(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline elevated = ts_bspline_init();
	size_t i;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		2, 4, 1, TS_CLAMPED, &spline, &status,
		100.0, 20.0, 40.0, 1.0,  /* P1 */
		200.0, 50.5, 38.3, 0.7)) /* P2 */

	for (i = 1; i < 10; i++) {
		___WHEN___
		C(ts_bspline_elevate_degree(&spline, i, POINT_EPSILON,
			&elevated, &status))

		___THEN___
		/* Elevated by `i'. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_degree(&spline) + i),
			(int) ts_bspline_degree(&elevated));
		/* Each elevation inserts a control point. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_num_control_points(&spline) + i),
			(int) ts_bspline_num_control_points(&elevated));
		/* Check internal state. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_num_control_points(&elevated)
				+ ts_bspline_order(&elevated)),
			(int) ts_bspline_num_knots(&elevated));
		assert_equal_shape(tc, &spline, &elevated);

		ts_bspline_free(&elevated);
	}

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&elevated);
}

void elevate_degree_bezier_curve(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline elevated = ts_bspline_init();
	size_t i;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 3, TS_CLAMPED, &spline, &status,
		1.7, 7.9,    /* P1 */
		-9.2, -3.1,  /* P2 */
		-0.1, 0.5,   /* P3 */
		10.0, 15.0)) /* P4 */

	for (i = 1; i < 10; i++) {
		___WHEN___
		C(ts_bspline_elevate_degree(&spline, i, POINT_EPSILON,
			&elevated, &status))

		___THEN___
		/* Elevated by `i'. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_degree(&spline) + i),
			(int) ts_bspline_degree(&elevated));
		/* Each elevation inserts a control point. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_num_control_points(&spline) + i),
			(int) ts_bspline_num_control_points(&elevated));
		/* Check internal state. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_num_control_points(&elevated)
				+ ts_bspline_order(&elevated)),
			(int) ts_bspline_num_knots(&elevated));
		assert_equal_shape(tc, &spline, &elevated);

		ts_bspline_free(&elevated);
	}

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&elevated);
}

void elevate_degree_bspline(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline elevated = ts_bspline_init();
	size_t i;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		5, 2, 2, TS_CLAMPED, &spline, &status,
		150.0, 200.0,   /* P1 */
		-150.0, -200.0, /* P2 */
		300.0, 400.0,   /* P3 */
		-300.0, -400.0, /* P4 */
		0.0, 0.0))      /* P5 */

	for (i = 1; i < 10; i++) {
		___WHEN___
		C(ts_bspline_elevate_degree(&spline, i, POINT_EPSILON,
			&elevated, &status))

		___THEN___
		/* Elevated by `i'. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_degree(&spline) + i),
			(int) ts_bspline_degree(&elevated));
		/* Each elevation inserts at least one control point. */
		CuAssertTrue(tc,
			ts_bspline_num_control_points(&elevated) >
			ts_bspline_num_control_points(&spline));
		/* Check internal state. */
		CuAssertIntEquals(tc,
			(int) (ts_bspline_num_control_points(&elevated)
				+ ts_bspline_order(&elevated)),
			(int) ts_bspline_num_knots(&elevated));
		assert_equal_shape(tc, &spline, &elevated);

		ts_bspline_free(&elevated);
	}

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&elevated);
}

CuSuite* get_elevate_degree_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, elevate_degree_point);
	SUITE_ADD_TEST(suite, elevate_degree_line);
	SUITE_ADD_TEST(suite, elevate_degree_bezier_curve);
	SUITE_ADD_TEST(suite, elevate_degree_bspline);
	return suite;
}
