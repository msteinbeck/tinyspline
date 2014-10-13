#ifndef BSPLINE_H
#define	BSPLINE_H

#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

    
// =============================
// De Boor
// =============================
typedef struct
{
    int k;             // <- the index [u_k, u_k+1)
    int s;             // <- the multiplicity of u_k
    int h;             // <- how many times u must be inserted
    size_t deg;        // <- degree of b-spline bassis function
    size_t dim;        // <- dimension of a control point
    size_t n_affected; // <- number of affected control points
    size_t n_points;   // <- number of control points
    size_t last_idx;   // <- index of last point in points
                       //    This field can be used for direct 
                       //    access of last point in points
    float* points;     // <- the control points of the de Boor net
} DeBoorNet;

void deboornet_free(DeBoorNet* deBoorNet);


// =============================
// B-Spline
// =============================
typedef enum 
{
    OPENED = 0, 
    CLAMPED
} BSplineType;

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
    const size_t deg, const size_t dim, const size_t n_ctrlp, const BSplineType type,
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
    DeBoorNet* deBoorNet
);

int bspline_split(
    const BSpline* bspline, const float u,
    BSpline (*split)[2] 
);


#ifdef	__cplusplus
}
#endif

#endif	/* BSPLINE_H */

