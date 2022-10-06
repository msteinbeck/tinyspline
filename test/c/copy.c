#include <testutils.h>

void
copy_deboornet_different(CuTest *tc)
{
	___SETUP___
	tsBSpline spline = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsDeBoorNet copied = ts_deboornet_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		2, 2, 1, TS_CLAMPED, &spline, &status,
		0.0, 0.0,    /* P1 */
		15.0, 20.0)) /* P2 */
	C(ts_bspline_eval(&spline, (tsReal) 0.0, &net, &status))
	CuAssertPtrNotNull(tc, net.pImpl);
	CuAssertPtrEquals(tc, NULL, copied.pImpl);

	___WHEN___
	ts_deboornet_copy(&net, &copied, &status);

	___THEN___
	CuAssertPtrNotNull(tc, net.pImpl);
	CuAssertPtrNotNull(tc, copied.pImpl);
	CuAssertTrue(tc, net.pImpl != copied.pImpl);

	___TEARDOWN___
	ts_bspline_free(&spline);
	ts_deboornet_free(&net);
	ts_deboornet_free(&copied);
}

void
copy_deboornet_same_ptr(CuTest *tc)
{
	___SETUP___
	tsBSpline point = ts_bspline_init();
	tsDeBoorNet net = ts_deboornet_init();
	tsDeBoorNet backup = ts_deboornet_init();

	___GIVEN___
	C(ts_bspline_new_with_control_points(
		2, 2, 1, TS_CLAMPED, &point, &status,
		1.0, 4.0,    /* P1 */
		15.0, 20.0)) /* P2 */
	C(ts_bspline_eval(&point, (tsReal) 0.0, &net, &status))
	CuAssertPtrNotNull(tc, net.pImpl);
	backup.pImpl = net.pImpl;

	___WHEN___
	ts_deboornet_copy(&net, &net, &status);

	___THEN___
	CuAssertPtrEquals(tc, backup.pImpl, net.pImpl);

	___TEARDOWN___
	ts_bspline_free(&point);
	ts_deboornet_free(&net);
}

CuSuite *
get_copy_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, copy_deboornet_different);
	SUITE_ADD_TEST(suite, copy_deboornet_same_ptr);
	return suite;
}
