#include "tinyspline.h"
#include "CuTest.h"
#include <stdlib.h>
#include <tinyspline.h>

void free_test_bspline(CuTest* tc)
{
	tsBSpline spline;

	ts_bspline_new(10, 2, 2, TS_CLAMPED, &spline, NULL);
	CuAssertPtrNotNull(tc, spline.pImpl);

	ts_bspline_free(&spline);
	CuAssertPtrEquals(tc, spline.pImpl, NULL);
}

void free_test_deboornet(CuTest* tc)
{
	tsBSpline spline;
	ts_bspline_new(10, 2, 2, TS_CLAMPED, &spline, NULL);

	tsDeBoorNet net;
	ts_bspline_eval(&spline, 0, &net, NULL);
	CuAssertPtrNotNull(tc, net.pImpl);

	ts_deboornet_free(&net);
	CuAssertPtrEquals(tc, net.pImpl, NULL);

	ts_bspline_free(&spline);
}

CuSuite* get_free_suite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, free_test_bspline);
	SUITE_ADD_TEST(suite, free_test_deboornet);

	return suite;
}
