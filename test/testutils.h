#ifndef TINYSPLINE_TESTUTILS_H
#define TINYSPLINE_TESTUTILS_H

#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define POINT_EPSILON TS_CONTROL_POINT_EPSILON

/**
 * Asserts that \p s1 and \p s2 have equal shape by evaluating them at
 * different knots and comparing the distance of the evaluated points with
 * \p eps (i.e., the distance of the evaluated points must be less than or
 * equal to \p eps). The given splines may have different dimensions. In this
 * case, the distance is computed with the lower dimension.
 *
 * @param tc
 * 	The active test container.
 * @param s1
 * 	First spline.
 * @param s2
 * 	Second spline.
 * @param eps
 * 	Maximum distance of evaluated points.
 */
void
assert_equal_shape_eps(CuTest *tc, tsBSpline *s1, tsBSpline *s2, tsReal eps);

/**
 * Asserts that \p s1 and \p s2 have equal shape by evaluating them at
 * different knots and comparing the distance of the evaluated points with
 * ::CTRLP_EPSILON (i.e., the distance of the evaluated points must be less
 * than or equal to ::CTRLP_EPSILON). The given splines may have different
 * dimensions. In this case, the distance is computed with the lower dimension.
 *
 * Implementation detail: This function uses ::assert_equal_shape_eps.
 *
 * @param tc
 * 	The active test container.
 * @param s1
 * 	First spline.
 * @param s2
 * 	Second spline.
 */
void
assert_equal_shape(CuTest *tc, tsBSpline *s1, tsBSpline *s2);

#endif	/* TINYSPLINE_TESTUTILS_H */
