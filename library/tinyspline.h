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
/**
 * This enum contains all error codes used by TinySpline.
 *
 * The following snippet shows how to handle errors:
 *      TsError err = ...       // any function call here
 *      if (err < 0) {          // or use err != TS_SUCCESS
 *          printf("we got an error!");
 *
 *          // you may want to reuse error codes
 *          // over several functions
 *          return err;
 *      }
 */
typedef enum
{
    TS_SUCCESS = 0,         /* no error */
    TS_MALLOC = -1,         /* malloc/realloc returned null */
    TS_OVER_UNDERFLOW = -2, /* overflow/underflow detected */
    TS_DIM_ZERO = -3,       /* the dimension of the control points are 0 */
    TS_DEG_GE_NCTRLP = -4,  /* degree of spline >= number of control points */
    TS_U_UNDEFINED = -5,    /* spline is not defined at u */
    TS_MULTIPLICITY = -6,   /* the multiplicity of a knot is greater than
                             * the order of the spline */
    TS_INPUT_EQ_OUTPUT = -7 /* input parameter is equals to output parameter */
} tsError;

/**
 * This enum describes how to fill the knot vector of a spline.
 *
 * If you don't know what an opened or clamped spline is, take a look at:
 * www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html
 */
typedef enum
{
/* setup knots as... */
    TS_OPENED = 0,  /* [uniformly spaced] */
    TS_CLAMPED = 1, /* [u_1 = u_2 = ..., uniformly spaced, ... = u_n-1 = u_n] */
    TS_NONE = 2     /* do not setup the knots; they may have any values */
} tsBSplineType;

typedef struct
{
    float u;         /* the knot u */
    size_t k;        /* the index [u_k, u_k+1) */
    size_t s;        /* the multiplicity of u_k */
    size_t h;        /* how many times u must be inserted */
    size_t dim;      /* dimension of a control point */
    size_t n_points; /* number of control points */
    float* points;   /* the control points of the de Boor net */
    float* result;   /* the result of the evaluation
 * Technically it is a pointer to the last point in points. Any changes made
 * here will modify points and vice versa. In case of a discontinuous B-Spline
 * at u, points can be used to get access to the first point and result to get
 * access to the second one. */
} tsDeBoorNet;

typedef struct
{
    size_t deg;     /* degree of b-spline basis function */
    size_t order;   /* degree + 1
 * This field is provided for convenience, because some libraries (e.g. OpenGL)
 * implicitly describing a polynomial of degree n with n + 1. */
    size_t dim;     /* dimension of a control point */
    size_t n_ctrlp; /* number of control points */
    size_t n_knots; /* number of knots (n_ctrlp + deg + 1) */
    float* ctrlp;   /* the control points of the spline */
    float* knots;   /* the knot vector of the spline (ascending) */
} tsBSpline;


/********************************************************
*                                                       *
* Methods                                               *
*                                                       *
********************************************************/
/**
 * The default constructor of tsDeBoorNet.
 *
 * All values of \deBoorNet are set to 0/NULL.
 */
void ts_deboornet_default(tsDeBoorNet* deBoorNet);

/**
 * The destructor of tsDeBoorNet.
 *
 * Frees all dynamically allocated memory and calls ::ts_deboornet_default
 * afterwards.
 */
void ts_deboornet_free(tsDeBoorNet* deBoorNet);

/**
 * The default constructor of tsBSpline.
 *
 * All values of \bspline are set to 0/NULL.
 */
void ts_bspline_default(tsBSpline* bspline);

/**
 * The destructor of tsBSpline.
 *
 * Frees all dynamically allocated memory and calls ::ts_deboornet_free
 * afterwards.
 */
void ts_bspline_free(tsBSpline* bspline);

/**
 * A convenient constructor for tsBSpline.
 *
 * Create a new spline of degree \deg with dimension \dim and \n_ctrlp
 * many control points. This function automatically calculates the necessary
 * number of knots and fills the knot vector according to \type.
 *
 * On error all values of \bspline are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_DIM_ZERO          if \deg == 0.
 * @return TS_DEG_GE_NCTRLP     if \deg >= \n_ctrlp.
 * @return TS_MALLOC            if allocating memory failed.
 */
tsError ts_bspline_new(
    const size_t deg, const size_t dim,
    const size_t n_ctrlp, const tsBSplineType type,
    tsBSpline* bspline
);

/**
 * Performs a cubic spline interpolation using thomas algorithm.
 * https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
 * http://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf
 * http://www.bakoma-tex.com/doc/generic/pst-bspline/pst-bspline-doc.pdf
 *
 * The resulting spline is a sequence of bezier curves connecting each point in
 * \points. Each bezier curve is of degree 3 with dimension \dim. The total
 * number of control points is (n-1)*4.
 *
 * This function does not free already allocated memory in \bspline.
 * If you want to reuse an instance of tsBSpline by using it in multiple
 * calls of this function, make sure to call ::ts_deboornet_free beforehand.
 *
 * On error all values of \bspline are 0/NULL.
 *
 * Note: \n is the number of points in \points and not the length of \points.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_DIM_ZERO          if \dim == 0.
 * @return TS_DEG_GE_NCTRLP     if \n < 2.
 * @return TS_MALLOC            if allocating memory failed.
 */
tsError ts_bspline_interpolate(
    const float* points, const size_t n, const size_t dim,
    tsBSpline* bspline
);

/**
 * The copy constructor of tsDeBoorNet.
 *
 * Creates a deep copy of \original and stores the result in \copy. This
 * function does not free already allocated memory in \copy. If you want to
 * reuse an instance of tsDeBoorNet by using it in multiple calls of this
 * function, make sure to call ::ts_deboornet_free beforehand.
 *
 * Unlike most other functions \original and \copy may not be the same pointer!
 *
 * On error all values of \copy are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_INPUT_EQ_OUTPUT   if \original == \copy
 * @return TS_MALLOC            if allocating memory failed.
 */
tsError ts_deboornet_copy(
    const tsDeBoorNet* original,
    tsDeBoorNet* copy
);

/**
 * The copy constructor of tsBSpline.
 *
 * Creates a deep copy of \original and stores the result in \copy. This
 * function does not free already allocated memory in \copy. If you want to
 * reuse an instance of tsBSpline by using it in multiple calls of this
 * function, make sure to call ::ts_bspline_free beforehand.
 *
 * Unlike most other functions \original and \copy may not be the same pointer!
 *
 * On error all values of \copy are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_INPUT_EQ_OUTPUT   if \original == \copy.
 * @return TS_MALLOC            if allocating memory failed.
 */
tsError ts_bspline_copy(
    const tsBSpline* original,
    tsBSpline* copy
);

/**
 * Fills the knot vector of \original according to \type and stores the result
 * in \result.
 *
 * This function creates a deep copy of \original, if \original != \result
 * and will never fail if \original == \result (always returns TS_SUCCESS).
 *
 * On error all values of \result are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if \original != \result and allocating
 *                              memory failed.
 */
tsError ts_bspline_setup_knots(
    const tsBSpline* original, const tsBSplineType type,
    tsBSpline* result
);

/**
 * Evaluates \bspline at knot value \u and stores result in \deBoorNet.
 *
 * This function does not free already allocated memory in \deBoorNet.
 * If you want to reuse an instance of tsDeBoorNet by using it in multiple
 * calls of this function, make sure to call ::ts_deboornet_free beforehand.
 *
 * On error all values of \deBoorNet are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if allocating memory failed.
 * @return TS_MULTIPLICITY      if multiplicity of \u > order of \bspline.
 * @return TS_U_UNDEFINED       if \bspline is not defined at \u.
 */
tsError ts_bspline_evaluate(
    const tsBSpline* bspline, const float u,
    tsDeBoorNet* deBoorNet
);

tsError ts_bspline_insert_knot(
    const tsBSpline* bspline, const float u, const size_t n,
    tsBSpline* result, size_t* k
);

/**
 * Resizes \bspline by \n (number of control points) and stores the result in
 * \resized. If \back != 0 the resulting splines is resized at the end. If
 * \back == 0 the resulting spline is resized at the front.
 *
 * Resizing a spline at the end is usually faster, because the current values
 * don't need to be copied.
 *
 * This function creates a deep copy of \bspline, if \bspline != \resized.
 *
 * On error (and if \bspline != \resized) all values of \resized are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_DEG_GE_NCTRLP     if the degree of \resized would be >= the number
 *                              of the control points of \resized.
 * @return TS_OVER_UNDERFLOW    if adding \n to the current length of the
 *                              control point/knot array would cause a
 *                              over/underflow.
 * @return TS_MALLOC            if (re)allocating memory failed.
 *
 * if \bspline != \resized:
 *      @return TS_DIM_ZERO         if the degree of \bspline == 0.
 *      @return TS_DEG_GE_NCTRLP    if the degree of \bspline >= the number of
 *                                  the control points of \bspline.
 *      @return TS_MALLOC           if allocating memory failed.
 */
tsError ts_bspline_resize(
    const tsBSpline* bspline, const int n, const int back,
    tsBSpline* resized
);

tsError ts_bspline_split(
    const tsBSpline* bspline, const float u,
    tsBSpline* split, size_t* k
);

/**
 * Buckles \original by \b and stores the result in \buckled.
 *
 * This function is based on:
 *      Holten, Danny. "Hierarchical edge bundles: Visualization of adjacency
 *      relations in hierarchical data." Visualization and Computer Graphics,
 *      IEEE Transactions on 12.5 (2006): 741-748.
 * Holten calls it "straightening" (page 744, equation 1).
 *
 * Usually the range of \b is: 0 <= \b <= 1, where 0 results in a line between
 * the first and the last control point (no buckle) and 1 keeps the original
 * shape (maximum buckle). If \b < 0 or \b > 1 the behaviour is undefined,
 * though it will not result in an error.
 *
 * This function creates a deep copy of \original, if \original != \buckled
 * and will never fail if \original == \buckled (always returns TS_SUCCESS).
 *
 * On error all values of \buckled are 0/NULL.
 *
 * @return TS_SUCCESS           on success.
 * @return TS_MALLOC            if \original != \buckled and allocating
 *                              memory failed.
 */
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
/**
 * Compares the float values \x and \y by using absolute and relative epsilon.
 *
 * @return 1    if \x is equals to \y.
 * @return 0    otherwise.
 */
int ts_fequals(const float x, const float y);

/**
 * Returns the error message associated to \err.
 */
char* ts_enum_str(const tsError err);


#ifdef	__cplusplus
}
#endif

#endif	/* TINYSPLINE_H */

