#include <stdio.h>
#include "CuTest.h"
#include "utils.h"

CuSuite* get_arr_fill_suite();
CuSuite* get_free_suite();
CuSuite* get_new_suite();
CuSuite* get_move_suite();
CuSuite* get_eval_suite();
CuSuite* get_set_knots_suite();
CuSuite* get_insert_knot_suite();
CuSuite* get_sample_suite();
CuSuite* get_to_beziers_suite();
CuSuite* get_interpolation_suite();
CuSuite* get_derive_suite();
CuSuite* get_bisect_suite();
CuSuite* get_save_load_suite();
CuSuite* get_elevate_degree_suite();
CuSuite* get_align_suite();

int main()
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_arr_fill_suite());
	CuSuiteAddSuite(suite, get_free_suite());
	CuSuiteAddSuite(suite, get_new_suite());
	CuSuiteAddSuite(suite, get_move_suite());
	CuSuiteAddSuite(suite, get_eval_suite());
	CuSuiteAddSuite(suite, get_set_knots_suite());
	CuSuiteAddSuite(suite, get_insert_knot_suite());
	CuSuiteAddSuite(suite, get_sample_suite());
	CuSuiteAddSuite(suite, get_to_beziers_suite());
	CuSuiteAddSuite(suite, get_interpolation_suite());
	CuSuiteAddSuite(suite, get_derive_suite());
	CuSuiteAddSuite(suite, get_bisect_suite());
	CuSuiteAddSuite(suite, get_save_load_suite());
	CuSuiteAddSuite(suite, get_elevate_degree_suite());
	CuSuiteAddSuite(suite, get_align_suite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	CuStringDelete(output);
	CuSuiteDelete(suite);

	return 0;
}

void
assert_equal_shape_eps(CuTest *tc, tsBSpline *s1, tsBSpline *s2, tsReal eps)
{
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *s1val = NULL, *s2val = NULL, dist;
	size_t k, dim;
	tsStatus status;

	/* Find minimum dimension. */
	dim = ts_bspline_dimension(s1);
	if (ts_bspline_dimension(s2) < dim)
		dim = ts_bspline_dimension(s2);

	TS_TRY(try, status.code, &status)
		for (k = 0; k < TS_MAX_NUM_KNOTS; k++) {
			/* Eval s1. */
			TS_CALL(try, status.code, ts_bspline_eval(
				s1, (tsReal)k / TS_MAX_NUM_KNOTS, &net,
				&status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &s1val, &status))
			ts_deboornet_free(&net);

			/* Eval s2. */
			TS_CALL(try, status.code, ts_bspline_eval(
				s2, (tsReal)k / TS_MAX_NUM_KNOTS, &net,
				&status))
			TS_CALL(try, status.code, ts_deboornet_result(
				&net, &s2val, &status))
			ts_deboornet_free(&net);

			/* Compare results. */
			dist = ts_distance(s1val, s2val, dim);
			CuAssertDblEquals(tc, 0, dist, eps);

			/* Clean up. */
			free(s1val);
			free(s2val);
			s1val = s2val = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_deboornet_free(&net);
		free(s1val);
		free(s2val);
	TS_END_TRY
}

void
assert_equal_shape(CuTest *tc, tsBSpline *s1, tsBSpline *s2)
{
	return assert_equal_shape_eps(tc, s1, s2, CTRLP_EPSILON);
}
