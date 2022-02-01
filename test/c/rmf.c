#include <testutils.h>

void
rmf_vectors_of_frames(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline derivative = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal knots[50], *result = NULL, dist, angle;
	tsFrame frames[50];
	size_t i;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		5, 3, 3, TS_CLAMPED, &spline, &status,
		100.0, 200.0, 0.0,   /* P1 */
		150.0, 220.0, 10.0,  /* P2 */
		190.0, 120.0, 50.0,  /* P3 */
		260.0, 70.0,  30.0,  /* P4 */
		300.0, 200.0, 20.0)) /* P5 */
	C(ts_bspline_derive(&spline,
	                    1,
	                    0,
	                    &derivative,
	                    &status))
	ts_bspline_uniform_knot_seq(&spline, 50, knots);

	___WHEN___
	C(ts_bspline_compute_rmf(&spline,
	                         knots,
	                         50,
	                         0,
	                         frames,
	                         &status))

	___THEN___
	for (i = 0; i < 50; i++) {
		/* Position */
		C(ts_bspline_eval(&spline, knots[i], &net, &status))
		C(ts_deboornet_result(&net, &result, &status))
		dist = ts_distance(result, frames[i].position, 3);
		CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
		ts_deboornet_free(&net);
		free(result);
		result = NULL;

		/* Tangent */
		C(ts_bspline_eval(&derivative, knots[i], &net, &status))
		C(ts_deboornet_result(&net, &result, &status))
		ts_vec_norm(result, 3, result);
		dist = ts_distance(result, frames[i].tangent, 3);
		CuAssertDblEquals(tc, 0, dist, POINT_EPSILON);
		ts_deboornet_free(&net);
		free(result);
		result = NULL;

		/* Normal */
		angle = (tsReal) 0.0;
		angle = ts_vec_angle(frames[i].normal,
		                     frames[i].tangent,
		                     NULL,
		                     3);
		CuAssertDblEquals(tc, 90.0, angle, ANGLE_EPSILON);
		angle = (tsReal) 0.0;
		angle = ts_vec_angle(frames[i].normal,
		                     frames[i].binormal,
		                     NULL,
		                     3);
		CuAssertDblEquals(tc, 90.0, angle, ANGLE_EPSILON);

		/* Binormal */
		angle = (tsReal) 0.0;
		angle = ts_vec_angle(frames[i].binormal,
		                     frames[i].tangent,
		                     NULL,
		                     3);
		CuAssertDblEquals(tc, 90.0, angle, ANGLE_EPSILON);
		angle = (tsReal) 0.0;
		angle = ts_vec_angle(frames[i].binormal,
		                     frames[i].normal,
		                     NULL,
		                     3);
		CuAssertDblEquals(tc, 90.0, angle, ANGLE_EPSILON);
	}

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&derivative);
	ts_deboornet_free(&net);
	free(result);
}

CuSuite *
get_rmf_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, rmf_vectors_of_frames);
	return suite;
}
