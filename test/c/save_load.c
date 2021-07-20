#include <testutils.h>

/* This epsilon environment can be chosen smaller than usual because the loss
 * of significance should be very small when serializing/deserializing floating
 * point values. */
#define CTRLP_EPSILON 0.00001

void save_load_load_equals_save(CuTest *tc)
{
	tsBSpline save = ts_bspline_init();
	tsBSpline load = ts_bspline_init();
	tsReal knot[2], *svals = NULL, *lvals = NULL;
	char *file = "save_load_test_file.txt";
	size_t i;
	tsStatus status;

	/* Add some value with decimal places. */
	tsReal ctrlp_in[12] = {
		 100,     0,
		 200,    -10.7,
		 500.5f,  40,
		-300,    -260,
		-50.1f,   200,
		-80,      130.2,
	};

	TS_TRY(try, status.code, &status)
/* ================================= Given ================================= */
		TS_CALL(try, status.code, ts_bspline_new(
			6, 2, 5, TS_OPENED, &save, &status))
		TS_CALL(try, status.code, ts_bspline_set_control_points(
			&save, ctrlp_in, &status))

/* ================================= When ================================== */
		TS_CALL(try, status.code, ts_bspline_save(
			&save, file, &status))
		TS_CALL(try, status.code, ts_bspline_load(
			file, &load, &status))

/* ================================= Then ================================== */
		/* Check instances. */
		CuAssertTrue(tc, save.pImpl != load.pImpl);

		/* Check degree. */
		CuAssertIntEquals(tc, ts_bspline_degree(&save),
			ts_bspline_degree(&load));

		/* Check dimension. */
		CuAssertIntEquals(tc, ts_bspline_dimension(&save),
			ts_bspline_dimension(&load));

		/* Check knots. */
		CuAssertIntEquals(tc, ts_bspline_num_knots(&save),
			ts_bspline_num_knots(&load));
		for (i = 0; i < ts_bspline_num_knots(&save); i++) {
			TS_CALL(try, status.code, ts_bspline_knot_at(
				&save, i, &knot[0], &status))
			TS_CALL(try, status.code, ts_bspline_knot_at(
				&load, i, &knot[1], &status))
			CuAssertDblEquals(tc, knot[0], knot[1],
				TS_KNOT_EPSILON);
		}

		/* Check control points. */
		CuAssertIntEquals(tc, ts_bspline_num_control_points(&save),
			ts_bspline_num_control_points(&load));
		CuAssertIntEquals(tc, 2, ts_bspline_dimension(&save));
		for (i = 0; i < ts_bspline_num_control_points(&save); i++) {
			TS_CALL(try, status.code, ts_bspline_control_point_at(
				&save, i, &svals, &status))
			TS_CALL(try, status.code, ts_bspline_control_point_at(
				&save, i, &lvals, &status))
			/* Compare each component. */
			CuAssertDblEquals(tc, svals[0], lvals[0],
				CTRLP_EPSILON);
			CuAssertDblEquals(tc, svals[1], lvals[1],
				CTRLP_EPSILON);
			/* Compare distance. */
			CuAssertDblEquals(tc, 0.f,
				ts_distance(svals, lvals, 2),
				CTRLP_EPSILON);
			free(svals);
			free(lvals);
			svals = lvals = NULL;
		}
	TS_CATCH(status.code)
		CuFail(tc, status.message);
	TS_FINALLY
		ts_bspline_free(&save);
		ts_bspline_free(&load);
		remove(file);
		file = NULL;
		free(svals);
		free(lvals);
		svals = lvals = NULL;
	TS_END_TRY
}

CuSuite* get_save_load_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, save_load_load_equals_save);
	return suite;
}