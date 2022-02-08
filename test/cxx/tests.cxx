#include <testutilscxx.h>

CuSuite* get_knots_suite();
CuSuite* get_frames_suite();
CuSuite* get_bspline_suite();
CuSuite* get_deboornet_suite();

int main()
{
	int fails;
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, get_knots_suite());
	CuSuiteAddSuite(suite, get_frames_suite());
	CuSuiteAddSuite(suite, get_bspline_suite());
	CuSuiteAddSuite(suite, get_deboornet_suite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("C++ Library:\n%s\n", output->buffer);
	fails = suite->failCount;

	CuStringDelete(output);
	CuSuiteDelete(suite);

	return fails? EXIT_FAILURE : EXIT_SUCCESS;
}
