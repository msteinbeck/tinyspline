/** @file */

#ifndef TINYSPLINE_H
#define	TINYSPLINE_H

#include <stddef.h>

#ifdef _MSC_VER
#define TINYSPLINE_SHARED_EXPORT __declspec(dllexport)
#define TINYSPLINE_SHARED_IMPORT __declspec(dllimport)
#else
#define TINYSPLINE_SHARED_EXPORT
#define TINYSPLINE_SHARED_IMPORT
#endif

#ifdef TINYSPLINE_SHARED
#ifdef TINYSPLINE_EXPORT
#define TINYSPLINE_API TINYSPLINE_SHARED_EXPORT
#else
#define TINYSPLINE_API TINYSPLINE_SHARED_IMPORT
#endif
#else
#define TINYSPLINE_API
#endif

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************
*                                                                             *
* :: Predefined Constants                                                     *
*                                                                             *
* The following constants should only be changed with caution because         *
* they have been aligned to maintain internal consistency. The default values *
* should be suitable for almost all environments.                             *
*                                                                             *
******************************************************************************/
/**
 * Maximum number of knots of a spline.
 */
#define TS_MAX_NUM_KNOTS 10000

/**
 * Default minimum of a spline's domain. This constant is used when setting up
 * new splines. Must be less than TS_DOMAIN_DEFAULT_MAX.
 */
#define TS_DOMAIN_DEFAULT_MIN 0.0f

/**
 * Default maximum of a spline's domain. This constant is used when setting up
 * new splines. Must be greater than TS_DOMAIN_DEFAULT_MIN
 */
#define TS_DOMAIN_DEFAULT_MAX 1.0f

/**
 * If the distance between two knots falls below this threshold, they are
 * considered equal. Must be positive ( > 0 ).
 */
#define TS_KNOT_EPSILON 1e-4f

/**
 * If the distance between two control points is less than or equal to this
 * threshold, they are considered equal. This constant is not used directly by
 * any function of the C interface but is intended to provide a reasonable
 * default value for functions requiring an epsilon environment for comparing
 * control points (the C++ interface, for example, uses this as default value
 * for its optional parameters).
 */
#define TS_CONTROL_POINT_EPSILON 1e-6f



/******************************************************************************
*                                                                             *
* :: API Configuration                                                        *
*                                                                             *
******************************************************************************/
#ifdef TINYSPLINE_FLOAT_PRECISION
typedef float tsReal;
#else
typedef double tsReal;
#endif



/******************************************************************************
*                                                                             *
* :: Error Handling                                                           *
*                                                                             *
* The following enums, structs, and macros are used to define and handle      *
* different types of errors. Use them as follows:                             *
*                                                                             *
*     tsStatus status;                                                        *
*     TS_TRY(any_label, status.code, &status)                                 *
*         // Wrap function calls with TS_CALL.                                *
*         TS_CALL(any_label, status.code, ts_bspline_to_beziers(              *
*             &spline, &beziers, &status))                                    *
*         if (...)                                                            *
*             // Use one of the TS_THROW macros to raise an error.            *
*             TS_THROW_0(any_label, &status, TS_MALLOC, "out of memory")      *
*     TS_CATCH(status.code)                                                   *
*         // Executed on error.                                               *
*         printf(status.message);                                             *
*     TS_FINALLY                                                              *
*         // Executed in any case.                                            *
*     TS_END_TRY                                                              *
*                                                                             *
* Although it is always advisable to properly handle errors, embedding your   *
* code into a TS_TRY/TS_END_TRY block as well as passing a pointer to a       *
* tsStatus object is entirely optional:                                       *
*                                                                             *
*     ts_bspline_to_beziers(&spline, &beziers, NULL);                         *
*                                                                             *
* Yet, you may check if a particular function failed, albeit you don't have   *
* access to the error message:                                                *
*                                                                             *
*     if (ts_bspline_to_beziers(&spline, &beziers, NULL))                     *
*         // an error occurred                                                *
*                                                                             *
******************************************************************************/
/**
 * Defines different error codes.
 */
typedef enum {
    TS_SUCCESS = 0, /**< No error. */
    TS_MALLOC = -1, /**< Unable to allocate memory. */
    TS_DIM_ZERO = -2, /**< Points with dimensionality 0. */
    TS_DEG_GE_NCTRLP = -3, /**< degree >= num_control_points. */
    TS_U_UNDEFINED = -4, /**< Undefined knot value. */
    TS_MULTIPLICITY = -5, /**< multiplicity(u) > order */
    TS_KNOTS_DECR = -6, /**< Decreasing knot vector. */
    TS_NUM_KNOTS = -7, /**< Unexpected number of knots. */
    TS_UNDERIVABLE = -8, /**< Spline is not derivable. */
    TS_LCTRLP_DIM_MISMATCH = -10, /**< len_control_points % dim != 0. */
    TS_IO_ERROR = -11, /**< Error while reading/writing a file. */
    TS_PARSE_ERROR = -12, /**< Error while parsing a serialized entity. */
    TS_INDEX_ERROR = -13, /**< Index does not exist. */
    TS_NO_RESULT = -14, /**< Function returns without result. */
    TS_NUM_POINTS = -15 /**< Unexpected number of points. */
} tsError;

/**
 * Stores an error code (::tsError) as well as a corresponding error message.
 */
typedef struct {
    tsError code; /**< The error code. */
    char message[100]; /**< The corresponding error message. */
} tsStatus;

#define TS_TRY(label, error, status)         \
{                                            \
	(error) = TS_SUCCESS;                \
	if ((status) != NULL) {              \
		(status)->code = TS_SUCCESS; \
		(status)->message[0] = '\0'; \
	}                                    \
	__ ## label ## __:                   \
	if (!(error)) {

#define TS_CALL(label, error, call)                  \
		(error) = (call);                    \
		if ((error)) goto __ ## label ## __;

#define TS_CATCH(error) \
	} if ((error)) {

#define TS_FINALLY \
	} {

#define TS_END_TRY \
	}          \
}

#define TS_END_TRY_RETURN(error)   \
	TS_END_TRY return (error);

#define TS_END_TRY_ROE(error)                 \
	TS_END_TRY if ((error)) return error;

#define TS_CALL_ROE(error, call)   \
{                                  \
	(error) = (call);          \
	if ((error)) return error; \
}

#define TS_RETURN_SUCCESS(status)            \
{                                            \
	if ((status) != NULL) {              \
		(status)->code = TS_SUCCESS; \
		(status)->message[0] = '\0'; \
	}                                    \
	return TS_SUCCESS;                   \
}

#define TS_RETURN_0(status, error, msg)          \
{                                                \
	if ((status) != NULL) {                  \
		(status)->code = error;          \
		sprintf((status)->message, msg); \
	}                                        \
	return error;                            \
}

#define TS_RETURN_1(status, error, msg, arg1)          \
{                                                      \
	if ((status) != NULL) {                        \
		(status)->code = error;                \
		sprintf((status)->message, msg, arg1); \
	}                                              \
	return error;                                  \
}

#define TS_RETURN_2(status, error, msg, arg1, arg2)          \
{                                                            \
	if ((status) != NULL) {                              \
		(status)->code = error;                      \
		sprintf((status)->message, msg, arg1, arg2); \
	}                                                    \
	return error;                                        \
}

#define TS_RETURN_3(status, error, msg, arg1, arg2, arg3)          \
{                                                                  \
	if ((status) != NULL) {                                    \
		(status)->code = error;                            \
		sprintf((status)->message, msg, arg1, arg2, arg3); \
	}                                                          \
	return error;                                              \
}

#define TS_THROW_0(label, error, status, val, msg) \
{                                                  \
	(error) = val;                             \
	if ((status) != NULL) {                    \
		(status)->code = val;              \
		sprintf((status)->message, msg);   \
	}                                          \
	goto __ ## label ## __;                    \
}

#define TS_THROW_1(label, error, status, val, msg, arg1) \
{                                                        \
	(error) = val;                                   \
	if ((status) != NULL) {                          \
		(status)->code = val;                    \
		sprintf((status)->message, msg, arg1);   \
	}                                                \
	goto __ ## label ## __;                          \
}

#define TS_THROW_2(label, error, status, val, msg, arg1, arg2) \
{                                                              \
	(error) = val;                                         \
	if ((status) != NULL) {                                \
		(status)->code = val;                          \
		sprintf((status)->message, msg, arg1, arg2);   \
	}                                                      \
	goto __ ## label ## __;                                \
}

#define TS_THROW_3(label, error, status, val, msg, arg1, arg2, arg3) \
{                                                                    \
	(error) = val;                                               \
	if ((status) != NULL) {                                      \
		(status)->code = val;                                \
		sprintf((status)->message, msg, arg1, arg2, arg3);   \
	}                                                            \
	goto __ ## label ## __;                                      \
}



/******************************************************************************
*                                                                             *
* :: Data Types                                                               *
*                                                                             *
* Using the PIMPL design pattern, the internal fields of the data types are   *
* encapsulated and can only be accessed by calling the corresponding field    *
* access functions.                                                           *
*                                                                             *
******************************************************************************/
/**
 * Describes the structure of the knot vector of a NURBS/B-Spline. For more
 * details, see:
 *
 *     www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-curve.html
 */
typedef enum
{
	/* Uniformly spaced knot vector. */
	TS_OPENED = 0,

	/* Uniformly spaced knot vector with clamped end knots. */
	TS_CLAMPED = 1,

	/* Uniformly spaced knot vector with s(u) = order of spline. */
	TS_BEZIERS = 2
} tsBSplineType;

/**
 * Represents a B-Spline, which may also be used for NURBS, Bezier curves,
 * lines, and points. NURBS use homogeneous coordinates to store their control
 * points (i.e. the last component of a control point stores the weight).
 * Bezier curves are B-Splines with 'num_control_points == order' and a
 * clamped knot vector, which lets them pass through their first and last
 * control point (a property which does not necessarily apply to B-Splines and
 * NURBS). Lines and points, on that basis, are Bezier curves of degree 1
 * (lines) and 0 (points).
 *
 * Two dimensional control points are stored as follows:
 *
 *     [x_0, y_0, x_1, y_1, ..., x_n-1, y_n-1]
 *
 * Tree dimensional control points are stored as follows:
 *
 *     [x_0, y_0, z_0, x_1, y_1, z_1, ..., x_n-1, y_n-1, z_n-1]
 *
 * ... and so on. As already mentioned, NURBS use homogeneous coordinates to
 * store their control points. For example, a NURBS in 2D stores its control
 * points as follows:
 *
 *     [x_0*w_0, y_0*w_0, w_0, x_1*w_1, y_1*w_1, w_1, ...]
 *
 * where 'w_i' is the weight of the i'th control point.
 */
typedef struct
{
	struct tsBSplineImpl *pImpl; /**< The actual implementation. */
} tsBSpline;

/**
 * Represents the output of De Boor's algorithm. It is used to evaluate a
 * spline at given knots by iteratively computing a net of intermediate values
 * until the result is available:
 *
 *     https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
 *     https://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/de-Boor.html
 *
 * All points of a net are stored in 'points' (ts_deboornet_points). The
 * resultant point is the last point in 'points' and, for the sake of
 * convenience, can be accessed with 'ts_deboornet_result'.
 *
 * Two dimensional points are stored as follows:
 *
 *     [x_0, y_0, x_1, y_1, ..., x_n-1, y_n-1]
 *
 * Tree dimensional points are stored as follows:
 *
 *     [x_0, y_0, z_0, x_1, y_1, z_1, ..., x_n-1, y_n-1, z_n-1]
 *
 * ... and so on. The output also supports homogeneous coordinates
 * (cf. tsBSpline).
 *
 * There is a special case in which the evaluation of a knot value 'u' returns
 * two results instead of one. It occurs when the multiplicity of 'u' ( s(u) )
 * is equals to the order of a spline, indicating that the corresponding spline
 * is discontinuous at 'u'. This is common practice for B-Splines (and NURBS)
 * consisting of connected Bezier curves where the endpoint of curve 'c_i' is
 * equal to the start point of curve 'c_i+1'. Yet, the end point of 'c_i' and
 * the start point of 'c_i+1' may still be completely different, yielding to
 * visible gaps. In such case (s(u) == order), 'points' stores only the two
 * resultant points (there is no net to calculate) and 'result' points to the
 * *first* point in 'points'. Since having gaps in splines is unusual, both
 * points in 'points' are generally equals, making it easy to handle this
 * special case by accessing 'result' as usual. However, you can access both
 * points if necessary:
 *
 *     ts_deboornet_result(...)[0] ...       // Stores the first component of
 *                                           // the first result.
 *
 *     ts_deboornet_result(...)[dim(spline)] // Stores the first component of
 *                                           // the second result.
 *
 * As if this wasn't complicated enough, there is an exception for this special
 * case, yielding to exactly one result (just like the regular case) even if
 * 's(u) == order'. It occurs when 'u' is the lower or upper bound of a
 * spline's domain. For instance, if 'b' is a spline with domain [0, 1] and is
 * evaluated at 'u = 0' or 'u = 1' then 'result' is *always* a single point
 * regardless of the multiplicity of 'u'.
 *
 * In summary, three different types of evaluation exist: i) the regular case,
 * in which all points of the corresponding net are returned, ii) a special
 * case, in which exactly two points are returned (required for splines with
 * gaps), and iii) the exception of ii), in which exactly one point is returned
 * even if 's(u) == order'. All in all this looks quite complex (and actually
 * it is), but for most applications you do not have to deal with this. Just
 * use 'ts_deboornet_result' to access the outcome of De Boor's algorithm.
 */
typedef struct
{
	struct tsDeBoorNetImpl *pImpl; /**< The actual implementation. */
} tsDeBoorNet;



/******************************************************************************
*                                                                             *
* :: Field Access Functions                                                   *
*                                                                             *
******************************************************************************/
/**
 * Returns the degree of \p spline.
 *
 * @param[in] spline
 * 	The spline whose degree is read.
 * @return
 * 	The degree of \p spline.
 */
size_t TINYSPLINE_API ts_bspline_degree(const tsBSpline *spline);

/**
 * Sets the degree of \p spline.
 *
 * @param[out] spline
 * 	The spline whose degree is set.
 * @param[in] deg
 * 	The degree to be set.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_DEG_GE_NCTRLP
 * 	If \p degree >= ts_bspline_get_control_points(spline).
 */
tsError TINYSPLINE_API ts_bspline_set_degree(tsBSpline *spline, size_t deg,
	tsStatus *status);

/**
 * Returns the order (degree + 1) of \p spline.
 *
 * @param[in] spline
 * 	The spline whose order is read.
 * @return
 * 	The order of \p spline.
 */
size_t TINYSPLINE_API ts_bspline_order(const tsBSpline *spline);

/**
 * Sets the order (degree + 1) of \p spline.
 *
 * @param[out] spline
 * 	The spline whose order is set.
 * @param[in] order
 * 	The order to be set.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_DEG_GE_NCTRLP
 * 	If \p order > ts_bspline_get_control_points(spline) or if \p order == 0
 * 	( due to the underflow resulting from: order - 1 => 0 - 1 => INT_MAX
 * 	which always is >= ts_bspline_get_control_points(spline) ).
 */
tsError TINYSPLINE_API  ts_bspline_set_order(tsBSpline *spline, size_t order,
	tsStatus *status);

/**
 * Returns the dimension of \p spline. The dimension of a spline describes the
 * number of components for each point in ts_bspline_get_control_points(spline).
 * One-dimensional splines are possible, albeit their benefit might be
 * questionable.
 *
 * @param[in] spline
 * 	The spline whose dimension is read.
 * @return
 * 	The dimension of \p spline.
 */
size_t TINYSPLINE_API  ts_bspline_dimension(const tsBSpline *spline);

/**
 * Sets the dimension of \p spline. The following conditions must be satisfied:
 *
 * 	(1) dim >= 1
 * 	(2) len_control_points(spline) % dim == 0
 *
 * with _len_control_points_ being the length of the control point array of \p
 * spline. The dimension of a spline describes the number of components for
 * each point in ts_bspline_get_control_points(spline). One-dimensional splines
 * are possible, albeit their benefit might be questionable.
 *
 * @param[out] spline
 * 	The spline whose dimension is set.
 * @param[in] dim
 * 	The dimension to be set.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_DIM_ZERO
 * 	If \p dimension == 0.
 * @return TS_LCTRLP_DIM_MISMATCH
 * 	If len_control_points(spline) % \p dim != 0
 */
tsError TINYSPLINE_API  ts_bspline_set_dimension(tsBSpline *spline, size_t dim,
	tsStatus *status);

/**
 * Returns the length of the control point array of \p spline.
 *
 * @param[in] spline
 * 	The spline with its control point array whose length is read.
 * @return
 * 	The length of the control point array of \p spline.
 */
size_t TINYSPLINE_API  ts_bspline_len_control_points(const tsBSpline *spline);

/**
 * Returns the number of control points of \p spline.
 *
 * @param[in] spline
 * 	The spline whose number of control points is read.
 * @return
 * 	The number of control points of \p spline.
 */
size_t TINYSPLINE_API ts_bspline_num_control_points(const tsBSpline *spline);

/**
 * Returns the size of the control point array of \p spline. This function may
 * be useful when copying control points using memcpy or memmove.
 *
 * @param[in] spline
 * 	The spline with its control point array whose size is read.
 * @return
 * 	The size of the control point array of \p spline.
 */
size_t TINYSPLINE_API ts_bspline_sof_control_points(const tsBSpline *spline);

/**
 * Returns a deep copy of the control points of \p spline.
 *
 * @param[in] spline
 * 	The spline whose control points are read.
 * @param[out] ctrlp
 * 	The output array.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_control_points(const tsBSpline *spline,
	tsReal **ctrlp, tsStatus *status);

/**
 * Returns a deep copy of the control point of \p spline at \p index.
 *
 * @param[in] spline
 * 	The spline whose control point is read at \p index.
 * @param[in] index
 * 	The zero-based index of the control point to return.
 * @param[out] ctrlp
 * 	The output array.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_INDEX_ERROR
 * 	If \p index is out of range.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_control_point_at(const tsBSpline *spline,
	size_t index, tsReal **ctrlp, tsStatus *status);

/**
 * Sets the control points of \p spline. Creates a deep copy of \p ctrlp.
 *
 * @param[out] spline
 * 	The spline whose control points are set.
 * @param[in] ctrlp
 * 	The values to deep copy.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 */
tsError TINYSPLINE_API ts_bspline_set_control_points(tsBSpline *spline,
	const tsReal *ctrlp, tsStatus *status);

/**
 * Sets the control point of \p spline at \p index. Creates a deep copy of
 * \p ctrlp.
 *
 * @param[out] spline
 * 	The spline whose control point is set at \p index.
 * @param[in] index
 * 	The zero-based index of the control point to set.
 * @param[in] ctrlp
 * 	The control point to be set at \p index.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_INDEX_ERROR
 * 	If \p index is out of range.
 */
tsError TINYSPLINE_API ts_bspline_set_control_point_at(tsBSpline *spline,
	size_t index, const tsReal *ctrlp, tsStatus *status);

/**
 * Returns the number of knots of \p spline.
 *
 * @param[in] spline
 * 	The spline whose number of knots is read.
 * @return
 * 	The number of knots of \p spline.
 */
size_t TINYSPLINE_API ts_bspline_num_knots(const tsBSpline *spline);

/**
 * Returns the size of the knot array of \p spline. This function may be useful
 * when copying knots using memcpy or memmove.
 *
 * @param[in] spline
 * 	The spline with its knot array whose size is read.
 * @return TS_SUCCESS
 * 	The size of the knot array of \p spline.
 */
size_t TINYSPLINE_API ts_bspline_sof_knots(const tsBSpline *spline);

/**
 * Returns a deep copy of the knots of \p spline.
 *
 * @param[in] spline
 * 	The spline whose knots are read.
 * @param[out] knots
 * 	The output array.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_knots(const tsBSpline *spline,
	tsReal **knots, tsStatus *status);

/**
 * Returns the knot of \p spline at \p index.
 *
 * @param[in] spline
 * 	The spline whose knot is read at \p index.
 * @param[in] index
 * 	The zero-based index of the knot to return.
 * @param[out] knot
 * 	The output value.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_INDEX_ERROR
 * 	If \p index is out of range.
 */
tsError TINYSPLINE_API ts_bspline_knot_at(const tsBSpline *spline,
	size_t index, tsReal *knot, tsStatus *status);

/**
 * Sets the knots of \p spline. Creates a deep copy of \p knots.
 *
 * @param[out] spline
 * 	The spline whose knots are set.
 * @param[in] knots
 * 	The values to deep copy and scale.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_KNOTS_DECR
 * 	If the knot vector is decreasing.
 * @return TS_MULTIPLICITY
 * 	If there is a knot with multiplicity > order
 */
tsError TINYSPLINE_API ts_bspline_set_knots(tsBSpline *spline,
	const tsReal *knots, tsStatus *status);

/**
 * Sets the knot of \p spline at \p index.
 *
 * @param[in] spline
 * 	The spline whose knot is set at \p index.
 * @param[in] index
 * 	The zero-based index of the knot to set.
 * @param[in] knot
 * 	The knot to be set at \p index.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_INDEX_ERROR
 * 	If \p index is out of range.
 * @return TS_KNOTS_DECR
 * 	If setting the knot at \p index results in a decreasing knot vector.
 * @return TS_MULTIPLICITY
 * 	If setting the knot at \p index results in a knot vector containing
 * 	\p knot with multiplicity greater than the order of \p spline.
 */
tsError TINYSPLINE_API ts_bspline_set_knot_at(tsBSpline *spline, size_t index,
	tsReal knot, tsStatus *status);

/* ------------------------------------------------------------------------- */

/**
 * Returns the knot (sometimes referred to as 'u' or 't') of \p net.
 *
 * @param[in] net
 * 	The net whose knot is read.
 * @return
 * 	The knot of \p net.
 */
tsReal TINYSPLINE_API ts_deboornet_knot(const tsDeBoorNet *net);

/**
 * Returns the index [u_k, u_k+1) with u being the knot of \p net.
 *
 * @param[in] net
 * 	The net whose index is read.
 * @return
 * 	The index [u_k, u_k+1) with u being the knot of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_index(const tsDeBoorNet *net);

/**
 * Returns the multiplicity of the knot of \p net.
 *
 * @param[in] net
 * 	The net whose multiplicity is read.
 * @return
 * 	The multiplicity of the knot of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_multiplicity(const tsDeBoorNet *net);

/**
 * Returns the number of insertion that were necessary to evaluate the knot of
 * \p net.
 *
 * @param[in] net
 * 	The net with its knot whose number of insertions is read.
 * @return
 * 	The number of insertions that were necessary to evaluate the knot of \p
 * 	net.
 */
size_t TINYSPLINE_API ts_deboornet_num_insertions(const tsDeBoorNet *net);

/**
 * Returns the dimension of \p net. The dimension of a net describes the number
 * of components for each point in ts_bspline_get_points(spline).
 * One-dimensional nets are possible, albeit their benefit might be
 * questionable.
 *
 * @param[in] net
 * 	The net whose dimension is read.
 * @return
 * 	The dimension of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_dimension(const tsDeBoorNet *net);

/**
 * Returns the length of the point array of \p net.
 *
 * @param[in] net
 * 	The net with its point array whose length is read.
 * @return
 * 	The length of the point array of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_len_points(const tsDeBoorNet *net);

/**
 * Returns the number of points of \p net.
 *
 * @param[in] net
 * 	The net whose number of points is read.
 * @return
 * 	The number of points of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_num_points(const tsDeBoorNet *net);

/**
 * Returns the size of the point array of \p net. This function may be useful
 * when copying points using memcpy or memmove.
 *
 * @param[in] net
 * 	The net with its point array whose size is read.
 * @return
 * 	The size of the point array of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_sof_points(const tsDeBoorNet *net);

/**
 * Returns a deep copy of the points of \p net.
 *
 * @param[in] net
 * 	The net whose points is read.
 * @param[out] points
 * 	The output array.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_deboornet_points(const tsDeBoorNet *net,
	tsReal **points, tsStatus *status);

/**
 * Returns the length of the result array of \p net.
 *
 * @param[in] net
 * 	The net with its result array whose length is read.
 * @return
 * 	The length of the result array of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_len_result(const tsDeBoorNet *net);

/**
 * Returns the number of points in the result array of \p net
 * (1 <= num_result <= 2).
 *
 * @param[in] net
 * 	The net with its result array whose number of points is read.
 * @return
 * 	The number of points in the result array of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_num_result(const tsDeBoorNet *net);

/**
 * Returns the size of the result array of \p net. This function may be useful
 * when copying results using memcpy or memmove.
 *
 * @param[in] net
 * 	The net with its result array whose size is read.
 * @return TS_SUCCESS
 * 	The size of the result array of \p net.
 */
size_t TINYSPLINE_API ts_deboornet_sof_result(const tsDeBoorNet *net);

/**
 * Returns a deep copy of the result of \p net.
 *
 * @param[in] net
 * 	The net whose result is read.
 * @param[out] result
 * 	The output array.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_deboornet_result(const tsDeBoorNet *net,
	tsReal **result, tsStatus *status);



/******************************************************************************
*                                                                             *
* :: Constructors, Destructors, Copy, and Move Functions                      *
*                                                                             *
******************************************************************************/
/**
 * Creates a new spline whose data points to NULL.
 * 
 * @return
 * 	A new spline whose data points to NULL.
 */
tsBSpline TINYSPLINE_API ts_bspline_init();

/**
 * Creates a new spline and stores the result in \p spline.
 *
 * @param[in] num_control_points
 * 	The number of control points of \p spline.
 * @param[in] dimension
 * 	The dimension of each control point of \p spline.
 * @param[in] degree
 * 	The degree of \p spline.
 * @param[in] type
 * 	How to setup the knot vector of \p spline.
 * @param[out] spline
 * 	The output spline.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_DIM_ZERO
 * 	If \p dimension == 0.
 * @return TS_DEG_GE_NCTRLP
 * 	If \p degree >= \p num_control_points.
 * @return TS_NUM_KNOTS
 * 	If \p type == ::TS_BEZIERS and
 * 	(\p num_control_points % \p degree + 1) != 0.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_new(size_t num_control_points,
	size_t dimension, size_t degree, tsBSplineType type, tsBSpline *spline,
	tsStatus *status);

/**
 * Creates a deep copy of \p src and stores the copied values in \p dest. Does
 * nothing, if \p src == \p dest.
 *
 * @param[in] src
 * 	The spline to deep copy.
 * @param[out] dest
 * 	The output spline.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_copy(const tsBSpline *src, tsBSpline *dest,
	tsStatus *status);

/**
 * Moves the ownership of the data of \p src to \p dest. After calling this
 * function, the data of \p src points to NULL. Does not free the data of
 * \p dest. Does nothing, if \p src == \p dest.
 * 
 * @param[out] src
 * 	The spline whose values are moved to \p dest.
 * @param[out] dest
 * 	The spline that receives the values of \p src.
 */
void TINYSPLINE_API ts_bspline_move(tsBSpline *src, tsBSpline *dest);

/**
 * Frees the data of \p spline. After calling this function, the data of
 * \p spline points to NULL.
 * 
 * @param[out] spline
 * 	The spline to free.
 */
void TINYSPLINE_API ts_bspline_free(tsBSpline *spline);

/* ------------------------------------------------------------------------- */

/**
 * Creates a new net whose data points to NULL.
 * 
 * @return
 * 	A new net whose data points to NULL.
 */
tsDeBoorNet TINYSPLINE_API ts_deboornet_init();

/**
 * Creates a deep copy of \p src and stores the copied values in \p dest. Does
 * nothing, if \p src == \p dest.
 *
 * @param[in] src
 * 	The net to deep copy.
 * @param[out] dest
 * 	The output net.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_deboornet_copy(const tsDeBoorNet *src,
	tsDeBoorNet *dest, tsStatus *status);

/**
 * Moves the ownership of the data of \p src to \p dest. After calling this
 * function, the data of \p src points to NULL. Does not free the data of
 * \p dest. Does nothing, if \p src == \p dest.
 * 
 * @param[out] src
 * 	The net whose values are moved to \p dest.
 * @param[out] dest
 * 	The net that receives the values of \p src.
 */
void TINYSPLINE_API ts_deboornet_move(tsDeBoorNet *src, tsDeBoorNet *dest);

/**
 * Frees the data of \p net. After calling this function, the data of \p net
 * points to NULL.
 * 
 * @param[out] net
 * 	The net to free.
 */
void TINYSPLINE_API ts_deboornet_free(tsDeBoorNet *net);



/******************************************************************************
*                                                                             *
* :: Interpolation and Approximation Functions                                *
*                                                                             *
******************************************************************************/
/**
 * Interpolates a cubic spline with natural end conditions. For more details
 * see:
 * 
 *     https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
 *     http://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf
 *     http://www.bakoma-tex.com/doc/generic/pst-bspline/pst-bspline-doc.pdf
 *
 * The resultant spline is a sequence of bezier curves connecting each point
 * in \p points. Each bezier curve is of degree 3 with dimensionality
 * \p dimension. The total number of control points is:
 *
 *     min(1, \p num_points - 1) * 4
 *
 * Note: \p num_points is the number of points in \p points and not the length
 * of \p points. For instance, the following point vector has
 * \p num_points = 4 and \p dimension = 2:
 * 
 *     [x0, y0, x1, y1, x2, y2, x3, y3]
 *
 * @param[in] points
 * 	The points to interpolate.
 * @param[in] num_points
 * 	The number of points in \p points.
 * @param[in] dimension
 * 	The dimension of each control point in \p spline.
 * @param[out] spline
 * 	The output spline.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_DIM_ZERO
 * 	If \p dimension is 0.
 * @return TS_NUM_POINTS
 * 	If \p num_points is 0.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_interpolate_cubic_natural(
	const tsReal *points, size_t num_points, size_t dimension,
	tsBSpline *spline, tsStatus *status);

/**
 * Interpolates a piecewise cubic spline by translating the given catmull-rom
 * control points into a sequence of bezier curves. In order to avoid division
 * by zero, successive control points with distance less than or equal to
 * \p epsilon are filtered out. If the resultant sequence contains only a
 * single point, a spline of degree 0 (a point) is created. Optionally, the
 * first and last control point can be specified (\p first and \p last).
 *
 * @param[in] points
 * 	The points to interpolate.
 * @param[in] num_points
 * 	The number of points in \p points.
 * @param[in] dimension
 * 	The dimensionality of the points.
 * @param[in] alpha
 * 	The knot parameterization: 0 => uniform, 0.5 => centripetal,
 * 	1 => chordal. The input value is automatically trimmed to the [0, 1]
 * 	interval.
 * @param[in] first
 * 	The first control point of the catmull-rom sequence. If NULL, an
 * 	appropriate point is generated based on the first two points in
 * 	\p points. If the distance between \p first and the first control point
 * 	in \p points is less than or equals to \p epsilon, \p first is treated
 * 	as NULL. This is necessary to avoid division by zero.
 * @param[in] last
 * 	The last control point of the catmull-rom sequence. If NULL, an
 * 	appropriate point is generated based on the last two points in
 * 	\p points. If the distance between \p last and the last control point
 * 	in \p points is less than or equals to \p epsilon, \p last is treated
 * 	as NULL. This is necessary to avoid division by zero.
 * @param[in] epsilon
 * 	The maximum distance between points with "same" coordinates. That is,
 * 	if the distance between neighboring points is less than or equal to
 * 	\p epsilon, they are considered equal. For the sake of fail-safeness,
 * 	the sign is removed with fabs. It is advisable to pass a value greater
 * 	than zero, however, it is not necessary.
 * @param[out] spline
 * 	The interpolated spline.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_DIM_ZERO
 * 	If \p dimension is 0.
 * @return TS_NUM_POINTS
 * 	If \p num_points is 0.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_interpolate_catmull_rom(const tsReal *points,
	size_t num_points, size_t dimension, tsReal alpha, const tsReal *first,
	const tsReal *last, tsReal epsilon, tsBSpline *spline,
	tsStatus *status);



/******************************************************************************
*                                                                             *
* :: Query Functions                                                          *
*                                                                             *
******************************************************************************/
/**
 * Evaluates \p spline at knot \p u and stores the result (cf. tsDeBoorNet) in
 * \p net.
 *
 * @param[in] spline
 * 	The spline to evaluate.
 * @param[in] u
 * 	The knot to evaluate \p spline at.
 * @param[out] net
 * 	The output parameter
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_U_UNDEFINED
 * 	If \p spline is not defined at knot value \p u.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_eval(const tsBSpline *spline, tsReal u,
	tsDeBoorNet *net, tsStatus *status);

/**
 * Evaluates \p spline at knots \p us and stores the resultant points in
 * \p points. If \p us contains one or more knots where \p spline is
 * discontinuous at, only the first point of the corresponding evaluation
 * result is taken. After calling this function \p points contains exactly
 * \p num * ts_bspline_dimension(spline) values.
 *
 * This function is in particular useful in cases where a multitude of knots
 * need to be evaluated, because only a single instance of tsDeBoorNet is
 * created and reused for all evaluation tasks (therefore the memory footprint
 * is reduced to a minimum).
 *
 * @param[in] spline
 * 	The spline to evaluate.
 * @param[in] us
 * 	The knot values to evaluate.
 * @param[in] num
 * 	The number of knots in \p us.
 * @param[out] points
 * 	The output parameter.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_U_UNDEFINED
 * 	If \p spline is not defined at one of the knot values in \p us.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_eval_all(const tsBSpline *spline,
	const tsReal *us, size_t num, tsReal **points, tsStatus *status);

/**
 * Generates a sequence of \p num different knots (The knots are equally
 * distributed between the minimum and the maximum of the domain of \p spline),
 * passes this sequence to ts_bspline_eval_all, and stores the resultant points
 * in \p points. If \p num is 0, the following default is taken as fallback:
 *
 * 	num = (ts_bspline_num_control_points(spline) -
 * 		ts_bspline_degree(spline)) * 30;
 *
 * That is, the fallback generates 30 knots per Bezier segment. For the sake
 * of stability regarding future changes, the actual number of generated knots
 * (which only differs from \p num if \p num is 0) is stored in \p actual_num.
 * If \p num is 1, the point located at the minimum of the domain of \p spline
 * is evaluated.
 *
 * @param[in] spline
 * 	The spline to evaluate.
 * @param[in] num
 * 	The number of knots to generate.
 * @param[out] points
 * 	The output parameter.
 * @param[out] actual_num
 * 	The actual number of generated knots. Differs from \p num only if
 * 	\p num is 0. Must not be NULL.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_sample(const tsBSpline *spline, size_t num,
	tsReal **points, size_t *actual_num, tsStatus *status);

/**
 * Tries to find a point P on \p spline such that:
 *
 *     ts_distance(P[index], value, 1) <= fabs(epsilon)
 *
 * This function is using the bisection method to determine P. Accordingly, it
 * is expected that the control points of \p spline are sorted at component
 * \p index either in ascending order (if \p ascending != 0) or in descending
 * order (if \p ascending == 0). If the control points of \p spline are not
 * sorted at component \p index, the behaviour of this function is undefined.
 * For the sake of fail-safeness, the distance of P[index] and \p value is
 * compared with the absolute value of \p epsilon (using fabs).
 *
 * The bisection method is an iterative approach which minimizes the error
 * (\p epsilon) with each iteration step until an "optimum" was found. However,
 * there may be no point P satisfying the distance condition. Thus, the number
 * of iterations must be limited (\p max_iter). Depending on the domain of the
 * control points of \p spline at component \p index and \p epsilon,
 * \p max_iter ranges from 7 to 50. In most cases \p max_iter == 30 should be
 * fine though. The parameter \p persnickety allows to define the behaviour of
 * this function is case no point was found after \p max_iter iterations. If
 * enabled (!= 0), TS_NO_RESULT is returned. If disabled (== 0), the best
 * fitting point is returned.
 *
 * @param[in] spline
 * 	The spline to evaluate
 * @param[in] value
 * 	The value (point at component \p index) to find.
 * @param[in] epsilon
 * 	The maximum distance (inclusive).
 * @param[in] persnickety
 * 	Indicates whether TS_NO_RESULT should be returned if there is no point
 * 	P satisfying the distance condition (!= 0 to enable, == 0 to disable).
 * 	If disabled, the best fitting point is returned.
 * @param[in] index
 * 	The point's component.
 * @param[in] ascending
 * 	Indicates whether the control points of \p spline are sorted in
 * 	ascending (!= 0) or in descending (== 0) order at component \p index.
 * @param[in] max_iter
 * 	The maximum number of iterations (30 is a sane default value).
 * @param[out] net
 * 	The output parameter.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_INDEX_ERROR
 * 	If the dimension of the control points of \p spline <= \p index.
 * @return TS_NO_RESULT
 * 	If \p persnickety is enabled (!= 0) and there is no point P satisfying
 * 	the distance condition.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_bisect(const tsBSpline *spline, tsReal value,
	tsReal epsilon, int persnickety, size_t index, int ascending,
	size_t max_iter,  tsDeBoorNet *net, tsStatus *status);

/**
 * Returns the domain of \p spline.
 *
 * @param[in] spline
 * 	The spline to query.
 * @param[out] min
 * 	The lower bound of the domain of \p spline.
 * @param[out] max
 * 	The upper bound of the domain of \p spline.
 */
void TINYSPLINE_API ts_bspline_domain(const tsBSpline *spline, tsReal *min,
	tsReal *max);

/**
 * Checks whether the distance of the endpoints of \p spline is less than or
 * equal to \p epsilon for the first 'ts_bspline_degree - 1' derivatives
 * (starting with the zeroth derivative).
 *
 * @param[in] spline
 * 	The spline to query.
 * @param[in] epsilon
 * 	The maximum distance.
 * @param[out] closed
 * 	The output parameter. 1 if true, 0 otherwise.
 * @param status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_is_closed(const tsBSpline *spline,
	tsReal epsilon, int *closed, tsStatus *status);



/******************************************************************************
*                                                                             *
* :: Transformation functions                                                 *
*                                                                             *
* TinySpline is a library focusing on transformations. That is, most          *
* functions are used to transform splines by modifying their state, e.g.,     *
* their number of control points, their degree, and so on. Accordingly, each  *
* transformation functions specifies an input and output parameter (along     *
* with the other parameters required to calculate the actual transformation). *
* By passing a different pointer to the output parameter, the transformation  *
* result is calculated and stored without changing the state of the input.    *
* This is in particular useful when dealing with errors as the original state *
* will never be modified. For instance, let's have a look at the following    *
* code snippet:                                                               *
*                                                                             *
*     tsBSpline in = ...    // an arbitrary spline                            *
*     tsBSpline out;        // result of transformation                       *
*                                                                             *
*     // Subdivide 'in' into sequence of bezier curves and store the result   *
*     // in 'out'. Does not change 'in' in any way.                           *
*     tsError err = ts_bspline_to_beziers(&in, &out);                         *
*     if (err != TS_SUCCESS) {                                                *
*         // fortunately, 'in' has not been changed                           *
*     }                                                                       *
*                                                                             *
* Even if 'ts_bspline_to_beziers' fails, the state of 'in' has not been       *
* changed allowing you to handle the error properly.                          *
*                                                                             *
* Unless stated otherwise, the order of the parameters for transformation     *
* functions is:                                                               *
*                                                                             *
*     function(input, [additional_input], output, [additional_output])        *
*                                                                             *
* 'additional_input' are parameters required to calculate the actual          *
* transformation. 'additional_output' are parameters storing further result.  *
*                                                                             *
* Note: None of TinySpline's transformation functions frees the memory of the *
*       output parameter. Thus, when using the same output parameter multiple *
*       times, make sure to free memory before each call. Otherwise, you will *
*       have a bad time with memory leaks:                                    *
*                                                                             *
*     tsBSpline in = ...                  // an arbitrary spline              *
*     tsBSpline out;                      // result of transformations        *
*                                                                             *
*     ts_bspline_to_beziers(&in, &out);    // first transformation            *
*     ...                                  // some code                       *
*     ts_bspline_free(&out);               // avoid memory leak.              *
*     ts_bspline_tension(&in, 0.85, &out); // next transformation             *
*                                                                             *
* If you want to modify your input directly without having a separate output, *
* pass it as input and output at once:                                        *
*                                                                             *
*     tsBSpline s = ...                       // an arbitrary spline          *
*     tsReal *knots = ...                     // a knot vector                *
*                                                                             *
*     ts_bspline_set_knots(&s, knots, &s);    // copy 'knots' into 's'        *
*                                                                             *
* Note: If a transformation function fails *and* input != output, all fields  *
*       of the output parameter are set to 0/NULL. If input == output, your   *
*       input may have an invalid state in case of errors.                    *
*                                                                             *
******************************************************************************/
/**
 * Returns the \p n'th derivative of \p spline and stores the result in
 * \p derivative. Creates a deep copy of \p spline if
 * \p spline != \p derivative. For more details, see:
 * 
 *     http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/B-spline/bspline-derv.html
 *
 * The derivative of a spline _s_ of degree _d_ (_d_ > 0) with _m_ control
 * points and _n_ knots is another spline _s'_ of degree _d-1_ with _m-1_
 * control points and _n-2_ knots, defined over _s_ as:
 * 
 * \f{eqnarray*}{
 *   s'(u) &=& \sum_{i=0}^{n-1} N_{i+1,p-1}(u) *
 * 		(P_{i+1} - P_{i}) * p / (u_{i+p+1}-u_{i+1}) \\
 *         &=& \sum_{i=1}^{n} N_{i,p-1}(u) *
 * 		(P_{i} - P_{i-1}) * p / (u_{i+p}-u_{i})
 * \f}
 * 
 * If _s_ has a clamped knot vector, it can be shown that:
 * 
 * \f{eqnarray*}{
 *   s'(u) &=& \sum_{i=0}^{n-1} N_{i,p-1}(u) *
 * 		(P_{i+1} - P_{i}) * p / (u_{i+p+1}-u_{i+1})
 * \f}
 * 
 * where the multiplicity of the first and the last knot value _u_ is _p_
 * rather than _p+1_. The derivative of a point (degree == 0) is another point
 * with coordinate 0.
 *
 * @param[in] spline
 * 	The spline to derive.
 * @param[in] n
 * 	The number of derivations.
 * @param[in] epsilon
 * 	The maximum distance of discontinuous points. If negative,
 * 	discontinuity is ignored and the derivative is computed based on the
 * 	first result of the corresponding DeBoorNet.
 * @param[out] derivative
 *	The derivative of \p spline.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_UNDERIVABLE
 * 	If \p spline is discontinuous at an internal knot and the distance
 * 	between the corresponding points is greater than \p epsilon.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_derive(const tsBSpline *spline, size_t n,
	tsReal epsilon, tsBSpline *derivative, tsStatus *status);

/**
 * Inserts the knot \p u up to \p num times into the knot vector of \p spline
 * and stores the result in \p result. Creates a deep copy of \p spline if
 * \p spline != \p result.
 * 
 * @param[in] spline
 * 	The spline with its knot vector into which \p u is inserted up to
 * 	\p num times.
 * @param[in] u
 * 	The knot to insert.
 * @param[in] num
 * 	How many times \p u should be inserted.
 * @param[out] result
 * 	The output spline.
 * @param[out] k
 * 	Stores the last index of \p u in \p result.
 * @param status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_insert_knot(const tsBSpline *spline,
	tsReal u, size_t num, tsBSpline *result, size_t *k, tsStatus *status);

/**
 * Splits \p spline at knot value \p u and stores the result in \p split. That
 * is, \p u is inserted _n_ times such that the multiplicity of \p u is equal
 * to the \p spline's order. Creates a deep copy of \p spline if
 * \p spline != \p split.
 * 
 * @param[in] spline
 * 	The spline to split.
 * @param[in] u
 * 	The split point (knot).
 * @param[out] split
 * 	The split spline.
 * @param[out] k
 * 	Stores the last index of \p u in \p split.
 * @param[out] status
 * 	Stores the last index of \p u in \p result.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_U_UNDEFINED
 * 	If \p spline is not defined at knot value \p u.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_split(const tsBSpline *spline, tsReal u,
	tsBSpline *split, size_t *k, tsStatus *status);

/**
 * Sets the control points of \p spline so that their tension corresponds the
 * given tension factor (0 => yields to a line connecting the first and the
 * last control point; 1 => keeps the original shape). If \p tension < 0 or if
 * \p tension > 1, the behaviour of this function is undefined, though, it will
 * not result in an error. Creates a deep copy of \p spline if
 * \p spline != \p out.
 *
 * This function is based on:
 * 
 *      Holten, Danny. "Hierarchical edge bundles: Visualization of adjacency
 *      relations in hierarchical data." Visualization and Computer Graphics,
 *      IEEE Transactions on 12.5 (2006): 741-748.
 * 
 * Holten calls it "straightening" (page 744, equation 1).
 *
 * @param[in] spline
 * 	The input spline.
 * @param[in] tension
 * 	The tension factor (0 <= \p tension <= 1).
 * @param[out] out
 * 	The output spline.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_tension(const tsBSpline *spline,
	tsReal tension, tsBSpline *out, tsStatus *status);

/**
 * Decomposes \p spline into a sequence of Bezier curves by splitting it at
 * each internal knot value. Creates a deep copy of \p spline if
 * \p spline != \p beziers.
 * 
 * @param[in] spline
 * 	The spline to decompose.
 * @param[out] beziers
 * 	The bezier decomposition of \p spline.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_to_beziers(const tsBSpline *spline,
	tsBSpline *beziers, tsStatus *status);



/******************************************************************************
*                                                                             *
* :: Serialization and Persistence Functions                                  *
*                                                                             *
* The following section contains functions to serialize and persist the data  *
* types listed above.                                                         *
*                                                                             *
******************************************************************************/
/**
 * Serializes \p spline to a null-terminated JSON string and stores the result
 * in \p json.
 *
 * @param[in] spline
 * 	The spline to serialize.
 * @param[out] json
 * 	The serialized JSON string.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_to_json(const tsBSpline *spline, char **json,
	tsStatus *status);

/**
 * Parses \p json and stores the result in \p spline.
 *
 * @param[in] json
 * 	The JSON string to parse.
 * @param[out] spline
 * 	The deserialized spline.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_PARSE_ERROR
 * 	If an error occurred while parsing \p json.
 * @return TS_DIM_ZERO
 * 	If the dimension is 0.
 * @return TS_LCTRLP_DIM_MISMATCH
 * 	If the length of the control point vector modulo dimension is not 0.
 * @return TS_DEG_GE_NCTRLP
 * 	If the degree is greater or equals to the number of control points.
 * @return TS_NUM_KNOTS
 * 	If the number of knots stored in \p json does not match to the number
 * 	of control points and the degree of the spline.
 * @return TS_KNOTS_DECR
 * 	If the knot vector is decreasing.
 * @return TS_MULTIPLICITY
 * 	If there is a knot with multiplicity greater than order.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_from_json(const char *json,
	tsBSpline *spline, tsStatus *status);

/**
 * Saves \p spline as JSON ASCII file.
 *
 * @param[in] spline
 * 	The spline to save.
 * @param[in] path
 * 	Path of the JSON file.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_IO_ERROR
 * 	If an error occurred while saving \p spline.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_save(const tsBSpline *spline,
	const char *path, tsStatus *status);

/**
 * Loads \p spline from a JSON ASCII file.
 *
 * @param[in] path
 * 	Path of the JSON file.
 * @param[out] spline
 * 	The output spline.
 * @param[ou] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_IO_ERROR
 * 	If \p path does not exist.
 * @return TS_PARSE_ERROR
 * 	If an error occurred while parsing the contents of \p path.
 * @return TS_DIM_ZERO
 * 	If the dimension is 0.
 * @return TS_LCTRLP_DIM_MISMATCH
 * 	If the length of the control point vector modulo dimension is not 0.
 * @return TS_DEG_GE_NCTRLP
 * 	If the degree is greater or equals to the number of control points.
 * @return TS_NUM_KNOTS
 * 	If the number of knots stored in \p json does not match to the number
 * 	of control points and the degree of the spline.
 * @return TS_KNOTS_DECR
 * 	If the knot vector is decreasing.
 * @return TS_MULTIPLICITY
 * 	If there is a knot with multiplicity greater than order.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError TINYSPLINE_API ts_bspline_load(const char *path, tsBSpline *spline,
	tsStatus *status);



/******************************************************************************
*                                                                             *
* :: Utility Functions                                                        *
*                                                                             *
* The following section contains utility functions used by TinySpline which   *
* also may be helpful when using this library.                                *
*                                                                             *
******************************************************************************/
/**
 * Checks whether \p x and \p y are equal with respect to the epsilon
 * environment TS_KNOT_EPSILON, i.e. their distance is less than
 * TS_KNOT_EPSILON.
 *
 * @param[in] x
 * 	A knot.
 * @param[in] y
 * 	A knot.
 * @return 1
 * 	If \p x and \p y are equal.
 * @return 0
 * 	If \p x and \p y are not equal.
 */
int TINYSPLINE_API ts_knots_equal(tsReal x, tsReal y);

/**
 * Fills the given array \p arr with \p val from \p arr+0 to \p arr+ \p num
 * (exclusive).
 *
 * @param[in] arr
 * 	The array to fill.
 * @param[in] num
 * 	The fill length.
 * @param[in] val
 * 	The value to fill into \p arr.
 */
void TINYSPLINE_API  ts_arr_fill(tsReal *arr, size_t num, tsReal val);

/**
 * Returns the euclidean distance of the points \p x and \p y.
 *
 * @param[in] x
 * 	The x value.
 * @param[in] y
 * 	The y value.
 * @param[in] dimension
 * 	The dimension of \p x and \p y.
 * @return
 * 	The euclidean distance of \p x and \p y.
 */
tsReal TINYSPLINE_API ts_distance(const tsReal *x, const tsReal *y,
	size_t dimension);



#ifdef	__cplusplus
}
#endif

#endif	/* TINYSPLINE_H */

