#include <testutils.h>

void
chord_lengths_line(CuTest *tc)
{
	___SETUP___
	tsBSpline line = ts_bspline_init();
	tsReal knots[10], lengths[10], t, knot, min, max;
	size_t i;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		2, 2, 1, TS_CLAMPED, &line, &status,
		0.0, 0.0,    /* P1 */
		10.0, 10.0)) /* P2 */
	ts_bspline_uniform_knot_seq(&line, 10, knots);
	ts_bspline_domain(&line, &min, &max);

	___WHEN___
	C(ts_bspline_chord_lengths(&line,
	                           knots,
	                           10,
	                           lengths,
	                           &status))

	___THEN___
	for (i = 0; i < 10; i++) {
		t = knots[i] / max;
		CuAssertTrue(tc, t >= (tsReal) 0.0 && t <= (tsReal) 1.0);
		C(ts_chord_lengths_t_to_knot(knots,
		                             lengths,
		                             10,
		                             t,
		                             &knot,
		                             &status))
		CuAssertDblEquals(tc, t, knot, TS_KNOT_EPSILON);
	}

	___TEARDOWN___
	ts_bspline_free(&line);
}

void
chord_lengths_equidistant_knot_seq_cubic_line(CuTest *tc)
{
	___SETUP___
	tsBSpline line = ts_bspline_init();
	tsReal knots[200], lengths[200], *points = NULL;
	tsReal arc_length, exp_dist, dist;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		4, 2, 3, TS_CLAMPED, &line, &status,
		0.0, 0.0,    /* P1 */
		1.0, 1.0,    /* P2 */
		10.0, 10.0,  /* P3 */
		30.0, 30.0)) /* P4 */
	ts_bspline_uniform_knot_seq(&line, 200, knots);
	ts_bspline_chord_lengths(&line, knots, 200, lengths, &status);
	arc_length = lengths[199];
	CuAssertDblEquals(tc, 42.426407, arc_length, POINT_EPSILON);
	exp_dist = arc_length / (tsReal) 4.0; /* 5 - 1 */

	___WHEN___
	ts_bspline_equidistant_knot_seq(&line, 5, knots, 200, &status);
	ts_bspline_eval_all(&line, knots, 5, &points, &status);

	___THEN___
	/* P1 -- P2 */
	dist = ts_distance(&points[0], &points[2], 2);
	CuAssertDblEquals(tc, exp_dist, dist, 0.001);
	/* P2 -- P3 */
	dist = ts_distance(&points[2], &points[4], 2);
	CuAssertDblEquals(tc, exp_dist, dist, 0.001);
	/* P3 -- P4 */
	dist = ts_distance(&points[4], &points[6], 2);
	CuAssertDblEquals(tc, exp_dist, dist, 0.001);
	/* P4 -- P5 */
	dist = ts_distance(&points[6], &points[8], 2);
	CuAssertDblEquals(tc, exp_dist, dist, 0.001);

	___TEARDOWN___
	ts_bspline_free(&line);
	if (points) free(points);
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4102)
#endif
void
chord_lengths_num_0(CuTest *tc)
{
	___SETUP___
	/* MSVC cannot allocate an array of constant size 0. */
	tsReal knots[1], lengths[1], knot;
	tsStatus stat;

	___GIVEN___

	___WHEN___
	ts_chord_lengths_t_to_knot(knots,
	                           lengths,
	                           0,
	                           (tsReal) 0.0,
	                           &knot,
	                           &stat);

	___THEN___
	CuAssertIntEquals(tc, TS_NO_RESULT, stat.code);

	___TEARDOWN___
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

void
chord_lengths_num_1(CuTest *tc)
{
	___SETUP___
	tsReal knots[1], lengths[1], knot;

	___GIVEN___
	knots[0] = (tsReal) 0.25;
	lengths[0] = (tsReal) 0.0;

	___WHEN___
	C(ts_chord_lengths_t_to_knot(knots,
	                             lengths,
	                             1,
	                             (tsReal) 0.0,
	                             &knot,
	                             &status))

	___THEN___
	CuAssertDblEquals(tc, knots[0], knot, TS_KNOT_EPSILON);

	___TEARDOWN___
}

void
chord_lengths_too_short(CuTest *tc)
{
	___SETUP___
	tsReal knots[2], lengths[2], knot;

	___GIVEN___
	knots[0] = (tsReal) 0.25;
	knots[1] = (tsReal) 0.5;
	lengths[0] = lengths[1] = (tsReal) 0.0;

	___WHEN___
	C(ts_chord_lengths_t_to_knot(knots,
	                             lengths,
	                             2,
	                             (tsReal) 1.0,
	                             &knot,
	                             &status))

	___THEN___
	CuAssertDblEquals(tc, knots[0], knot, TS_KNOT_EPSILON);

	___TEARDOWN___
}

CuSuite *
get_chord_lengths_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, chord_lengths_line);
	SUITE_ADD_TEST(suite, chord_lengths_equidistant_knot_seq_cubic_line);
	SUITE_ADD_TEST(suite, chord_lengths_num_0);
	SUITE_ADD_TEST(suite, chord_lengths_num_1);
	SUITE_ADD_TEST(suite, chord_lengths_too_short);
	return suite;
}
