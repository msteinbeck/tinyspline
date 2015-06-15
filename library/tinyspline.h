#ifndef TINYSPLINE_H
#define	TINYSPLINE_H

#include <stddef.h>


#ifdef	__cplusplus
extern "C" {
#endif


/********************************************************
*                                                       *
* Data                                                  *
*                                                       *
********************************************************/
typedef enum
{
    TS_SUCCESS = 0,         // <- no error 
    TS_MALLOC = -1,         // <- malloc/realloc returned null
    TS_OVER_UNDERFLOW = -2, // <- overflow/underflow detected
    TS_DIM_ZERO = -3,       // <- the dimension of the control points are 0
    TS_DEG_GE_NCTRLP = -4,  // <- degree of spline >= number of control points
    TS_U_UNDEFINED = -5,    // <- spline is not defined at u
    TS_MULTIPLICITY = -6,   // <- the multiplicity of a knot is greater than 
                            //    the order of the spline
    TS_INPUT_EQ_OUTPUT = -7 // <- input parameter is equals to output parameter
} tsError;

typedef enum 
{
// if you don't know what an opened or clamped b-splines is, take a look at:
// www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html
// setup knots as...
    TS_OPENED = 0,  // <- [uniformly spaced]
    TS_CLAMPED = 1, // <- [u_1 = u_2 = ..., uniformly spaced, ... = u_n-1 = u_n]
    TS_NONE = 2     // <- do not setup the knots; they may have any values
} tsBSplineType;

typedef struct
{
    float u;         // <- the knot u
    size_t k;        // <- the index [u_k, u_k+1)
    size_t s;        // <- the multiplicity of u_k
    size_t h;        // <- how many times u must be inserted
    size_t dim;      // <- dimension of a control point
    size_t n_points; // <- number of control points
    float* points;   // <- the control points of the de Boor net
    float* result;   // <- the result of the evaluation
                     //    Technically it is a pointer to the last point
                     //    in points. Any changes made here will modify
                     //    points and vice versa. In case of a discontinuous
                     //    B-Spline at u, points can be used to get access
                     //    to the first point and result to get access to
                     //    the second one.
} tsDeBoorNet;

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
    float* knots;   // <- the knot vector of the b-spline (acsending)
} tsBSpline;



/********************************************************
*                                                       *
* Methods                                               *
*                                                       *
********************************************************/
void ts_deboornet_default(tsDeBoorNet* deBoorNet);
void ts_deboornet_free(tsDeBoorNet* deBoorNet);
void ts_bspline_default(tsBSpline* bspline);
void ts_bspline_free(tsBSpline* bspline);

/**
 * Creates a new B-Spline. On error \bspline is setup with default values.
 * @param deg       The degree of the B-Spline basis function
 * @param dim       The dimension of one control point
 * @param n_ctrlp   The number of control points
 * @param type      The type of the B-Spline. 
 *                  If type is ::TS_NONE the knot vector will not be setup.
 * @param bspline   The B-Spline to create.
 * @return          On success ::TS_SUCCESS. 
 *                  On error one of ::TS_DIM_ZERO, TS_DEG_GE_NCTRLP, TS_MALLOC.
 */
tsError ts_bspline_new(
    const size_t deg, const size_t dim, const size_t n_ctrlp, const tsBSplineType type,
    tsBSpline* bspline
);

/**
 * Setup the knot vector of \original. On error \result is setup with default 
 * values. \original and \result might be the same pointer.
 * @param original  The original B-Spline.
 * @param type      The type of the B-Spline.
 *                  If type is ::TS_NONE the knot vector's values are 
 *                  the same as \original
 * @param result    The B-Spline with its knot vector to setup.
 * @return          On success ::TS_SUCCESS.
 *                  On error with \original != \result one of
 *                  ::TS_DIM_ZERO, TS_DEG_GE_NCTRLP, TS_MALLOC.
 *                  If \original == \result this function will never return
 *                  an error.
 */
tsError ts_bspline_setup_knots(
    const tsBSpline* original, const tsBSplineType type,
    tsBSpline* result
);

/**
 * Creates a deep copy of \original. On error \copy is setup with default
 * values. \original and \copy might NOT be the same pointer.
 * @param original  The B-Spline to copy from.
 * @param copy      The B-Spline to copy to.
 * @return          On success ::TS_SUCCESS.
 *                  On error one of ::TS_INPUT_EQ_OUTPUT, ::TS_DIM_ZERO, 
 *                  TS_DEG_GE_NCTRLP, TS_MALLOC.
 */
tsError ts_bspline_copy(
    const tsBSpline* original,
    tsBSpline* copy
);

/**
 * Evaluates \bspline at knot value \u. As the knots of B-Splines are floats, 
 * the field tsDeBoorNet::u might not be == to to \u but it will always be 
 * ::ts_fequals. On error \net is setup with default values.
 * @param bspline   The B-spline to evaluate.
 * @param u         The knot value on which \bspline will be evaluated.
 * @param deBoorNet The de Boor net to create.
 * @return          On success one of 0, 1, 2 where
 *                  0: regular evaluation, 
 *                  1: evaluation at the begin/end of \bspline with s == order
 *                  2: evaluation inside of \bspline with s == order
 *                  On error one of ::TS_MALLOC, ::TS_U_UNDEFINED, 
 *                  ::TS_MULTIPLICITY
 */
tsError ts_bspline_evaluate(
    const tsBSpline* bspline, const float u, 
    tsDeBoorNet* deBoorNet
);

/**
 * Inserts the knot value \u \n times into \bspline. On error \result is setup 
 * with default values. \bspline and \result might be the same pointer.
 * @param bspline   The B-Spline on which to insert \u.
 * @param u         The knot value to insert.
 * @param n         How many times to insert \u.
 * @param result    The B-Spline with its knot vector to increase.
 * @return          On success ::TS_SUCCESS.
 *                  On error one of ::TS_MALLOC, ::TS_U_UNDEFINED, 
 *                  ::TS_MULTIPLICITY, ::TS_DEG_GE_NCTRLP, ::TS_DIM_ZERO,
 *                  ::TS_OVER_UNDERFLOW
 */
tsError ts_bspline_insert_knot(
    const tsBSpline* bspline, const float u, const size_t n,
    tsBSpline* result, size_t* k
);

/**
 * Resizes \bspline at the front/back by \n. If \n is negative, \resized will 
 * be smaller than \bspline. If \n is positive, \resized will be greater than
 * \bspline. Resizing \bspline at the back is usually faster. On error \resized 
 * is setup with default values. \bspline and \resized might be the same 
 * pointer.
 * @param bspline   The B-Spline to resize.
 * @param n         The number of control points/knots to add/remove
 * @param back      The flag to determine where to resize the B-Spline.
 * @param resized
 * @return 
 */
tsError ts_bspline_resize(
    const tsBSpline* bspline, const int n, const int back,
    tsBSpline* resized
);

tsError ts_bspline_split(
    const tsBSpline* bspline, const float u,
    tsBSpline* split, size_t* k
);

tsError ts_bspline_buckle(
    const tsBSpline* original, const float b,
    tsBSpline* buckled
);

tsError ts_bspline_to_beziers(
    const tsBSpline* bspline,
    tsBSpline* beziers
);

int ts_bspline_equals(
    const tsBSpline* x, const tsBSpline* y
);



/********************************************************
*                                                       *
* Utility                                               *
*                                                       *
********************************************************/
int ts_fequals(const float x, const float y);


#ifdef	__cplusplus
}
#endif

#endif	/* TINYSPLINE_H */

