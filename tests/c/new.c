#include "tinyspline.h"
#include "CuTest.h"
#include "utils.h"
#include <stdint.h>

void new_test_bspline_zero_dim(CuTest* tc)
{
	tsBSpline bspline;
	tsError err;

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(3, 0, 7, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DIM_ZERO, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(2, 0, 10, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DIM_ZERO, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(50, 0, 100, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DIM_ZERO, err);
	ctests_assert_default_bspline(tc, &bspline);
}

void new_test_bspline_deg_greater_nctrlp(CuTest* tc)
{
	tsBSpline bspline;
	tsError err;

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(2, 1, 3, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(4, 2, 6, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(3, 3, 10, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(90, 4, 100, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);
}

void new_test_bspline_deg_equals_nctrlp(CuTest* tc)
{
	tsBSpline bspline;
	tsError err;

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(0, 1, 0, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(1, 1, 1, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(2, 1, 2, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(5, 2, 5, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(10, 4, 10, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
	ctests_assert_default_bspline(tc, &bspline);
}

void new_test_bspline_malloc_failed(CuTest* tc)
{
	tsBSpline bspline;
	tsError err;

	ctests_init_non_default_bspline(&bspline);
	/*
	 * The size to allocate is calculated as follows:
	 *      (n_ctrlp*dim + n_ctrlp+deg+1) * sizeof(tsReal)
	 * With dim = 1 and deg = 0 we get:
	 *      (n_ctrlp*1 + n_ctrlp+0+1) * sizeof(tsReal)
	 * Simplified:
	 *      (2*n_ctrlp + 1) * sizeof(tsReal)
	 * Thus, lets pass
	 *      n_ctrlp = (SIZE_MAX/2 - 1) / sizeof(tsReal)
	 * for maximum heap allocation without size_t overflow.
	 * We can assume that this value exceeds the available
	 * memory regarding SIZE_MAX. If not, disable this test.
	 */
	err = ts_bspline_new((SIZE_MAX/2 - 1) / sizeof(tsReal),
		1, 0, TS_NONE, &bspline);
	CuAssertIntEquals(tc, TS_MALLOC, err);
	ctests_assert_default_bspline(tc, &bspline);
}

void new_test_bspline_beziers_setup_failed(CuTest* tc)
{
	tsBSpline bspline;
	tsError err;

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(7, 3, 3, TS_BEZIERS, &bspline);
	CuAssertIntEquals(tc, TS_NUM_KNOTS, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(11, 2, 3, TS_BEZIERS, &bspline);
	CuAssertIntEquals(tc, TS_NUM_KNOTS, err);
	ctests_assert_default_bspline(tc, &bspline);

	ctests_init_non_default_bspline(&bspline);
	err = ts_bspline_new(15, 1, 5, TS_BEZIERS, &bspline);
	CuAssertIntEquals(tc, TS_NUM_KNOTS, err);
	ctests_assert_default_bspline(tc, &bspline);
}

CuSuite* get_new_suite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, new_test_bspline_zero_dim);
	SUITE_ADD_TEST(suite, new_test_bspline_deg_greater_nctrlp);
	SUITE_ADD_TEST(suite, new_test_bspline_deg_equals_nctrlp);
	SUITE_ADD_TEST(suite, new_test_bspline_malloc_failed);
	SUITE_ADD_TEST(suite, new_test_bspline_beziers_setup_failed);

	return suite;
}
