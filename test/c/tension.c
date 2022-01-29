#include <testutils.h>

void
tension_beta_0(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline line = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 3, TS_CLAMPED, &spline, &status,
		200.0, 100.0, /* P1 */
		360.0, 40.0,  /* P2 */
		410.0, 20.0,  /* P3 */
		380.0, 50.0)) /* P4 */
	C(ts_bspline_new_with_control_points(
		2, 2, 1, TS_CLAMPED, &line, &status,
		200.0, 100.0, /* P1 */
		380.0, 50.0)) /* P2 */

	___WHEN___
	C(ts_bspline_tension(&spline, 0.f, &spline, &status))

	___THEN___
	assert_equal_shape(tc, &spline, &line);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&line);
}

void
tension_beta_1(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline expected = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 3, TS_CLAMPED, &spline, &status,
		200.0, 100.0, /* P1 */
		360.0, 40.0,  /* P2 */
		410.0, 20.0,  /* P3 */
		380.0, 50.0)) /* P4 */
	C(ts_bspline_copy(&spline, &expected, &status))

	___WHEN___
	C(ts_bspline_tension(&spline, 1.f, &spline, &status))

	___THEN___
	assert_equal_shape(tc, &spline, &expected);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&expected);
}

CuSuite* get_tension_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, tension_beta_0);
	SUITE_ADD_TEST(suite, tension_beta_1);
	return suite;
}
