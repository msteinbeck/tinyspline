#include <testutilscxx.h>

void
assert_equals(CuTest *tc,
              const DeBoorNet &n1,
              const DeBoorNet &n2)
{
	CuAssertIntEquals(tc,
	                  (int) n1.knot(),
	                  (int) n2.knot());
	CuAssertIntEquals(tc,
	                  (int) n1.index(),
	                  (int) n2.index());
	CuAssertIntEquals(tc,
	                  (int) n1.multiplicity(),
	                  (int) n2.multiplicity());
	CuAssertIntEquals(tc,
	                  (int) n1.numInsertions(),
	                  (int) n2.numInsertions());
	CuAssertIntEquals(tc,
	                  (int) n1.dimension(),
	                  (int) n2.dimension());

	vector<real> p1 = n1.points();
	vector<real> p2 = n2.points();
	CuAssertIntEquals(tc,
	                  (int) p1.size(),
	                  (int) p2.size());
	for (size_t i = 0; i < p1.size(); i++) {
		CuAssertDblEquals(tc,
		                  p1[i],
		                  p2[i],
		                  POINT_EPSILON);
	}
}

void
deboornet_copy_ctor(CuTest *tc)
{
	// Given
	BSpline spline(4, 2, 3);
	spline.setControlPoints({
			100, -100,
			200, -200,
			300, -300,
			400, -400
		});
	DeBoorNet net = spline((real) 0.5);

	// When
	DeBoorNet copy(net);

	// Then
	assert_equals(tc, net, copy);
}

void
deboornet_move_ctor(CuTest *tc)
{
	// Given
	BSpline spline(4, 2, 3);
	spline.setControlPoints({
			100, -100,
			200, -200,
			300, -300,
			400, -400
		});
	DeBoorNet net = spline((real) 0.5);
	DeBoorNet toBeMoved(net);

	// When
	DeBoorNet move(std::move(toBeMoved));

	// Then
	assert_equals(tc, net, move);
}

void
deboornet_copy_assign(CuTest *tc)
{
	// Given
	BSpline spline(4, 2, 3);
	spline.setControlPoints({
			100, -100,
			200, -200,
			300, -300,
			400, -400
		});
	DeBoorNet net = spline((real) 0.5);
	DeBoorNet copy = spline((real) 0.0);
	CuAssertTrue(tc, net.knot() != copy.knot());

	// When
	copy = net;

	// Then
	assert_equals(tc, net, copy);
}

void
deboornet_move_assign(CuTest *tc)
{
	// Given
	BSpline spline(4, 2, 3);
	spline.setControlPoints({
			100, -100,
			200, -200,
			300, -300,
			400, -400
		});
	DeBoorNet net = spline((real) 0.5);
	DeBoorNet toBeMoved(net);
	DeBoorNet move = spline((real) 0.0);
	CuAssertTrue(tc, net.knot() != move.knot());

	// When
	move = std::move(toBeMoved);

	// Then
	assert_equals(tc, net, move);
}

CuSuite *
get_deboornet_suite()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, deboornet_copy_ctor);
	SUITE_ADD_TEST(suite, deboornet_move_ctor);
	SUITE_ADD_TEST(suite, deboornet_copy_assign);
	SUITE_ADD_TEST(suite, deboornet_move_assign);
	return suite;
}
