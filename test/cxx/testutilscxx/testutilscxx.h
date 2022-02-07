#ifndef TINYSPLINE_TESTUTILSCXX_H
#define TINYSPLINE_TESTUTILSCXX_H

#include <testutils.h>
#include <tinysplinecxx.h>

using namespace tinyspline;
using namespace std;

/**
 * C++ version of ::assert_equal_shape_eps from testutils.h.
 */
void
assert_equal_shape_eps(CuTest *tc,
                       const BSpline &s1,
                       const BSpline &s2,
                       double eps);

/**
 * C++ version of ::assert_equal_shape from testutils.h.
 */
void
assert_equal_shape(CuTest *tc,
                   const BSpline &s1,
                   const BSpline &s2);

#endif /* TINYSPLINE_TESTUTILSCXX_H */
