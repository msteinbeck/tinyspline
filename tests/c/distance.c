#include "tinyspline.h"
#include "CuTest.h"
#include <math.h>

const double distance_tests_delta = 0.000001;

void distance_test_dist2_non_negativity(CuTest *tc)
{
	tsReal x, y, dist;
	tsReal xs[3], ys[3];

	/* x positive, y negative */
	x = (tsReal) 1.0;
	y = (tsReal) -1.0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);
	x = (tsReal) 5478.6582;
	y = (tsReal) -1785.2364;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);

	/* x negative, y positive */
	x = (tsReal) -1.0;
	y = (tsReal) 1.0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);
	x = (tsReal) -6789.489032;
	y = (tsReal) 3479.168127;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);

	/* x negative, y negative */
	x = (tsReal) -1.0;
	y = (tsReal) -1.0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);
	x = (tsReal) -9324.3753;
	y = (tsReal) -2458.8404;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);

	/* x zero, y positive */
	x = (tsReal) .0;
	y = (tsReal) 1.0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);
	x = (tsReal) .0;
	y = (tsReal) 72347.75963;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);

	/* x zero, y negative */
	x = (tsReal) .0;
	y = (tsReal) -1.0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);
	x = (tsReal) .0;
	y = (tsReal) -57649.347598;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);

	/* x positive, y zero */
	x = (tsReal) 1.0;
	y = (tsReal) .0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);
	x = (tsReal) 92347.34759;
	y = (tsReal) .0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);

	/* x negative, y zero */
	x = (tsReal) -1.0;
	y = (tsReal) .0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);
	x = (tsReal) -1264887.7598;
	y = (tsReal) .0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);

	/* x zero, y zero */
	x = (tsReal) .0;
	y = (tsReal) .0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertTrue(tc, dist >= 0);

	/* multi dim */
	xs[0] = (tsReal) 2382.242;
	xs[1] = (tsReal) -234791.2432;
	xs[2] = (tsReal) 34759821.123;
	ys[0] = (tsReal) -43759.123;
	ys[1] = (tsReal) 48602.4234;
	ys[2] = (tsReal) -34728.4234;
	dist = ts_ctrlp_dist2(xs, ys, 3);
	CuAssertTrue(tc, dist >= 0);
}

void distance_test_dist2_identity(CuTest *tc)
{
	tsReal x, y, dist;
	tsReal xs[3], ys[3];

	/* zero */
	x = (tsReal) 0.0;
	y = (tsReal) 0.0;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

	/* small positive */
	x = (tsReal) 1.5432;
	y = (tsReal) 1.5432;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

	/* large positive */
	x = (tsReal) 74978587.348;
	y = (tsReal) 74978587.348;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

	/* small negative */
	x = (tsReal) -1.85034;
	y = (tsReal) -1.85034;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

	/* large negative */
	x = (tsReal) -237492332.23648;
	y = (tsReal) -237492332.23648;
	dist = ts_ctrlp_dist2(&x, &y, 1);
	CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

	/* multi dim */
	xs[0] = ys[0] = (tsReal) 3743.128;
	xs[1] = ys[1] = (tsReal) 2739.325;
	xs[2] = ys[2] = (tsReal) 68098.21347;
	dist = ts_ctrlp_dist2(xs, ys, 3);
	CuAssertDblEquals(tc, 0, dist, distance_tests_delta);
}

void distance_test_dist2_symmetry(CuTest *tc)
{
	tsReal x, y;
	tsReal xs[3], ys[3];

	/* x and y positive */
	x = (tsReal) 73945.123478;
	y = (tsReal) 5789346.1234;
	CuAssertDblEquals(tc, ts_ctrlp_dist2(&x, &y, 1),
		ts_ctrlp_dist2(&y, &x, 1), distance_tests_delta);

	/* x and y negative */
	x = (tsReal) -3759.23423;
	y = (tsReal) -9824357.2342;
	CuAssertDblEquals(tc, ts_ctrlp_dist2(&x, &y, 1),
		ts_ctrlp_dist2(&y, &x, 1), distance_tests_delta);

	/* positive and negative */
	x = (tsReal) 385802.2346;
	y = (tsReal) -78424.8742;
	CuAssertDblEquals(tc, ts_ctrlp_dist2(&x, &y, 1),
		ts_ctrlp_dist2(&y, &x, 1), distance_tests_delta);

	/* multi dim */
	xs[0] = (tsReal) 856904.2342;
	xs[1] = (tsReal) 12352.234;
	xs[2] = (tsReal) -6989348.234;
	ys[0] = (tsReal) -749323.42342;
	ys[1] = (tsReal) 6834.2342;
	ys[2] = (tsReal) 48604342.4234;
	CuAssertDblEquals(tc, ts_ctrlp_dist2(xs, ys, 3),
		ts_ctrlp_dist2(ys, xs, 3), distance_tests_delta);
}

void distance_test_dist2_subadditivity(CuTest *tc)
{
	tsReal x, y, z;
	tsReal xs[2], ys[2], zs[2];

	x = (tsReal) -1.5;
	y = (tsReal) 1.5;
	z = (tsReal) 5;
	CuAssertTrue(tc, ts_ctrlp_dist2(&x, &z, 1) <=
		ts_ctrlp_dist2(&x, &y, 1) + ts_ctrlp_dist2(&y, &z, 1)
	);

	xs[0] = (tsReal) 1.0;
	xs[1] = (tsReal) 0.0;
	ys[0] = (tsReal) 2.5;
	ys[1] = (tsReal) 3.5;
	zs[0] = (tsReal) 5.0;
	zs[1] = (tsReal) 0.0;
	CuAssertTrue(tc, ts_ctrlp_dist2(xs, zs, 2) <=
		ts_ctrlp_dist2(xs, ys, 2) + ts_ctrlp_dist2(ys, zs, 2)
	);
}

void distance_test_dist2_standard_cases(CuTest *tc)
{
	size_t i;
	tsReal x, y;
	tsReal xs[2], ys[2];
	tsReal xss[3], yss[3];
	tsReal xsss[25], ysss[25];

	x = (tsReal) 0.0;
	y = (tsReal) 1.0;
	CuAssertDblEquals(tc,
		1.0, ts_ctrlp_dist2(&x, &y, 1),
		distance_tests_delta);

	x = (tsReal) 10.0;
	y = (tsReal) -5.0;
	CuAssertDblEquals(tc,
		15.0, ts_ctrlp_dist2(&x, &y, 1),
		distance_tests_delta);

	x = (tsReal) 3.14;
	y = (tsReal) -3.14;
	CuAssertDblEquals(tc,
		6.28, ts_ctrlp_dist2(&x, &y, 1),
		distance_tests_delta);

	xs[0] = (tsReal) 0.0;
	xs[1] = (tsReal) 0.0;
	ys[0] = (tsReal) 1.0;
	ys[1] = (tsReal) 1.0;
	CuAssertDblEquals(tc,
		sqrt(2.0), ts_ctrlp_dist2(xs, ys, 2),
		distance_tests_delta);

	xss[0] = (tsReal) 0.0;
	xss[1] = (tsReal) 0.0;
	xss[2] = (tsReal) 0.0;
	yss[0] = (tsReal) 1.0;
	yss[1] = (tsReal) 1.0;
	yss[2] = (tsReal) 1.0;
	CuAssertDblEquals(tc,
		sqrt(3.0), ts_ctrlp_dist2(xss, yss, 3),
		distance_tests_delta);

	for (i = 0; i < 25; i++) {
		xsss[i] = (tsReal) 0.0;
		ysss[i] = (tsReal) 1.0;
	}
	CuAssertDblEquals(tc,
		sqrt(25.0), ts_ctrlp_dist2(xsss, ysss, 25),
		distance_tests_delta);

	for (i = 0; i < 25; i++) {
		xsss[i] = (tsReal) 0.0;
		ysss[i] = (tsReal) -1.0;
	}
	CuAssertDblEquals(tc,
		sqrt(25.0), ts_ctrlp_dist2(xsss, ysss, 25),
		distance_tests_delta);
}

void distance_test_dist2_zero_dim(CuTest *tc)
{
	tsReal x = (tsReal) 0.0;
	tsReal y = (tsReal) 1.0;
	CuAssertDblEquals(tc,
		0, ts_ctrlp_dist2(&x, &y, 0),
		distance_tests_delta);
}

CuSuite* get_distance_suite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, distance_test_dist2_non_negativity);
	SUITE_ADD_TEST(suite, distance_test_dist2_identity);
	SUITE_ADD_TEST(suite, distance_test_dist2_symmetry);
	SUITE_ADD_TEST(suite, distance_test_dist2_subadditivity);
	SUITE_ADD_TEST(suite, distance_test_dist2_standard_cases);
	SUITE_ADD_TEST(suite, distance_test_dist2_zero_dim);

	return suite;
}
