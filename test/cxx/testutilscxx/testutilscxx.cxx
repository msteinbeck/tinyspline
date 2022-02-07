#include "testutilscxx.h"

void
assert_equal_shape_eps(CuTest *tc,
                       const BSpline &s1,
                       const BSpline &s2,
                       double eps)
{
	real dim = s1.dimension() > s2.dimension()
		? s1.dimension() : s2.dimension();

	for (size_t k = 0; k < TS_MAX_NUM_KNOTS; k++) {
		vector<real> p1 = s1.eval(k).result();
		vector<real> p2 = s2.eval(k).result();

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
