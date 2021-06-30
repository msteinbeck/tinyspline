#ifndef TINYSPLINE_UTILS_H
#define TINYSPLINE_UTILS_H

#include <stdlib.h>
#include <tinyspline.h>
#include "CuTest.h"

#define CTRLP_EPSILON 0.00001

/**
 * Asserts that \p s1 and \p s2 have equal shape by evaluating them at
 * different knots and comparing the distance of the evaluated points with
 * ::CTRLP_EPSILON. The given splines may have different dimensions. In this
 * case, the distance is computed with the lower dimension.
 *
 * @param tc
 * 	The active test container.
 * @param s1
 * 	First spline.
 * @param s2
 * 	Second spline.
 */
void assert_equal_shape(CuTest *tc, tsBSpline *s1, tsBSpline *s2);

#endif //TINYSPLINE_UTILS_H
