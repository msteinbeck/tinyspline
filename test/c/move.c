#include <tinyspline.h>
#include "tinyspline.h"
#include "CuTest.h"

void move_test_bspline_different_ptr(CuTest* tc)
{
	tsBSpline spline, moved;

	ts_bspline_new(6, 3, 3, TS_OPENED, &spline, NULL);
	CuAssertPtrNotNull(tc, spline.pImpl);
	moved.pImpl = NULL;

	ts_bspline_move(&spline, &moved);
	CuAssertPtrEquals(tc, spline.pImpl, NULL);
	CuAssertPtrNotNull(tc, moved.pImpl);

	ts_bspline_free(&moved);
}

CuSuite* get_move_suite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, move_test_bspline_different_ptr);

	return suite;
}
