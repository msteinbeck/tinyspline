#ifndef BSPLINE_H
#define	BSPLINE_H

#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

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
    size_t n_knots; // <- number of knots (n_ctrlp + k + 1)
    float* ctrlp;   // <- the control points of the spline
    float* knots;   // <- the knot vector of the spline 
                    //    each value is within [0.0, 1.0]
} BSpline;


int bspline_new_clamped(
    const size_t deg, const size_t dim, const size_t n_ctrlp, 
    BSpline* bspline
);

void bspline_free(BSpline* bspline);


#ifdef	__cplusplus
}
#endif

#endif	/* BSPLINE_H */

