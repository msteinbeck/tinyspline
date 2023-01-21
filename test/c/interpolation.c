#include <testutils.h>

void
interpolation_cubic_natural(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal points[10];
	tsReal dist, *ctrlp = NULL, *knots = NULL;

	___GIVEN___
	points[0] =  1.0; points[1] = -1.0;
	points[2] = -1.0; points[3] =  2.0;
	points[4] =  1.0; points[5] =  4.0;
	points[6] =  4.0; points[7] =  3.0;
	points[8] =  7.0; points[9] =  5.0;

	___WHEN___
	C(ts_bspline_interpolate_cubic_natural(points, 5, 2, &spline, &status))

	___THEN___
	CuAssertIntEquals(tc, 16,
		(int) ts_bspline_num_control_points(&spline));
	C(ts_bspline_control_points(&spline, &ctrlp, &status))

	/* First bezier. */
	dist = ts_distance_varargs(tc, 2, ctrlp, 1.0, -1.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 2, 0.0, 0.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 4, -1.0, 1.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 6, -1.0, 2.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Second bezier. */
	dist = ts_distance_varargs(tc, 2, ctrlp + 8, -1.0, 2.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 10, -1.0, 3.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 12, 0.0, 4.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 14, 1.0, 4.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Third bezier. */
	dist = ts_distance_varargs(tc, 2, ctrlp + 16, 1.0, 4.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 18, 2.0, 4.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 20, 3.0, 3.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 22, 4.0, 3.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Forth bezier. */
	dist = ts_distance_varargs(tc, 2, ctrlp + 24, 4.0, 3.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 26, 5.0, 3.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 28, 6.0, 4.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 30, 7.0, 5.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	CuAssertIntEquals(tc, 20,(int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
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

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(ctrlp);
	free(knots);
}

void
interpolation_cubic_natural_single_point(CuTest *tc)
{

	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline point = ts_bspline_init();
	tsReal ctrlp[3] = { (tsReal) -5.0, (tsReal) 5.0, (tsReal) 3.2 };

	___GIVEN___
	C(ts_bspline_new(1, 3, 0, TS_CLAMPED, &point, &status))
	C(ts_bspline_set_control_points(&point, ctrlp, &status))

	___WHEN___
	C(ts_bspline_interpolate_cubic_natural(
		ctrlp, 1, 3, &spline, &status))

	___THEN___
	CuAssertIntEquals(tc, 3, (int) ts_bspline_degree(&spline));
	CuAssertIntEquals(tc, 4, (int) ts_bspline_num_control_points(&spline));
	CuAssertIntEquals(tc, 3, (int) ts_bspline_dimension(&spline));
	assert_equal_shape(tc, &spline, &point);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&point);
}

void
interpolation_issue32(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal points[6];
	tsReal dist, *ctrlp = NULL, *knots = NULL;

	___GIVEN___
	points[0] = -1.0; points[1] = 0.5;
	points[2] =  0.0; points[3] = 0.0;
	points[4] =  3.0; points[5] = 3.0;

	___WHEN___
	C(ts_bspline_interpolate_cubic_natural(points, 3, 2, &spline, &status))

	___THEN___
	CuAssertIntEquals(tc, 8, (int) ts_bspline_num_control_points(&spline));
	C(ts_bspline_control_points(&spline, &ctrlp, &status))

	/* First bezier. */
	dist = ts_distance_varargs(tc, 2, ctrlp, -1.0, 0.5);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 2,
		-0.83333335816860199, 0.041666667908430099);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 4,
		-0.66666668653488159, -0.41666667908430099);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 6, 0.0, 0.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Second bezier. */
	dist = ts_distance_varargs(tc, 2, ctrlp + 8, 0.0, 0.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 10,
		0.66666668653488159, 0.41666667908430099);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 12,
		1.8333333879709244, 1.7083333842456341);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, ctrlp + 14, 3.0, 3.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	CuAssertIntEquals(tc, 12,(int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[10], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[11], TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(ctrlp);
	free(knots);
}

void interpolation_issue226(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	const tsReal *ctrlp;
	tsReal dist;

	___GIVEN___
	tsReal points[9] = {
		(tsReal) -30.1021881, (tsReal) 1.06910026, (tsReal) 13.1142778,
		(tsReal) -30.2044067, (tsReal) 1.17131793, (tsReal) 13.1142778,
		(tsReal) -30.3066235, (tsReal) 1.06910026, (tsReal) 13.1142778
	};

	___WHEN___
	C(ts_bspline_interpolate_cubic_natural(points, 3, 3, &spline, &status))

	___THEN___
	CuAssertIntEquals(tc, 24, (int) ts_bspline_len_control_points(&spline));
	ctrlp = ts_bspline_control_points_ptr(&spline);

	/* First bezier. */
	dist = ts_distance_varargs(tc, 3, ctrlp,
	                           -30.1021881,
	                           1.06910026,
	                           13.1142778);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 3, ctrlp + 3,
	                           -30.136262014797236,
	                           1.1202091283848326,
	                           13.114278190835936);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 3, ctrlp + 6,
	                           -30.170335032479358,
	                           1.1713179649079948,
	                           13.114278190835936);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 3, ctrlp + 9,
	                           -30.204407600161467,
	                           1.1713179649079948,
	                           13.114278190835936);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Second bezier. */
	dist = ts_distance_varargs(tc, 3, ctrlp + 12,
	                           -30.204407600161467,
	                           1.1713179649079948,
	                           13.114278190835936);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 3, ctrlp + 15,
	                           -30.238480167843576,
	                           1.1713179649079948,
	                           13.114278190835936);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 3, ctrlp + 18,
	                           -30.27255228552567,
	                           1.1202091283848326,
	                           13.114278190835936);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 3, ctrlp + 21,
	                           -30.3066235,
	                           1.06910026,
	                           13.1142778);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void
interpolation_catmull_rom(CuTest *tc)
{

	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal dist, eps, *result = NULL;
	size_t i;

	___GIVEN___
	/* POINT_EPSILON is slightly too small to compare the interpolated
	 * points with their reference points. Thus, we use an adjusted epsilon
	 * environment in this test to assert the distances. */
	eps = (tsReal) 0.000017; /* from 1e-5f (0.00001) */
#ifdef TINYSPLINE_FLOAT_PRECISION
	eps = (tsReal) 0.0016; /* from 1e-3f (0.001) */
#endif

	/* Points to be interpolated. */
	tsReal points[14] = {
		0.0, 1.5, /* P1 */
		2.0, 2.0, /* P2 */
		3.0, 1.0, /* P3 */
		4.0, 0.5, /* P4 */
		5.0, 1.0, /* P5 */
		6.0, 2.0, /* P6 */
		7.0, 3.0  /* P7 */
	};

	/* Generated with catmullrom.py (alpha = 0.5). */
	tsReal samples[800] = {
		#include "res/interpolation_catmull_rom.txt"
	};

	___WHEN___
	C(ts_bspline_interpolate_catmull_rom(
		points, 7, 2, 0.5, NULL, NULL,
		POINT_EPSILON, &spline, &status))

	___THEN___
	for (i = 0; i < 400; i++) {
		C(ts_bspline_bisect(&spline,
		                    samples[i * 2],
		                    POINT_EPSILON,
		                    1,  /* persnickety */
		                    0,  /* index */
		                    1,  /* ascending */
		                    50, /* max_iter */
		                    &net,
		                    &status))
		C(ts_deboornet_result(&net, &result, &status))

		dist = ts_distance(samples + i*2, result, 2);
		CuAssertDblEquals(tc, 0, dist, eps);

		ts_deboornet_free(&net);
		free(result);
	}

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void
interpolation_catmull_rom_single_point(CuTest *tc)
{

	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline point = ts_bspline_init();
	tsReal ctrlp[2] = { 10.0, 10.0 };

	___GIVEN___
	C(ts_bspline_new(1, 2, 0, TS_CLAMPED, &point, &status))
	C(ts_bspline_set_control_points(&point, ctrlp, &status))

	___WHEN___
	C(ts_bspline_interpolate_catmull_rom(
		ctrlp, 1, 2, 0.5, NULL, NULL,
		POINT_EPSILON, &spline, &status))

	___THEN___
	CuAssertIntEquals(tc, 3, (int) ts_bspline_degree(&spline));
	CuAssertIntEquals(tc, 4, (int) ts_bspline_num_control_points(&spline));
	CuAssertIntEquals(tc, 2, (int) ts_bspline_dimension(&spline));
	assert_equal_shape(tc, &spline, &point);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&point);
}

void
interpolation_catmull_rom_same_point(CuTest *tc)
{

	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline point = ts_bspline_init();
	tsReal ctrlp[6] = { (tsReal) 1.0, (tsReal) 2.0,
		            (tsReal) 1.1, (tsReal) 2.0,
		            (tsReal) 1.0, (tsReal) 2.1 };

	___GIVEN___
	C(ts_bspline_new(1, 2, 0, TS_CLAMPED, &point, &status))
	C(ts_bspline_set_control_points(&point, ctrlp, &status))

	___WHEN___
	C(ts_bspline_interpolate_catmull_rom(
		ctrlp, 3, 2, 0.5, NULL, NULL,
		(tsReal) 0.2, &spline, &status))

	___THEN___
	CuAssertIntEquals(tc, 3, (int) ts_bspline_degree(&spline));
	CuAssertIntEquals(tc, 4, (int) ts_bspline_num_control_points(&spline));
	CuAssertIntEquals(tc, 2, (int) ts_bspline_dimension(&spline));
	assert_equal_shape(tc, &spline, &point);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&point);
}

CuSuite* get_interpolation_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, interpolation_cubic_natural);
	SUITE_ADD_TEST(suite, interpolation_cubic_natural_single_point);
	SUITE_ADD_TEST(suite, interpolation_issue226);
	SUITE_ADD_TEST(suite, interpolation_issue32);
	SUITE_ADD_TEST(suite, interpolation_catmull_rom);
	SUITE_ADD_TEST(suite, interpolation_catmull_rom_single_point);
	SUITE_ADD_TEST(suite, interpolation_catmull_rom_same_point);
	return suite;
}
