#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define EPSILON 0.0001

void sample_num_1(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *points = NULL;
	size_t num;
	tsReal dist;
	tsStatus status;

	tsReal ctrlp[12] = {
		-1.75f, 1.0f,
		-1.5f, -0.5f,
		-1.5f,  0.f,
		-1.25f, 0.5f
	};

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 3, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status));

		/* Check sample with num = 1. */
		TS_CALL(try, status.code, ts_bspline_sample(
			&spline, 1, &points, &num, &status))
		CuAssertTrue(tc, num == 1);
		dist = ts_distance(points, ctrlp,
			ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(points);
		points = NULL;
	TS_END_TRY
}

void sample_num_2(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *points = NULL;
	size_t num;
	tsReal dist;
	tsStatus status;

	tsReal ctrlp[12] = {
		-1.75f,  1.0f,
		-1.5f,  -0.5f,
		-1.5f,   0.f,
		 1.25f, -0.5f
	};

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 3, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status));

		/* Check sample with num = 2. */
		TS_CALL(try, status.code, ts_bspline_sample(
			&spline, 2, &points, &num, &status))
		CuAssertTrue(tc, num == 2);
		dist = ts_distance(points, ctrlp,
			ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, EPSILON);
		dist = ts_distance(
			points +   ts_bspline_dimension(&spline),
			ctrlp  + 3*ts_bspline_dimension(&spline),
			ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(points);
		points = NULL;
	TS_END_TRY
}

void sample_num_3(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *middle = NULL, *points = NULL;
	tsReal min, max, dist;
	size_t num;
	tsStatus status;

	tsReal ctrlp[12] = {
		-1.75f,  1.0f,
		-1.5f,   0.5f,
		-1.5f,   0.0f,
		 1.25f, -0.5f
	};

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 3, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status));
		ts_bspline_domain(&spline, &min, &max);

		/* Eval middle point. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, (max-min)/2.f, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &middle, &status))

		/* Check sample with num = 3. */
		TS_CALL(try, status.code, ts_bspline_sample(
			&spline, 3, &points, &num, &status))
		CuAssertTrue(tc, num == 3);
		dist = ts_distance(points, ctrlp,
			ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, EPSILON);
		dist = ts_distance(points + ts_bspline_dimension(&spline),
			middle, ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, EPSILON);
		dist = ts_distance(
			points + 2*ts_bspline_dimension(&spline),
			ctrlp  + 3*ts_bspline_dimension(&spline),
			ts_bspline_dimension(&spline));
		CuAssertDblEquals(tc, 0, dist, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
		free(middle);
		middle = NULL;
		free(points);
		points = NULL;
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