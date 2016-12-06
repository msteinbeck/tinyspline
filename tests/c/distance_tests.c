#include "tinyspline.h"
#include "CuTest.h"
#include <math.h>

int dist2_test_equals(const tsRational x, const tsRational y)
{
    return fabs(x-y) < 0.000001;
}

void dist2_test_non_negativity(CuTest* tc)
{
    tsRational x;
    tsRational xs[3];
    tsRational y;
    tsRational ys[3];
    tsRational dist;

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

void dist2_test_identity(CuTest* tc)
{
    tsRational x;
    tsRational xs[3];
    tsRational y;
    tsRational ys[3];
    tsRational dist;

    /* zero */
    x = (tsRational) 0.0;
    y = (tsRational) 0.0;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist2_test_equals(dist, 0));

    /* small positive */
    x = (tsRational) 1.5432;
    y = (tsRational) 1.5432;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist2_test_equals(dist, 0));

    /* large positive */
    x = (tsRational) 74978587.348;
    y = (tsRational) 74978587.348;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist2_test_equals(dist, 0));

    /* small negative */
    x = (tsRational) -1.85034;
    y = (tsRational) -1.85034;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist2_test_equals(dist, 0));

    /* large negative */
    x = (tsRational) -237492332.23648;
    y = (tsRational) -237492332.23648;
    dist = ts_ctrlp_dist2(&x, &y, 1);
    CuAssertTrue(tc, dist2_test_equals(dist, 0));

    /* multi dim */
    xs[0] = ys[0] = (tsRational) 3743.128;
    xs[1] = ys[1] = (tsRational) 2739.325;
    xs[2] = ys[2] = (tsRational) 68098.21347;
    dist = ts_ctrlp_dist2(xs, ys, 3);
    CuAssertTrue(tc, dist2_test_equals(dist, 0));
}

CuSuite* get_distance_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, dist2_test_non_negativity);
    SUITE_ADD_TEST(suite, dist2_test_identity);

    return suite;
}