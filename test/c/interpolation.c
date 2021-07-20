#include <testutils.h>

#define EPSILON 0.0001

void interpolation_cubic_natural(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal points[10];
	tsReal *ctrlp = NULL;
	tsReal *knots = NULL;
	tsStatus status;

	points[0] =  1.0;
	points[1] = -1.0;
	points[2] = -1.0;
	points[3] =  2.0;
	points[4] =  1.0;
	points[5] =  4.0;
	points[6] =  4.0;
	points[7] =  3.0;
	points[8] =  7.0;
	points[9] =  5.0;

	TS_TRY(try, status.code, &status)
/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_interpolate_cubic_natural(
			points, 5, 2, &spline, &status))

/* ================================= Then ================================== */
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		CuAssertIntEquals(tc, 16,
			(int) ts_bspline_num_control_points(&spline));
		CuAssertDblEquals(tc,  1.0, ctrlp[0],  EPSILON);
		CuAssertDblEquals(tc, -1.0, ctrlp[1],  EPSILON);
		CuAssertDblEquals(tc,  0.0, ctrlp[2],  EPSILON);
		CuAssertDblEquals(tc,  0.0, ctrlp[3],  EPSILON);
		CuAssertDblEquals(tc, -1.0, ctrlp[4],  EPSILON);
		CuAssertDblEquals(tc,  1.0, ctrlp[5],  EPSILON);
		CuAssertDblEquals(tc, -1.0, ctrlp[6],  EPSILON);
		CuAssertDblEquals(tc,  2.0, ctrlp[7],  EPSILON);
		CuAssertDblEquals(tc, -1.0, ctrlp[8],  EPSILON);
		CuAssertDblEquals(tc,  2.0, ctrlp[9],  EPSILON);
		CuAssertDblEquals(tc, -1.0, ctrlp[10], EPSILON);
		CuAssertDblEquals(tc,  3.0, ctrlp[11], EPSILON);
		CuAssertDblEquals(tc,  0.0, ctrlp[12], EPSILON);
		CuAssertDblEquals(tc,  4.0, ctrlp[13], EPSILON);
		CuAssertDblEquals(tc,  1.0, ctrlp[14], EPSILON);
		CuAssertDblEquals(tc,  4.0, ctrlp[15], EPSILON);
		CuAssertDblEquals(tc,  1.0, ctrlp[16], EPSILON);
		CuAssertDblEquals(tc,  4.0, ctrlp[17], EPSILON);
		CuAssertDblEquals(tc,  2.0, ctrlp[18], EPSILON);
		CuAssertDblEquals(tc,  4.0, ctrlp[19], EPSILON);
		CuAssertDblEquals(tc,  3.0, ctrlp[20], EPSILON);
		CuAssertDblEquals(tc,  3.0, ctrlp[21], EPSILON);
		CuAssertDblEquals(tc,  4.0, ctrlp[22], EPSILON);
		CuAssertDblEquals(tc,  3.0, ctrlp[23], EPSILON);
		CuAssertDblEquals(tc,  4.0, ctrlp[24], EPSILON);
		CuAssertDblEquals(tc,  3.0, ctrlp[25], EPSILON);
		CuAssertDblEquals(tc,  5.0, ctrlp[26], EPSILON);
		CuAssertDblEquals(tc,  3.0, ctrlp[27], EPSILON);
		CuAssertDblEquals(tc,  6.0, ctrlp[28], EPSILON);
		CuAssertDblEquals(tc,  4.0, ctrlp[29], EPSILON);
		CuAssertDblEquals(tc,  7.0, ctrlp[30], EPSILON);
		CuAssertDblEquals(tc,  5.0, ctrlp[31], EPSILON);

		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc,  0.0,  knots[0],  EPSILON);
		CuAssertDblEquals(tc,  0.0,  knots[1],  EPSILON);
		CuAssertDblEquals(tc,  0.0,  knots[2],  EPSILON);
		CuAssertDblEquals(tc,  0.0,  knots[3],  EPSILON);
		CuAssertDblEquals(tc,  0.25, knots[4],  EPSILON);
		CuAssertDblEquals(tc,  0.25, knots[5],  EPSILON);
		CuAssertDblEquals(tc,  0.25, knots[6],  EPSILON);
		CuAssertDblEquals(tc,  0.25, knots[7],  EPSILON);
		CuAssertDblEquals(tc,  0.5,  knots[8],  EPSILON);
		CuAssertDblEquals(tc,  0.5,  knots[9],  EPSILON);
		CuAssertDblEquals(tc,  0.5,  knots[10], EPSILON);
		CuAssertDblEquals(tc,  0.5,  knots[11], EPSILON);
		CuAssertDblEquals(tc,  0.75, knots[12], EPSILON);
		CuAssertDblEquals(tc,  0.75, knots[13], EPSILON);
		CuAssertDblEquals(tc,  0.75, knots[14], EPSILON);
		CuAssertDblEquals(tc,  0.75, knots[15], EPSILON);
		CuAssertDblEquals(tc,  1.0,  knots[16], EPSILON);
		CuAssertDblEquals(tc,  1.0,  knots[17], EPSILON);
		CuAssertDblEquals(tc,  1.0,  knots[18], EPSILON);
		CuAssertDblEquals(tc,  1.0,  knots[19], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(ctrlp);
		free(knots);
	TS_END_TRY
}

void interpolation_issue32(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal points[6];
	tsReal *ctrlp = NULL;
	tsReal *knots = NULL;
	tsStatus status;

	points[0] = -1.0;
	points[1] =  0.5;
	points[2] =  0.0;
	points[3] =  0.0;
	points[4] =  3.0;
	points[5] =  3.0;

	TS_TRY(try, status.code, &status)
/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_interpolate_cubic_natural(
			points, 3, 2, &spline, &status))

/* ================================= Then ================================== */
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		CuAssertIntEquals(tc, 8,
			(int) ts_bspline_num_control_points(&spline));
		CuAssertDblEquals(tc, -1.0,    ctrlp[0],  EPSILON);
		CuAssertDblEquals(tc,  0.5,    ctrlp[1],  EPSILON);
		CuAssertDblEquals(tc, -0.8333, ctrlp[2],  EPSILON);
		CuAssertDblEquals(tc,  0.0416, ctrlp[3],  EPSILON);
		CuAssertDblEquals(tc, -0.6666, ctrlp[4],  EPSILON);
		CuAssertDblEquals(tc, -0.4166, ctrlp[5],  EPSILON);
		CuAssertDblEquals(tc,  0.0,    ctrlp[6],  EPSILON);
		CuAssertDblEquals(tc,  0.0,    ctrlp[7],  EPSILON);
		CuAssertDblEquals(tc,  0.0,    ctrlp[8],  EPSILON);
		CuAssertDblEquals(tc,  0.0,    ctrlp[9],  EPSILON);
		CuAssertDblEquals(tc,  0.6666, ctrlp[10], EPSILON);
		CuAssertDblEquals(tc,  0.4166, ctrlp[11], EPSILON);
		CuAssertDblEquals(tc,  1.8333, ctrlp[12], EPSILON);
		CuAssertDblEquals(tc,  1.7083, ctrlp[13], EPSILON);
		CuAssertDblEquals(tc,  3.0,    ctrlp[14], EPSILON);
		CuAssertDblEquals(tc,  3.0,    ctrlp[15], EPSILON);

		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc,  0.0,  knots[0],  EPSILON);
		CuAssertDblEquals(tc,  0.0,  knots[1],  EPSILON);
		CuAssertDblEquals(tc,  0.0,  knots[2],  EPSILON);
		CuAssertDblEquals(tc,  0.0,  knots[3],  EPSILON);
		CuAssertDblEquals(tc,  0.5,  knots[4],  EPSILON);
		CuAssertDblEquals(tc,  0.5,  knots[5],  EPSILON);
		CuAssertDblEquals(tc,  0.5,  knots[6],  EPSILON);
		CuAssertDblEquals(tc,  0.5,  knots[7],  EPSILON);
		CuAssertDblEquals(tc,  1.0,  knots[8],  EPSILON);
		CuAssertDblEquals(tc,  1.0,  knots[9],  EPSILON);
		CuAssertDblEquals(tc,  1.0,  knots[10], EPSILON);
		CuAssertDblEquals(tc,  1.0,  knots[11], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(ctrlp);
		free(knots);
	TS_END_TRY
}

CuSuite* get_interpolation_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, interpolation_cubic_natural);
	SUITE_ADD_TEST(suite, interpolation_issue32);
	return suite;
}