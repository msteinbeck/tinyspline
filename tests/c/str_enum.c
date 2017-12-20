#include "tinyspline.h"
#include "CuTest.h"
#include <string.h>

void str_enum_test_bijection(CuTest* tc)
{
	char *str;
	int i, j;
	for (i = 0; i > -9; i--) {
		str = (char *)ts_enum_str((tsError) i);
		j = strcmp("unknown error", str);
		if (j == 0) /* TS_SUCCESS */
			CuAssertTrue(tc, !j); /* equal */
		else /* actual error */
			CuAssertTrue(tc, j); /* unequal */
		j = ts_str_enum(str);
		CuAssertIntEquals(tc, i, j);
	}
}

void str_enum_test_unknown(CuTest* tc)
{
	char *str = (char *)ts_enum_str((tsError) 0);
	CuAssertStrEquals(tc, "unknown error", str);
	str = (char *)ts_enum_str((tsError) 1);
	CuAssertStrEquals(tc, "unknown error", str);
	str = (char *)ts_enum_str((tsError) 2);
	CuAssertStrEquals(tc, "unknown error", str);
	str = (char *)ts_enum_str((tsError) 100);
	CuAssertStrEquals(tc, "unknown error", str);
}

CuSuite* get_str_enum_suite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, str_enum_test_bijection);
	SUITE_ADD_TEST(suite, str_enum_test_unknown);

	return suite;
}
