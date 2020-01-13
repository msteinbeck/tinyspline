#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define EPSILON 0.0001

void derive_sequence_of_four_points(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *ctrlp = NULL, *knots = NULL;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		/* Create derivative. */
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 0, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &spline,
			&status))

		/* Check control points of derivative. */
		CuAssertIntEquals(tc, 1,
			(int) ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		CuAssertDblEquals(tc, 0.f, ctrlp[0], EPSILON);
		CuAssertDblEquals(tc, 0.f, ctrlp[1], EPSILON);

		/* Check knots of derivative. */
		CuAssertIntEquals(tc, 2,
			(int) ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, TS_MIN_KNOT_VALUE, knots[0], EPSILON);
		CuAssertDblEquals(tc, TS_MAX_KNOT_VALUE, knots[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(ctrlp);
		free(knots);
	TS_END_TRY
}

void derive_sequence_of_two_point_with_custom_knots(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *ctrlp = NULL, *knots = NULL;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		/* Create derivative with custom knots. */
		TS_CALL(try, status.code, ts_bspline_new(
			2, 3, 0, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 0, -10.f, &status))
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 2, 10.f, &status))
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &spline,
			&status))

		/* Check control points of derivative. */
		CuAssertIntEquals(tc, 1,
			(int) ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		CuAssertDblEquals(tc, 0.f, ctrlp[0], EPSILON);
		CuAssertDblEquals(tc, 0.f, ctrlp[1], EPSILON);

		/* Check knots of derivative. */
		CuAssertIntEquals(tc, 2,
			(int) ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, -10.f, knots[0], EPSILON);
		CuAssertDblEquals(tc,  10.f, knots[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(ctrlp);
		free(knots);
	TS_END_TRY
}

void derive_single_line(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *ctrlp = NULL, *knots = NULL;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			2, 2, 1, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		ctrlp[0] =  1.0; ctrlp[1] = -2.0;
		ctrlp[2] =  3.0; ctrlp[3] =  6.0;
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		free(ctrlp);
		ctrlp = NULL;

		/* Create derivative. */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &spline,
			&status))

		/* Check control points of derivative. */
		CuAssertIntEquals(tc, 1,
			(int) ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		CuAssertDblEquals(tc, 2.f, ctrlp[0], EPSILON);
		CuAssertDblEquals(tc, 8.f, ctrlp[1], EPSILON);

		/* Check knots of derivative. */
		CuAssertIntEquals(tc, 2,
			(int) ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, TS_MIN_KNOT_VALUE, knots[0], EPSILON);
		CuAssertDblEquals(tc, TS_MAX_KNOT_VALUE, knots[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(ctrlp);
		free(knots);
	TS_END_TRY
}

void derive_single_line_with_custom_knots(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsReal *ctrlp = NULL, *knots = NULL;
	tsStatus status;

	TS_TRY(try, status.code, &status)
		/* Create spline. */
		TS_CALL(try, status.code, ts_bspline_new(
			2, 2, 1, TS_CLAMPED, &spline, &status))

		/* Setup control points. */
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		ctrlp[0] =  1.0; ctrlp[1] = -2.0;
		ctrlp[2] =  3.0; ctrlp[3] =  6.0;
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		free(ctrlp);
		ctrlp = NULL;

		/* Setup knots. */
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 0, -2.f, &status))
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 1, -1.f, &status))
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 2, 1.f, &status))
		TS_CALL(try, status.code, ts_bspline_set_knot_at(
			&spline, 3, 2.f, &status))

		/* Create derivative. */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &spline,
			&status))

		/* Check control points of derivative. */
		CuAssertIntEquals(tc, 1,
			(int) ts_bspline_num_control_points(&spline));
		TS_CALL(try, status.code, ts_bspline_control_points(
			&spline, &ctrlp, &status))
		CuAssertDblEquals(tc, 2.f, ctrlp[0], EPSILON);
		CuAssertDblEquals(tc, 8.f, ctrlp[1], EPSILON);

		/* Check knots of derivative. */
		CuAssertIntEquals(tc, 2,
			(int) ts_bspline_num_knots(&spline));
		TS_CALL(try, status.code, ts_bspline_knots(
			&spline, &knots, &status))
		CuAssertDblEquals(tc, -1.f, knots[0], EPSILON);
		CuAssertDblEquals(tc,  1.f, knots[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		free(ctrlp);
		free(knots);
	TS_END_TRY
}

void derive_discontinuous_and_compare_with_continuous(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsBSpline beziers = ts_bspline_init();
	tsBSpline derivative = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result_beziers = NULL, *result_derivative = NULL;
	tsReal min, max, knot, dist;
	tsStatus status;

	tsReal ctrlp[14];
	ctrlp[0]  = -1.75; ctrlp[1]  = -1.0;
	ctrlp[2]  = -1.5;  ctrlp[3]  = -0.5;
	ctrlp[4]  = -1.5;  ctrlp[5]  =  0.0;
	ctrlp[6]  = -1.25; ctrlp[7]  =  0.5;
	ctrlp[8]  = -0.75; ctrlp[9]  =  0.75;
	ctrlp[10] =  0.0;  ctrlp[11] =  0.5;
	ctrlp[12] =  0.5;  ctrlp[13] =  0.0;

	TS_TRY(try, status.code, &status)
		/* Create base spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			7, 2, 3, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		ts_bspline_domain(&spline, &min, &max);

		/* Create beziers (to_beziers => derive). */
		TS_CALL(try, status.code, ts_bspline_to_beziers(
			&spline, &beziers, &status))
		TS_CALL(try, status.code, ts_bspline_derive(
			&beziers, 1, TS_CONTROL_POINT_EPSILON, &beziers,
			&status))

		/* Create derivative (derive => to_beziers). */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &derivative,
			&status))
		TS_CALL(try, status.code, ts_bspline_to_beziers(
			&derivative, &derivative, &status))

		/* Check that beziers and derivative are different splines. */
		CuAssertTrue(tc, beziers.pImpl != derivative.pImpl);

		/* Compare beziers (discontinuous derivation)
		 * with derivative (continuous derivation). */
		for (knot = min; knot < max;
			knot += (max - min) / TS_MAX_NUM_KNOTS) {

			/* Eval beziers. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&beziers, knot, &net, &status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &result_beziers, &status));
			ts_deboornet_free(&net);

			/* Eval derivative. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&derivative, knot, &net, &status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &result_derivative, &status));
			ts_deboornet_free(&net);

			/* Compare results. */
			dist = ts_distance(result_beziers, result_derivative,
				ts_bspline_dimension(&spline));
			CuAssertDblEquals(tc, 0.f, dist, EPSILON);
			free(result_beziers);
			result_beziers = NULL;
			free(result_derivative);
			result_derivative = NULL;
		}

		/* Eval known values. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&beziers, 0.3, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result_beziers, &status));
		CuAssertIntEquals(tc, 1,
			(int) ts_deboornet_num_result(&net));
		CuAssertDblEquals(tc, 0.7, result_beziers[0], EPSILON);
		CuAssertDblEquals(tc, 2.3, result_beziers[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&beziers);
		ts_bspline_free(&derivative);
		ts_deboornet_free(&net);
		free(result_beziers);
		free(result_derivative);
	TS_END_TRY
}

void derive_discontinuous_lines_exceeding_epsilon(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsStatus status;
	tsError err;
	tsReal ctrlp[8];
	tsReal knots[6];

	ctrlp[0]  = 0; ctrlp[1] = 0;
	ctrlp[2]  = 1; ctrlp[3] = 1;
	ctrlp[4]  = 1; ctrlp[5] = 1.5;
	ctrlp[6]  = 2; ctrlp[7] = 2;

	knots[0] = 0;
	knots[1] = 0;
	knots[2] = 0.4;
	knots[3] = 0.4;
	knots[4] = 1;
	knots[5] = 1;

	TS_TRY(try, status.code, &status)
		/* Create spline with control points and knots. */
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 1, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		TS_CALL(try, status.code, ts_bspline_set_knots(
			&spline, knots, &status))

		/* Check if derive fails. */
		err = ts_bspline_derive(&spline, 1, EPSILON, &spline, NULL);
		CuAssertIntEquals(tc, TS_UNDERIVABLE, err);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
	TS_END_TRY
}

void derive_discontinuous_lines_ignoring_epsilon(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result = NULL;
	tsStatus status;
	tsError err;
	tsReal ctrlp[8];
	tsReal knots[6];

	ctrlp[0]  = 2; ctrlp[1] = 2;
	ctrlp[2]  = 3; ctrlp[3] = 3;
	ctrlp[4]  = 4; ctrlp[5] = 4;
	ctrlp[6]  = 1; ctrlp[7] = 1;

	knots[0] = 0;
	knots[1] = 0;
	knots[2] = 0.7;
	knots[3] = 0.7;
	knots[4] = 1;
	knots[5] = 1;

	TS_TRY(try, status.code, &status)
		/* Create spline with control points and knots. */
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 1, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		TS_CALL(try, status.code, ts_bspline_set_knots(
			&spline, knots, &status))

		/* Create derivative. */
		err = ts_bspline_derive(&spline, 1, -1.f, &spline, NULL);
		CuAssertIntEquals(tc, TS_SUCCESS, err);

		/* Eval derivative. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, 0.7, &net, &status))
		CuAssertIntEquals(tc, 2, (int)ts_deboornet_num_result(&net));
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		CuAssertDblEquals(tc,  1.0, result[0], EPSILON);
		CuAssertDblEquals(tc,  1.0, result[1], EPSILON);
		CuAssertDblEquals(tc, -2.0, result[2], EPSILON);
		CuAssertDblEquals(tc, -2.0, result[3], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
		free(result);
	TS_END_TRY
}

void derive_continuous_spline(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result = NULL;
	tsStatus status;

	tsReal ctrlp[20];
	ctrlp[0]  = -1.75; ctrlp[1]  = -1.0;
	ctrlp[2]  = -1.5;  ctrlp[3]  = -0.5;
	ctrlp[4]  = -1.5;  ctrlp[5]  =  0.0;
	ctrlp[6]  = -1.25; ctrlp[7]  =  0.5;
	ctrlp[8]  = -0.75; ctrlp[9]  =  0.75;
	ctrlp[10] =  0.0;  ctrlp[11] =  0.5;
	ctrlp[12] =  0.5;  ctrlp[13] =  0.0;
	ctrlp[14] =  1.5;  ctrlp[15] =  0.5;
	ctrlp[16] =  2.0;  ctrlp[17] =  1.5;
	ctrlp[18] =  2.5;  ctrlp[19] =  1.0;

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			10, 2, 2, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))

		/* Create derivative. */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &spline,
			&status))

		/* Eval derivative. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, 0.8, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		CuAssertDblEquals(tc,  6.4, result[0], EPSILON);
		CuAssertDblEquals(tc,  5.6, result[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
		free(result);
	TS_END_TRY
}

void derive_continuous_spline_with_custom_knots(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result = NULL;
	tsStatus status;
	tsReal ctrlp[20];
	tsReal knots[13];

	ctrlp[0]  = -1.75; ctrlp[1]  = -1.0;
	ctrlp[2]  = -1.5;  ctrlp[3]  = -0.5;
	ctrlp[4]  = -1.5;  ctrlp[5]  =  0.0;
	ctrlp[6]  = -1.25; ctrlp[7]  =  0.5;
	ctrlp[8]  = -0.75; ctrlp[9]  =  0.75;
	ctrlp[10] =  0.0;  ctrlp[11] =  0.5;
	ctrlp[12] =  0.5;  ctrlp[13] =  0.0;
	ctrlp[14] =  1.5;  ctrlp[15] =  0.5;
	ctrlp[16] =  2.0;  ctrlp[17] =  1.5;
	ctrlp[18] =  2.5;  ctrlp[19] =  1.0;

	knots[0]  =  2.f;
	knots[1]  =  2.f;
	knots[2]  =  2.f;
	knots[3]  =  3.f;
	knots[4]  =  4.f;
	knots[5]  =  5.f;
	knots[6]  =  6.f;
	knots[7]  =  7.f;
	knots[8]  =  8.f;
	knots[9]  =  9.f;
	knots[10] = 10.f;
	knots[11] = 10.f;
	knots[12] = 10.f;

	TS_TRY(try, status.code, &status)
		/* Create spline with control points and custom knots. */
		TS_CALL(try, status.code, ts_bspline_new(
			10, 2, 2, TS_CLAMPED, &spline, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		TS_CALL(try, status.code, ts_bspline_set_knots(
			&spline, knots, &status))

		/* Create derivative. */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, TS_CONTROL_POINT_EPSILON, &spline,
			&status))

		/* Eval derivative. */
		TS_CALL(try, status.code, ts_bspline_eval(
			&spline, 8.4, &net, &status))
		TS_CALL(try, status.code, ts_deboornet_result(
			&net, &result, &status))
		CuAssertDblEquals(tc,  6.4, result[0], EPSILON);
		CuAssertDblEquals(tc,  5.6, result[1], EPSILON);
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_deboornet_free(&net);
		free(result);
	TS_END_TRY
}

void derive_compare_third_derivative_with_three_times(CuTest *tc)
{
	tsBSpline spline = ts_bspline_init();
	tsBSpline third = ts_bspline_init();
	tsBSpline three = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result_third = NULL, *result_three = NULL;
	tsReal min, max, knot, dist;
	tsStatus status;

	tsReal ctrlp[8];
	ctrlp[0] = 1.f; ctrlp[0] = 1.f;
	ctrlp[1] = 2.f; ctrlp[0] = 4.f;
	ctrlp[1] = 3.f; ctrlp[0] = 3.f;
	ctrlp[1] = 4.f; ctrlp[0] = 0.f;

	TS_TRY(try, status.code, &status)
		/* Create spline with control points. */
		TS_CALL(try, status.code, ts_bspline_new(
			4, 2, 3, TS_OPENED, &spline, &status));
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&spline, ctrlp, &status))
		ts_bspline_domain(&spline, &min, &max);

		/* Create third (derive with n = 3). */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 3, EPSILON, &third, &status))

		/* Create three (derive three times). */
		TS_CALL(try, status.code, ts_bspline_derive(
			&spline, 1, EPSILON, &three, &status));
		TS_CALL(try, status.code, ts_bspline_derive(
			&three, 1, EPSILON, &three, &status));
		TS_CALL(try, status.code, ts_bspline_derive(
			&three, 1, EPSILON, &three, &status));

		/* Check that third and three are different splines. */
		CuAssertTrue(tc, third.pImpl != three.pImpl);

		/* Compare third and three. */
		for (knot = min; knot < max;
			knot += (max - min) / TS_MAX_NUM_KNOTS) {

			/* Eval third. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&third, knot, &net, &status));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &result_third, &status))
			ts_deboornet_free(&net);

			/* Eval three. */
			TS_CALL(try, status.code, ts_bspline_eval(
				&three, knot, &net, &status));
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &result_three, &status))
			ts_deboornet_free(&net);

			/* Compare results. */
			dist = ts_distance(result_third, result_three,
				ts_bspline_dimension(&spline));
			CuAssertDblEquals(tc, 0.f, dist, EPSILON);
			free(result_third);
			result_third = NULL;
			free(result_three);
			result_three = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&spline);
		ts_bspline_free(&third);
		ts_bspline_free(&three);
		ts_deboornet_free(&net);
		free(result_third);
		free(result_three);
	TS_END_TRY
}

CuSuite* get_derive_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, derive_sequence_of_four_points);
	SUITE_ADD_TEST(suite, derive_sequence_of_two_point_with_custom_knots);
	SUITE_ADD_TEST(suite, derive_single_line);
	SUITE_ADD_TEST(suite, derive_single_line_with_custom_knots);
	SUITE_ADD_TEST(suite, derive_discontinuous_and_compare_with_continuous);
	SUITE_ADD_TEST(suite, derive_discontinuous_lines_exceeding_epsilon);
	SUITE_ADD_TEST(suite, derive_discontinuous_lines_ignoring_epsilon);
	SUITE_ADD_TEST(suite, derive_continuous_spline);
	SUITE_ADD_TEST(suite, derive_continuous_spline_with_custom_knots);
	SUITE_ADD_TEST(suite, derive_compare_third_derivative_with_three_times);
	return suite;
}