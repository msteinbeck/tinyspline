#ifndef BSPLINE_H
#define	BSPLINE_H

#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    OPENED = 0, 
    CLAMPED
} BSplineType;

/** A B-Spline curve. */
typedef struct
{
    size_t deg;     // <- degree of b-spline basis function
    size_t order;   // <- degree + 1
                    //    This field is provided for convenience, because
                    //    some libraries (e.g. OpenGL) implicitly describing 
                    //    a polynomial of degree n with n + 1.
    size_t dim;     // <- dimension of a control point
    size_t n_ctrlp; // <- number of control points
    size_t n_knots; // <- number of knots (n_ctrlp + deg + 1)
    float* ctrlp;   // <- the control points of the spline
    float* knots;   // <- the knot vector of the spline 
                    //    each value is within [0.0, 1.0]
} BSpline;


int bspline_new(
    const size_t deg, const size_t dim, const size_t n_ctrlp, BSplineType type,
    BSpline* bspline
);

void bspline_free(BSpline* bspline);


/**
 * Returns the de boor net at u if the given spline is continuous at u.
 * The last control point of the returned array contains the point 
 * on the spline.
 * 
 * If the given spline in discontinuous at u because u refers the
 * first/last control point, one of them is returned.
 * 
 * If the given spline is discontinuous at u and refers an internal 
 * control point instead the first/last, the two control points laying on
 * u are returned.
 * 
 * @param bspline
 *      The spline to evaluate
 * @param u
 *      The element of the knot vector to evaluate.
 * @param n_points
 *      The number of returned points.
 * @param points
 *      Either the points of the de boor net, 
 *      or the point(s) directly referred by u.
 * @return
 *      A positive (inclusive 0) value on success, a negative on failure.
 * 
 *      0 = The returned points are the de boor net at u.
 *      1 = The spline is discontinuous at u and points to the 
 *          first/last control point.
 *      2 = The spline is discontinuous at u and points to two control points.
 *          (both must be inner control points)
 * 
 *      -1 = If the spline is not defined at u.
 *      -2 = If u has multiplicity of > order, thus the knot vector of the given
 *           spline is corrupted.
 *      -3 = If malloc failed.
 */
int bspline_evaluate(
    const BSpline* bspline, const float u, 
    size_t* n_points, float** points
);


#ifdef	__cplusplus
}
#endif

#endif	/* BSPLINE_H */

