#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

void set_knots_custom_interval(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *result = NULL;
	tsStatus status;

	tsReal knots[11] = {
		-2.f, -1.f, -1.f, -0.5f, 1.25f, 1.5f, 1.75f, 2.f, 3.f, 4.f, 7.f
	};

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			7, 2, 3, TS_OPENED, &spline, &status))

/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_set_knots(
			&spline, knots, &status))

/* ================================= Then ================================== */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &result, &status))

		CuAssertTrue(tc, knots != result);
		CuAssertDblEquals(tc, -2.f,   result[0],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, -1.f,   result[1],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, -1.f,   result[2],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, -0.5f,  result[3],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc,  1.25f, result[4],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc,  1.5f,  result[5],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc,  1.75f, result[6],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc,  2.f,   result[7],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc,  3.f,   result[8],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc,  4.f,   result[9],  TS_KNOT_EPSILON);
		CuAssertDblEquals(tc,  7.f,   result[10], TS_KNOT_EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(result);
		result = NULL;
	TS_END_TRY
}

void set_knots_decreasing_knot_vector(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *result = NULL;
	tsStatus status;
	tsError err = TS_SUCCESS;

	tsReal knots[6] = {
		1.f, 1.f, 2.f, 2.f, 1.f, 3.f
	};

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			3, 2, 2, TS_CLAMPED, &spline, &status))
		CuAssertIntEquals(tc, TS_SUCCESS, err);

/* ================================= When ================================== */
		err = ts_bspline_set_knots(&spline, knots, NULL);

/* ================================= Then ================================== */
		CuAssertIntEquals(tc, TS_KNOTS_DECR, err);

		/* Check if knots changed. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &result, &status))

		CuAssertTrue(tc, knots != result);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			result[0], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			result[1], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			result[2], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			result[3], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			result[4], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			result[5], TS_KNOT_EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(result);
		result = NULL;
	TS_END_TRY
}

void set_knots_exceeding_multiplicity(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *result = NULL;
	tsStatus status;
	tsError err = TS_SUCCESS;

	tsReal knots[6] = {
		2.f, 2.f, 2.f, 2.f, 3.f, 3.f
	};

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			3, 2, 2, TS_CLAMPED, &spline, &status))
		CuAssertIntEquals(tc, TS_SUCCESS, err);

/* ================================= When ================================== */
		err = ts_bspline_set_knots(&spline, knots, NULL);

/* ================================= Then ================================== */
		CuAssertIntEquals(tc, TS_MULTIPLICITY, err);

		/* Check if knots changed. */
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &result, &status))

		CuAssertTrue(tc, knots != result);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			result[0], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			result[1], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN,
			result[2], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			result[3], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			result[4], TS_KNOT_EPSILON);
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX,
			result[5], TS_KNOT_EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(result);
		result = NULL;
	TS_END_TRY
}

void set_knot_at(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal knot;
	tsStatus status;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			2, 1, 1, TS_OPENED, &spline, &status))

		/* Check default knots. */
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot,
			TS_KNOT_EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 3, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot,
			TS_KNOT_EPSILON);

/* ================================ When (1) =============================== */
		/* Set and check custom knot at index 0. */
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 0, TS_DOMAIN_DEFAULT_MIN - 1, &status))
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
/* ================================ Then (1) =============================== */
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN - 1,
			knot, TS_KNOT_EPSILON);

/* ================================ When (2) =============================== */
		/* Set and check custom knot at index 3. */
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 3, TS_DOMAIN_DEFAULT_MAX + 1, &status))
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 3, &knot, &status))
/* ================================ Then (2) =============================== */
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX + 1,
			knot, TS_KNOT_EPSILON);
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
	tsError err = TS_SUCCESS;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			1, 2, 0, TS_CLAMPED, &spline, &status))
		CuAssertIntEquals(tc, TS_SUCCESS, err);

/* ================================ When (1) =============================== */
		/* Set knot at invalid index 2 and check if knots changed. */
		err = ts_bspline_set_knot_at(&spline, 2,
			TS_DOMAIN_DEFAULT_MAX + 1, &status);
/* ================================ Then (1) =============================== */
		CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot,
			TS_KNOT_EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 1, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot,
			TS_KNOT_EPSILON);
		err = TS_SUCCESS; /* restore */

/* ================================ When (2) =============================== */
		/* Set knot at invalid index 10 and check if knots changed. */
		CuAssertIntEquals(tc, TS_SUCCESS, err);
		err = ts_bspline_set_knot_at(&spline, 10,
			TS_DOMAIN_DEFAULT_MAX + 1, &status);
/* ================================ Then (2) =============================== */
		CuAssertIntEquals(tc, TS_INDEX_ERROR, err);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot,
			TS_KNOT_EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 1, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot,
			TS_KNOT_EPSILON);
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
	tsError err = TS_SUCCESS;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			1, 2, 0, TS_CLAMPED, &spline, &status))
		CuAssertIntEquals(tc, TS_SUCCESS, err);

/* ================================= When ================================== */
		err = ts_bspline_set_knot_at( &spline, 1,
			TS_DOMAIN_DEFAULT_MIN - 1, &status);

/* ================================= Then ================================== */
		CuAssertIntEquals(tc, TS_KNOTS_DECR, err);

		/* Check if knots changed. */
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot,
			TS_KNOT_EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 1, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot,
			TS_KNOT_EPSILON);
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
	tsError err = TS_SUCCESS;

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			1, 2, 0, TS_CLAMPED, &spline, &status))
		CuAssertIntEquals(tc, TS_SUCCESS, err);

/* ================================= When ================================== */
		err = ts_bspline_set_knot_at(&spline, 1,
			TS_DOMAIN_DEFAULT_MIN, &status);

/* ================================= Then ================================== */
		CuAssertIntEquals(tc, TS_MULTIPLICITY, err);

		/* Check if knots changed. */
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 0, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MIN, knot,
			TS_KNOT_EPSILON);
		TS_CALL(try, status.code, ts_bspline_knot_at(
			&spline, 1, &knot, &status))
		CuAssertDblEquals(tc, TS_DOMAIN_DEFAULT_MAX, knot,
			TS_KNOT_EPSILON);
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