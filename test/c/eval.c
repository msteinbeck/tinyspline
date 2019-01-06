#include <stdlib.h>
#include "tinyspline.h"
#include "CuTest.h"

#define EPSILON 0.0001

void eval_domain_min(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *ctrlp = NULL, *result = NULL;
	tsError err;

	err = ts_bspline_new(7, 2, 3, TS_CLAMPED, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	err = ts_bspline_control_points(&spline, &ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
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
	err = ts_bspline_set_control_points(&spline, ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	err = ts_bspline_eval(&spline,
		ts_bspline_domain_min(&spline), &net, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertTrue(tc, ts_deboornet_num_result(&net) == 1);
	CuAssertTrue(tc, ts_deboornet_dimension(&net) == 2);

	err = ts_deboornet_result(&net, &result, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, -1.75f, result[0], EPSILON);
	CuAssertDblEquals(tc, -1.0f, result[1], EPSILON);

	ts_bspline_free(&spline);
	free(ctrlp);
	ts_deboornet_free(&net);
	free(result);
}

void eval_domain_max(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *ctrlp = NULL, *result = NULL;
	tsError err;

	err = ts_bspline_new(7, 2, 3, TS_CLAMPED, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	err = ts_bspline_control_points(&spline, &ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
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
	err = ts_bspline_set_control_points(&spline, ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	err = ts_bspline_eval(&spline,
		ts_bspline_domain_max(&spline), &net, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertTrue(tc, ts_deboornet_num_result(&net) == 1);
	CuAssertTrue(tc, ts_deboornet_dimension(&net) == 2);

	err = ts_deboornet_result(&net, &result, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, 0.5f, result[0], EPSILON);
	CuAssertDblEquals(tc, 0.0f, result[1], EPSILON);

	ts_bspline_free(&spline);
	free(ctrlp);
	ts_deboornet_free(&net);
	free(result);
}

void eval_001(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *ctrlp = NULL, *result = NULL;
	tsError err;

	err = ts_bspline_new(7, 2, 3, TS_CLAMPED, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	err = ts_bspline_control_points(&spline, &ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
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
	err = ts_bspline_set_control_points(&spline, ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	err = ts_bspline_eval(&spline, 0.4f, &net, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertTrue(tc, ts_deboornet_num_result(&net) == 1);
	CuAssertTrue(tc, ts_deboornet_dimension(&net) == 2);

	err = ts_deboornet_result(&net, &result, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, -1.338333f, result[0], EPSILON);
	CuAssertDblEquals(tc,  0.288333f, result[1], EPSILON);

	ts_bspline_free(&spline);
	free(ctrlp);
	ts_deboornet_free(&net);
	free(result);
}

void eval_002(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *ctrlp = NULL, *result = NULL;
	tsError err;

	err = ts_bspline_new(7, 2, 3, TS_CLAMPED, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	err = ts_bspline_control_points(&spline, &ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
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
	err = ts_bspline_set_control_points(&spline, ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	err = ts_bspline_eval(&spline, 0.8f, &net, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertTrue(tc, ts_deboornet_num_result(&net) == 1);
	CuAssertTrue(tc, ts_deboornet_dimension(&net) == 2);

	err = ts_deboornet_result(&net, &result, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, -0.470667f, result[0], EPSILON);
	CuAssertDblEquals(tc,  0.618667f, result[1], EPSILON);

	ts_bspline_free(&spline);
	free(ctrlp);
	ts_deboornet_free(&net);
	free(result);
}

void eval_003(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *ctrlp = NULL, *result = NULL;
	tsError err;

	err = ts_bspline_new(8, 2, 3, TS_BEZIERS, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	err = ts_bspline_control_points(&spline, &ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
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
	ctrlp[14] = -0.3f;  /* x7 */
	ctrlp[15] = -1.0f;  /* y7 */
	err = ts_bspline_set_control_points(&spline, ctrlp, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	err = ts_bspline_eval(&spline, 0.5f, &net, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertTrue(tc, ts_deboornet_num_result(&net) == 2);
	CuAssertTrue(tc, ts_deboornet_dimension(&net) == 2);

	err = ts_deboornet_result(&net, &result, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, -1.25, result[0], EPSILON);
	CuAssertDblEquals(tc,  0.5f, result[1], EPSILON);
	CuAssertDblEquals(tc, -0.75f, result[2], EPSILON);
	CuAssertDblEquals(tc,  0.75f, result[3], EPSILON);

	ts_bspline_free(&spline);
	free(ctrlp);
	ts_deboornet_free(&net);
	free(result);
}

CuSuite* get_eval_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, eval_domain_min);
	SUITE_ADD_TEST(suite, eval_domain_max);
	SUITE_ADD_TEST(suite, eval_001);
	SUITE_ADD_TEST(suite, eval_002);
	SUITE_ADD_TEST(suite, eval_003);
	return suite;
}