#include <testutilscxx.h>

void
assert_equals(CuTest *tc,
              const BSpline &s1,
              const BSpline &s2)
{
	CuAssertIntEquals(tc, s1.degree(), s2.degree());
	CuAssertIntEquals(tc, s1.numControlPoints(), s2.numControlPoints());
	CuAssertIntEquals(tc, s1.knots().size(), s2.knots().size());
	CuAssertIntEquals(tc, s1.dimension(), s2.dimension());
	assert_equal_shape(tc, s1, s2);
}

void
bspline_copy_ctor(CuTest *tc)
{
	// Given
	BSpline spline(2, 2, 1);
	spline.setControlPoints({
			100, 100,
			200, 200
		});

	// When
	BSpline copy(spline);

	// Then
	assert_equals(tc, spline, copy);
}

void
bspline_move_ctor(CuTest *tc)
{
	// Given
	BSpline spline(2, 2, 1);
	spline.setControlPoints({
			100, 100,
			200, 200
		});
	BSpline toBeMoved(spline);

	// When
	BSpline move(std::move(toBeMoved));

	// Then
	assert_equals(tc, spline, move);
}

void
bspline_copy_assign(CuTest *tc)
{
	// Given
	BSpline spline(2, 2, 1);
	spline.setControlPoints({
			100, 100,
			200, 200
		});
	BSpline copy;

	// When
	copy = spline;

	// Then
	assert_equals(tc, spline, copy);
}

void
bspline_move_assign(CuTest *tc)
{
	// Given
	BSpline spline(2, 2, 1);
	spline.setControlPoints({
			100, 100,
			200, 200
		});
	BSpline toBeMoved(spline);
	BSpline move;

	// When
	move = std::move(toBeMoved);

	// Then
	assert_equals(tc, spline, move);
}

CuSuite *
get_bspline_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, bspline_copy_ctor);
	SUITE_ADD_TEST(suite, bspline_move_ctor);
	SUITE_ADD_TEST(suite, bspline_copy_assign);
	SUITE_ADD_TEST(suite, bspline_move_assign);
	return suite;
}
