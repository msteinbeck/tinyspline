#include "tinyspline.h"
#include "CuTest.h"
#include <stdlib.h>

void free_test_bspline(CuTest* tc)
{
	tsBSpline b;
	b.deg = 3;
	b.order = 4;
	b.dim = 2;
	b.n_ctrlp = 10;
	b.n_knots = 14;
	b.ctrlp = (tsReal*) malloc(10*2*4);
	b.knots = b.ctrlp + 5;

	CuAssertPtrNotNull(tc, b.ctrlp);
	CuAssertPtrNotNull(tc, b.knots);

	ts_bspline_free(&b);
	CuAssertTrue(tc, b.deg == 0);
	CuAssertTrue(tc, b.order == 0);
	CuAssertTrue(tc, b.dim == 0);
	CuAssertTrue(tc, b.n_ctrlp == 0);
	CuAssertTrue(tc, b.n_knots == 0);
	CuAssertPtrEquals(tc, b.ctrlp, NULL);
	CuAssertPtrEquals(tc, b.knots, NULL);
}

void free_test_deboornet(CuTest* tc)
{
	tsDeBoorNet net;
	net.u = 3.14f;
	net.k = 4;
	net.s = 2;
	net.h = 5;
	net.dim = 15;
	net.n_points = 156;
	net.points = (tsReal*) malloc(156*15*4);
	net.result = net.points + 50;

	CuAssertPtrNotNull(tc, net.result);
	CuAssertPtrNotNull(tc, net.points);

	ts_deboornet_free(&net);
	CuAssertTrue(tc, net.u == 0.f);
	CuAssertTrue(tc, net.k == 0);
	CuAssertTrue(tc, net.s == 0);
	CuAssertTrue(tc, net.h == 0);
	CuAssertTrue(tc, net.dim == 0);
	CuAssertTrue(tc, net.n_points == 0);
	CuAssertPtrEquals(tc, net.points, NULL);
	CuAssertPtrEquals(tc, net.result, NULL);
}

CuSuite* get_free_suite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, free_test_bspline);
	SUITE_ADD_TEST(suite, free_test_deboornet);

	return suite;
}
