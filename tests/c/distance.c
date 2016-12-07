#include "tinyspline.h"
#include "CuTest.h"
#include <math.h>

const double distance_tests_delta = 0.000001;

void distance_test_dist2_non_negativity(CuTest *tc)
{
    tsRational x, y, dist;
    tsRational xs[3], ys[3];

    /* x positive, y negative */
    x = (tsRational) 1.0;
    y = (tsRational) -1.0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);
    x = (tsRational) 5478.6582;
    y = (tsRational) -1785.2364;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);

    /* x negative, y positive */
    x = (tsRational) -1.0;
    y = (tsRational) 1.0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);
    x = (tsRational) -6789.489032;
    y = (tsRational) 3479.168127;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);

    /* x negative, y negative */
    x = (tsRational) -1.0;
    y = (tsRational) -1.0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);
    x = (tsRational) -9324.3753;
    y = (tsRational) -2458.8404;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);

    /* x zero, y positive */
    x = (tsRational) .0;
    y = (tsRational) 1.0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);
    x = (tsRational) .0;
    y = (tsRational) 72347.75963;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);

    /* x zero, y negative */
    x = (tsRational) .0;
    y = (tsRational) -1.0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);
    x = (tsRational) .0;
    y = (tsRational) -57649.347598;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);

    /* x positive, y zero */
    x = (tsRational) 1.0;
    y = (tsRational) .0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);
    x = (tsRational) 92347.34759;
    y = (tsRational) .0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);

    /* x negative, y zero */
    x = (tsRational) -1.0;
    y = (tsRational) .0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);
    x = (tsRational) -1264887.7598;
    y = (tsRational) .0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);

    /* x zero, y zero */
    x = (tsRational) .0;
    y = (tsRational) .0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist >= 0);

    /* multi dim */
    xs[0] = (tsRational) 2382.242;
    xs[1] = (tsRational) -234791.2432;
    xs[2] = (tsRational) 34759821.123;
    ys[0] = (tsRational) -43759.123;
    ys[1] = (tsRational) 48602.4234;
    ys[2] = (tsRational) -34728.4234;
    dist = ts_ctrlp_dist2(xs, ys, 3);
    CuAssertTrue(tc, dist >= 0);
}

void distance_test_dist2_identity(CuTest *tc)
{
    tsRational x, y, dist;
    tsRational xs[3], ys[3];

    /* zero */
    x = (tsRational) 0.0;
    y = (tsRational) 0.0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

    /* small positive */
    x = (tsRational) 1.5432;
    y = (tsRational) 1.5432;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

    /* large positive */
    x = (tsRational) 74978587.348;
    y = (tsRational) 74978587.348;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

    /* small negative */
    x = (tsRational) -1.85034;
    y = (tsRational) -1.85034;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

    /* large negative */
    x = (tsRational) -237492332.23648;
    y = (tsRational) -237492332.23648;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertDblEquals(tc, 0, dist, distance_tests_delta);

    /* multi dim */
    xs[0] = ys[0] = (tsRational) 3743.128;
    xs[1] = ys[1] = (tsRational) 2739.325;
    xs[2] = ys[2] = (tsRational) 68098.21347;
    dist = ts_ctrlp_dist2(xs, ys, 3);
    CuAssertDblEquals(tc, 0, dist, distance_tests_delta);
}

void distance_test_dist2_symmetry(CuTest *tc)
{
    tsRational x, y;
    tsRational xs[3], ys[3];

    /* x and y positive */
    x = (tsRational) 73945.123478;
    y = (tsRational) 5789346.1234;
    CuAssertDblEquals(tc, ts_ctrlp_dist2(&x, &y, 1),
        ts_ctrlp_dist2(&y, &x, 1), distance_tests_delta);

    /* x and y negative */
    x = (tsRational) -3759.23423;
    y = (tsRational) -9824357.2342;
    CuAssertDblEquals(tc, ts_ctrlp_dist2(&x, &y, 1),
        ts_ctrlp_dist2(&y, &x, 1), distance_tests_delta);

    /* positive and negative */
    x = (tsRational) 385802.2346;
    y = (tsRational) -78424.8742;
    CuAssertDblEquals(tc, ts_ctrlp_dist2(&x, &y, 1),
        ts_ctrlp_dist2(&y, &x, 1), distance_tests_delta);

    /* multi dim */
    xs[0] = (tsRational) 856904.2342;
    xs[1] = (tsRational) 12352.234;
    xs[2] = (tsRational) -6989348.234;
    ys[0] = (tsRational) -749323.42342;
    ys[1] = (tsRational) 6834.2342;
    ys[2] = (tsRational) 48604342.4234;
    CuAssertDblEquals(tc, ts_ctrlp_dist2(xs, ys, 3),
        ts_ctrlp_dist2(ys, xs, 3), distance_tests_delta);
}

void distance_test_dist2_subadditivity(CuTest *tc)
{
    tsRational x, y, z;
    tsRational xs[2], ys[2], zs[2];

    x = (tsRational) -1.5;
    y = (tsRational) 1.5;
    z = (tsRational) 5;
    CuAssertTrue(tc, ts_ctrlp_dist2(&x, &z, 1) <=
        ts_ctrlp_dist2(&x, &y, 1) + ts_ctrlp_dist2(&y, &z, 1)
    );

    xs[0] = (tsRational) 1.0;
    xs[1] = (tsRational) 0.0;
    ys[0] = (tsRational) 2.5;
    ys[1] = (tsRational) 3.5;
    zs[0] = (tsRational) 5.0;
    zs[1] = (tsRational) 0.0;
    CuAssertTrue(tc, ts_ctrlp_dist2(xs, zs, 2) <=
        ts_ctrlp_dist2(xs, ys, 2) + ts_ctrlp_dist2(ys, zs, 2)
    );
}

void distance_test_dist2_standard_cases(CuTest *tc)
{
    size_t i;
    tsRational x, y;
    tsRational xs[2], ys[2];
    tsRational xss[3], yss[3];
    tsRational xsss[25], ysss[25];

    x = (tsRational) 0.0;
    y = (tsRational) 1.0;
    CuAssertDblEquals(tc,
        1.0, ts_ctrlp_dist2(&x, &y, 1),
        distance_tests_delta);

    x = (tsRational) 10.0;
    y = (tsRational) -5.0;
    CuAssertDblEquals(tc,
        15.0, ts_ctrlp_dist2(&x, &y, 1),
        distance_tests_delta);

    x = (tsRational) 3.14;
    y = (tsRational) -3.14;
    CuAssertDblEquals(tc,
        6.28, ts_ctrlp_dist2(&x, &y, 1),
        distance_tests_delta);

    xs[0] = (tsRational) 0.0;
    xs[1] = (tsRational) 0.0;
    ys[0] = (tsRational) 1.0;
    ys[1] = (tsRational) 1.0;
    CuAssertDblEquals(tc,
        sqrt(2.0), ts_ctrlp_dist2(xs, ys, 2),
        distance_tests_delta);

    xss[0] = (tsRational) 0.0;
    xss[1] = (tsRational) 0.0;
    xss[2] = (tsRational) 0.0;
    yss[0] = (tsRational) 1.0;
    yss[1] = (tsRational) 1.0;
    yss[2] = (tsRational) 1.0;
    CuAssertDblEquals(tc,
        sqrt(3.0), ts_ctrlp_dist2(xss, yss, 3),
        distance_tests_delta);

    for (i = 0; i < 25; i++) {
        xsss[i] = (tsRational) 0.0;
        ysss[i] = (tsRational) 1.0;
    }
    CuAssertDblEquals(tc,
        sqrt(25.0), ts_ctrlp_dist2(xsss, ysss, 25),
        distance_tests_delta);

    for (i = 0; i < 25; i++) {
        xsss[i] = (tsRational) 0.0;
        ysss[i] = (tsRational) -1.0;
    }
    CuAssertDblEquals(tc,
        sqrt(25.0), ts_ctrlp_dist2(xsss, ysss, 25),
        distance_tests_delta);
}

void distance_test_dist2_zero_dim(CuTest *tc)
{
    tsRational x = (tsRational) 0.0;
    tsRational y = (tsRational) 1.0;
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