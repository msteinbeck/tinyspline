#include <tinyspline.h>
#include "CuTest.h"
#include "utils.h"

void assert_compatible(CuTest *tc, tsBSpline *s1, tsBSpline *s2)
{
	CuAssertIntEquals(tc,
		ts_bspline_degree(s1),
		ts_bspline_degree(s2));
	CuAssertIntEquals(tc,
		ts_bspline_num_control_points(s1),
		ts_bspline_num_control_points(s2));
	CuAssertIntEquals(tc,
		ts_bspline_num_knots(s1),
		ts_bspline_num_knots(s2));
}

void align_point_with_spline(CuTest *tc)
{
/* ================================= Set Up ================================ */
	tsBSpline point = ts_bspline_init();
	tsBSpline point_aligned = ts_bspline_init();
	tsBSpline spline = ts_bspline_init();
	struct tsBSplineImpl *spline_ptr;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new_with_control_points(
			1, 4, 0, TS_CLAMPED, &point, &status,
			100.0, 200.0, -100.0, -500.5,
			300.0, 400.0, -200.0, 456.45))
		TS_CALL(try, status.code, ts_bspline_new(
			/* Arbitrary control points. */
			10, 2, 3, TS_CLAMPED, &spline, &status))
		spline_ptr = spline.pImpl;
/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_align(
			&spline, &point, CTRLP_EPSILON, &spline,
			&point_aligned, &status))
/* ================================= Then ================================== */
		CuAssertTrue(tc, point.pImpl != point_aligned.pImpl);
		CuAssertTrue(tc, spline.pImpl == spline_ptr);
		assert_equal_shape(tc, &point, &point_aligned);
		assert_compatible(tc, &point_aligned, &spline);
/* =============================== Tear Down =============================== */
	TS_FINALLY
		ts_bspline_free(&point);
		ts_bspline_free(&spline);
		ts_bspline_free(&point_aligned);
	TS_END_TRY
}

void align_line_with_spline(CuTest *tc)
{
/* ================================= Set Up ================================ */
	tsBSpline line = ts_bspline_init();
	tsBSpline line_aligned = ts_bspline_init();
	tsBSpline spline = ts_bspline_init();
	struct tsBSplineImpl *spline_ptr;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new_with_control_points(
			2, 3, 1, TS_CLAMPED, &line, &status,
			100.0, 200.0, -10.0,
			300.0, 400.0, -20.0))
		TS_CALL(try, status.code, ts_bspline_new(
			/* Arbitrary control points. */
			5, 2, 3, TS_CLAMPED, &spline, &status))
		spline_ptr = spline.pImpl;
/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_align(
			&line, &spline, CTRLP_EPSILON, &line_aligned, &spline,
			&status))
/* ================================= Then ================================== */
		CuAssertTrue(tc, line.pImpl != line_aligned.pImpl);
		CuAssertTrue(tc, spline.pImpl == spline_ptr);
		assert_equal_shape(tc, &line, &line_aligned);
		assert_compatible(tc, &line_aligned, &spline);
/* =============================== Tear Down =============================== */
	TS_FINALLY
		ts_bspline_free(&line);
		ts_bspline_free(&spline);
		ts_bspline_free(&line_aligned);
	TS_END_TRY
}

void align_spline_to_itself(CuTest *tc)
{
/* ================================= Set Up ================================ */
	tsBSpline spline = ts_bspline_init();
	tsBSpline copy = ts_bspline_init();
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			/* Arbitrary control points. */
			15, 3, 7, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_copy(
			&spline, &copy, &status))
/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_align(
			&spline, &spline, CTRLP_EPSILON, &spline, &spline,
			&status))
/* ================================= Then ================================== */
		CuAssertTrue(tc, spline.pImpl != copy.pImpl);
		assert_equal_shape(tc, &spline, &copy);
		assert_compatible(tc, &spline, &copy);
/* =============================== Tear Down =============================== */
	TS_FINALLY
		ts_bspline_free(&spline);
	TS_END_TRY
}

CuSuite* get_align_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, align_line_with_spline);
	SUITE_ADD_TEST(suite, align_point_with_spline);
	SUITE_ADD_TEST(suite, align_spline_to_itself);
	return suite;
}
