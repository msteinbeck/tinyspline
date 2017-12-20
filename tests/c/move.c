#include "tinyspline.h"
#include "CuTest.h"
#include "utils.h"

void move_test_bspline_different_ptr(CuTest* tc)
{
	tsBSpline spline, moved, control;

	ctests_init_non_default_bspline(&spline);
	control.deg = spline.deg;
	control.order = spline.order;
	control.dim = spline.dim;
	control.n_ctrlp = spline.n_ctrlp;
	control.n_knots = spline.n_knots;
	control.ctrlp = spline.ctrlp;
	control.knots = spline.knots;

	ts_bspline_move(&spline, &moved);
	ctests_assert_default_bspline(tc, &spline);

	CuAssertIntEquals(tc, (int) control.deg, (int) moved.deg);
	CuAssertIntEquals(tc, (int) control.order, (int) moved.order);
	CuAssertIntEquals(tc, (int) control.dim, (int) moved.dim);
	CuAssertIntEquals(tc, (int) control.n_ctrlp, (int) moved.n_ctrlp);
	CuAssertIntEquals(tc, (int) control.n_knots, (int) moved.n_knots);
	CuAssertPtrEquals(tc, control.ctrlp, moved.ctrlp);
	CuAssertPtrEquals(tc, control.knots, moved.knots);
}

void move_test_bspline_same_ptr(CuTest* tc)
{
	tsBSpline move, control;
	ctests_init_non_default_bspline(&move);
	control.deg = move.deg;
	control.order = move.order;
	control.dim = move.dim;
	control.n_ctrlp = move.n_ctrlp;
	control.n_knots = move.n_knots;
	control.ctrlp = move.ctrlp;
	control.knots = move.knots;
	ts_bspline_move(&move, &move);
	CuAssertIntEquals(tc, (int) control.deg, (int) move.deg);
	CuAssertIntEquals(tc, (int) control.order, (int) move.order);
	CuAssertIntEquals(tc, (int) control.dim, (int) move.dim);
	CuAssertIntEquals(tc, (int) control.n_ctrlp, (int) move.n_ctrlp);
	CuAssertIntEquals(tc, (int) control.n_knots, (int) move.n_knots);
	CuAssertPtrEquals(tc, control.ctrlp, move.ctrlp);
	CuAssertPtrEquals(tc, control.knots, move.knots);
}

CuSuite* get_move_suite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, move_test_bspline_different_ptr);
	SUITE_ADD_TEST(suite, move_test_bspline_same_ptr);

	return suite;
}
