#ifndef TINYSPLINE_H
#define	TINYSPLINE_H

#include <stddef.h>

/********************************************************
*                                                       *
* System dependent configuration                        *
*                                                       *
********************************************************/
#define FLT_MAX_ABS_ERROR 1e-7
#define FLT_MAX_REL_ERROR 1e-5


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
    TS_SUCCESS = 0,        // <- no error 
    TS_MALLOC = -1,        // <- malloc retuned null
    TS_DIM_ZERO = -2,      // <- the dimension of the control points are 0
    TS_DEG_GE_NCTRLP = -3, // <- degree of spline >= number of control points
    TS_U_UNDEFINED = -4,   // <- spline is not defined at u
    TS_MULTIPLICITY = -5   // <- the multiplicity of a knot is greater than 
                           //    the order of the spline
} TS_Error;

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



/********************************************************
*                                                       *
* De Boor methods                                       *
*                                                       *
********************************************************/
void ts_deboornet_default(DeBoorNet* deBoorNet);
void ts_deboornet_free(DeBoorNet* deBoorNet);



/********************************************************
*                                                       *
* B-Spline methods                                      *
*                                                       *
********************************************************/
void ts_bspline_default(BSpline* bspline);
void ts_bspline_free(BSpline* bspline);

TS_Error ts_bspline_new(
    const size_t deg, const size_t dim, const size_t n_ctrlp, const BSplineType type,
    BSpline* bspline
);

TS_Error ts_bspline_copy(
    const BSpline* original,
    BSpline* copy
);

TS_Error ts_bspline_evaluate(
    const BSpline* bspline, const float u, 
    DeBoorNet* deBoorNet
);

TS_Error ts_bspline_split(
    const BSpline* bspline, const float u,
    BSpline (*split)[2] 
);



/********************************************************
*                                                       *
* Utility methods.                                      *
*                                                       *
********************************************************/
int ts_fequals(const float x, const float y);


#ifdef	__cplusplus
}
#endif

#endif	/* TINYSPLINE_H */

