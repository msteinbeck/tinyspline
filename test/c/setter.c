#include <stdlib.h>
#include "tinyspline.h"
#include "CuTest.h"

#define EPSILON 0.0001

void setter_knots_default_interval(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *knots = NULL;
	tsError err;

	err = ts_bspline_new(7, 2, 3, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertIntEquals(tc, 11, (int) ts_bspline_num_knots(&spline));
	err = ts_bspline_knots(&spline, &knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	knots[0]  = 0.f;
	knots[1]  = 0.f;
	knots[2]  = 0.f;
	knots[3]  = 0.f;
	knots[4]  = 0.25f;
	knots[5]  = 0.5f;
	knots[6]  = 0.75f;
	knots[7]  = 1.f;
	knots[8]  = 1.f;
	knots[9]  = 1.f;
	knots[10] = 1.f;
	err = ts_bspline_set_knots(&spline, knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	free(knots);
	err = ts_bspline_knots(&spline, &knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, 0.f,   knots[0],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[1],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[2],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[3],  EPSILON);
	CuAssertDblEquals(tc, 0.25f, knots[4],  EPSILON);
	CuAssertDblEquals(tc, 0.5f,  knots[5],  EPSILON);
	CuAssertDblEquals(tc, 0.75f, knots[6],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[7],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[8],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[9],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[10], EPSILON);

	ts_bspline_free(&spline);
	free(knots);
}

void setter_knots_scaled_interval_1_2(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *knots = NULL;
	tsError err;

	err = ts_bspline_new(7, 2, 3, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertIntEquals(tc, 11, (int) ts_bspline_num_knots(&spline));
	err = ts_bspline_knots(&spline, &knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	knots[0]  = 1.f;
	knots[1]  = 1.f;
	knots[2]  = 1.f;
	knots[3]  = 1.f;
	knots[4]  = 1.25f;
	knots[5]  = 1.5f;
	knots[6]  = 1.75f;
	knots[7]  = 2.f;
	knots[8]  = 2.f;
	knots[9]  = 2.f;
	knots[10] = 2.f;
	err = ts_bspline_set_knots(&spline, knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	free(knots);
	err = ts_bspline_knots(&spline, &knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, 0.f,   knots[0],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[1],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[2],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[3],  EPSILON);
	CuAssertDblEquals(tc, 0.25f, knots[4],  EPSILON);
	CuAssertDblEquals(tc, 0.5f,  knots[5],  EPSILON);
	CuAssertDblEquals(tc, 0.75f, knots[6],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[7],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[8],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[9],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[10], EPSILON);

	ts_bspline_free(&spline);
	free(knots);
}

void setter_knots_scaled_interval_5_9(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *knots = NULL;
	tsError err;

	err = ts_bspline_new(7, 2, 3, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertIntEquals(tc, 11, (int) ts_bspline_num_knots(&spline));
	err = ts_bspline_knots(&spline, &knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	knots[0]  = 5.f;
	knots[1]  = 5.f;
	knots[2]  = 5.f;
	knots[3]  = 5.f;
	knots[4]  = 6.f;
	knots[5]  = 7.f;
	knots[6]  = 8.f;
	knots[7]  = 9.f;
	knots[8]  = 9.f;
	knots[9]  = 9.f;
	knots[10] = 9.f;
	err = ts_bspline_set_knots(&spline, knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	free(knots);
	err = ts_bspline_knots(&spline, &knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, 0.f,   knots[0],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[1],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[2],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[3],  EPSILON);
	CuAssertDblEquals(tc, 0.25f, knots[4],  EPSILON);
	CuAssertDblEquals(tc, 0.5f,  knots[5],  EPSILON);
	CuAssertDblEquals(tc, 0.75f, knots[6],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[7],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[8],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[9],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[10], EPSILON);

	ts_bspline_free(&spline);
	free(knots);
}

void setter_knots_scaled_interval_negative_9_5(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *knots = NULL;
	tsError err;

	err = ts_bspline_new(7, 2, 3, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertIntEquals(tc, 11, (int) ts_bspline_num_knots(&spline));
	err = ts_bspline_knots(&spline, &knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	knots[0]  = -9.f;
	knots[1]  = -9.f;
	knots[2]  = -9.f;
	knots[3]  = -9.f;
	knots[4]  = -8.f;
	knots[5]  = -7.f;
	knots[6]  = -6.f;
	knots[7]  = -5.f;
	knots[8]  = -5.f;
	knots[9]  = -5.f;
	knots[10] = -5.f;
	err = ts_bspline_set_knots(&spline, knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);

	free(knots);
	err = ts_bspline_knots(&spline, &knots, NULL);
	CuAssertIntEquals(tc, TS_SUCCESS, err);
	CuAssertDblEquals(tc, 0.f,   knots[0],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[1],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[2],  EPSILON);
	CuAssertDblEquals(tc, 0.f,   knots[3],  EPSILON);
	CuAssertDblEquals(tc, 0.25f, knots[4],  EPSILON);
	CuAssertDblEquals(tc, 0.5f,  knots[5],  EPSILON);
	CuAssertDblEquals(tc, 0.75f, knots[6],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[7],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[8],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[9],  EPSILON);
	CuAssertDblEquals(tc, 1.f,   knots[10], EPSILON);

	ts_bspline_free(&spline);
	free(knots);
}

CuSuite* get_setter_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, setter_knots_default_interval);
	SUITE_ADD_TEST(suite, setter_knots_scaled_interval_1_2);
	SUITE_ADD_TEST(suite, setter_knots_scaled_interval_5_9);
	SUITE_ADD_TEST(suite, setter_knots_scaled_interval_negative_9_5);
	return suite;
}