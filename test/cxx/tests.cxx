#include <testutilscxx.h>

CuSuite* get_knots_suite();

int main()
{
	int fails;
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_knots_suite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	fails = suite->failCount;

	CuStringDelete(output);
	CuSuiteDelete(suite);

	return fails? EXIT_FAILURE : EXIT_SUCCESS;
}
