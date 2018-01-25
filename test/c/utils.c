#include "tinyspline.h"
#include "CuTest.h"

/* See default tests */
void ctests_init_non_default_bspline(tsBSpline *bspline)
{
	bspline->deg = 1;
	bspline->order = 2;
	bspline->dim = 3;
	bspline->n_ctrlp = 4;
	bspline->n_knots = 5;
	bspline->ctrlp = (tsReal*) &bspline->deg;
	bspline->knots = (tsReal*) &bspline->dim;
}

/* See default tests */
void ctests_assert_default_bspline(CuTest *tc, tsBSpline *bspline)
{
	CuAssertTrue(tc, bspline->deg == 0);
	CuAssertTrue(tc, bspline->order == 0);
	CuAssertTrue(tc, bspline->dim == 0);
	CuAssertTrue(tc, bspline->n_ctrlp == 0);
	CuAssertTrue(tc, bspline->n_knots == 0);
	CuAssertPtrEquals(tc, bspline->ctrlp, NULL);
	CuAssertPtrEquals(tc, bspline->knots, NULL);
}
