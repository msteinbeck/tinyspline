#include "tinyspline.h"
#include "CuTest.h"

const double fill_tests_delta = 0.000001;

void arr_fill_test_standard_case(CuTest *tc)
{
    tsRational arr[10];
    size_t i;

    for (i = 0; i < 10; i++)
        arr[i] = (tsRational) i;

    ts_arr_fill(arr, 10, (tsRational) -3.0);
    for (i = 0; i < 10; i++)
        CuAssertDblEquals(tc, -3.0, arr[i], fill_tests_delta);

    ts_arr_fill(arr, 10, (tsRational) 10.586);
    for (i = 0; i < 10; i++)
        CuAssertDblEquals(tc, 10.586, arr[i], fill_tests_delta);
}

void arr_fill_test_subset(CuTest *tc)
{
    tsRational arr[10];
    size_t i;

    for (i = 0; i < 10; i++)
        arr[i] = (tsRational) i;

    ts_arr_fill(arr, 5, (tsRational) -15.0);
    for (i = 0; i < 5; i++)
        CuAssertDblEquals(tc, -15.0, arr[i], fill_tests_delta);
    for (i = 5; i < 10; i++)
        CuAssertDblEquals(tc, i, arr[i], fill_tests_delta);
    ts_arr_fill(arr, 1, (tsRational) -20);
    CuAssertDblEquals(tc, -20, arr[0], fill_tests_delta);
    for (i = 1; i < 5; i++)
        CuAssertDblEquals(tc, -15.0, arr[i], fill_tests_delta);
    for (i = 5; i < 10; i++)
        CuAssertDblEquals(tc, i, arr[i], fill_tests_delta);
}

CuSuite* get_arr_fill_suite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, arr_fill_test_standard_case);
    SUITE_ADD_TEST(suite, arr_fill_test_subset);

    return suite;
}