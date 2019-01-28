/** @file */

#ifndef TINYSPLINE_H
#define	TINYSPLINE_H

#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************
*                                                                             *
* :: Predefined Constants                                                     *
*                                                                             *
* The following constants should only be changed with caution. Otherwise, the *
* internal consistency can not be guaranteed anymore. The predefined values   *
* should be suitable for almost all environments, though.                     *
*                                                                             *
******************************************************************************/
/**
 * The maximum number of knots of a spline.
 */
#define TS_MAX_NUM_KNOTS 10000

/**
 * The minimum knot value of a spline.
 */
#define TS_MIN_KNOT_VALUE 0.0f

/**
 * The maximum knot value of a spline.
 */
#define TS_MAX_KNOT_VALUE 1.0f

/**
 * Used to check whether two knots are equal.
 */
#define TS_KNOT_EPSILON 1e-4



/******************************************************************************
*                                                                             *
* :: System Dependent Configuration                                           *
*                                                                             *
* The following configuration values must be adapted to your system. Some of  *
* them may be configured with preprocessor definitions. The default values    *
* should be fine for most modern hardware, such as x86, x86_64, and arm.      *
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
* The following section defines enums, structs, and macros that are used to   *
* handle different types of errors. The following listing shows an example:   *
*                                                                             *
*     tsStatus status;                                                        *
*     TS_TRY(any_label, status.code, &status)                                 *
*         // Use TS_CALL when calling functions of this library.              *
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
* code into TS_TRY/TS_END_TRY as well as passing a pointer to a tsStatus      *
* object is entirely optional, as shown by the following example:             *
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
    TS_DIM_ZERO = -2, /**< The dimension of the control points are 0. */
    TS_DEG_GE_NCTRLP = -3, /**< degree >= num_control_points. */
    TS_U_UNDEFINED = -4, /**< Undefined knot value. */
    TS_MULTIPLICITY = -5, /**< s(u) > order */
    TS_KNOTS_DECR = -6, /**< Decreasing knot vector. */
    TS_NUM_KNOTS = -7, /**< Unexpected number of knots. */
    TS_UNDERIVABLE = -8, /**< Spline is not derivable. */
    TS_LCTRLP_DIM_MISMATCH = -10, /**< len_control_points % dim != 0. */
    TS_IO_ERROR = -11, /**< Error while reading/writing a file. */
    TS_PARSE_ERROR = -12, /**< Error while parsing a serialized entity. */
    TS_INDEX_ERROR = -13 /**< Index does not exist. */
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
* The following section defines all available data types.                     *
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
 * Represents a B-Spline which may also be used for NURBS, Bezier curves,
 * lines, and points. NURBS are represented by homogeneous coordinates where
 * the last component of a control point stores the weight of this control
 * point. Bezier curves are B-Splines with 'num_control_points == order' and a
 * clamped knot vector, therefore passing through their first and last control
 * point (a property which does not necessarily apply to B-Splines and NURBS).
 * If a Bezier curve consists of two control points only, we call it a line.
 * Points, ultimately, are just very short lines consisting of a single control
 * point.
 *
 * Two dimensional control points are stored as follows:
 *
 *     [x_0, y_0, x_1, y_1, ..., x_n-1, y_n-1]
 *
 * Tree dimensional control points are stored as follows:
 *
 *     [x_0, y_0, z_0, x_1, y_1, z_1, ..., x_n-1, y_n-1, z_n-1]
 *
 * ... and so on. NURBS are represented by homogeneous coordinates. For
 * example, let's say we have a NURBS in 2D consisting of 11 control points
 * where 'w_i' is the weight of the i'th control point. Then, the corresponding
 * control points are stored as follows:
 *
 *     [x_0*w_0, y_0*w_0, w_0, x_1*w_1, y_1*w_1, w_1, ..., x_10*w_10, y_10*w_10, w_10]
 */
typedef struct
{
	struct tsBSplineImpl *pImpl; /**< The actual implementation. */
} tsBSpline;

/**
 * Represents the output of De Boor's algorithm. De Boor's algorithm is used to
 * evaluate a spline at given knot value 'u' by iteratively computing a net of
 * intermediate values until the result is available:
 *
 *     https://en.wikipedia.org/wiki/De_Boor%27s_algorithm
 *     https://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/de-Boor.html
 *
 * All points of a net are stored in 'points'. The resulting point is the last
 * point in 'points' and, for the sake of convenience, may be accessed with
 * 'result':
 *
 *     tsDeBoorNet net = ...    // evaluate an arbitrary spline and store
 *                              // the resulting net of points in 'net'
 *
 *     ts_deboornet_result(...) // use 'result' to access the resulting point
 *
 * Two dimensional points are stored as follows:
 *
 *     [x_0, y_0, x_1, y_1, ..., x_n-1, y_n-1]
 *
 * Tree dimensional points are stored as follows:
 *
 *     [x_0, y_0, z_0, x_1, y_1, z_1, ..., x_n-1, y_n-1, z_n-1]
 *
 * ... and so on. The output also supports homogeneous coordinates described
 * above.
 *
 * There is a special case in which the evaluation of a knot value 'u' returns
 * two results instead of one. It occurs when the multiplicity of 'u' ( s(u) )
 * is equals to a spline's order, indicating that the spline is discontinuous
 * at 'u'. This is common practice for B-Splines (and NURBS) consisting of
 * connected Bezier curves where the endpoint of curve 'c_i' is equals to the
 * start point of curve 'c_i+1'. The end point of 'c_i' and the start point of
 * 'c_i+1' may still be completely different though, yielding to a spline
 * having a (real and visible) gap at 'u'. Consequently, De Boor's algorithm
 * must return two results if 's(u) == order' in order to give access to the
 * desired points.  In such case, 'points' stores only the two resulting points
 * (there is no net to calculate) and 'result' points to the *first* point in
 * 'points'. Since having (real) gaps in splines is unusual, both points in
 * 'points' are generally equals, making it easy to handle this special case by
 * accessing 'result' as already shown above for regular cases:
 *
 *     tsDeBoorNet net = ...    // evaluate a spline which is discontinuous at
 *                              // the given knot value, yielding to a net with
 *                              // two results
 *
 *     ts_deboornet_result(...) // use 'result' to access the resulting point
 *
 * However, you can access both points if necessary:
 *
 *     tsDeBoorNet net = ...    // evaluate a spline which is discontinuous at
 *                              // the given knot value, yielding to a net with
 *                              // two results
 *
 *     ts_deboornet_result(...)[0] ...       // stores the first component of
 *                                           // the first point
 *
 *     ts_deboornet_result(...)[dim(spline)] // stores the first component of
 *                                           // the second point
 *
 * As if this wasn't complicated enough, there is an exception for this special
 * case, yielding to exactly one result (just like the regular case) even if
 * 's(u) == order'. It occurs when 'u' is the lower or upper bound of a
 * spline's domain. For instance, if 'b' is a spline with domain [0, 1] and is
 * evaluated at 'u = 0' or 'u = 1' then 'result' is *always* a single point
 * regardless of the multiplicity of 'u'. Hence, handling this exception is
 * straightforward:
 *
 *     tsDeBoorNet net = ...    // evaluate a spline at the lower or upper
 *                              // bound of its domain, for instance, 0 or 1
 *
 *     ts_deboornet_result(...) // use 'result' to access the resulting point
 *
 * In summary, we have three different types of evaluation. 1) The regular case
 * returning all points of the net we used to calculate the resulting point. 2)
 * The special case returning exactly two points which is required for splines
 * with (real) gaps. 3) The exception of 2) returning exactly one point even if
 * 's(u) == order'. All in all this looks quite complex (and actually it is)
 * but for most applications you do not have to deal with it. Just use 'result'
 * to access the outcome of De Boor's algorithm.
 */
typedef struct
{
	struct tsDeBoorNetImpl *pImpl; /**< The actual implementation. */
} tsDeBoorNet;



/******************************************************************************
*                                                                             *
* :: Field Access Functions                                                   *
*                                                                             *
* The following section contains getter and setter functions for the internal *
* state of the structs listed above.                                          *
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
size_t ts_bspline_degree(const tsBSpline *spline);

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
tsError ts_bspline_set_degree(tsBSpline *spline, size_t deg, tsStatus *status);

/**
 * Returns the order (degree + 1) of \p spline.
 *
 * @param[in] spline
 * 	The spline whose order is read.
 * @return
 * 	The order of \p spline.
 */
size_t ts_bspline_order(const tsBSpline *spline);

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
tsError ts_bspline_set_order(tsBSpline *spline, size_t order,
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
size_t ts_bspline_dimension(const tsBSpline *spline);

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
tsError ts_bspline_set_dimension(tsBSpline *spline, size_t dim,
	tsStatus *status);

/**
 * Returns the length of the control point array of \p spline.
 *
 * @param[in] spline
 * 	The spline with its control point array whose length is read.
 * @return
 * 	The length of the control point array of \p spline.
 */
size_t ts_bspline_len_control_points(const tsBSpline *spline);

/**
 * Returns the number of control points of \p spline.
 *
 * @param[in] spline
 * 	The spline whose number of control points is read.
 * @return
 * 	The number of control points of \p spline.
 */
size_t ts_bspline_num_control_points(const tsBSpline *spline);

/**
 * Returns the size of the control point array of \p spline. This function may
 * be useful when copying control points using memcpy or memmove.
 *
 * @param[in] spline
 * 	The spline with its control point array whose size is read.
 * @return
 * 	The size of the control point array of \p spline.
 */
size_t ts_bspline_sof_control_points(const tsBSpline *spline);

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
tsError ts_bspline_control_points(const tsBSpline *spline, tsReal **ctrlp,
	tsStatus *status);

/**
 * Returns a deep copy of the control point of \p spline at \p index (index 0
 * is the first control points, index 1 is the second control point, and so on).
 *
 * @param[in] spline
 * 	The spline whose control point is read.
 * @param[in] index
 * 	The zero based index of the requested control point.
 * @param[out] ctrlp
 * 	The output array.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_INDEX_ERROR
 * 	If there is no control point at \p index.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_control_point_at(const tsBSpline *spline, size_t index,
	tsReal **ctrlp, tsStatus *status);

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
tsError ts_bspline_set_control_points(tsBSpline *spline, const tsReal *ctrlp,
	tsStatus *status);

/**
 * Sets the control point of \p spline at \p index. Creates a deep copy of
 * \p ctrlp.
 *
 * @param[out] spline
 * 	The spline whose control point is set.
 * @param[in] index
 * 	The zero based index of the control point to set.
 * @param[in] ctrlp
 * 	The values to deep copy.
 * @param[out] status
 * 	The status of this function. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_INDEX_ERROR
 * 	If there is no control point at \p index.
 */
tsError ts_bspline_set_control_point_at(tsBSpline *spline, size_t index,
	const tsReal *ctrlp, tsStatus *status);

/**
 * Returns the number of knots of \p spline.
 *
 * @param[in] spline
 * 	The spline whose number of knots is read.
 * @return
 * 	The number of knots of \p spline.
 */
size_t ts_bspline_num_knots(const tsBSpline *spline);

/**
 * Returns the size of the knot array of \p spline. This function may be useful
 * when copying knots using memcpy or memmove.
 *
 * @param[in] spline
 * 	The spline with its knot array whose size is read.
 * @return TS_SUCCESS
 * 	The size of the knot array of \p spline.
 */
size_t ts_bspline_sof_knots(const tsBSpline *spline);

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
tsError ts_bspline_knots(const tsBSpline *spline, tsReal **knots,
	tsStatus *status);

/**
 * Sets the knots of \p spline. Creates a deep copy of \p knots and scales it's
 * values to [TS_MIN_KNOT_VALUE, TS_MAX_KNOT_VALUE].
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
tsError ts_bspline_set_knots(tsBSpline *spline, const tsReal *knots,
	tsStatus *status);

/* ------------------------------------------------------------------------- */

/**
 * Returns the knot (sometimes also called 'u' or 't') of \p net.
 *
 * @param[in] net
 * 	The net whose knot is read.
 * @return
 * 	The knot of \p net.
 */
tsReal ts_deboornet_knot(const tsDeBoorNet *net);

/**
 * Returns the index [u_k, u_k+1) with u being the knot of \p net.
 *
 * @param[in] net
 * 	The net whose index is read.
 * @return
 * 	The index [u_k, u_k+1) with u being the knot of \p net.
 */
size_t ts_deboornet_index(const tsDeBoorNet *net);

/**
 * Returns the multiplicity of the knot of \p net.
 *
 * @param[in] net
 * 	The net whose multiplicity is read.
 * @return
 * 	The multiplicity of the knot of \p net.
 */
size_t ts_deboornet_multiplicity(const tsDeBoorNet *net);

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
size_t ts_deboornet_num_insertions(const tsDeBoorNet *net);

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
size_t ts_deboornet_dimension(const tsDeBoorNet *net);

/**
 * Returns the length of the point array of \p net.
 *
 * @param[in] net
 * 	The net with its point array whose length is read.
 * @return
 * 	The length of the point array of \p net.
 */
size_t ts_deboornet_len_points(const tsDeBoorNet *net);

/**
 * Returns the number of points of \p net.
 *
 * @param[in] net
 * 	The net whose number of points is read.
 * @return
 * 	The number of points of \p net.
 */
size_t ts_deboornet_num_points(const tsDeBoorNet *net);

/**
 * Returns the size of the point array of \p net. This function may be useful
 * when copying points using memcpy or memmove.
 *
 * @param[in] net
 * 	The net with its point array whose size is read.
 * @return
 * 	The size of the point array of \p net.
 */
size_t ts_deboornet_sof_points(const tsDeBoorNet *net);

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
tsError ts_deboornet_points(const tsDeBoorNet *net, tsReal **points,
	tsStatus *status);

/**
 * Returns the length of the result array of \p net.
 *
 * @param[in] net
 * 	The net with its result array whose length is read.
 * @return
 * 	The length of the result array of \p net.
 */
size_t ts_deboornet_len_result(const tsDeBoorNet *net);

/**
 * Returns the number of points in the result array of \p net
 * (1 <= num_result <= 2).
 *
 * @param[in] net
 * 	The net with its result array whose number of points is read.
 * @return
 * 	The number of points in the result array of \p net.
 */
size_t ts_deboornet_num_result(const tsDeBoorNet *net);

/**
 * Returns the size of the result array of \p net. This function may be useful
 * when copying results using memcpy or memmove.
 *
 * @param[in] net
 * 	The net with its result array whose size is read.
 * @return TS_SUCCESS
 * 	The size of the result array of \p net.
 */
size_t ts_deboornet_sof_result(const tsDeBoorNet *net);

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
tsError ts_deboornet_result(const tsDeBoorNet *net, tsReal **result,
	tsStatus *status);



/******************************************************************************
*                                                                             *
* :: Constructors, Destructors, Copy, and Move Functions                      *
*                                                                             *
* The following section contains functions to create and delete instances of  *
* the data types listed above. Additionally, each data type has a copy and    *
* move function.                                                              *
*                                                                             *
******************************************************************************/
/**
 * Creates a new spline whose data points to NULL.
 * 
 * @return
 * 	A new spline whose data points to NULL.
 */
tsBSpline ts_bspline_init();

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
 * 	If \p degree == 0.
 * @return TS_DEG_GE_NCTRLP
 * 	If \p degree >= \p num_control_points.
 * @return TS_NUM_KNOTS
 * 	If \p type == ::TS_BEZIERS and
 * 	(\p num_control_points % \p degree + 1) != 0.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_new(size_t num_control_points, size_t dimension,
	size_t degree, tsBSplineType type, tsBSpline *spline,
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
tsError ts_bspline_copy(const tsBSpline *src, tsBSpline *dest,
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
void ts_bspline_move(tsBSpline *src, tsBSpline *dest);

/**
 * Frees the data of \p spline. After calling this function, the data of
 * \p spline points to NULL.
 * 
 * @param[out] spline
 * 	The spline to free.
 */
void ts_bspline_free(tsBSpline *spline);

/* ------------------------------------------------------------------------- */

/**
 * Creates a new net whose data points to NULL.
 * 
 * @return
 * 	A new net whose data points to NULL.
 */
tsDeBoorNet ts_deboornet_init();

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
tsError ts_deboornet_copy(const tsDeBoorNet *src, tsDeBoorNet *dest,
	tsStatus *status);

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
void ts_deboornet_move(tsDeBoorNet *src, tsDeBoorNet *dest);

/**
 * Frees the data of \p net. After calling this function, the data of \p net
 * points to NULL.
 * 
 * @param[out] net
 * 	The net to free.
 */
void ts_deboornet_free(tsDeBoorNet *net);



/******************************************************************************
*                                                                             *
* :: Interpolation and Approximation Functions                                *
*                                                                             *
* The following section contains functions to interpolate and approximate     *
* arbitrary splines.                                                          *
*                                                                             *
******************************************************************************/
/**
 * Performs a cubic spline interpolation using the thomas algorithm, see:
 * 
 *     https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
 *     http://www.math.ucla.edu/~baker/149.1.02w/handouts/dd_splines.pdf
 *     http://www.bakoma-tex.com/doc/generic/pst-bspline/pst-bspline-doc.pdf
 *
 * The resulting spline is a sequence of bezier curves connecting each point
 * in \p points. Each bezier curve _b_ is of degree 3 with \p dim being the
 * dimension of the each control point in _b_. The total number of control
 * points is (\p n - 1) * 4.
 *
 * On error, all values of \p \_spline\_ are set to 0/NULL.
 *
 * Note: \p n is the number of points in \p points and not the length of
 * \p points. For instance, the follwing point vector yields to \p n = 4 and
 * \p dim = 2:
 * 
 *     [x0, y0, x1, y1, x2, y2, x3, y3]
 *
 * @param points
 * 	The points to interpolate.
 * @param n
 * 	The number of points in \p points.
 * @param dim
 * 	The dimension of each control point in \p \_spline\_.
 * @param \_spline\_
 * 	The output parameter storing the result of this function.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_DIM_ZERO
 * 	If \p dim == 0.
 * @return TS_DEG_GE_NCTRLP
 * 	If \p n < 2.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_interpolate_cubic(const tsReal *points, size_t n,
	size_t dim, tsBSpline *_spline_, tsStatus *status);



/******************************************************************************
*                                                                             *
* :: Query Functions                                                          *
*                                                                             *
* The following section contains functions to query splines.                  *
*                                                                             *
******************************************************************************/
/**
 * Evaluates \p spline at knot value \p u and stores the result in
 * \p \_deBoorNet\_.
 *
 * @param spline
 * 	The spline to evaluate.
 * @param u
 * 	The knot value to evaluate.
 * @param \_deBoorNet\_
 * 	The output parameter.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_U_UNDEFINED
 * 	If \p spline is not defined at knot value \p u.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_eval(const tsBSpline *spline, tsReal u,
	tsDeBoorNet *_deBoorNet_, tsStatus *status);

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
void ts_bspline_domain(const tsBSpline *spline, tsReal *min, tsReal *max);

/**
 * Returns whether the distance of the first and the last point (not
 * necessarily the control points) of \p spline is less than or equal to
 * \p epsilon. The distance of possible inner gaps is not considered.
 *
 * @param spline
 * 	The spline to query.
 * @param epsilon
 * 	The maximum distance.
 * @param closed
 * 	The output parameter. Is set to 1 if the distance of the first and the
 * 	last point (not necessarily the control points) of \p spline is less
 * 	than or equal to \p epsilon. Is set to 0 otherwise.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_is_closed(const tsBSpline *spline, tsReal epsilon,
	int *closed, tsStatus *status);



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
*     ts_bspline_to_beziers(&in, &out);   // first transformation             *
*     ...                                 // some code                        *
*     ts_bspline_free(&out);              // avoid memory leak.               *
*     ts_bspline_buckle(&in, &out);       // next transformation              *
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
 * \p \_derivative\_. Creates a deep copy of \p spline, if
 * \p spline != \p \_result\_. For more details, see:
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
 * with coordinates 0.
 *
 * @param spline
 * 	The spline to derive.
 * @param \_derivative\_
 *	The output parameter.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_UNDERIVABLE
 * 	If the multiplicity of an internal knot of \p spline is greater than
 * 	the degree of \p spline. NOTE: This will be fixed in the future.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_derive(const tsBSpline *spline, size_t n,
	tsBSpline *_derivative_, tsStatus *status);

/**
 * Inserts the knot value \p u up to \p n times into \p spline and stores the
 * result in \p \_result\_. Creates a deep copy of \p spline, if
 * \p spline != \p \_result\_.
 * 
 * @param spline
 * 	The spline whose knot vector is extended.
 * @param u
 * 	The knot value to insert.
 * @param n
 * 	How many times \p u should be inserted.
 * @param \_result\_
 * 	The output parameter.
 * @param \_k\_
 * 	Stores the last index of \p u in \p \_result\_.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_insert_knot(const tsBSpline *spline, tsReal u, size_t n,
	tsBSpline *_result_, size_t *_k_, tsStatus *status);

/**
 * Splits \p spline at knot value \p u and stores the result in \p \_split\_.
 * That is, \p u is inserted _n_ times such that the multiplicity of \p u in
 * \p spline is equal to the spline's order. Creates a deep copy of \p spline,
 * if \p spline != \p \_split\_.
 * 
 * @param spline
 * 	The spline to split.
 * @param u
 * 	The split point.
 * @param \_split\_
 * 	The output parameter.
 * @param \_k\_
 * 	Output parameter. Stores the last index of \p u in \p \_split\_.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_U_UNDEFINED
 * 	If \p spline is not defined at knot value \p u.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_split(const tsBSpline *spline, tsReal u, tsBSpline *_split_,
	size_t *_k_, tsStatus *status);

/**
 * Buckles \p spline by \p b and stores the result in \p \_buckled\_. Creates
 * a deep copy of \p spline, if \p spline != \p \_buckled\_.
 *
 * This function is based on:
 * 
 *      Holten, Danny. "Hierarchical edge bundles: Visualization of adjacency
 *      relations in hierarchical data." Visualization and Computer Graphics,
 *      IEEE Transactions on 12.5 (2006): 741-748.
 * 
 * Holten calls it "straightening" (page 744, equation 1).
 *
 * Usually, the range of \p b is: 0.0 <= \p b <= 1.0 with 0 yielding to a line
 * connecting the first and the last control point (no buckle) and 1 keeping
 * the original shape (maximum buckle). If \b < 0 or \b > 1 the behaviour is
 * undefined, though, it will not result in an error.
 *
 * @param spline
 * 	The spline to buckle.
 * @param b
 * 	The buckle factor (usually 0.0 <= \p b <= 1.0).
 * @param \_buckled\_
 * 	The output parameter.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_buckle(const tsBSpline *spline, tsReal b,
	tsBSpline *_buckled_, tsStatus *status);

/**
 * Subdivides \p spline into a sequence of Bezier curves by splitting it at
 * each internal knot value. Creates a deep copy of \p spline, if
 * \p spline != \p \_beziers\_.
 * 
 * @param spline
 * 	The spline to subdivide.
 * @param \_beziers\_
 * 	The output parameter.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_to_beziers(const tsBSpline *spline, tsBSpline *_beziers_,
	tsStatus *status);



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
 * in \p \_json\_.
 *
 * @param spline
 * 	The spline to serialize.
 * @param json
 * 	The output parameter.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_to_json(const tsBSpline *spline, char **_json_,
	tsStatus *status);

/**
 * Parses \p json and stores the result in \p \_spline\_.
 *
 * @param json
 * 	The JSON string to parse.
 * @param _spline_
 * 	The output parameter.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
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
tsError ts_bspline_from_json(const char *json, tsBSpline *_spline_,
	tsStatus *status);

/**
 * Saves \p spline as JSON ASCII file.
 *
 * @param spline
 * 	The spline to save.
 * @param path
 * 	Path of the JSON file.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
 * @return TS_SUCCESS
 * 	On success.
 * @return TS_IO_ERROR
 * 	If an error occurred while saving \p spline.
 * @return TS_MALLOC
 * 	If allocating memory failed.
 */
tsError ts_bspline_save_json(const tsBSpline *spline, const char *path,
	tsStatus *status);

/**
 * Loads the contents of \p path and stores the result in \p \_spline\_.
 *
 * @param path
 * 	Path of the JSON file.
 * @param _spline_
 * 	The output parameter.
 * @param status
 * 	Output parameter. Store the returned error code and a descriptive error
 * 	message. May be NULL.
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
tsError ts_bspline_load_json(const char *path, tsBSpline *_spline_,
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
 * Compares the values \p x and \p y using TS_EPSILON.
 *
 * @param x
 * 	The x value to compare.
 * @param y
 * 	The y value to compare.
 * @return 1
 * 	If \p x is equals to \p y according to TS_EPSILON.
 * @return 0
 * 	Otherwise.
 */
int ts_fequals(tsReal x, tsReal y);

/**
 * Fills the given array \p arr with \p val from \p arr+0 to \p arr+ \p num
 * (exclusive).
 */
void ts_arr_fill(tsReal *arr, size_t num, tsReal val);

/**
 * Returns the euclidean distance of \p x and \p y consisting of \p dim
 * components, respectively.
 *
 * @param x
 * 	The x value.
 * @param y
 * 	The y value.
 * @param dim
 * 	The dimension of \p x and \p y.
 * @return
 * 	The euclidean distanc of \p x and \p y.
 */
tsReal ts_ctrlp_dist2(const tsReal *x, const tsReal *y, size_t dim);



#ifdef	__cplusplus
}
#endif

#endif	/* TINYSPLINE_H */

