#include "tinyspline.h"
#include "CuTest.h"
#include <stdint.h>

void new_test_bspline_zero_dim(CuTest* tc)
{
	tsBSpline spline;
	tsError err;

	err = ts_bspline_new(3, 0, 7, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DIM_ZERO, err);

	err = ts_bspline_new(2, 0, 10, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DIM_ZERO, err);

	err = ts_bspline_new(50, 0, 100, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DIM_ZERO, err);
}

void new_test_bspline_deg_greater_nctrlp(CuTest* tc)
{
	tsBSpline spline;
	tsError err;

	err = ts_bspline_new(2, 1, 3, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);

	err = ts_bspline_new(4, 2, 6, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);

	err = ts_bspline_new(3, 3, 10, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);

	err = ts_bspline_new(90, 4, 100, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
}

void new_test_bspline_deg_equals_nctrlp(CuTest* tc)
{
	tsBSpline spline;
	tsError err;

	err = ts_bspline_new(0, 1, 0, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);

	err = ts_bspline_new(1, 1, 1, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);

	err = ts_bspline_new(2, 1, 2, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);

	err = ts_bspline_new(5, 2, 5, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);

	err = ts_bspline_new(10, 4, 10, TS_OPENED, &spline, NULL);
	CuAssertIntEquals(tc, TS_DEG_GE_NCTRLP, err);
}

void new_test_bspline_beziers_setup_failed(CuTest* tc)
{
	tsBSpline spline;
	tsError err;

	err = ts_bspline_new(7, 3, 3, TS_BEZIERS, &spline, NULL);
	CuAssertIntEquals(tc, TS_NUM_KNOTS, err);

	err = ts_bspline_new(11, 2, 3, TS_BEZIERS, &spline, NULL);
	CuAssertIntEquals(tc, TS_NUM_KNOTS, err);

	err = ts_bspline_new(15, 1, 5, TS_BEZIERS, &spline, NULL);
	CuAssertIntEquals(tc, TS_NUM_KNOTS, err);
}

CuSuite* get_new_suite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, new_test_bspline_zero_dim);
	SUITE_ADD_TEST(suite, new_test_bspline_deg_greater_nctrlp);
	SUITE_ADD_TEST(suite, new_test_bspline_deg_equals_nctrlp);
	SUITE_ADD_TEST(suite, new_test_bspline_beziers_setup_failed);

	return suite;
}
