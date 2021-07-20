#include <testutils.h>

#define CTRLP_EPSILON 0.001

void to_beziers_issue143(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();
	tsStatus status;

	tsReal ctrlp[32] = {
		-8837.0980824304952,
		5410.0643332926420,
		-8837.0980824304952,
		5410.1222121646779,
		-8837.1450025330378,
		5410.1691322672204,
		-8837.2028814355908,
		5410.1691322672204,
		-8837.2607603381439,
		5410.1691322672204,
		-8837.3076804101693,
		5410.1222121646779,
		-8837.3076804101693,
		5410.0643332926420,
		-8837.3076804101693,
		5410.0064544206061,
		-8837.2607603381439,
		5409.9595343485826,
		-8837.2028814355908,
		5409.9595343485826,
		-8837.1634778588214,
		5409.9595343485826,
		-8837.0980824304952,
		5409.9595343485826,
		-8837.0980824304952,
		5409.9595343485826,
		-8837.0980824304952,
		5409.9595343485826,
		-8837.0980824304952,
		5410.0206728110606,
		-8837.0980824304952,
		5410.0643332926420
	};

	tsReal knots[20] = {
		-0.19999998770654204,
		-0.19999998770654204,
		-0.19999998770654204,
		0.00000000000000000,
		0.20000000307336449,
		0.20000000307336449,
		0.20000000307336449,
		0.40000000614672898,
		0.40000000614672898,
		0.40000000614672898,
		0.60000002412125475,
		0.60000002412125475,
		0.60000002412125475,
		0.80000001229345796,
		0.80000001229345796,
		0.80000001229345796,
		1.0000000000000000,
		1.2000000030733644,
		1.2000000030733644,
		1.2000000030733644
	};

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			16, 2, 3, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		TS_CALL(try, status.code, ts_bspline_set_knots(
			&spline, knots, &status))

/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_to_beziers(
			&spline, &beziers, &status))

/* ================================= Then ================================== */
		CuAssertTrue(tc, 1); /* no error */
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&beziers);
	TS_END_TRY
}

void to_beziers_clamped(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	size_t i;
	tsReal *sval = NULL, *bval = NULL;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new_with_control_points(
			7, 2, 3, TS_CLAMPED, &spline, &status,
			100.0, 0.0,
			200.0, -10.0,
			500.0, 40.0,
			300.0, 260.0,
			-50.0, 200.0,
			-80.0, 130.0,
			-100.0, 0.0))

/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_to_beziers(
			&spline, &beziers, &status))

/* ================================= Then ================================== */
		/* Check knot vector of beziers. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&beziers, &sval, &status))
		CuAssertIntEquals(tc, 20, ts_bspline_num_knots(&beziers));
		CuAssertDblEquals(tc, 0.f,   sval[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, sval[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, sval[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, sval[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, sval[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  sval[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  sval[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  sval[10], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  sval[11], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, sval[12], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, sval[13], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, sval[14], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, sval[15], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[16], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[17], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[18], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[19], TS_KNOT_EPSILON);
		free(sval);
		sval = NULL;

		/* Compare control points by evaluating spline and beziers. */
		CuAssertIntEquals(tc, 2, (int)ts_bspline_dimension(&spline));
		for (i = 0; i < TS_MAX_NUM_KNOTS; i++) {
			/* Eval spline. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&spline, (tsReal)i / TS_MAX_NUM_KNOTS,
				&net, &status))
			CuAssertIntEquals(tc,
				(int)ts_bspline_dimension(&spline),
				(int)ts_deboornet_dimension(&net));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &sval, &status))
			ts_deboornet_free(&net);

			/* Eval beziers. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&beziers, (tsReal)i / TS_MAX_NUM_KNOTS,
				&net, &status))
			CuAssertIntEquals(tc,
				/* Compare with dimension of spline. */
				(int)ts_bspline_dimension(&spline),
				(int)ts_deboornet_dimension(&net));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &bval, &status))
			ts_deboornet_free(&net);

			/* Compare results. */
			CuAssertDblEquals(tc, 0.0,
				ts_distance(sval, bval, 2),
				CTRLP_EPSILON);
			free(sval);
			free(bval);
			sval = bval = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&beziers);
		ts_deboornet_free(&net);
		free(sval);
		free(bval);
		sval = bval = NULL;
	TS_END_TRY
}

void to_beziers_opened(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *sval = NULL, *bval = NULL;
	tsReal min, max, knot;
	size_t i;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new_with_control_points(
			7, 2, 3, TS_OPENED, &spline, &status,
			100.0, 0.0,
			200.0, -10.0,
			500.0, 40.0,
			300.0, 260.0,
			-50.0, 200.0,
			-80.0, 130.0,
			-100.0, 0.0))

/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_to_beziers(
			&spline, &beziers, &status))

/* ================================= Then ================================== */
		/* Check if spline and beziers have same domain. */
		ts_bspline_domain(&spline, &min, &max);
		CuAssertDblEquals(tc, 0.3f, min, TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.7f, max, TS_KNOT_EPSILON);
		ts_bspline_domain(&beziers, &min, &max);
		CuAssertDblEquals(tc, 0.3f, min, TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.7f, max, TS_KNOT_EPSILON);

		/* Check knot vector of beziers. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&beziers, &sval, &status))
		CuAssertIntEquals(tc, 20, ts_bspline_num_knots(&beziers));
		CuAssertDblEquals(tc, 0.3f, sval[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.3f, sval[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.3f, sval[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.3f, sval[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.4f, sval[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.4f, sval[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.4f, sval[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.4f, sval[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f, sval[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f, sval[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f, sval[10], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f, sval[11], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.6f, sval[12], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.6f, sval[13], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.6f, sval[14], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.6f, sval[15], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.7f, sval[16], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.7f, sval[17], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.7f, sval[18], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.7f, sval[19], TS_KNOT_EPSILON);
		free(sval);
		sval = NULL;

		/* Compare control points by evaluating spline and beziers. */
		CuAssertIntEquals(tc, 2, (int)ts_bspline_dimension(&spline));
		for (i = 0; i < TS_MAX_NUM_KNOTS; i++) {
			knot = (tsReal)i / TS_MAX_NUM_KNOTS;
			knot *= (max - min);
			knot += min;

			/* Eval spline. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&spline, knot, &net, &status))
			CuAssertIntEquals(tc,
				(int)ts_bspline_dimension(&spline),
				(int)ts_deboornet_dimension(&net));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &sval, &status))
			ts_deboornet_free(&net);

			/* Eval beziers. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&beziers, knot, &net, &status))
			CuAssertIntEquals(tc,
				/* Compare with dimension of spline. */
				(int)ts_bspline_dimension(&spline),
				(int)ts_deboornet_dimension(&net));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &bval, &status))
			ts_deboornet_free(&net);

			/* Compare results. */
			CuAssertDblEquals(tc, 0.0,
				ts_distance(sval, bval, 2),
				CTRLP_EPSILON);
			free(sval);
			free(bval);
			sval = bval = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&beziers);
		ts_deboornet_free(&net);
		free(sval);
		free(bval);
		sval = bval = NULL;
	TS_END_TRY
}

CuSuite* get_to_beziers_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, to_beziers_issue143);
	SUITE_ADD_TEST(suite, to_beziers_clamped);
	SUITE_ADD_TEST(suite, to_beziers_opened);
	return suite;
}
