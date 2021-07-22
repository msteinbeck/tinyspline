#include <testutils.h>

void derive_sequence_of_four_points(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal dist, *ctrlp = NULL, *knots = NULL;

	___GIVEN___
	C(ts_bspline_new(4, 2, 0, TS_CLAMPED, &spline, &status))

	___WHEN___
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, &status))

	___THEN___
	/* Check control points of derivative. */
	CuAssertIntEquals(tc, 1, (int) ts_bspline_num_control_points(&spline));
	C(ts_bspline_control_points( &spline, &ctrlp, &status))
	dist = ts_distance_varargs(tc, 2, ctrlp, 0.0, 0.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Check knots of derivative. */
	CuAssertIntEquals(tc, 2, (int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc,
		   TS_DOMAIN_DEFAULT_MIN, knots[0],
		   TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
		   TS_DOMAIN_DEFAULT_MAX, knots[1],
		   TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(ctrlp);
	free(knots);
}

void derive_sequence_of_two_point_with_custom_knots(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal dist, *ctrlp = NULL, *knots = NULL;

	___GIVEN___
	C(ts_bspline_new(2, 3, 0, TS_CLAMPED, &spline, &status))
	C(ts_bspline_set_knots_varargs(&spline, &status,
		(tsReal) -10.0, 1.0, 10.0))

	___WHEN___
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, &status))

	___THEN___
	/* Check control points of derivative. */
	CuAssertIntEquals(tc, 1, (int) ts_bspline_num_control_points(&spline));
	C(ts_bspline_control_points(&spline, &ctrlp, &status))
	dist = ts_distance_varargs(tc, 3, ctrlp, 0.0, 0.0, 0.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Check knots of derivative. */
	CuAssertIntEquals(tc, 2, (int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc, -10.0, knots[0], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  10.0, knots[1], TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(ctrlp);
	free(knots);
}

void derive_single_line(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal dist, *ctrlp = NULL, *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		2, 2, 1, TS_CLAMPED, &spline, &status,
		1.0, -2.0,  /* P1 */
		3.0,  6.0)) /* P2 */

	___WHEN___
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, &status))

	___THEN___
	/* Check control points of derivative. */
	CuAssertIntEquals(tc, 1, (int) ts_bspline_num_control_points(&spline));
	C(ts_bspline_control_points(&spline, &ctrlp, &status))
	dist = ts_distance_varargs(tc, 2, ctrlp, 2.0, 8.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Check knots of derivative. */
	CuAssertIntEquals(tc, 2, (int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc,
		TS_DOMAIN_DEFAULT_MIN, knots[0],
		TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,
		TS_DOMAIN_DEFAULT_MAX, knots[1],
		TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	free(ctrlp);
	free(knots);
}

void derive_single_line_with_custom_knots(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *ctrlp = NULL, *knots = NULL, *result = NULL;
	tsReal min, max, span, step, dist, slope[2];
	size_t i, num_samples = 3;

	___GIVEN___
	C(ts_bspline_new(2, 2, 1, TS_CLAMPED, &spline, &status))

	/* Set up control points. */
	C(ts_bspline_control_points(&spline, &ctrlp, &status))
	ctrlp[0] = 1.0; ctrlp[1] = -2.0; /* P1 */
	ctrlp[2] = 3.0; ctrlp[3] =  6.0; /* P2 */
	C(ts_bspline_set_control_points(&spline, ctrlp, &status))

	/* Set up knots. */
	C(ts_bspline_set_knots_varargs(&spline, &status,
		(tsReal) -2.0, -1.0, 1.0, 2.0))

	/* Confirm that the spline is a line with desired slope. */
	ts_bspline_domain(&spline, &min, &max);
	span = max - min;
	slope[0] = (ctrlp[2] - ctrlp[0]) / span;
	slope[1] = (ctrlp[3] - ctrlp[1]) / span;
	step = span / (tsReal) (num_samples - 1);
	for(i = 0; i < num_samples; ++i) {
		C(ts_bspline_eval(&spline, min + step * i, &net, &status))
		C(ts_deboornet_result(&net, &result, &status))
		dist = ts_distance_varargs(tc, 2, result,
			ctrlp[0] + slope[0] * (step * i),
			ctrlp[1] + slope[1] * (step * i));
		CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
		ts_deboornet_free(&net);
		free(result);
		result = NULL;
	}
	free(ctrlp);
	ctrlp = NULL;

	___WHEN___
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, &status))

	___THEN___
	/* Check control points of derivative. */
	CuAssertIntEquals(tc, 1, (int) ts_bspline_num_control_points(&spline));
	C(ts_bspline_control_points(&spline, &ctrlp, &status))
	                                   /* Scaled with domain [-1, 1]. */
	dist = ts_distance_varargs(tc, 2, ctrlp, 2.0 / 2.0, 8.0 / 2.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Check knots of derivative. */
	CuAssertIntEquals(tc, 2, (int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc, -1.0, knots[0], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc,  1.0, knots[1], TS_KNOT_EPSILON);

	/* Evaluate derivative at mid-span. */
	C(ts_bspline_eval(&spline, (tsReal) (min + span / 2.0), &net, &status))
	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance(result, slope, ts_bspline_dimension(&spline));
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
	free(ctrlp);
	free(knots);
}

void derive_single_parabola_with_custom_knots(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	size_t degree, i, num_samples = 10;
	tsReal slope, span, step, ctrlp[3], knots[6], *result = NULL;

	___GIVEN___
	/* Set the initial position, slope, and final position. Note that this
	 * defines a parabola by degree, but a line by shape. */
	degree = 2;
	slope = (tsReal) 3.0;
	span = (tsReal) 5.0;

	ctrlp[0] = (tsReal) -7.0;
	ctrlp[1] = ctrlp[0] + slope * span / degree;
	ctrlp[2] = ctrlp[0] + slope * span;

	knots[0] = (tsReal) -1.0;
	knots[1] = knots[0];
	knots[2] = knots[0];
	knots[3] = knots[0] + span;
	knots[4] = knots[0] + span;
	knots[5] = knots[0] + span;

	/* Create spline with control points and custom knots. */
	C(ts_bspline_new(3, 1, degree, TS_CLAMPED, &spline, &status))
	C(ts_bspline_set_control_points(&spline, ctrlp, &status))
	C(ts_bspline_set_knots(&spline, knots, &status))

	/* Confirm that the spline is a line with desired slope. */
	step = (span - knots[0]) / (tsReal) (num_samples + 1);
	for(i = 0; i < num_samples; ++i) {
		C(ts_bspline_eval(&spline, knots[0] + (step * i),
			&net, &status))
		C(ts_deboornet_result(&net, &result, &status))
		CuAssertDblEquals(tc, /* One-dimensional spline. */
			ctrlp[0] + slope * (step * i), result[0],
			POINT_EPSILON);
		ts_deboornet_free(&net);
		free(result);
		result = NULL;
	}

	___WHEN___
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, &status))

	___THEN___
	for(i = 0; i < num_samples; ++i) {
		C(ts_bspline_eval(&spline, knots[0] + (step * i),
			&net, &status))
		C(ts_deboornet_result(&net, &result, &status))
		/* One-dimensional spline. */
		CuAssertDblEquals(tc,  slope, result[0], POINT_EPSILON);
		ts_deboornet_free(&net);
		free(result);
		result = NULL;
	}

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void derive_discontinuous_and_compare_with_continuous(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();
	tsBSpline derivative = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal dist, *result = NULL;

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

	___WHEN___
	/* Create beziers (to_beziers => derive). */
	C(ts_bspline_to_beziers(&spline, &beziers, &status))
	C(ts_bspline_derive(&beziers, 1, POINT_EPSILON, &beziers, &status))

	/* Create derivative (derive => to_beziers). */
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &derivative, &status))
	C(ts_bspline_to_beziers(&derivative, &derivative, &status))

	___THEN___
	/* Check that beziers and derivative are different splines. */
	CuAssertTrue(tc, beziers.pImpl != derivative.pImpl);

	assert_equal_shape(tc, &beziers, &derivative);

	/* Eval known values. */
	C(ts_bspline_eval(&beziers, (tsReal) 0.3, &net, &status))
	CuAssertIntEquals(tc, 1, (int) ts_deboornet_num_result(&net));
	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, 0.7, 2.3);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&beziers);
	ts_bspline_free(&derivative);
	ts_deboornet_free(&net);
	free(result);
}

void derive_discontinuous_lines_exceeding_epsilon(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsError err;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 1, TS_CLAMPED, &spline, &status,
		0.0, 0.0,  /* P1 */
		1.0, 1.0,  /* P2 */
		1.0, 1.5,  /* P3 */
		2.0, 2.0)) /* P4 */
	C(ts_bspline_set_knots_varargs(&spline, &status,
		(tsReal) 0.0, 0.0, 0.4, 0.4, 1.0, 1.0))

	___WHEN___
	err = ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, NULL);

	___THEN___
	CuAssertIntEquals(tc, TS_UNDERIVABLE, err);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void derive_discontinuous_lines_ignoring_epsilon(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsError err;
	tsReal dist, *result = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 1, TS_CLAMPED, &spline, &status,
		2.0, 2.0,  /* P1 */
		3.0, 3.0,  /* P2 */
		4.0, 4.0,  /* P3 */ /* <-- Removed by derivation. */
		1.0, 1.0)) /* P4 */
	C(ts_bspline_set_knots_varargs(&spline, &status,
		(tsReal) 0.0, 0.0, 0.7, 0.7, 1.0, 1.0))

	___WHEN___
	err = ts_bspline_derive(&spline, 1, (tsReal) -1.0, &spline, NULL);

	___THEN___
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	/* Eval derivative. */
	C(ts_bspline_eval(&spline, (tsReal) 0.7, &net, &status))
	CuAssertIntEquals(tc, 2, (int)ts_deboornet_num_result(&net));
	C(ts_deboornet_result(&net, &result, &status))

	dist = ts_distance_varargs(tc, 2, result, 1.0 / 0.7, 1.0 / 0.7);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	dist = ts_distance_varargs(tc, 2, result + 2, -2.0 / 0.3, -2.0 / 0.3);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void derive_continuous_spline(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal dist, *result = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		10, 2, 2, TS_CLAMPED, &spline, &status,
		-1.75, -1.0,  /* P1 */
		-1.5,  -0.5,  /* P2 */
		-1.5,   0.0,  /* P3 */
		-1.25,  0.5,  /* P4 */
		-0.75,  0.75, /* P5 */
		 0.0,   0.5,  /* P6 */
		 0.5,   0.0,  /* P7 */
		 1.5,   0.5,  /* P8 */
		 2.0,   1.5,  /* P9 */
		 2.5,   1.0)) /* P10 */

	___WHEN___
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, &status))

	___THEN___
	C(ts_bspline_eval(&spline, (tsReal) 0.8, &net, &status))
	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, 6.4, 5.6);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void derive_continuous_spline_with_custom_knots(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal dist, *result = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		10, 2, 2, TS_CLAMPED, &spline, &status,
		-1.75, -1.0,  /* P1 */
		-1.5,  -0.5,  /* P2 */
		-1.5,   0.0,  /* P3 */
		-1.25,  0.5,  /* P4 */
		-0.75,  0.75, /* P5 */
		 0.0,   0.5,  /* P6 */
		 0.5,   0.0,  /* P7 */
		 1.5,   0.5,  /* P8 */
		 2.0,   1.5,  /* P9 */
		 2.5,   1.0)) /* P10 */
	C(ts_bspline_set_knots_varargs(&spline, &status,
		2.0, 2.0, 2.0, 3.0, 4.0, 5.0,
		6.0, 7.0, 8.0, 9.0, 10.0, 10.0, 10.0));

	___WHEN___
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, &status))

	___THEN___
	C(ts_bspline_eval(&spline, (tsReal) 8.4, &net, &status))
	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, 0.8, 0.7);
	CuAssertDblEquals(tc,  0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void derive_compare_third_derivative_with_three_times(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline third = ts_bspline_init();
	tsBSpline three = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 3, TS_OPENED, &spline, &status,
		1.0, 1.0,  /* P1 */
		2.0, 4.0,  /* P2 */
		3.0, 3.0,  /* P3 */
		4.0, 0.0)) /* P4 */

	___WHEN___
	/* Create third (derive with n = 3). */
	C(ts_bspline_derive(&spline, 3, POINT_EPSILON, &third, &status))

	/* Create three (derive three times). */
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &three, &status))
	C(ts_bspline_derive(&three, 1, POINT_EPSILON, &three, &status))
	C(ts_bspline_derive(&three, 1, POINT_EPSILON, &three, &status))

	___THEN___
	/* Check that third and three are different splines. */
	CuAssertTrue(tc, third.pImpl != three.pImpl);

	assert_equal_shape(tc, &third, &three);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&third);
	ts_bspline_free(&three);
}

CuSuite* get_derive_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, derive_sequence_of_four_points);
	SUITE_ADD_TEST(suite, derive_sequence_of_two_point_with_custom_knots);
	SUITE_ADD_TEST(suite, derive_single_line);
	SUITE_ADD_TEST(suite, derive_single_line_with_custom_knots);
	SUITE_ADD_TEST(suite, derive_single_parabola_with_custom_knots);
	SUITE_ADD_TEST(suite, derive_discontinuous_and_compare_with_continuous);
	SUITE_ADD_TEST(suite, derive_discontinuous_lines_exceeding_epsilon);
	SUITE_ADD_TEST(suite, derive_discontinuous_lines_ignoring_epsilon);
	SUITE_ADD_TEST(suite, derive_continuous_spline);
	SUITE_ADD_TEST(suite, derive_continuous_spline_with_custom_knots);
	SUITE_ADD_TEST(suite, derive_compare_third_derivative_with_three_times);
	return suite;
}