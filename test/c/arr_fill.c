#include "tinyspline.h"
#include "CuTest.h"

#define EPSILON 0.0001

void arr_fill_size10_negative_integer(CuTest *tc)
{
	tsReal arr[10];
	size_t i;

/* ================================= Given ================================= */
	for (i = 0; i < 10; i++)
		arr[i] = (tsReal) i;

/* ================================= When ================================== */
	ts_arr_fill(arr, 10, (tsReal) -3.0);

/* ================================= Then ================================== */
	for (i = 0; i < 10; i++)
		CuAssertDblEquals(tc, -3.0, (double) arr[i], EPSILON);
}

void arr_fill_size75_real(CuTest *tc)
{
	tsReal arr[75];
	size_t i;

/* ================================= Given ================================= */
	for (i = 0; i < 75; i++)
		arr[i] = (tsReal) i;

/* ================================= When ================================== */
	ts_arr_fill(arr, 75, (tsReal) 10.586);

/* ================================= Then ================================== */
	for (i = 0; i < 75; i++)
		CuAssertDblEquals(tc, 10.586, (double) arr[i], EPSILON);
}

void arr_fill_subset(CuTest *tc)
{
	tsReal arr[50];
	size_t i;

/* ================================= Given ================================= */
	for (i = 0; i < 50; i++)
		arr[i] = (tsReal) i;

/* ================================= When ================================== */
	ts_arr_fill(arr, 5, (tsReal) 15.0);

/* ================================= Then ================================== */
	for (i = 0; i < 5; i++)
		CuAssertDblEquals(tc, 15.0, (double) arr[i], EPSILON);
	for (i = 5; i < 50; i++)
		CuAssertDblEquals(tc, (double) i, (double) arr[i], EPSILON);
}

void arr_fill_subset_with_offset(CuTest *tc)
{
	tsReal arr[13];
	size_t i;

/* ================================= Given ================================= */
	for (i = 0; i < 13; i++)
		arr[i] = (tsReal) i;

/* ================================= When ================================== */
	ts_arr_fill(arr + 10, 3, (tsReal) 0.0);

/* ================================= Then ================================== */
	for (i = 0; i < 10; i++)
		CuAssertDblEquals(tc, (double) i, (double) arr[i], EPSILON);
	for (i = 10; i < 13; i++)
		CuAssertDblEquals(tc, 0.0, (double) arr[i], EPSILON);
}

CuSuite* get_arr_fill_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, arr_fill_size10_negative_integer);
	SUITE_ADD_TEST(suite, arr_fill_size75_real);
	SUITE_ADD_TEST(suite, arr_fill_subset);
	SUITE_ADD_TEST(suite, arr_fill_subset_with_offset);
	return suite;
}
