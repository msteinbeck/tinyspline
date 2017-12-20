#include "CuTest.h"
#include <stdio.h>

CuSuite* get_distance_suite();
CuSuite* get_arr_fill_suite();
CuSuite* get_str_enum_suite();
CuSuite* get_default_suite();
CuSuite* get_free_suite();
CuSuite* get_new_suite();
CuSuite* get_move_suite();

int main()
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_distance_suite());
	CuSuiteAddSuite(suite, get_arr_fill_suite());
	CuSuiteAddSuite(suite, get_str_enum_suite());
	CuSuiteAddSuite(suite, get_default_suite());
	CuSuiteAddSuite(suite, get_free_suite());
	CuSuiteAddSuite(suite, get_new_suite());
	CuSuiteAddSuite(suite, get_move_suite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	CuStringDelete(output);
	CuSuiteDelete(suite);
	
	return 0;
}
