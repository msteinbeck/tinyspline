#include "testutilscxx.h"

void
assert_equal_shape_eps(CuTest *tc,
                       const BSpline &s1,
                       const BSpline &s2,
                       double eps)
{
	size_t dim = s1.dimension() > s2.dimension()
		? s1.dimension() : s2.dimension();

	for (size_t k = 0; k < TS_MAX_NUM_KNOTS; k++) {
		real min = s1.domain().min();
		real max = s1.domain().max();
		real knot = (tsReal)k / TS_MAX_NUM_KNOTS;
		knot = ( (max - min) * knot ) + min;
		vector<real> p1 = s1.eval(knot).result();

		min = s2.domain().min();
		max = s2.domain().max();
		knot = ( (max - min) * knot ) + min;
		vector<real> p2 = s2.eval(knot).result();

		real dist = ts_distance(p1.data(), p2.data(), dim);
		CuAssertDblEquals(tc, 0, dist, (tsReal) eps);
	}
}

void
assert_equal_shape(CuTest *tc,
                   const BSpline &s1,
                   const BSpline &s2)
{
	assert_equal_shape_eps(tc, s1, s2, POINT_EPSILON);
}
