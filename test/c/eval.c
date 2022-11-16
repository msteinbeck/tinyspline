#include <testutils.h>

void
eval_domain_min(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal min, max, dist, *result = NULL;

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
	ts_bspline_domain(&spline, &min, &max);
	C(ts_bspline_eval(&spline, min, &net, &status))

	___THEN___
	CuAssertIntEquals(tc, 1, (int) ts_deboornet_num_result(&net));
	CuAssertIntEquals(tc, 2, (int) ts_deboornet_dimension(&net));
	CuAssertIntEquals(tc, 3, (int) ts_deboornet_index(&net));
	CuAssertIntEquals(tc, 4, (int) ts_deboornet_multiplicity(&net));
	CuAssertIntEquals(tc, 0, (int )ts_deboornet_num_insertions(&net));
	CuAssertDblEquals(tc, min, ts_deboornet_knot(&net), TS_KNOT_EPSILON);

	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, -1.75, -1.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void
eval_domain_max(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal min, max, dist, *result = NULL;

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
	ts_bspline_domain(&spline, &min, &max);
	C(ts_bspline_eval(&spline, max, &net, &status))

	___THEN___
	CuAssertIntEquals(tc, 1,  (int) ts_deboornet_num_result(&net));
	CuAssertIntEquals(tc, 2,  (int) ts_deboornet_dimension(&net));
	CuAssertIntEquals(tc, 10, (int) ts_deboornet_index(&net));
	CuAssertIntEquals(tc, 4,  (int) ts_deboornet_multiplicity(&net));
	CuAssertIntEquals(tc, 0,  (int) ts_deboornet_num_insertions(&net));
	CuAssertDblEquals(tc, max, ts_deboornet_knot(&net), POINT_EPSILON);

	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, 0.5, 0.0);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void
eval_001(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
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
	C(ts_bspline_eval(&spline, (tsReal) 0.4, &net, &status))

	___THEN___
	CuAssertIntEquals(tc, 1, (int) ts_deboornet_num_result(&net));
	CuAssertIntEquals(tc, 2, (int) ts_deboornet_dimension(&net));
	CuAssertIntEquals(tc, 4, (int) ts_deboornet_index(&net));
	CuAssertIntEquals(tc, 0, (int) ts_deboornet_multiplicity(&net));
	CuAssertIntEquals(tc, 3, (int) ts_deboornet_num_insertions(&net));
	CuAssertDblEquals(tc, 0.4, ts_deboornet_knot(&net), POINT_EPSILON);

	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, -1.338333, 0.288333);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void
eval_002(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
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
	C(ts_bspline_eval(&spline, (tsReal) 0.8, &net, &status))

	___THEN___
	CuAssertIntEquals(tc, 1, (int) ts_deboornet_num_result(&net));
	CuAssertIntEquals(tc, 2, (int) ts_deboornet_dimension(&net));
	CuAssertIntEquals(tc, 6, (int) ts_deboornet_index(&net));
	CuAssertIntEquals(tc, 0, (int) ts_deboornet_multiplicity(&net));
	CuAssertIntEquals(tc, 3, (int) ts_deboornet_num_insertions(&net));
	CuAssertDblEquals(tc, 0.8, ts_deboornet_knot(&net), POINT_EPSILON);

	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, -0.470667, 0.618667);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void
eval_003(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
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
	C(ts_bspline_eval(&spline, (tsReal) 0.75, &net, &status))

	___THEN___
	CuAssertIntEquals(tc, 1, (int) ts_deboornet_num_result(&net));
	CuAssertIntEquals(tc, 2, (int) ts_deboornet_dimension(&net));
	CuAssertIntEquals(tc, 6, (int) ts_deboornet_index(&net));
	CuAssertIntEquals(tc, 1, (int) ts_deboornet_multiplicity(&net));
	CuAssertIntEquals(tc, 2, (int) ts_deboornet_num_insertions(&net));
	CuAssertDblEquals(tc, 0.75, ts_deboornet_knot(&net), POINT_EPSILON);

	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, -0.645833, 0.645833);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void
eval_two_points(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal dist, *result = NULL;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		8, 2, 3, TS_BEZIERS, &spline, &status,
		-1.75, -1.0,  /* P1 */
		-1.5,  -0.5,  /* P2 */
		-1.5,   0.0,  /* P3 */
		-1.25,  0.5,  /* P4 */
		-0.75,  0.75, /* P5 */
		 0.0,   0.5,  /* P6 */
		 0.5,   0.0,  /* P7 */
		-0.3,  -1.0)) /* P8 */

	___WHEN___
	C(ts_bspline_eval(&spline, (tsReal) 0.5, &net, &status))

	___THEN___
	CuAssertIntEquals(tc, 2, (int) ts_deboornet_num_result(&net));
	CuAssertIntEquals(tc, 2, (int) ts_deboornet_dimension(&net));
	CuAssertIntEquals(tc, 7, (int) ts_deboornet_index(&net));
	CuAssertIntEquals(tc, 4, (int) ts_deboornet_multiplicity(&net));
	CuAssertIntEquals(tc, 0, (int) ts_deboornet_num_insertions(&net));
	CuAssertDblEquals(tc, 0.5, ts_deboornet_knot(&net), POINT_EPSILON);

	C(ts_deboornet_result(&net, &result, &status))
	dist = ts_distance_varargs(tc, 2, result, -1.25, 0.5);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
	dist = ts_distance_varargs(tc, 2, result + 2, -0.75, 0.75);
	CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	free(result);
}

void
eval_undefined_knot(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal step;

	___GIVEN___
	C(ts_bspline_new(7, 3, 3, TS_OPENED, &spline, &status));
	step = (TS_DOMAIN_DEFAULT_MAX - TS_DOMAIN_DEFAULT_MIN) /
		(tsReal) (ts_bspline_num_control_points(&spline) +
			ts_bspline_degree(&spline));

	___WHEN___ ___THEN___
	CuAssertIntEquals(tc, TS_U_UNDEFINED,
		ts_bspline_eval(&spline, TS_DOMAIN_DEFAULT_MIN, &net, NULL));
	CuAssertIntEquals(tc, TS_U_UNDEFINED,
		ts_bspline_eval(&spline, step, &net, NULL));
	CuAssertIntEquals(tc, TS_U_UNDEFINED,
		ts_bspline_eval(&spline, step * 2, &net, NULL));
	CuAssertIntEquals(tc, TS_U_UNDEFINED,
		ts_bspline_eval(&spline, step * 8, &net, NULL));
	CuAssertIntEquals(tc, TS_U_UNDEFINED,
		ts_bspline_eval(&spline, step * 9, &net, NULL));
	CuAssertIntEquals(tc, TS_U_UNDEFINED,
		ts_bspline_eval(&spline, TS_DOMAIN_DEFAULT_MAX, &net, NULL));

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
}

void
eval_near_miss_knot(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();

	___GIVEN___
	C(ts_bspline_new(3, 4, 2, TS_CLAMPED, &spline, &status))
	CuAssertIntEquals(tc, 6, (int) ts_bspline_num_knots(&spline));

	C(ts_bspline_set_knots_varargs(&spline, &status,
		(tsReal) 0.1, 0.2, 0.3, 0.3, 0.4, 0.6))

	___WHEN___
	C(ts_bspline_eval(&spline, (tsReal) 0.2999999, &net, &status))

	___THEN___
	CuAssertIntEquals(tc, 1, (int) ts_deboornet_num_result(&net));
	CuAssertIntEquals(tc, 4, (int) ts_deboornet_dimension(&net));
	CuAssertIntEquals(tc, 3, (int) ts_deboornet_index(&net));
	CuAssertIntEquals(tc, 2, (int) ts_deboornet_multiplicity(&net));
	CuAssertIntEquals(tc, 0, (int) ts_deboornet_num_insertions(&net));
	CuAssertTrue(tc, ts_deboornet_knot(&net) >= (tsReal) 0.3);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
}

void
eval_issue_222(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();

	___GIVEN___
	C(ts_bspline_new(7, 2, 3, TS_CLAMPED, &spline, &status))

	___WHEN___
	C(ts_bspline_eval(&spline, (tsReal) (0.0 - 0.00001), &net, &status))
	/* No infinite loop. */

	___THEN___
	CuAssertDblEquals(tc, 0.0, ts_deboornet_knot(&net), TS_KNOT_EPSILON);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
}

CuSuite *
get_eval_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, eval_domain_min);
	SUITE_ADD_TEST(suite, eval_domain_max);
	SUITE_ADD_TEST(suite, eval_001);
	SUITE_ADD_TEST(suite, eval_002);
	SUITE_ADD_TEST(suite, eval_003);
	SUITE_ADD_TEST(suite, eval_two_points);
	SUITE_ADD_TEST(suite, eval_undefined_knot);
	SUITE_ADD_TEST(suite, eval_near_miss_knot);
	SUITE_ADD_TEST(suite, eval_issue_222);
	return suite;
}
