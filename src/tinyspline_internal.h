#ifndef TINYSPLINE_INTERNAL_H
#define TINYSPLINE_INTERNAL_H

#ifndef TINYSPLINE_DISABLE_INT_HEADER_WARN
#warning "You are including the TinySpline internal header! The functions in this header \
may change with TinySpline versions and the API may be unstable! You are advised to pin \
the version of TinySpline used. Use these APIs are your own risk."
#endif

#include "tinyspline.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*! @name Internal Structs and Functions
 *
 * Internal functions are prefixed with \e ts_int (int for internal).
 *
 * @{
 */
/**
 * Stores the private data of ::tsBSpline.
 */
struct tsBSplineImpl
{
	size_t deg; /**< Degree of B-Spline basis function. */
	size_t dim; /**< Dimensionality of the control points (2D => x, y). */
	size_t n_ctrlp; /**< Number of control points. */
	size_t n_knots; /**< Number of knots (n_ctrlp + deg + 1). */
};

/**
 * Stores the private data of ::tsDeBoorNet.
 */
struct tsDeBoorNetImpl
{
	tsReal u; /**< The evaluated knot. */
	size_t k; /**< The index [u_k, u_k+1) */
	size_t s; /**< Multiplicity of u_k. */
	size_t h; /**< Number of insertions required to obtain result. */
	size_t dim; /**< Dimensionality of the points (2D => x, y). */
	size_t n_points; /** Number of points in `points'. */
};

void ts_int_bspline_init(tsBSpline *spline);
size_t ts_int_bspline_sof_state(const tsBSpline *spline);
tsReal* ts_int_bspline_access_ctrlp(const tsBSpline *spline);
tsReal* ts_int_bspline_access_knots(const tsBSpline *spline);

tsError ts_int_bspline_access_ctrlp_at(
    const tsBSpline *spline,
    size_t index,
    tsReal **ctrlp,
    tsStatus *status);

tsError ts_int_bspline_access_knot_at(
    const tsBSpline *spline,
    size_t index,
    tsReal *knot,
    tsStatus *status
);

void ts_int_deboornet_init(tsDeBoorNet *net);
size_t ts_int_deboornet_sof_state(const tsDeBoorNet *net);
tsReal* ts_int_deboornet_access_points(const tsDeBoorNet *net);
tsReal* ts_int_deboornet_access_result(const tsDeBoorNet *net);
tsError ts_int_bspline_generate_knots(const tsBSpline *spline,
    tsBSplineType type,
    tsStatus *status
);

tsError ts_int_deboornet_new(
    const tsBSpline *spline,
    tsDeBoorNet *net,
    tsStatus *status
);

tsError ts_int_cubic_point(
    const tsReal *point,
    size_t dim,
    tsBSpline *spline,
    tsStatus *status
);

tsError ts_int_thomas_algorithm(
    const tsReal *a,
    const tsReal *b,
    const tsReal *c,
    size_t num,
    size_t dim,
    tsReal *d,
    tsStatus *status
);

tsError ts_int_relaxed_uniform_cubic_bspline(
    const tsReal *points,
    size_t n,
    size_t dim,
    tsBSpline *spline,
    tsStatus *status
);

tsError ts_int_bspline_find_knot(
    const tsBSpline *spline,
    tsReal *knot,
    size_t *idx,
    size_t *mult,
    tsStatus *status
);

tsError ts_int_bspline_eval_woa(
    const tsBSpline *spline,
    tsReal u,
    tsDeBoorNet *net,
    tsStatus *status
);

tsError ts_int_bspline_resize(
    const tsBSpline *spline,
    int n,
    int back,
    tsBSpline *resized,
    tsStatus *status
);
tsError ts_int_bspline_insert_knot(
    const tsBSpline *spline,
    const tsDeBoorNet *net,
    size_t n,
    tsBSpline *result,
    tsStatus *status
);

#ifdef	__cplusplus
}
#endif

#endif