#include <testutils.h>

void
sub_spline_assert_sub_spline(CuTest *tc,
                             tsBSpline *spline,
                             tsBSpline *sub)
{
	size_t dim, i, n;
	tsStatus status;
	tsReal *pts = NULL, dist, eps;
	tsDeBoorNet net = ts_deboornet_init();

	/* POINT_EPSILON is slightly too small to compare the sub-spline with
	 * its reference spline. We therefore use an adjusted epsilon
	 * environment in this test to assert the distances of corresponding
	 * points. */
	eps = (tsReal) 0.00002; /* from 1e-5f (0.00001) */
#ifdef TINYSPLINE_FLOAT_PRECISION
	eps = (tsReal) 0.002; /* from 1e-3f (0.001) */
#endif

	CuAssertIntEquals(tc,
	                  (int) ts_bspline_dimension(spline),
	                  (int) ts_bspline_dimension(sub));
	dim = ts_bspline_dimension(spline);

	TS_TRY(try, status.code, &status)
		C(ts_bspline_sample(sub, 10000, &pts, &n, &status))
		for (i = 0; i < n; i++) {
			/* Find corresponding point. */
			C(ts_bspline_bisect(spline,
			                    pts[i * dim],
			                    TS_POINT_EPSILON, /* epsilon */
			                    1,   /* persnickety */
			                    0,   /* index */
			                    1,   /* ascending */
			                    50,  /* max_iter */
			                    &net,
			                    &status))
			dist = ts_distance(ts_deboornet_result_ptr(&net),
			                   pts + i * dim, dim);
			CuAssertDblEquals(tc, 0.0, dist, eps);
			ts_deboornet_free(&net);
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		if (pts) free(pts);
		ts_deboornet_free(&net);
	TS_END_TRY
}

void
sub_spline_equal_knots(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsStatus stat;

	___GIVEN___
	C(ts_bspline_new(1, 1, 0, TS_CLAMPED, &spline, &status))

	___WHEN___
	ts_bspline_sub_spline(&spline,
	                      (tsReal) 0.5,
	                      (tsReal) 0.5,
	                      &spline,
	                      &stat);

	___THEN___
	CuAssertIntEquals(tc, TS_NO_RESULT, stat.code);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void
sub_spline_undefined_knot_lower(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsStatus stat;

	___GIVEN___
	C(ts_bspline_new(1, 1, 0, TS_OPENED, &spline, &status))

	___WHEN___
	ts_bspline_sub_spline(&spline,
	                      (tsReal) -1.0,
	                      (tsReal) 0.5,
	                      &spline,
	                      &stat);

	___THEN___
	CuAssertIntEquals(tc, TS_U_UNDEFINED, stat.code);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void
sub_spline_undefined_knot_upper(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsStatus stat;

	___GIVEN___
	C(ts_bspline_new(1, 1, 0, TS_OPENED, &spline, &status))

	___WHEN___
	ts_bspline_sub_spline(&spline,
	                      (tsReal) 0.5,
	                      (tsReal) 2.0,
	                      &spline,
	                      &stat);

	___THEN___
	CuAssertIntEquals(tc, TS_U_UNDEFINED, stat.code);

	___TEARDOWN___
	ts_bspline_free(&spline);
}

void
sub_spline_clamped(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline sub = ts_bspline_init();
	const tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		-150.0, -100.0,  /* P1 */
		-100.0,  -50.0,  /* P2 */
		 -80.0,    0.0,  /* P3 */
		   0.0,   50.0,  /* P4 */
		 100.0,  275.0,  /* P5 */
		 160.0,   30.0,  /* P6 */
		 235.0,   85.0)) /* P7 */

	___WHEN___
	C(ts_bspline_sub_spline(&spline,
	                        (tsReal) 0.2,
	                        (tsReal) 0.4,
	                        &sub,
	                        &status))

	___THEN___
	knots = ts_bspline_knots_ptr(&sub);
	CuAssertIntEquals(tc, 9, (int) ts_bspline_num_knots(&sub));
	CuAssertDblEquals(tc, 0.2,  knots[0], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.2,  knots[1], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.2,  knots[2], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.2,  knots[3], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4,  knots[5], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4,  knots[6], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4,  knots[7], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4,  knots[8], TS_KNOT_EPSILON);

	CuAssertIntEquals(tc, 5, (int) ts_bspline_num_control_points(&sub));
	sub_spline_assert_sub_spline(tc, &spline, &sub);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&sub);
}

void
sub_spline_clamped_domain_min(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline sub = ts_bspline_init();
	const tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		-150.0, -100.0,  /* P1 */
		-100.0,  -50.0,  /* P2 */
		 -80.0,    0.0,  /* P3 */
		   0.0,   50.0,  /* P4 */
		 100.0,  275.0,  /* P5 */
		 160.0,   30.0,  /* P6 */
		 235.0,   85.0)) /* P7 */

	___WHEN___
	C(ts_bspline_sub_spline(&spline,
	                        (tsReal) 0.0,
	                        (tsReal) 0.7,
	                        &sub,
	                        &status))

	___THEN___
	knots = ts_bspline_knots_ptr(&sub);
	CuAssertIntEquals(tc, 10, (int) ts_bspline_num_knots(&sub));
	CuAssertDblEquals(tc, 0.0,  knots[0], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5, knots[5], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7,  knots[6], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7,  knots[7], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7,  knots[8], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.7,  knots[9], TS_KNOT_EPSILON);

	CuAssertIntEquals(tc, 6, (int) ts_bspline_num_control_points(&sub));
	sub_spline_assert_sub_spline(tc, &spline, &sub);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&sub);
}

void
sub_spline_clamped_domain_max(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline sub = ts_bspline_init();
	const tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		-150.0, -100.0,  /* P1 */
		-100.0,  -50.0,  /* P2 */
		 -80.0,    0.0,  /* P3 */
		   0.0,   50.0,  /* P4 */
		 100.0,  275.0,  /* P5 */
		 160.0,   30.0,  /* P6 */
		 235.0,   85.0)) /* P7 */

	___WHEN___
	C(ts_bspline_sub_spline(&spline,
	                        (tsReal) 0.6,
	                        (tsReal) 1.0,
	                        &sub,
	                        &status))

	___THEN___
	knots = ts_bspline_knots_ptr(&sub);
	CuAssertIntEquals(tc, 9, (int) ts_bspline_num_knots(&sub));
	CuAssertDblEquals(tc, 0.6,  knots[0], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[1], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[2], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[3], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[4], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[5], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[6], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[7], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[8], TS_KNOT_EPSILON);

	CuAssertIntEquals(tc, 5, (int) ts_bspline_num_control_points(&sub));
	sub_spline_assert_sub_spline(tc, &spline, &sub);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&sub);
}

void
sub_spline_clamped_domain_min_max(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline sub = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		 50.0,  100.0,  /* P1 */
		100.0,   50.0,  /* P2 */
		128.0,    0.0,  /* P3 */
		256.0,  -50.0,  /* P4 */
		512.0,  275.0,  /* P5 */
		600.0,   30.0,  /* P6 */
		700.0,   85.0)) /* P7 */

	___WHEN___
	C(ts_bspline_sub_spline(&spline,
	                        (tsReal) 0.0,
	                        (tsReal) 1.0,
	                        &sub,
	                        &status))

	___THEN___
	assert_equal_shape(tc, &spline, &sub);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&sub);
}

void
sub_spline_opened(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline sub = ts_bspline_init();
	const tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_OPENED, &spline, &status,
		 20.0, -100.0,  /* P1 */
		 40.0,  -50.0,  /* P2 */
		 80.0,    0.0,  /* P3 */
		160.0,   50.0,  /* P4 */
		320.0,  275.0,  /* P5 */
		640.0,   30.0,  /* P6 */
		700.0,   85.0)) /* P7 */

	___WHEN___
	C(ts_bspline_sub_spline(&spline,
	                        (tsReal) 0.4,
	                        (tsReal) 0.6,
	                        &sub,
	                        &status))

	___THEN___
	knots = ts_bspline_knots_ptr(&sub);
	CuAssertIntEquals(tc, 9, (int) ts_bspline_num_knots(&sub));
	CuAssertDblEquals(tc, 0.4,  knots[0], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4,  knots[1], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4,  knots[2], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.4,  knots[3], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[4], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[5], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[6], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[7], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.6,  knots[8], TS_KNOT_EPSILON);

	CuAssertIntEquals(tc, 5, (int) ts_bspline_num_control_points(&sub));
	sub_spline_assert_sub_spline(tc, &spline, &sub);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&sub);
}

void
sub_spline_opened_domain_min_max(CuTest *tc)
{
	___SETUP___
	tsReal min, max;
	tsBSpline spline = ts_bspline_init();
	tsBSpline sub = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_OPENED, &spline, &status,
		 20.0, -100.0,  /* P1 */
		 40.0,  -50.0,  /* P2 */
		 80.0,    0.0,  /* P3 */
		160.0,   50.0,  /* P4 */
		320.0,  275.0,  /* P5 */
		640.0,   30.0,  /* P6 */
		700.0,   85.0)) /* P7 */
	ts_bspline_domain(&spline, &min, &max);

	___WHEN___
	C(ts_bspline_sub_spline(&spline,
	                        min,
	                        max,
	                        &sub,
	                        &status))

	___THEN___
	assert_equal_shape(tc, &spline, &sub);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&sub);
}

void
sub_spline_bezier(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline sub = ts_bspline_init();
	const tsReal *knots = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		7, 2, 3, TS_CLAMPED, &spline, &status,
		-150.0, -100.0,  /* P1 */
		-100.0,  -50.0,  /* P2 */
		 -80.0,    0.0,  /* P3 */
		   0.0,   50.0,  /* P4 */
		 100.0,  275.0,  /* P5 */
		 160.0,   30.0,  /* P6 */
		 235.0,   85.0)) /* P7 */

	___WHEN___
	C(ts_bspline_sub_spline(&spline,
	                        (tsReal) 0.25,
	                        (tsReal) 0.5,
	                        &sub,
	                        &status))

	___THEN___
	knots = ts_bspline_knots_ptr(&sub);
	CuAssertIntEquals(tc, 8, (int) ts_bspline_num_knots(&sub));
	CuAssertDblEquals(tc, 0.25, knots[0], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[1], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[2], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[3], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[4], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[6], TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[7], TS_KNOT_EPSILON);

	CuAssertIntEquals(tc, 4, (int) ts_bspline_num_control_points(&sub));
	sub_spline_assert_sub_spline(tc, &spline, &sub);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&sub);
}

void
sub_spline_reverse_entire_spline(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline expected = ts_bspline_init();
	tsBSpline reversed = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		5, 3, 3, TS_CLAMPED, &spline, &status,
		100.0, 200.0, 0.0,   /* P1 */
		150.0, 220.0, 10.0,  /* P2 */
		190.0, 120.0, 50.0,  /* P3 */
		260.0, 70.0,  30.0,  /* P4 */
		300.0, 200.0, 20.0)) /* P5 */
	C(ts_bspline_new_with_control_points(
		5, 3, 3, TS_CLAMPED, &expected, &status,
		300.0, 200.0, 20.0, /* P1 */
		260.0, 70.0,  30.0, /* P2 */
		190.0, 120.0, 50.0, /* P3 */
		150.0, 220.0, 10.0, /* P4 */
		100.0, 200.0, 0.0)) /* P5 */

	___WHEN___
	C(ts_bspline_sub_spline(&spline,
	                        (tsReal) 1.0,
	                        (tsReal) 0.0,
	                        &reversed,
	                        &status))

	___THEN___
	assert_equal_shape(tc, &expected, &reversed);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&expected);
	ts_bspline_free(&reversed);
}

CuSuite *
get_sub_spline_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, sub_spline_equal_knots);
	SUITE_ADD_TEST(suite, sub_spline_undefined_knot_lower);
	SUITE_ADD_TEST(suite, sub_spline_undefined_knot_upper);
	SUITE_ADD_TEST(suite, sub_spline_clamped);
	SUITE_ADD_TEST(suite, sub_spline_clamped_domain_min);
	SUITE_ADD_TEST(suite, sub_spline_clamped_domain_max);
	SUITE_ADD_TEST(suite, sub_spline_clamped_domain_min_max);
	SUITE_ADD_TEST(suite, sub_spline_opened);
	/* `opened_domain_min` and `opened_domain_max` not strictly needed. */
	SUITE_ADD_TEST(suite, sub_spline_opened_domain_min_max);
	SUITE_ADD_TEST(suite, sub_spline_bezier);
	SUITE_ADD_TEST(suite, sub_spline_reverse_entire_spline);
	return suite;
}
