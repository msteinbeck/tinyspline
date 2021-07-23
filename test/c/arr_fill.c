#include <testutils.h>

#ifdef _MSC_VER
#pragma warning(push)
/* unreferenced label */
#pragma warning(disable:4102)
#endif

void arr_fill_size10_negative_integer(CuTest *tc)
{
	___SETUP___
	tsReal arr[10];
	size_t i;

	___GIVEN___
	for (i = 0; i < 10; i++)
		arr[i] = (tsReal) i;

	___WHEN___
	ts_arr_fill(arr, 10, (tsReal) -3.0);

	___THEN___
	for (i = 0; i < 10; i++)
		CuAssertDblEquals(tc, -3.0, (double) arr[i], POINT_EPSILON);

	___TEARDOWN___
}

void arr_fill_size75_real(CuTest *tc)
{
	___SETUP___
	tsReal arr[75];
	size_t i;

	___GIVEN___
	for (i = 0; i < 75; i++)
		arr[i] = (tsReal) i;

	___WHEN___
	ts_arr_fill(arr, 75, (tsReal) 10.586);

	___THEN___
	for (i = 0; i < 75; i++)
		CuAssertDblEquals(tc, 10.586, (double) arr[i], POINT_EPSILON);

	___TEARDOWN___
}

void arr_fill_subset(CuTest *tc)
{
	___SETUP___
	tsReal arr[50];
	size_t i;

	___GIVEN___
	for (i = 0; i < 50; i++)
		arr[i] = (tsReal) i;

	___WHEN___
	ts_arr_fill(arr, 5, (tsReal) 15.0);

	___THEN___
	for (i = 0; i < 5; i++)
		CuAssertDblEquals(tc, 15.0, (double) arr[i], POINT_EPSILON);
	for (i = 5; i < 50; i++) {
		CuAssertDblEquals(tc,
			(double) i, (double) arr[i],
			POINT_EPSILON);
	}

	___TEARDOWN___
}

void arr_fill_subset_with_offset(CuTest *tc)
{
	___SETUP___
	tsReal arr[13];
	size_t i;

	___GIVEN___
	for (i = 0; i < 13; i++)
		arr[i] = (tsReal) i;

	___WHEN___
	ts_arr_fill(arr + 10, 3, (tsReal) 0.0);

	___THEN___
	for (i = 0; i < 10; i++) {
		CuAssertDblEquals(tc,
			(double) i, (double) arr[i],
			POINT_EPSILON);
	}
	for (i = 10; i < 13; i++)
		CuAssertDblEquals(tc, 0.0, (double) arr[i], POINT_EPSILON);

	___TEARDOWN___
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

#ifdef _MSC_VER
#pragma warning(pop)
#endif
