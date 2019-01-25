#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define CTRLP_EPSILON 0.0001
#define NUM_CTRLP 7
#define DIMENSION 2
#define DEGREE    3

void insert_knot_one(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *ctrlp = NULL, *knots = NULL;
	size_t k = 0;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			7, 2, 3, TS_CLAMPED, &spline, &status))

		/* Set control points. */
		CuAssertIntEquals(tc, 7, (int)
			ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		ctrlp[0]  = -1.75f; /* x0 */
		ctrlp[1]  = -1.0f;  /* y0 */
		ctrlp[2]  = -1.5f;  /* x1 */
		ctrlp[3]  = -0.5f;  /* y1 */
		ctrlp[4]  = -1.5f;  /* x2 */
		ctrlp[5]  =  0.0f;  /* y2 */
		ctrlp[6]  = -1.25f; /* x3 */
		ctrlp[7]  =  0.5f;  /* y3 */
		ctrlp[8]  = -0.75f; /* x4 */
		ctrlp[9]  =  0.75f; /* y4 */
		ctrlp[10] =  0.0f;  /* x5 */
		ctrlp[11] =  0.5f;  /* y5 */
		ctrlp[12] =  0.5f;  /* x6 */
		ctrlp[13] =  0.0f;  /* y6 */
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		free(ctrlp);
		ctrlp = NULL;

		/* Check initial knots. */
		CuAssertIntEquals(tc, 11, (int)
			ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, 0.f,   knots[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, knots[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  knots[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, knots[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[10], TS_KNOT_EPSILON);
		free(knots);
		knots = NULL;

		/* Insert knot = 0.3 once. */
		TS_CALL(try, status.code, ts_bspline_insert_knot(
			&spline, 0.3f, 1, &spline, &k, &status))

		/* Check resulting control points. */
		CuAssertIntEquals(tc, 8, (int)
			ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		CuAssertDblEquals(tc, -1.75f,    ctrlp[0],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.0f,     ctrlp[1],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.5f,     ctrlp[2],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -0.5f,     ctrlp[3],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.5f,     ctrlp[4],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -0.2f,     ctrlp[5],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.4f,     ctrlp[6],  CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.2f,     ctrlp[7],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.21666f, ctrlp[8],  CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.51666f, ctrlp[9],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -0.75f,    ctrlp[10], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.75f,    ctrlp[11], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.f,      ctrlp[12], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.5f,     ctrlp[13], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.5f,     ctrlp[14], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.f,      ctrlp[15], CTRLP_EPSILON);
		free(ctrlp);
		ctrlp = NULL;

		/* Check resulting knots and index k. */
		CuAssertIntEquals(tc, 12, (int) ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, 0.f,   knots[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, knots[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.3f,  knots[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  knots[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, knots[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[10], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[11], TS_KNOT_EPSILON);
		CuAssertIntEquals(tc, 5, (int) k);
		free(knots);
		knots = NULL;
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(knots);
	TS_END_TRY
}

void insert_knot_two(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *ctrlp = NULL, *knots = NULL;
	size_t k = 0;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			7, 2, 3, TS_CLAMPED, &spline, &status))

		/* Set control points. */
		CuAssertIntEquals(tc, 7, (int)
			ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		ctrlp[0]  = -1.75f; /* x0 */
		ctrlp[1]  = -1.0f;  /* y0 */
		ctrlp[2]  = -1.5f;  /* x1 */
		ctrlp[3]  = -0.5f;  /* y1 */
		ctrlp[4]  = -1.5f;  /* x2 */
		ctrlp[5]  =  0.0f;  /* y2 */
		ctrlp[6]  = -1.25f; /* x3 */
		ctrlp[7]  =  0.5f;  /* y3 */
		ctrlp[8]  = -0.75f; /* x4 */
		ctrlp[9]  =  0.75f; /* y4 */
		ctrlp[10] =  0.0f;  /* x5 */
		ctrlp[11] =  0.5f;  /* y5 */
		ctrlp[12] =  0.5f;  /* x6 */
		ctrlp[13] =  0.0f;  /* y6 */
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		free(ctrlp);
		ctrlp = NULL;

		/* Check initial knots. */
		CuAssertIntEquals(tc, 11, (int)
			ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, 0.f,   knots[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, knots[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  knots[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, knots[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[10], TS_KNOT_EPSILON);
		free(knots);
		knots = NULL;

		/* Insert knot = 0.6 twice. */
		TS_CALL(try, status.code, ts_bspline_insert_knot(
			&spline, 0.6f, 2, &spline, &k, &status))

		/* Check resulting control points. */
		CuAssertIntEquals(tc, 9, (int)
			ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		CuAssertDblEquals(tc, -1.75f,    ctrlp[0],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.0f,     ctrlp[1],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.5f,     ctrlp[2],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -0.5f,     ctrlp[3],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.5f,     ctrlp[4],  CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.0f,     ctrlp[5],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.3f,     ctrlp[6],  CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.4f,     ctrlp[7],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -1.10166f, ctrlp[8],  CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.55166f, ctrlp[9],  CTRLP_EPSILON);
		CuAssertDblEquals(tc, -0.93333f, ctrlp[10], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.63333f, ctrlp[11], CTRLP_EPSILON);
		CuAssertDblEquals(tc, -0.6f,     ctrlp[12], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.7f,     ctrlp[13], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.f,      ctrlp[14], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.5f,     ctrlp[15], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.5f,     ctrlp[16], CTRLP_EPSILON);
		CuAssertDblEquals(tc,  0.f,      ctrlp[17], CTRLP_EPSILON);
		free(ctrlp);
		ctrlp = NULL;

		/* Check resulting knots and index k. */
		CuAssertIntEquals(tc, 13, (int) ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, 0.f,   knots[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   knots[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, knots[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  knots[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.6f,  knots[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.6f,  knots[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, knots[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[10], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[11], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   knots[12], TS_KNOT_EPSILON);
		CuAssertIntEquals(tc, 7, (int) k);
		free(knots);
		knots = NULL;
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(knots);
	TS_END_TRY
}

void insert_knot_three(CuTest *tc)
{
	const size_t num_insertions = 3;
	const tsReal knot_to_insert = 0.8f;
	tsBSpline spline = ts_bspline_init();
	tsBSpline result = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *sval = NULL, *rval = NULL;
	size_t k = 0;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			NUM_CTRLP, DIMENSION, DEGREE,
			TS_CLAMPED, &spline, &status))

		/* Set control points. */
		CuAssertIntEquals(tc, NUM_CTRLP, (int)
			ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &sval, &status))
		sval[0]  = -1.75f; /* x0 */
		sval[1]  = -1.0f;  /* y0 */
		sval[2]  = -1.5f;  /* x1 */
		sval[3]  = -0.5f;  /* y1 */
		sval[4]  = -1.5f;  /* x2 */
		sval[5]  =  0.0f;  /* y2 */
		sval[6]  = -1.25f; /* x3 */
		sval[7]  =  0.5f;  /* y3 */
		sval[8]  = -0.75f; /* x4 */
		sval[9]  =  0.75f; /* y4 */
		sval[10] =  0.0f;  /* x5 */
		sval[11] =  0.5f;  /* y5 */
		sval[12] =  0.5f;  /* x6 */
		sval[13] =  0.0f;  /* y6 */
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, sval, &status))
		free(sval);
		sval = NULL;

		/* Check initial knots. */
		CuAssertIntEquals(tc, NUM_CTRLP + DEGREE + 1, (int)
			ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &sval, &status))
		CuAssertDblEquals(tc, 0.f,   sval[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, sval[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  sval[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, sval[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[10], TS_KNOT_EPSILON);
		free(sval);
		sval = NULL;

		/* Insert knot = 0.8 three times. */
		TS_CALL(try, status.code, ts_bspline_insert_knot(
			&spline, knot_to_insert, num_insertions,
			&result, &k, &status))

		/* Check resulting knots and index k. */
		CuAssertIntEquals(tc, NUM_CTRLP + DEGREE + 1 + num_insertions,
			(int)ts_bspline_num_knots(&result));
		TS_CALL(try, status.code, ts_bspline_knots(
			&result, &sval, &status))
		CuAssertDblEquals(tc, 0.f,   sval[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.f,   sval[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.25f, sval[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.5f,  sval[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.75f, sval[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.8f,  sval[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.8f,  sval[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 0.8f,  sval[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[10], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[11], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[12], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, 1.f,   sval[13], TS_KNOT_EPSILON);
		CuAssertIntEquals(tc, 9, (int) k);
		free(sval);
		sval = NULL;

		for (k = 0; k < TS_MAX_NUM_KNOTS; k++) {
			/* Eval spline. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&spline, (tsReal)k / TS_MAX_NUM_KNOTS,
				&net, &status))
			CuAssertIntEquals(tc, DIMENSION,
				(int)ts_deboornet_dimension(&net));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &sval, &status))
			ts_deboornet_free(&net);

			/* Eval result. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&result, (tsReal)k / TS_MAX_NUM_KNOTS,
				&net, &status))
			CuAssertIntEquals(tc, DIMENSION,
				(int)ts_deboornet_dimension(&net));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &rval, &status))
			ts_deboornet_free(&net);

			/* Compare results. */
			CuAssertDblEquals(tc, sval[0], rval[0], 0.001);
			CuAssertDblEquals(tc, sval[1], rval[1], 0.001);
			free(sval);
			free(rval);
			sval = rval = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&result);
		ts_deboornet_free(&net);
		free(sval);
		free(rval);
	TS_END_TRY
}

CuSuite* get_insert_knot_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, insert_knot_one);
	SUITE_ADD_TEST(suite, insert_knot_two);
	SUITE_ADD_TEST(suite, insert_knot_three);
	return suite;
}
