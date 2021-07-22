#include <testutils.h>

void to_beziers_issue143(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();

	tsReal ctrlp[32] = {
		(tsReal) -8837.0980824304952,
		(tsReal)  5410.0643332926420,
		(tsReal) -8837.0980824304952,
		(tsReal)  5410.1222121646779,
		(tsReal) -8837.1450025330378,
		(tsReal)  5410.1691322672204,
		(tsReal) -8837.2028814355908,
		(tsReal)  5410.1691322672204,
		(tsReal) -8837.2607603381439,
		(tsReal)  5410.1691322672204,
		(tsReal) -8837.3076804101693,
		(tsReal)  5410.1222121646779,
		(tsReal) -8837.3076804101693,
		(tsReal)  5410.0643332926420,
		(tsReal) -8837.3076804101693,
		(tsReal)  5410.0064544206061,
		(tsReal) -8837.2607603381439,
		(tsReal)  5409.9595343485826,
		(tsReal) -8837.2028814355908,
		(tsReal)  5409.9595343485826,
		(tsReal) -8837.1634778588214,
		(tsReal)  5409.9595343485826,
		(tsReal) -8837.0980824304952,
		(tsReal)  5409.9595343485826,
		(tsReal) -8837.0980824304952,
		(tsReal)  5409.9595343485826,
		(tsReal) -8837.0980824304952,
		(tsReal)  5409.9595343485826,
		(tsReal) -8837.0980824304952,
		(tsReal)  5410.0206728110606,
		(tsReal) -8837.0980824304952,
		(tsReal)  5410.0643332926420
	};

	tsReal knots[20] = {
		(tsReal) -0.19999998770654204,
		(tsReal) -0.19999998770654204,
		(tsReal) -0.19999998770654204,
		(tsReal)  0.00000000000000000,
		(tsReal)  0.20000000307336449,
		(tsReal)  0.20000000307336449,
		(tsReal)  0.20000000307336449,
		(tsReal)  0.40000000614672898,
		(tsReal)  0.40000000614672898,
		(tsReal)  0.40000000614672898,
		(tsReal)  0.60000002412125475,
		(tsReal)  0.60000002412125475,
		(tsReal)  0.60000002412125475,
		(tsReal)  0.80000001229345796,
		(tsReal)  0.80000001229345796,
		(tsReal)  0.80000001229345796,
		(tsReal)  1.0000000000000000,
		(tsReal)  1.2000000030733644,
		(tsReal)  1.2000000030733644,
		(tsReal)  1.2000000030733644
	};

	___GIVEN___
	C(ts_bspline_new(16, 2, 3, TS_CLAMPED, &spline, &status))
	C(ts_bspline_set_control_points(&spline, ctrlp, &status))
	C(ts_bspline_set_knots(&spline, knots, &status))

	___WHEN___
	C(ts_bspline_to_beziers(&spline, &beziers, &status))

	___THEN___
	/* No error. */

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&beziers);
}

void to_beziers_clamped(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		 100.0,    0.0,  /* 1 */
		 200.0, - 10.0,  /* 2 */
		 500.0,   40.0,  /* 3 */
		 300.0,  260.0,  /* 4 */
		- 50.0,  200.0,  /* 5 */
		- 80.0,  130.0,  /* 6 */
		-100.0,    0.0)) /* 7 */

	___WHEN___
	C(ts_bspline_to_beziers(&spline, &beziers, &status))

	___THEN___
	/* Check knot vector of beziers. */
	C(ts_bspline_knots(&beziers, &knots, &status))
	CuAssertIntEquals(tc, 20, (int) ts_bspline_num_knots(&beziers));
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[10], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[11], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[12], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[13], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[14], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[15], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[16], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[17], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[18], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[19], TS_KNOT_EPSILON);

	assert_equal_shape(tc, &spline, &beziers);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&beziers);
	ts_deboornet_free(&net);
	free(knots);
}

void to_beziers_opened(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal min, max, *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_OPENED, &spline, &status,
		 100.0,    0.0,  /* 1 */
		 200.0, - 10.0,  /* 2 */
		 500.0,   40.0,  /* 3 */
		 300.0,  260.0,  /* 4 */
		- 50.0,  200.0,  /* 5 */
		- 80.0,  130.0,  /* 6 */
		-100.0,    0.0)) /* 7 */

	___WHEN___
	C(ts_bspline_to_beziers(&spline, &beziers, &status))

	___THEN___
	/* Check if spline and beziers have same domain. */
	ts_bspline_domain(&spline, &min, &max);
	CuAssertDblEquals(tc, 0.3, min, TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7, max, TS_KNOT_EPSILON);
	ts_bspline_domain(&beziers, &min, &max);
	CuAssertDblEquals(tc, 0.3, min, TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7, max, TS_KNOT_EPSILON);

	/* Check knot vector of beziers. */
	C(ts_bspline_knots(&beziers, &knots, &status))
	CuAssertIntEquals(tc, 20, (int) ts_bspline_num_knots(&beziers));
	CuAssertDblEquals(tc, 0.3, knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.3, knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.3, knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.3, knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4, knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4, knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4, knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5, knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5, knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5, knots[10], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5, knots[11], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6, knots[12], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6, knots[13], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6, knots[14], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6, knots[15], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7, knots[16], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7, knots[17], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7, knots[18], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7, knots[19], TS_KNOT_EPSILON);

	assert_equal_shape(tc, &spline, &beziers);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&beziers);
	ts_deboornet_free(&net);
	free(knots);
}

CuSuite* get_to_beziers_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, to_beziers_issue143);
	SUITE_ADD_TEST(suite, to_beziers_clamped);
	SUITE_ADD_TEST(suite, to_beziers_opened);
	return suite;
}
