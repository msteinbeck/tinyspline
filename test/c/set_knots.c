#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define EPSILON 0.0001

void set_knots_custom_interval(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *knots = NULL;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			7, 2, 3, TS_OPENED, &spline, &status))

		/* Set knots. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		knots[0]  = -2.f;
		knots[1]  = -1.f;
		knots[2]  = -1.f;
		knots[3]  = -0.5f;
		knots[4]  =  1.25f;
		knots[5]  =  1.5f;
		knots[6]  =  1.75f;
		knots[7]  =  2.f;
		knots[8]  =  3.f;
		knots[9]  =  4.f;
		knots[10] =  7.f;
		TS_CALL(try, status.code, ts_bspline_set_knots(
			&spline, knots, &status))
		free(knots);
		knots = NULL;

		/* Check knots. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, -2.f,   knots[0],  EPSILON);
		CuAssertDblEquals(tc, -1.f,   knots[1],  EPSILON);
		CuAssertDblEquals(tc, -1.f,   knots[2],  EPSILON);
		CuAssertDblEquals(tc, -0.5f,  knots[3],  EPSILON);
		CuAssertDblEquals(tc,  1.25f, knots[4],  EPSILON);
		CuAssertDblEquals(tc,  1.5f,  knots[5],  EPSILON);
		CuAssertDblEquals(tc,  1.75f, knots[6],  EPSILON);
		CuAssertDblEquals(tc,  2.f,   knots[7],  EPSILON);
		CuAssertDblEquals(tc,  3.f,   knots[8],  EPSILON);
		CuAssertDblEquals(tc,  4.f,   knots[9],  EPSILON);
		CuAssertDblEquals(tc,  7.f,   knots[10], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(knots);
	TS_END_TRY
}

void set_knots_decreasing_knot_vector(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *knots = NULL;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			3, 2, 2, TS_CLAMPED, &spline, &status))

		/* Set knots. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		knots[0]  = 1.f;
		knots[1]  = 1.f;
		knots[2]  = 2.f;
		knots[3]  = 2.f;
		knots[4]  = 1.f;
		knots[5]  = 3.f;
		CuAssertIntEquals(tc, TS_KNOTS_DECR,
			ts_bspline_set_knots(&spline, knots, NULL));
		free(knots);
		knots = NULL;

		/* Check if knots changed. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			knots[0], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			knots[1], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			knots[2], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			knots[3], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			knots[4], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			knots[5], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(knots);
	TS_END_TRY
}

void set_knots_exceeding_multiplicity(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *knots = NULL;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			3, 2, 2, TS_CLAMPED, &spline, &status))

		/* Set knots. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		knots[0]  = 2.f;
		knots[1]  = 2.f;
		knots[2]  = 2.f;
		knots[3]  = 2.f;
		knots[4]  = 3.f;
		knots[5]  = 3.f;
		CuAssertIntEquals(tc, TS_MULTIPLICITY,
			ts_bspline_set_knots(&spline, knots, NULL));
		free(knots);
		knots = NULL;

		/* Check if knots changed. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			knots[0], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			knots[1], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			knots[2], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			knots[3], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			knots[4], EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			knots[5], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(knots);
	TS_END_TRY
}

void set_knot_at(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal knot;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			2, 1, 1, TS_OPENED, &spline, &status))

		/* Check default knots. */
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot, EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 3, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot, EPSILON);

		/* Set and check custom knot at index 0. */
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 0, TS_DOMAIN_DEFAULT_MIN - 1, &status))
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN - 1,
			knot, EPSILON);

		/* Set and check custom knot at index 3. */
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 3, TS_DOMAIN_DEFAULT_MAX + 1, &status))
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 3, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX + 1,
			knot, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
	TS_END_TRY
}

void set_knot_at_invalid_index(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal knot;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			1, 2, 0, TS_CLAMPED, &spline, &status))

		/* Set knot at invalid index 2 and check if knots changed. */
		CuAssertIntEquals(tc, TS_INDEX_ERROR, ts_bspline_set_knot_at(
			&spline, 2, TS_DOMAIN_DEFAULT_MAX + 1, &status));
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot, EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 1, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot, EPSILON);

		/* Set knot at invalid index 10 and check if knots changed. */
		CuAssertIntEquals(tc, TS_INDEX_ERROR, ts_bspline_set_knot_at(
			&spline, 10, TS_DOMAIN_DEFAULT_MAX + 1, &status));
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot, EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 1, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
	TS_END_TRY
}

void set_knot_at_decreasing_knot_vector(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal knot;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			1, 2, 0, TS_CLAMPED, &spline, &status))

		/* Set knot. */
		CuAssertIntEquals(tc, TS_KNOTS_DECR, ts_bspline_set_knot_at(
			&spline, 1, TS_DOMAIN_DEFAULT_MIN - 1, &status));

		/* Check if knots changed. */
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot, EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 1, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
	TS_END_TRY
}

void set_knot_at_exceeding_multiplicity(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal knot;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		TS_CALL(try, status.code, ts_bspline_new(
			1, 2, 0, TS_CLAMPED, &spline, &status))

		/* Set knot. */
		CuAssertIntEquals(tc, TS_MULTIPLICITY, ts_bspline_set_knot_at(
			&spline, 1, TS_DOMAIN_DEFAULT_MIN, &status));

		/* Check if knots changed. */
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot, EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 1, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot, EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
	TS_END_TRY
}

CuSuite* get_set_knots_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, set_knots_custom_interval);
	SUITE_ADD_TEST(suite, set_knots_decreasing_knot_vector);
	SUITE_ADD_TEST(suite, set_knots_exceeding_multiplicity);
	SUITE_ADD_TEST(suite, set_knot_at);
	SUITE_ADD_TEST(suite, set_knot_at_invalid_index);
	SUITE_ADD_TEST(suite, set_knot_at_decreasing_knot_vector);
	SUITE_ADD_TEST(suite, set_knot_at_exceeding_multiplicity);
	return suite;
}