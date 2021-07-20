#include <testutils.h>

void move_test_bspline_different_ptr(CuTest *tc)
{
	tsBSpline spline, moved;

/* ================================= Given ================================= */
	ts_bspline_new(6, 3, 3, TS_OPENED, &spline, NULL);
	CuAssertPtrNotNull(tc, spline.pImpl);
	moved.pImpl = NULL;

/* ================================= When ================================== */
	ts_bspline_move(&spline, &moved);

/* ================================= Then ================================== */
	CuAssertPtrEquals(tc, spline.pImpl, NULL);
	CuAssertPtrNotNull(tc, moved.pImpl);

	ts_bspline_free(&moved);
}

void move_test_deboornet_different_ptr(CuTest *tc)
{
	tsBSpline spline;
	tsDeBoorNet net, moved;

/* ================================= Given ================================= */
	ts_bspline_new(6, 3, 3, TS_OPENED, &spline, NULL);
	net.pImpl = moved.pImpl = NULL;
	ts_bspline_eval(&spline, 0.5f, &net, NULL);
	CuAssertPtrNotNull(tc, net.pImpl);

/* ================================= When ================================== */
	ts_deboornet_move(&net, &moved);

/* ================================= Then ================================== */
	CuAssertPtrEquals(tc, net.pImpl, NULL);
	CuAssertPtrNotNull(tc, moved.pImpl);

	ts_bspline_free(&spline);
	ts_deboornet_free(&moved);
}

CuSuite* get_move_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, move_test_bspline_different_ptr);
	SUITE_ADD_TEST(suite, move_test_deboornet_different_ptr);
	return suite;
}
