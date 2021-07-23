#include <testutils.h>

void move_bspline_different_ptr(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsBSpline moved = ts_bspline_init();

	___GIVEN___
	C(ts_bspline_new(6, 3, 3, TS_OPENED, &spline, &status))
	CuAssertPtrNotNull(tc, spline.pImpl);
	CuAssertPtrEquals(tc, NULL, moved.pImpl);

	___WHEN___
	ts_bspline_move(&spline, &moved);

	___THEN___
	CuAssertPtrEquals(tc, spline.pImpl, NULL);
	CuAssertPtrNotNull(tc, moved.pImpl);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_bspline_free(&moved);
}

void move_deboornet_different_ptr(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsDeBoorNet moved = ts_deboornet_init();

	___GIVEN___
	C(ts_bspline_new(6, 3, 3, TS_OPENED, &spline, &status))
	C(ts_bspline_eval(&spline, (tsReal) 0.5, &net, &status))
	CuAssertPtrNotNull(tc, net.pImpl);
	CuAssertPtrEquals(tc, NULL, moved.pImpl);

	___WHEN___
	ts_deboornet_move(&net, &moved);

	___THEN___
	CuAssertPtrEquals(tc, NULL, net.pImpl);
	CuAssertPtrNotNull(tc, moved.pImpl);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	ts_deboornet_free(&moved);
}

CuSuite* get_move_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, move_bspline_different_ptr);
	SUITE_ADD_TEST(suite, move_deboornet_different_ptr);
	return suite;
}
