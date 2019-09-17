#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define EPSILON 0.0001

void sample_num_1(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *points = NULL;
	size_t num;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 3, TS_CLAMPED, &spline, &status))

		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &points, &status))
		points[0]  = -1.75f; /* x0 */
		points[1]  =  1.0f;  /* y0 */
		points[2]  = -1.5f;  /* x1 */
		points[3]  = -0.5f;  /* y1 */
		points[4]  = -1.5f;  /* x2 */
		points[5]  =  0.0f;  /* y2 */
		points[6]  = -1.25f; /* x3 */
		points[7]  =  0.5f;  /* y3 */
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, points, &status));
		free(points);
		points = NULL;

		TS_CALL(try, status.code, ts_bspline_sample(
			&spline, 1, &points, &num, &status))
		CuAssertTrue(tc, num == 1);
		CuAssertDblEquals(tc, -1.75f, points[0], EPSILON);
		CuAssertDblEquals(tc,  1.0f , points[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(points);
	TS_END_TRY
}

void sample_num_2(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *points = NULL;
	size_t num;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 3, TS_CLAMPED, &spline, &status))

		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &points, &status))
		points[0]  = -1.75f; /* x0 */
		points[1]  =  1.0f;  /* y0 */
		points[2]  = -1.5f;  /* x1 */
		points[3]  =  0.5f;  /* y1 */
		points[4]  = -1.5f;  /* x2 */
		points[5]  =  0.0f;  /* y2 */
		points[6]  =  1.25f; /* x3 */
		points[7]  = -0.5f;  /* y3 */
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, points, &status));
		free(points);
		points = NULL;

		TS_CALL(try, status.code, ts_bspline_sample(
			&spline, 2, &points, &num, &status))
		CuAssertTrue(tc, num == 2);
		CuAssertDblEquals(tc, -1.75f, points[0], EPSILON);
		CuAssertDblEquals(tc,  1.0f , points[1], EPSILON);
		CuAssertDblEquals(tc,  1.25f, points[2], EPSILON);
		CuAssertDblEquals(tc, -0.5f , points[3], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(points);
	TS_END_TRY
}

void sample_num_3(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *points = NULL, *result = NULL;
	size_t num;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 3, TS_CLAMPED, &spline, &status))

		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &points, &status))
		points[0]  = -1.75f; /* x0 */
		points[1]  =  1.0f;  /* y0 */
		points[2]  = -1.5f;  /* x1 */
		points[3]  =  0.5f;  /* y1 */
		points[4]  = -1.5f;  /* x2 */
		points[5]  =  0.0f;  /* y2 */
		points[6]  =  1.25f; /* x3 */
		points[7]  = -0.5f;  /* y3 */
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, points, &status));
		free(points);
		points = NULL;

		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, 0.5f, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))

		TS_CALL(try, status.code, ts_bspline_sample(
			&spline, 3, &points, &num, &status))
		CuAssertTrue(tc, num == 3);
		CuAssertDblEquals(tc, -1.75f, points[0], EPSILON);
		CuAssertDblEquals(tc,  1.0f , points[1], EPSILON);
		CuAssertDblEquals(tc, result[0], points[2], EPSILON);
		CuAssertDblEquals(tc, result[1], points[3], EPSILON);
		CuAssertDblEquals(tc,  1.25f, points[4], EPSILON);
		CuAssertDblEquals(tc, -0.5f , points[5], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(points);
		ts_deboornet_free(&net);
		free(result);
	TS_END_TRY
}

CuSuite* get_sample_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, sample_num_1);
	SUITE_ADD_TEST(suite, sample_num_2);
	SUITE_ADD_TEST(suite, sample_num_3);
	return suite;
}