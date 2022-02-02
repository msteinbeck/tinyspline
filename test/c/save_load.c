#include <testutils.h>

/* This epsilon environment can be chosen smaller than usual because the loss
 * of significance should be very small when serializing/deserializing floating
 * point values. */
#define EPSILON 0.00001

void save_load_load_equals_save(CuTest *tc)
{
	___SETUP___
	tsBSpline save = ts_bspline_init();
	tsBSpline load = ts_bspline_init();
	const tsReal *svals = NULL, *lvals = NULL;
	tsReal dist, knot[2];
	char *file = "save_load_test_file.txt";
	size_t i;

	___GIVEN___
	/* Add some value with decimal places. */
	C(ts_bspline_new_with_control_points(
		6, 2, 5, TS_OPENED, &save, &status,
		 100.0,    0.0,   /* P1 */
		 200.0,  -10.7,   /* P2 */
		 500.5,   40.0,   /* P3 */
		-300.0, -260.0,   /* P4 */
		-50.1,   200.0,   /* P5 */
		-80.0,   130.24)) /* P6 */

	___WHEN___
	C(ts_bspline_save(&save, file, &status))
	C(ts_bspline_load(file, &load, &status))

	___THEN___
	/* Check instances. */
	CuAssertTrue(tc, save.pImpl != load.pImpl);

	/* Check degree. */
	CuAssertIntEquals(tc, (int) ts_bspline_degree(&save),
		(int) ts_bspline_degree(&load));

	/* Check dimension. */
	CuAssertIntEquals(tc, (int) ts_bspline_dimension(&save),
		(int) ts_bspline_dimension(&load));

	/* Check knots. */
	CuAssertIntEquals(tc, (int) ts_bspline_num_knots(&save),
		(int) ts_bspline_num_knots(&load));
	for (i = 0; i < ts_bspline_num_knots(&save); i++) {
		C(ts_bspline_knot_at(&save, i, &knot[0], &status))
		C(ts_bspline_knot_at(&load, i, &knot[1], &status))
		CuAssertDblEquals(tc, knot[0], knot[1], TS_KNOT_EPSILON);
	}

	/* Check control points. */
	CuAssertIntEquals(tc, (int) ts_bspline_num_control_points(&save),
		(int) ts_bspline_num_control_points(&load));
	CuAssertIntEquals(tc, 2, (int) ts_bspline_dimension(&save));
	for (i = 0; i < ts_bspline_num_control_points(&save); i++) {
		C(ts_bspline_control_point_at_ptr(&save, i, &svals, &status))
		C(ts_bspline_control_point_at_ptr(&save, i, &lvals, &status))
		/* Compare each component. */
		CuAssertDblEquals(tc, svals[0], lvals[0], EPSILON);
		CuAssertDblEquals(tc, svals[1], lvals[1], EPSILON);
		/* Compare distance. */
		dist = ts_distance(svals, lvals, 2);
		CuAssertDblEquals(tc, 0.0, dist, EPSILON);
		svals = lvals = NULL;
	}

	___TEARDOWN___
	ts_bspline_free(&save);
	ts_bspline_free(&load);
	remove(file);
}

CuSuite* get_save_load_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, save_load_load_equals_save);
	return suite;
}
