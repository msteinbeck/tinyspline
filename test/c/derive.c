#include <testutils.h>

#define EPSILON 0.0001

void derive_sequence_of_four_points(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsReal exp[2] = {0.0, 0.0}, dist, *ctrlp = NULL, *knots = NULL;

	___GIVEN___
	C(ts_bspline_new(4, 2, 0, TS_CLAMPED, &spline, &status))

	___WHEN___
	C(ts_bspline_derive(&spline, 1, POINT_EPSILON, &spline, &status))

	___THEN___
	/* Check control points of derivative. */
	CuAssertIntEquals(tc, 1, (int) ts_bspline_num_control_points(&spline));
	C(ts_bspline_control_points( &spline, &ctrlp, &status))
	dist = ts_distance(ctrlp, exp, ts_bspline_dimension(&spline));
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
	tsReal exp[3] = {0.0, 0.0, 0.0}, dist, *ctrlp = NULL, *knots = NULL;

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
	dist = ts_distance(ctrlp, exp, ts_bspline_dimension(&spline));
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
	tsReal exp[2] = {2.0, 8.0}, dist, *ctrlp = NULL, *knots = NULL;

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
	dist = ts_distance(ctrlp, exp, ts_bspline_dimension(&spline));
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
	tsReal min, max, span, step, dist, slope[2], exp[2];
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
		exp[0] = ctrlp[0] + slope[0] * (step * i);
		exp[1] = ctrlp[1] + slope[1] * (step * i);
		dist = ts_distance(result, exp, ts_bspline_dimension(&spline));
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
	exp[0] = (tsReal) (2.0 / 2.0); /* Scaled with domain [-1, 1]. */
	exp[1] = (tsReal) (8.0 / 2.0); /* Scaled with domain [-1, 1]. */
	dist = ts_distance(ctrlp, exp, ts_bspline_dimension(&spline));
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	/* Check knots of derivative. */
	CuAssertIntEquals(tc, 2, (int) ts_bspline_num_knots(&spline));
	C(ts_bspline_knots(&spline, &knots, &status))
	CuAssertDblEquals(tc, -1.0, knots[0], EPSILON);
	CuAssertDblEquals(tc,  1.0, knots[1], EPSILON);

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
	tsReal dist, exp[2] = {0.7, 2.3}, *result = NULL;

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
	C(ts_bspline_eval(&beziers, 0.3, &net, &status))
	CuAssertIntEquals(tc, 1, (int) ts_deboornet_num_result(&net));
	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance(result, exp, ts_bspline_dimension(&spline));
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
	tsReal dist, exp[2], *result = NULL;

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
	err = ts_bspline_derive(&spline, 1, -1.f, &spline, NULL);

	___THEN___
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	/* Eval derivative. */
	C(ts_bspline_eval(&spline, 0.7, &net, &status))
	CuAssertIntEquals(tc, 2, (int)ts_deboornet_num_result(&net));
	C(ts_deboornet_result(&net, &result, &status))

	exp[0] = (tsReal) (1.0 / 0.7);
	exp[1] = (tsReal) (1.0 / 0.7);
	dist = ts_distance(result, exp, ts_bspline_dimension(&spline));
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	exp[0] = (tsReal) (-2.0 / 0.3);
	exp[1] = (tsReal) (-2.0 / 0.3);
	dist = ts_distance(result + ts_bspline_dimension(&spline),
		exp, ts_bspline_dimension(&spline));
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void derive_continuous_spline(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result = NULL;
	tsStatus status;

	tsReal ctrlp[20];
	ctrlp[0]  = -1.75; ctrlp[1]  = -1.0;
	ctrlp[2]  = -1.5;  ctrlp[3]  = -0.5;
	ctrlp[4]  = -1.5;  ctrlp[5]  =  0.0;
	ctrlp[6]  = -1.25; ctrlp[7]  =  0.5;
	ctrlp[8]  = -0.75; ctrlp[9]  =  0.75;
	ctrlp[10] =  0.0;  ctrlp[11] =  0.5;
	ctrlp[12] =  0.5;  ctrlp[13] =  0.0;
	ctrlp[14] =  1.5;  ctrlp[15] =  0.5;
	ctrlp[16] =  2.0;  ctrlp[17] =  1.5;
	ctrlp[18] =  2.5;  ctrlp[19] =  1.0;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			10, 2, 2, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))

/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &spline,
			&status))

/* ================================= Then ================================== */
		/* Eval derivative. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, 0.8, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		CuAssertDblEquals(tc,  6.4, result[0], EPSILON);
		CuAssertDblEquals(tc,  5.6, result[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
		free(result);
	TS_END_TRY
}

void derive_continuous_spline_with_custom_knots(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result = NULL;
	tsStatus status;
	tsReal ctrlp[20];
	tsReal knots[13];

	ctrlp[0]  = -1.75; ctrlp[1]  = -1.0;
	ctrlp[2]  = -1.5;  ctrlp[3]  = -0.5;
	ctrlp[4]  = -1.5;  ctrlp[5]  =  0.0;
	ctrlp[6]  = -1.25; ctrlp[7]  =  0.5;
	ctrlp[8]  = -0.75; ctrlp[9]  =  0.75;
	ctrlp[10] =  0.0;  ctrlp[11] =  0.5;
	ctrlp[12] =  0.5;  ctrlp[13] =  0.0;
	ctrlp[14] =  1.5;  ctrlp[15] =  0.5;
	ctrlp[16] =  2.0;  ctrlp[17] =  1.5;
	ctrlp[18] =  2.5;  ctrlp[19] =  1.0;

	knots[0]  =  2.f;
	knots[1]  =  2.f;
	knots[2]  =  2.f;
	knots[3]  =  3.f;
	knots[4]  =  4.f;
	knots[5]  =  5.f;
	knots[6]  =  6.f;
	knots[7]  =  7.f;
	knots[8]  =  8.f;
	knots[9]  =  9.f;
	knots[10] = 10.f;
	knots[11] = 10.f;
	knots[12] = 10.f;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		/* Create spline with control points and custom knots. */
		TS_CALL(try, status.code, ts_bspline_new(
			10, 2, 2, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		TS_CALL(try, status.code, ts_bspline_set_knots(
			&spline, knots, &status))

/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &spline,
			&status))

/* ================================= Then ================================== */
		/* Eval derivative. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, 8.4, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		CuAssertDblEquals(tc,  0.8, result[0], EPSILON);
		CuAssertDblEquals(tc,  0.7, result[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
		free(result);
	TS_END_TRY
}

void derive_compare_third_derivative_with_three_times(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsBSpline third = ts_bspline_init();
	tsBSpline three = ts_bspline_init();
	tsStatus status;

	tsReal ctrlp[8];
	ctrlp[0] = 1.f; ctrlp[1] = 1.f;
	ctrlp[2] = 2.f; ctrlp[3] = 4.f;
	ctrlp[4] = 3.f; ctrlp[5] = 3.f;
	ctrlp[6] = 4.f; ctrlp[7] = 0.f;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 3, TS_OPENED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))

/* ================================= When ================================== */
		/* Create third (derive with n = 3). */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 3, EPSILON, &third, &status))

		/* Create three (derive three times). */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, EPSILON, &three, &status))
		TS_CALL(try, status.code, ts_bspline_derive(
			&three, 1, EPSILON, &three, &status))
		TS_CALL(try, status.code, ts_bspline_derive(
			&three, 1, EPSILON, &three, &status))

/* ================================= Then ================================== */
		/* Check that third and three are different splines. */
		CuAssertTrue(tc, third.pImpl != three.pImpl);

		/* Compare third and three. */
		assert_equal_shape(tc, &third, &three);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&third);
		ts_bspline_free(&three);
	TS_END_TRY
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