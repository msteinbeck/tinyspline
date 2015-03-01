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
    TS_SUCCESS = 0,         // <- no error 
    TS_MALLOC = -1,         // <- malloc/realloc retuned null
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
    TS_OPENED = 0, 
    TS_CLAMPED
} tsBSplineType;

typedef struct
{
    float u;           // <- the knot u
    size_t k;          // <- the index [u_k, u_k+1)
    size_t s;          // <- the multiplicity of u_k
    size_t h;          // <- how many times u must be inserted
    size_t dim;        // <- dimension of a control point
    size_t n_affected; // <- number of affected control points
    size_t n_points;   // <- number of control points
    float* points;     // <- the control points of the de Boor net
    float* result;     // <- the result of the evalutation
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

tsError ts_bspline_new(
    const size_t deg, const size_t dim, const size_t n_ctrlp, const tsBSplineType type,
    tsBSpline* bspline
);

tsError ts_bspline_fill_knots(
    const tsBSpline* original, const tsBSplineType type,
    tsBSpline* result
);

tsError ts_bspline_copy(
    const tsBSpline* original,
    tsBSpline* copy
);

tsError ts_bspline_evaluate(
    const tsBSpline* bspline, const float u, 
    tsDeBoorNet* deBoorNet
);

tsError ts_bspline_insert_knot(
    const tsBSpline* bspline, const float u, const size_t n,
    tsBSpline* result
);

tsError ts_bspline_resize_back(
    const tsBSpline* bspline, const int n,
    tsBSpline* resized
);

tsError ts_bspline_resize_front(
    const tsBSpline* bspline, const int n,
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

