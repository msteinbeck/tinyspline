#include <testutilscxx.h>

void
knots_read_values(CuTest* tc)
{
	// Given
	BSpline spline(7);

	// When
	vector<real> knots = spline.knots();

	// Then
	CuAssertIntEquals(tc, 11, (int) knots.size());
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[10], TS_KNOT_EPSILON);
}

void
knots_set_values(CuTest *tc)
{
	// Given
	BSpline spline(7);
	vector<real> knots = spline.knots();

	// When
	knots[3]  = (real) 0.1;
	knots[10] = (real) 2.0;
	spline.setKnots(knots);
	knots = spline.knots();

	// Then
	CuAssertIntEquals(tc, 11, (int) knots.size());
	CuAssertDblEquals(tc, 0.0,  knots[0],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[1],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.0,  knots[2],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.1,  knots[3],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.25, knots[4],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.5,  knots[5],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 0.75, knots[6],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[7],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[8],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 1.0,  knots[9],  TS_KNOT_EPSILON);
	CuAssertDblEquals(tc, 2.0,  knots[10], TS_KNOT_EPSILON);
}

CuSuite *
get_knots_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, knots_read_values);
	SUITE_ADD_TEST(suite, knots_set_values);
	return suite;
}
