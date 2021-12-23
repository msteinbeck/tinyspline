#include <testutils.h>

void
morph_line_to_line(CuTest *tc)
{
	___SETUP___
	tsBSpline source = ts_bspline_init();
	tsBSpline target = ts_bspline_init();
	tsBSpline morph = ts_bspline_init();
	tsBSpline expected = ts_bspline_init();
	tsReal t, *ctrlp = NULL, tmp[4], vec[2];
	size_t i;

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		2, 2, 1, TS_CLAMPED, &source, &status,
		1.0, 2.0,  /* P1 */
		3.0, 4.0)) /* P2 */
	C(ts_bspline_control_points(&source, &ctrlp, &status))
	memcpy(tmp, ctrlp, 4 * sizeof(tsReal));
	ts_vec_sub(ctrlp + 2, ctrlp, 2, vec);
	ts_vec_add(ctrlp + 2, vec, 2, tmp + 2);
	/* Creates a cubic spline. */
	C(ts_bspline_interpolate_cubic_natural(
		tmp, 2, 2, &target, &status))

	for (i = 0; i < 10; i++)
	{
		___WHEN___
		t = (tsReal)i / (tsReal)10;
		C(ts_bspline_morph(&source,
		                   &target,
		                   t,
		                   POINT_EPSILON,
		                   &morph,
		                   &status))

		___THEN___
		ts_vec_mul(vec, 2, t, tmp);
		ts_vec_add(ctrlp + 2, tmp, 2, tmp + 2);
		memcpy(tmp, ctrlp, 2 * sizeof(tsReal));
		C(ts_bspline_new(2, 2, 1, TS_CLAMPED, &expected, &status))
		C(ts_bspline_set_control_points(&expected, tmp, &status))
		assert_equal_shape(tc, &expected, &morph);
		ts_bspline_free(&expected);

		/* This forces the next morph to re-allocate `morph'. */
		C(ts_bspline_elevate_degree(&morph,
		                            3,
		                            POINT_EPSILON,
		                            &morph,
		                            &status))
	}

	___TEARDOWN___
	ts_bspline_free(&source);
	ts_bspline_free(&target);
	ts_bspline_free(&morph);
	free(ctrlp);
}

CuSuite *
get_morph_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, morph_line_to_line);
	return suite;
}
