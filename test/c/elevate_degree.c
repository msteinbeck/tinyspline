#include <tinyspline.h>
#include "CuTest.h"
#include "utils.h"

void elevate_degree_point(CuTest *tc)
{
/* ================================= Set Up ================================ */
	tsBSpline spline = ts_bspline_init();
	tsBSpline elevated = ts_bspline_init();
	size_t i;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new_with_control_points(
			1, 2, 0, TS_CLAMPED, &spline, &status,
			1024.0, 768.0))

		for (i = 1; i < 10; i++) {
/* ================================= When ================================== */
			TS_CALL(try, status.code, ts_bspline_elevate_degree(
				&spline, i, &elevated, &status))
/* ================================= Then ================================== */
			/* Elevated by `i'. */
			CuAssertIntEquals(tc,
				ts_bspline_degree(&spline) + i,
				ts_bspline_degree(&elevated));
			/* Each elevation inserts a control point. */
			CuAssertIntEquals(tc,
				ts_bspline_num_control_points(&spline) + i,
				ts_bspline_num_control_points(&elevated));
			/* Check internal state. */
			CuAssertIntEquals(tc,
				ts_bspline_num_control_points(&elevated)
					+ ts_bspline_order(&elevated),
				ts_bspline_num_knots(&elevated));
			assert_equal_shape(tc, &spline, &elevated);
			ts_bspline_free(&elevated);
		}
/* =============================== Tear Down =============================== */
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&elevated);
	TS_END_TRY
}

void elevate_degree_line(CuTest *tc)
{
/* ================================= Set Up ================================ */
	tsBSpline spline = ts_bspline_init();
	tsBSpline elevated = ts_bspline_init();
	size_t i;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new_with_control_points(
			2, 4, 1, TS_CLAMPED, &spline, &status,
			100.0, 20.0, 40.0, 1.0,
			200.0, 50.5, 38.3, 0.7))

		for (i = 1; i < 10; i++) {
/* ================================= When ================================== */
			TS_CALL(try, status.code, ts_bspline_elevate_degree(
				&spline, i, &elevated, &status))
/* ================================= Then ================================== */
			/* Elevated by `i'. */
			CuAssertIntEquals(tc,
				ts_bspline_degree(&spline) + i,
				ts_bspline_degree(&elevated));
			/* Each elevation inserts a control point. */
			CuAssertIntEquals(tc,
				ts_bspline_num_control_points(&spline) + i,
				ts_bspline_num_control_points(&elevated));
			/* Check internal state. */
			CuAssertIntEquals(tc,
				ts_bspline_num_control_points(&elevated)
					+ ts_bspline_order(&elevated),
				ts_bspline_num_knots(&elevated));
			assert_equal_shape(tc, &spline, &elevated);
			ts_bspline_free(&elevated);
		}
/* =============================== Tear Down =============================== */
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&elevated);
	TS_END_TRY
}

void elevate_degree_bezier_curve(CuTest *tc)
{
/* ================================= Set Up ================================ */
	tsBSpline spline = ts_bspline_init();
	tsBSpline elevated = ts_bspline_init();
	size_t i;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new_with_control_points(
			4, 2, 3, TS_CLAMPED, &spline, &status,
			1.7, 7.9,
			-9.2, -3.1,
			-0.1, 0.5,
			10.0, 15.0))

		for (i = 1; i < 10; i++) {
/* ================================= When ================================== */
			TS_CALL(try, status.code, ts_bspline_elevate_degree(
				&spline, i, &elevated, &status))
/* ================================= Then ================================== */
			/* Elevated by `i'. */
			CuAssertIntEquals(tc,
				  ts_bspline_degree(&spline) + i,
				  ts_bspline_degree(&elevated));
			/* Each elevation inserts a control point. */
			CuAssertIntEquals(tc,
				ts_bspline_num_control_points(&spline) + i,
				ts_bspline_num_control_points(&elevated));
			/* Check internal state. */
			CuAssertIntEquals(tc,
				ts_bspline_num_control_points(&elevated)
					+ ts_bspline_order(&elevated),
				ts_bspline_num_knots(&elevated));
			assert_equal_shape(tc, &spline, &elevated);
			ts_bspline_free(&elevated);
		}
/* =============================== Tear Down =============================== */
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&elevated);
	TS_END_TRY
}

void elevate_degree_bspline(CuTest *tc)
{
/* ================================= Set Up ================================ */
	tsBSpline spline = ts_bspline_init();
	tsBSpline elevated = ts_bspline_init();
	size_t i;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new_with_control_points(
			5, 2, 2, TS_CLAMPED, &spline, &status,
			150.0, 200.0,
			-150.0, -200.0,
			300.0, 400.0,
			-300.0, -400.0,
			0.0, 0.0))

		for (i = 1; i < 10; i++) {
/* ================================= When ================================== */
			TS_CALL(try, status.code, ts_bspline_elevate_degree(
				&spline, i, &elevated, &status))
/* ================================= Then ================================== */
			/* Elevated by `i'. */
			CuAssertIntEquals(tc,
				ts_bspline_degree(&spline) + i,
				ts_bspline_degree(&elevated));
			/* Each elevation inserts at least one control
			 * point. */
			CuAssertTrue(tc,
				ts_bspline_num_control_points(&elevated) >
				ts_bspline_num_control_points(&spline));
			/* Check internal state. */
			CuAssertIntEquals(tc,
				ts_bspline_num_control_points(&elevated)
					+ ts_bspline_order(&elevated),
				ts_bspline_num_knots(&elevated));
			assert_equal_shape(tc, &spline, &elevated);
			ts_bspline_free(&elevated);
		}
/* =============================== Tear Down =============================== */
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&elevated);
	TS_END_TRY
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