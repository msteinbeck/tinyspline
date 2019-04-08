#include "tinyspline.h"
#include "parson.h" /* serialization */

#include <stdlib.h> /* malloc, free */
#include <math.h>   /* fabs, sqrt */
#include <string.h> /* memcpy, memmove, strcmp */
#include <stdio.h>  /* FILE, fopen */

/* Suppress some useless MSVC warnings. */
#ifdef _MSC_VER
#pragma warning(push)
/* address of dllimport */
#pragma warning(disable:4232)
/* function not inlined */
#pragma warning(disable:4710)
/* byte padding */
#pragma warning(disable:4820)
/* meaningless deprecation */
#pragma warning(disable:4996)
/* Spectre mitigation */
#pragma warning(disable:5045)
#endif

#define INIT_OUT_BSPLINE(in, out)              \
	if ((in) != (out))                     \
		ts_internal_bspline_init(out);



/******************************************************************************
*                                                                             *
* :: Data Types                                                               *
*                                                                             *
******************************************************************************/
/**
 * Stores the private data of a ::tsBSpline.
 */
struct tsBSplineImpl
{
	size_t deg; /**< Degree of B-Spline basis function. */
	size_t dim; /**< Dimension of control points (2D => x, y) */
	size_t n_ctrlp; /**< Number of control points. */
	size_t n_knots; /**< Number of knots (n_ctrlp + deg + 1). */
};

/**
 * Stores the private data of a ::tsDeBoorNet.
 */
struct tsDeBoorNetImpl
{
	tsReal u; /**< The evaluated knot value. */
	size_t k; /**< The index [u_k, u_k+1) */
	size_t s; /**< Multiplicity of u_k. */
	size_t h; /**< Number of insertions required to obtain result. */
	size_t dim; /**< Dimension of points. (2D => x, y) */
	size_t n_points; /** Number of points in 'points'. */
};



/******************************************************************************
*                                                                             *
* :: Forward Declarations & Internal Utility Functions                        *
*                                                                             *
******************************************************************************/
void ts_internal_bspline_init(tsBSpline *_spline_)
{
	_spline_->pImpl = NULL;
}

size_t ts_internal_bspline_sof_state(const tsBSpline *spline)
{
	return sizeof(struct tsBSplineImpl) +
	       ts_bspline_sof_control_points(spline) +
	       ts_bspline_sof_knots(spline);
}

tsReal * ts_internal_bspline_access_ctrlp(const tsBSpline *spline)
{
	return (tsReal *) (& spline->pImpl[1]);
}

tsReal * ts_internal_bspline_access_knots(const tsBSpline *spline)
{
	return ts_internal_bspline_access_ctrlp(spline)
		+ ts_bspline_len_control_points(spline);
}

void ts_internal_deboornet_init(tsDeBoorNet *_deBoorNet_)
{
	_deBoorNet_->pImpl = NULL;
}

size_t ts_internal_deboornet_sof_state(const tsDeBoorNet *net)
{
	return sizeof(struct tsDeBoorNetImpl) +
	       ts_deboornet_sof_points(net) +
	       ts_deboornet_sof_result(net);
}

tsReal * ts_internal_deboornet_access_points(const tsDeBoorNet *net)
{
	return (tsReal *) (& net->pImpl[1]);
}

tsReal * ts_internal_deboornet_access_result(const tsDeBoorNet *net)
{
	if (ts_deboornet_num_result(net) == 2) {
		return ts_internal_deboornet_access_points(net);
	} else {
		return ts_internal_deboornet_access_points(net) +
			/* Last point in `points`. */
		       (ts_deboornet_len_points(net) -
			ts_deboornet_dimension(net));
	}
}

tsError ts_internal_bspline_find_u(const tsBSpline *spline, tsReal u,
	size_t *k, size_t *s, tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	const tsReal *knots = ts_internal_bspline_access_knots(spline);
	tsReal min, max;

	*k = *s = 0;
	for (; *k < num_knots; (*k)++) {
		const tsReal uk = knots[*k];
		if (ts_knots_equal(u, uk)) {
			(*s)++;
		} else if (u < uk) {
			break;
		}
	}

	/* keep in mind that currently k is k+1 */
	ts_bspline_domain(spline, &min, &max);
	if (*k <= deg) {
		/* u < u_min */
		TS_RETURN_2(status, TS_U_UNDEFINED,
			   "knot (%f) < min(domain) (%f)", u, min)
	}
	if (*k == num_knots && *s == 0) {
		/* u > u_last */
		TS_RETURN_2(status, TS_U_UNDEFINED,
			   "knot (%f) > max(domain) (%f)", u, max)
	}
	if (*k > num_knots-deg + *s-1)  {
		/* u > u_max */
		TS_RETURN_2(status, TS_U_UNDEFINED,
			   "knot (%f) > max(domain) (%f)", u, max)
	}
	(*k)--; /* k+1 - 1 will never underflow */
	TS_RETURN_SUCCESS(status)
}

tsError ts_internal_bspline_access_ctrlp_at(const tsBSpline *spline,
	size_t index, tsReal **ctrlp, tsStatus *status)
{
	if (index >= ts_bspline_num_control_points(spline)) {
		TS_RETURN_2(status, TS_INDEX_ERROR,
			   "index (%lu) >= num(control_points) (%lu)",
			    (unsigned long) index,
			    (unsigned long) ts_bspline_num_control_points(spline))
	}
	*ctrlp = ts_internal_bspline_access_ctrlp(spline) +
		index * ts_bspline_dimension(spline);
	TS_RETURN_SUCCESS(status)
}

tsError ts_internal_bspline_generate_knots(const tsBSpline *spline,
	tsBSplineType type, tsStatus *status)
{
	const size_t n_knots = ts_bspline_num_knots(spline);
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = ts_bspline_order(spline);
	tsReal fac; /**< Factor used to calculate the knot values. */
	size_t i; /**< Used in for loops. */
	tsReal *knots; /**< Pointer to the knots of \p _result_. */

	/* order >= 1 implies 2*order >= 2 implies n_knots >= 2 */
	if (type == TS_BEZIERS && n_knots % order != 0) {
		TS_RETURN_2(status, TS_NUM_KNOTS,
			   "num(knots) (%lu) %% order (%lu) != 0",
			    (unsigned long) n_knots, (unsigned long) order)
	}

	knots = ts_internal_bspline_access_knots(spline);

	if (type == TS_OPENED) {
		knots[0] = TS_MIN_KNOT_VALUE; /* n_knots >= 2 */
		fac = (TS_MAX_KNOT_VALUE - TS_MIN_KNOT_VALUE)
			/ (n_knots - 1); /* n_knots >= 2 */
		for (i = 1; i < n_knots-1; i++)
			knots[i] = TS_MIN_KNOT_VALUE + i*fac;
		knots[i] = TS_MAX_KNOT_VALUE; /* n_knots >= 2 */
	} else if (type == TS_CLAMPED) {
		/* n_knots >= 2*order == 2*(deg+1) == 2*deg + 2 > 2*deg - 1 */
		fac = (TS_MAX_KNOT_VALUE - TS_MIN_KNOT_VALUE)
			/ (n_knots - 2*deg - 1);
		ts_arr_fill(knots, order, TS_MIN_KNOT_VALUE);
		for (i = order ;i < n_knots-order; i++)
			knots[i] = TS_MIN_KNOT_VALUE + (i-deg)*fac;
		ts_arr_fill(knots + i, order, TS_MAX_KNOT_VALUE);
	} else if (type == TS_BEZIERS) {
		/* n_knots >= 2*order implies n_knots/order >= 2 */
		fac = (TS_MAX_KNOT_VALUE - TS_MIN_KNOT_VALUE)
			/ (n_knots/order - 1);
		ts_arr_fill(knots, order, TS_MIN_KNOT_VALUE);
		for (i = order; i < n_knots-order; i += order)
			ts_arr_fill(knots + i, order,
				    TS_MIN_KNOT_VALUE + (i/order)*fac);
		ts_arr_fill(knots + i, order, TS_MAX_KNOT_VALUE);
	}
	TS_RETURN_SUCCESS(status)
}

tsError ts_internal_bspline_resize(const tsBSpline *spline, int n, int back,
	tsBSpline *_resized_, tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t sof_real = sizeof(tsReal);

	const size_t num_ctrlp = ts_bspline_num_control_points(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	const size_t nnum_ctrlp = num_ctrlp + n; /**< New length of ctrlp. */
	const size_t nnum_knots = num_knots + n; /**< New length of knots. */
	const size_t min_num_ctrlp_vec = n < 0 ? nnum_ctrlp : num_ctrlp;
	const size_t min_num_knots_vec = n < 0 ? nnum_knots : num_knots;

	const size_t sof_min_num_ctrlp = min_num_ctrlp_vec * dim * sof_real;
	const size_t sof_min_num_knots = min_num_knots_vec * sof_real;

	tsBSpline tmp; /**< Temporarily stores the result. */
	const tsReal* from_ctrlp = ts_internal_bspline_access_ctrlp(spline);
	const tsReal* from_knots = ts_internal_bspline_access_knots(spline);
	tsReal* to_ctrlp = NULL; /**< Pointer to the control points of tmp. */
	tsReal* to_knots = NULL; /**< Pointer to the knots of tmp. */

	tsError err;

	if (n == 0)
		return ts_bspline_copy(spline, _resized_, status);

	INIT_OUT_BSPLINE(spline, _resized_)
	TS_CALL_ROE(err, ts_bspline_new(
		nnum_ctrlp, dim, deg, TS_OPENED, &tmp, status))
	to_ctrlp = ts_internal_bspline_access_ctrlp(&tmp);
	to_knots = ts_internal_bspline_access_knots(&tmp);

	/* Copy control points and knots. */
	if (!back && n < 0) {
		memcpy(to_ctrlp, from_ctrlp + n*dim, sof_min_num_ctrlp);
		memcpy(to_knots, from_knots + n    , sof_min_num_knots);
	} else if (!back && n > 0) {
		memcpy(to_ctrlp + n*dim, from_ctrlp, sof_min_num_ctrlp);
		memcpy(to_knots + n    , from_knots, sof_min_num_knots);
	} else {
		/* n != 0 implies back == true */
		memcpy(to_ctrlp, from_ctrlp, sof_min_num_ctrlp);
		memcpy(to_knots, from_knots, sof_min_num_knots);
	}

	if (spline == _resized_)
		ts_bspline_free(_resized_);
	ts_bspline_move(&tmp, _resized_);
	TS_RETURN_SUCCESS(status)
}



/******************************************************************************
*                                                                             *
* :: Field Access Functions                                                   *
*                                                                             *
******************************************************************************/
size_t ts_bspline_degree(const tsBSpline *spline)
{
	return spline->pImpl->deg;
}

tsError ts_bspline_set_degree(tsBSpline *spline, size_t deg, tsStatus *status)
{
	if (deg >= ts_bspline_num_control_points(spline)) {
		TS_RETURN_2(status, TS_DEG_GE_NCTRLP,
			   "degree (%lu) >= num(control_points) (%lu)",
			    (unsigned long) deg,
			    (unsigned long) ts_bspline_num_control_points(spline))
	}
	spline->pImpl->deg = deg;
	TS_RETURN_SUCCESS(status)
}

size_t ts_bspline_order(const tsBSpline *spline)
{
	return ts_bspline_degree(spline) + 1;
}

tsError ts_bspline_set_order(tsBSpline *spline, size_t order, tsStatus *status)
{
	if (order == 0 || order > ts_bspline_num_control_points(spline)) {
		TS_RETURN_2(status, TS_DEG_GE_NCTRLP,
			   "order (%lu) > num(control_points) (%lu)",
			    (unsigned long) order,
			    (unsigned long) ts_bspline_num_control_points(spline))
	}
	return ts_bspline_set_degree(spline, order - 1, status);
}

size_t ts_bspline_dimension(const tsBSpline *spline)
{
	return spline->pImpl->dim;
}

tsError ts_bspline_set_dimension(tsBSpline *spline, size_t dim,
	tsStatus *status)
{
	if (dim == 0)
		TS_RETURN_0(status, TS_DIM_ZERO, "unsupported dimension: 0")
	if (ts_bspline_len_control_points(spline) % dim != 0) {
		TS_RETURN_2(status, TS_LCTRLP_DIM_MISMATCH,
			   "len(control_points) (%lu) %% dimension (%lu) != 0",
			    (unsigned long) ts_bspline_len_control_points(spline),
			    (unsigned long) dim)
	}
	spline->pImpl->dim = dim;
	TS_RETURN_SUCCESS(status)
}

size_t ts_bspline_len_control_points(const tsBSpline *spline)
{
	return ts_bspline_num_control_points(spline) *
	       ts_bspline_dimension(spline);
}

size_t ts_bspline_num_control_points(const tsBSpline *spline)
{
	return spline->pImpl->n_ctrlp;
}

size_t ts_bspline_sof_control_points(const tsBSpline *spline)
{
	return ts_bspline_len_control_points(spline) * sizeof(tsReal);
}

tsError ts_bspline_control_points(const tsBSpline *spline, tsReal **ctrlp,
	tsStatus *status)
{
	const size_t size = ts_bspline_sof_control_points(spline);
	*ctrlp = (tsReal*) malloc(size);
	if (!*ctrlp)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	memcpy(*ctrlp, ts_internal_bspline_access_ctrlp(spline), size);
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_control_point_at(const tsBSpline *spline, size_t index,
	tsReal **ctrlp, tsStatus *status)
{
	tsReal *from;
	size_t size;
	tsError err;
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_internal_bspline_access_ctrlp_at(
			spline, index, &from, status))
		size = ts_bspline_dimension(spline) * sizeof(tsReal);
		*ctrlp = (tsReal*) malloc(size);
		if (!*ctrlp) {
			TS_THROW_0(try, err, status, TS_MALLOC,
				   "out of memory")
		}
		memcpy(*ctrlp, from, size);
	TS_CATCH(err)
		*ctrlp = NULL;
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_set_control_points(tsBSpline *spline, const tsReal *ctrlp,
	tsStatus *status)
{
	const size_t size = ts_bspline_sof_control_points(spline);
	memmove(ts_internal_bspline_access_ctrlp(spline), ctrlp, size);
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_set_control_point_at(tsBSpline *spline, size_t index,
	const tsReal *ctrlp, tsStatus *status)
{
	tsReal *to;
	size_t size;
	tsError err;
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_internal_bspline_access_ctrlp_at(
			spline, index, &to, status))
		size = ts_bspline_dimension(spline) * sizeof(tsReal);
		memcpy(to, ctrlp, size);
	TS_END_TRY_RETURN(err)
}

size_t ts_bspline_num_knots(const tsBSpline *spline)
{
	return spline->pImpl->n_knots;
}

size_t ts_bspline_sof_knots(const tsBSpline *spline)
{
	return ts_bspline_num_knots(spline) * sizeof(tsReal);
}

tsError ts_bspline_knots(const tsBSpline *spline, tsReal **knots,
	tsStatus *status)
{
	const size_t size = ts_bspline_sof_knots(spline);
	*knots = (tsReal*) malloc(size);
	if (!*knots)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	memcpy(*knots, ts_internal_bspline_access_knots(spline), size);
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_set_knots(tsBSpline *spline, const tsReal *knots,
	tsStatus *status)
{
	size_t num_knots, order, idx, mult;
	tsReal lst_knot, knot, *to_knots;
	num_knots = ts_bspline_num_knots(spline);
	order = ts_bspline_order(spline);
	lst_knot = knots[0];
	mult = 1;
	for (idx = 1; idx < num_knots; idx++) {
		knot = knots[idx];
		if (ts_knots_equal(lst_knot, knot)) {
			mult++;
		} else if (lst_knot > knot) {
			TS_RETURN_1(status, TS_KNOTS_DECR,
				   "decreasing knot vector at index: %lu",
				    (unsigned long) idx)
		} else {
			mult = 0;
		}
		if (mult > order) {
			TS_RETURN_3(status, TS_MULTIPLICITY,
				   "mult(%f) (%lu) > order (%lu)",
				   knot, (unsigned long) mult,
				   (unsigned long) order)
		}
		lst_knot = knot;
	}
	to_knots = ts_internal_bspline_access_knots(spline);
	for (idx = 0; idx < num_knots; idx++) {
		to_knots[idx] = (knots[idx] - knots[0]) /
			(knots[num_knots - 1] - knots[0]);
		to_knots[idx] *= TS_MAX_KNOT_VALUE - TS_MIN_KNOT_VALUE;
		to_knots[idx] += TS_MIN_KNOT_VALUE;
		/* Handle floating point precision. */
		to_knots[idx] = to_knots[idx] < TS_MIN_KNOT_VALUE
			? TS_MIN_KNOT_VALUE : to_knots[idx];
		to_knots[idx] = to_knots[idx] > TS_MAX_KNOT_VALUE
			? TS_MAX_KNOT_VALUE : to_knots[idx];
	}
	TS_RETURN_SUCCESS(status)
}

/* ------------------------------------------------------------------------- */

tsReal ts_deboornet_knot(const tsDeBoorNet *net)
{
	return net->pImpl->u;
}

size_t ts_deboornet_index(const tsDeBoorNet *net)
{
	return net->pImpl->k;
}

size_t ts_deboornet_multiplicity(const tsDeBoorNet *net)
{
	return net->pImpl->s;
}

size_t ts_deboornet_num_insertions(const tsDeBoorNet *net)
{
	return net->pImpl->h;
}

size_t ts_deboornet_dimension(const tsDeBoorNet *net)
{
	return net->pImpl->dim;
}

size_t ts_deboornet_len_points(const tsDeBoorNet *net)
{
	return ts_deboornet_num_points(net) * ts_deboornet_dimension(net);
}

size_t ts_deboornet_num_points(const tsDeBoorNet *net)
{
	return net->pImpl->n_points;
}

size_t ts_deboornet_sof_points(const tsDeBoorNet *net)
{
	return ts_deboornet_len_points(net) * sizeof(tsReal);
}

tsError ts_deboornet_points(const tsDeBoorNet *net, tsReal **points,
	tsStatus *status)
{
	const size_t size = ts_deboornet_sof_points(net);
	*points = (tsReal*) malloc(size);
	if (!*points)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	memcpy(*points, ts_internal_deboornet_access_points(net), size);
	TS_RETURN_SUCCESS(status)
}

size_t ts_deboornet_len_result(const tsDeBoorNet *net)
{
	return ts_deboornet_num_result(net) * ts_deboornet_dimension(net);
}

size_t ts_deboornet_num_result(const tsDeBoorNet *net)
{
	return ts_deboornet_num_points(net) == 2 ? 2 : 1;
}

size_t ts_deboornet_sof_result(const tsDeBoorNet *net)
{
	return ts_deboornet_len_result(net) * sizeof(tsReal);
}

tsError ts_deboornet_result(const tsDeBoorNet *net, tsReal **result,
	tsStatus *status)
{
	const size_t size = ts_deboornet_sof_result(net);
	*result = (tsReal*) malloc(size);
	if (!*result)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	memcpy(*result, ts_internal_deboornet_access_result(net), size);
	TS_RETURN_SUCCESS(status)
}



/******************************************************************************
*                                                                             *
* :: Constructors, Destructors, Copy, and Move Functions                      *
*                                                                             *
******************************************************************************/
tsBSpline ts_bspline_init()
{
	tsBSpline spline;
	spline.pImpl = NULL;
	return spline;
}

tsError ts_bspline_new(size_t num_control_points, size_t dimension,
	size_t degree, tsBSplineType type, tsBSpline *spline, tsStatus *status)
{
	const size_t order = degree + 1;
	const size_t num_knots = num_control_points + order;
	const size_t len_ctrlp = num_control_points * dimension;

	const size_t sof_real = sizeof(tsReal);
	const size_t sof_impl = sizeof(struct tsBSplineImpl);
	const size_t sof_ctrlp_vec = len_ctrlp * sof_real;
	const size_t sof_knots_vec = num_knots * sof_real;
	const size_t sof_spline = sof_impl + sof_ctrlp_vec + sof_knots_vec;
	tsError err;

	ts_internal_bspline_init(spline);

	if (dimension < 1) {
		TS_RETURN_0(status, TS_DIM_ZERO, "unsupported dimension: 0")
	}
	if (num_knots > TS_MAX_NUM_KNOTS) {
		TS_RETURN_2(status, TS_NUM_KNOTS,
			   "unsupported number of knots: %lu > %i",
			    (unsigned long) num_knots, TS_MAX_NUM_KNOTS)
	}
	if (degree >= num_control_points) {
		TS_RETURN_2(status, TS_DEG_GE_NCTRLP,
			   "degree (%lu) >= num(control_points) (%lu)",
			    (unsigned long) degree,
			    (unsigned long) num_control_points)
	}

	spline->pImpl = (struct tsBSplineImpl *) malloc(sof_spline);
	if (!spline->pImpl)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")

	spline->pImpl->deg = degree;
	spline->pImpl->dim = dimension;
	spline->pImpl->n_ctrlp = num_control_points;
	spline->pImpl->n_knots = num_knots;

	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_internal_bspline_generate_knots(
			spline, type, status))
	TS_CATCH(err)
		ts_bspline_free(spline);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_copy(const tsBSpline *src, tsBSpline *dest,
	tsStatus *status)
{
	size_t size;
	if (src == dest)
		TS_RETURN_SUCCESS(status)
	ts_internal_bspline_init(dest);
	size = ts_internal_bspline_sof_state(src);
	dest->pImpl = (struct tsBSplineImpl *) malloc(size);
	if (!dest->pImpl)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	memcpy(dest->pImpl, src->pImpl, size);
	TS_RETURN_SUCCESS(status)
}

void ts_bspline_move(tsBSpline *src, tsBSpline *dest)
{
	if (src == dest)
		return;
	dest->pImpl = src->pImpl;
	ts_internal_bspline_init(src);
}

void ts_bspline_free(tsBSpline *spline)
{
	if (spline->pImpl)
		free(spline->pImpl);
	ts_internal_bspline_init(spline);
}

/* ------------------------------------------------------------------------- */

tsDeBoorNet ts_deboornet_init()
{
	tsDeBoorNet net;
	net.pImpl = NULL;
	return net;
}

tsError ts_internal_deboornet_new(const tsBSpline *spline,
	tsDeBoorNet *_deBoorNet_, tsStatus *status)
{
	const size_t dim = ts_bspline_dimension(spline);
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = ts_bspline_order(spline);
	const size_t num_points = (size_t)(order * (order+1) * 0.5f);
	/* Handle case order == 1 which generates too few points. */
	const size_t fixed_num_points = num_points < 2 ? 2 : num_points;

	const size_t sof_real = sizeof(tsReal);
	const size_t sof_impl = sizeof(struct tsDeBoorNetImpl);
	const size_t sof_points_vec = fixed_num_points * dim * sof_real;
	const size_t sof_net = sof_impl * sof_points_vec;

	_deBoorNet_->pImpl = (struct tsDeBoorNetImpl *) malloc(sof_net);
	if (!_deBoorNet_->pImpl)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")

	_deBoorNet_->pImpl->u = 0.f;
	_deBoorNet_->pImpl->k = 0;
	_deBoorNet_->pImpl->s = 0;
	_deBoorNet_->pImpl->h = deg;
	_deBoorNet_->pImpl->dim = dim;
	_deBoorNet_->pImpl->n_points = fixed_num_points;
	TS_RETURN_SUCCESS(status)
}

void ts_deboornet_free(tsDeBoorNet *net)
{
	if (net->pImpl)
		free(net->pImpl);
	ts_internal_deboornet_init(net);
}

tsError ts_deboornet_copy(const tsDeBoorNet *src, tsDeBoorNet *dest,
	tsStatus *status)
{
	size_t size;
	if (src == dest)
		TS_RETURN_SUCCESS(status)
	ts_internal_deboornet_init(dest);
	size = ts_internal_deboornet_sof_state(src);
	dest->pImpl = (struct tsDeBoorNetImpl *) malloc(size);
	if (!dest->pImpl)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	memcpy(dest->pImpl, src->pImpl, size);
	TS_RETURN_SUCCESS(status)
}

void ts_deboornet_move(tsDeBoorNet *src, tsDeBoorNet *dest)
{
	if (src == dest)
		return;
	dest->pImpl = src->pImpl;
	ts_internal_deboornet_init(src);
}



/******************************************************************************
*                                                                             *
* :: Interpolation and Approximation Functions                                *
*                                                                             *
******************************************************************************/
tsError ts_internal_bspline_thomas_algorithm(const tsReal *points, size_t n,
	size_t dim, tsReal *_result_, tsStatus *status)
{
	const size_t sof_real = sizeof(tsReal);
	const size_t sof_ctrlp = dim * sof_real;
	tsReal* m;      /* Array of weights. */
	size_t len_m;   /* Length of m. */
	size_t lst;     /* Index of the last control point in \p points. */
	size_t i, d;    /* Used in for loops. */
	size_t j, k, l; /* Used as temporary indices. */

	/* input validation */
	if (dim == 0)
		TS_RETURN_0(status, TS_DIM_ZERO, "unsupported dimension: 0")
	if (n == 0)
		TS_RETURN_0(status, TS_DEG_GE_NCTRLP, "num(points) == 0")
	if (n <= 2) {
		memcpy(_result_, points, n * sof_ctrlp);
		TS_RETURN_SUCCESS(status)
	}

	/* In the following n >= 3 applies... */
	len_m = n-2; /* ... len_m >= 1 */
	lst = (n-1)*dim; /* ... lst >= 2*dim */

	/* m_0 = 1/4, m_{k+1} = 1/(4-m_k), for k = 0,...,n-2 */
	m = (tsReal*) malloc(len_m * sof_real);
	if (m == NULL)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	m[0] = 0.25f;
	for (i = 1; i < len_m; i++)
		m[i] = 1.f/(4 - m[i-1]);

	/* forward sweep */
	ts_arr_fill(_result_, n*dim, 0.f);
	memcpy(_result_, points, sof_ctrlp);
	memcpy(_result_+lst, points+lst, sof_ctrlp);
	for (d = 0; d < dim; d++) {
		k = dim+d;
		_result_[k] = 6*points[k];
		_result_[k] -= points[d];
	}
	for (i = 2; i <= n-2; i++) {
		for (d = 0; d < dim; d++) {
			j = (i-1)*dim+d;
			k = i*dim+d;
			l = (i+1)*dim+d;
			_result_[k] = 6*points[k];
			_result_[k] -= _result_[l];
			_result_[k] -= m[i-2]*_result_[j]; /* i >= 2 */
		}
	}

	/* back substitution */
	if (n > 3)
		ts_arr_fill(_result_+lst, dim, 0.f);
	for (i = n-2; i >= 1; i--) {
		for (d = 0; d < dim; d++) {
			k = i*dim+d;
			l = (i+1)*dim+d;
			/* The following line is the reason why it's important
			 * to not fill \p _result_ with 0 if n = 3. On the
			 * other hand, if n > 3 subtracting 0 is exactly what
			 * we want. */
			_result_[k] -= _result_[l];
			_result_[k] *= m[i-1]; /* i >= 1 */
		}
	}
	if (n > 3)
		memcpy(_result_+lst, points+lst, sof_ctrlp);

	/* we are done */
	free(m);
	TS_RETURN_SUCCESS(status)
}

tsError ts_internal_relaxed_uniform_cubic_bspline(const tsReal *points,
	size_t n, size_t dim, tsBSpline *_spline_, tsStatus *status)
{
	const size_t order = 4;    /**< Order of spline to interpolate. */
	const tsReal as = 1.f/6.f; /**< The value 'a sixth'. */
	const tsReal at = 1.f/3.f; /**< The value 'a third'. */
	const tsReal tt = 2.f/3.f; /**< The value 'two third'. */
	size_t sof_ctrlp;          /**< Size of a single control point. */
	const tsReal* b = points;  /**< Array of the b values. */
	tsReal* s;                 /**< Array of the s values. */
	size_t i, d;               /**< Used in for loops */
	size_t j, k, l;            /**< Used as temporary indices. */
	tsReal *ctrlp; /**< Pointer to the control points of \p _spline_. */
	tsError err;

	/* input validation */
	if (dim == 0)
		TS_RETURN_0(status, TS_DIM_ZERO, "unsupported dimension: 0")
	if (n <= 1) {
		TS_RETURN_1(status, TS_DEG_GE_NCTRLP,
			   "num(points) (%lu) <= 1", (unsigned long) n)
	}
	/* in the following n >= 2 applies */

	sof_ctrlp = dim * sizeof(tsReal); /* dim > 0 implies sof_ctrlp > 0 */

	s = NULL;
	TS_TRY(try, err, status)
		/* n >= 2 implies n-1 >= 1 implies (n-1)*4 >= 4 */
		TS_CALL(try, err, ts_bspline_new(
			(n-1)*4, dim, order-1, TS_BEZIERS, _spline_, status));
		ctrlp = ts_internal_bspline_access_ctrlp(_spline_);

		s = (tsReal*) malloc(n * sof_ctrlp);
		if (!s) {
			TS_THROW_0(try, err, status, TS_MALLOC,
				   "out of memory")
		}

		/* set s_0 to b_0 and s_n = b_n */
		memcpy(s, b, sof_ctrlp);
		memcpy(s + (n-1)*dim, b + (n-1)*dim, sof_ctrlp);

		/* set s_i = 1/6*b_i + 2/3*b_{i-1} + 1/6*b_{i+1}*/
		for (i = 1; i < n-1; i++) {
			for (d = 0; d < dim; d++) {
				j = (i-1)*dim+d;
				k = i*dim+d;
				l = (i+1)*dim+d;
				s[k] = as * b[j];
				s[k] += tt * b[k];
				s[k] += as * b[l];
			}
		}

		/* create beziers from b and s */
		for (i = 0; i < n-1; i++) {
			for (d = 0; d < dim; d++) {
				j = i*dim+d;
				k = i*4*dim+d;
				l = (i+1)*dim+d;
				ctrlp[k] = s[j];
				ctrlp[k+dim] = tt*b[j] + at*b[l];
				ctrlp[k+2*dim] = at*b[j] + tt*b[l];
				ctrlp[k+3*dim] = s[l];
			}
		}
	TS_CATCH(err)
		ts_bspline_free(_spline_);
	TS_FINALLY
		if (s)
			free(s);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_interpolate_cubic(const tsReal *points, size_t n,
	size_t dim, tsBSpline *_spline_, tsStatus *status)
{
	tsError err;
	tsReal* thomas;
	ts_internal_bspline_init(_spline_);
	thomas = NULL;
	TS_TRY(try, err, status)
		thomas = (tsReal*) malloc(n*dim*sizeof(tsReal));
		if (!thomas) {
			TS_THROW_0(try, err, status, TS_MALLOC,
				   "out of memory")
		}
		TS_CALL(try, err, ts_internal_bspline_thomas_algorithm(
			points, n, dim, thomas, status))
		TS_CALL(try, err, ts_internal_relaxed_uniform_cubic_bspline(
			thomas, n, dim, _spline_, status))
	TS_CATCH(err)
		ts_bspline_free(_spline_);
	TS_FINALLY
		free(thomas);
	TS_END_TRY_RETURN(err)
}



/******************************************************************************
*                                                                             *
* :: Query Functions                                                          *
*                                                                             *
******************************************************************************/
tsError ts_bspline_eval(const tsBSpline *spline, tsReal u,
	tsDeBoorNet *_deBoorNet_, tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = ts_bspline_order(spline);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	const size_t sof_ctrlp = dim * sizeof(tsReal);

	const tsReal *ctrlp = ts_internal_bspline_access_ctrlp(spline);
	const tsReal *knots = ts_internal_bspline_access_knots(spline);
	tsReal *points;  /**< Pointer to the points of \p _deBoorNet_. */

	size_t k;        /**< Index of \p u. */
	size_t s;        /**< Multiplicity of \p u. */

	tsReal uk;       /**< The actual used u. */
	size_t from;     /**< Offset used to copy values. */
	size_t fst;      /**< First affected control point, inclusive. */
	size_t lst;      /**< Last affected control point, inclusive. */
	size_t N;        /**< Number of affected control points. */

	/* The following indices are used to create the DeBoor net. */
	size_t lidx;     /**< Current left index. */
	size_t ridx;     /**< Current right index. */
	size_t tidx;     /**< Current to index. */
	size_t r, i, d;  /**< Used in for loop. */
	tsReal ui;       /**< Knot value at index i. */
	tsReal a, a_hat; /**< Weighting factors of control points. */

	tsError err;

	/* 1. Initialize the net and find index k such that u is in between
	 *    [u_k, u_k+1).
	 * 2. Setup already known values.
	 * 3. Decide by multiplicity of u how to calculate point P(u). */

	/* 1. */
	ts_internal_deboornet_init(_deBoorNet_);
	k = s = 0;
	points = NULL;
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_internal_deboornet_new(
			spline, _deBoorNet_, status))
		points = ts_internal_deboornet_access_points(_deBoorNet_);
		TS_CALL(try, err, ts_internal_bspline_find_u(
			spline, u, &k, &s, status))
	TS_CATCH(err)
		ts_deboornet_free(_deBoorNet_);
 		return err;
	TS_END_TRY

	/* 2. */
	uk = knots[k];          /* Ensures that with any precision of  */
	_deBoorNet_->pImpl->u = /* tsReal the knot vector stays valid. */
		ts_knots_equal(u, uk) ? uk : u;
	_deBoorNet_->pImpl->k = k;
	_deBoorNet_->pImpl->s = s;
        _deBoorNet_->pImpl->h = deg < s ? 0 : deg-s; /* prevent underflow */

	/* 3. (by 1. s <= order)
	 *
	 * 3a) Check for s = order.
	 *     Take the two points k-s and k-s + 1. If one of
	 *     them doesn't exist, take only the other.
	 * 3b) Use de boor algorithm to find point P(u). */
	if (s == order) {
		/* only one of the two control points exists */
		if (k == deg || /* only the first */
		    k == num_knots - 1) { /* only the last */
			from = k == deg ? 0 : (k-s) * dim;
			_deBoorNet_->pImpl->n_points = 1;
			memcpy(points, ctrlp + from, sof_ctrlp);
		} else {
			from = (k-s) * dim;
			_deBoorNet_->pImpl->n_points = 2;
			memcpy(points, ctrlp + from, 2 * sof_ctrlp);
		}
	} else { /* by 3a) s <= deg (order = deg+1) */
		fst = k-deg; /* by 1. k >= deg */
		lst = k-s; /* s <= deg <= k */
		N = lst-fst + 1; /* lst <= fst implies N >= 1 */

		_deBoorNet_->pImpl->n_points = (size_t)(N * (N+1) * 0.5f);

		/* copy initial values to output */
		memcpy(points, ctrlp + fst*dim, N * sof_ctrlp);

		lidx = 0;
		ridx = dim;
		tidx = N*dim; /* N >= 1 implies tidx > 0 */
		r = 1;
		for (;r <= ts_deboornet_num_insertions(_deBoorNet_); r++) {
			i = fst + r;
			for (; i <= lst; i++) {
				ui = knots[i];
				a = (ts_deboornet_knot(_deBoorNet_) - ui) /
					(knots[i+deg-r+1] - ui);
				a_hat = 1.f-a;

				for (d = 0; d < dim; d++) {
					points[tidx++] =
						a_hat * points[lidx++] +
						a     * points[ridx++];
				}
			}
			lidx += dim;
			ridx += dim;
		}
	}
	TS_RETURN_SUCCESS(status)
}

void ts_bspline_domain(const tsBSpline *spline, tsReal *min, tsReal *max)
{
	*min = ts_internal_bspline_access_knots(spline)
		[ts_bspline_degree(spline)];
	*max = ts_internal_bspline_access_knots(spline)
		[ts_bspline_num_knots(spline) - ts_bspline_order(spline)];
}

tsError ts_bspline_is_closed(const tsBSpline *spline, tsReal epsilon,
	int *closed, tsStatus *status)
{
	tsReal min, max;
	tsDeBoorNet first, last;
	tsError err;
	ts_bspline_domain(spline, &min, &max);
	ts_internal_deboornet_init(&first);
	ts_internal_deboornet_init(&last);
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_bspline_eval(spline, min, &first, status))
		TS_CALL(try, err, ts_bspline_eval(spline, max, &last, status))
		*closed = ts_distance(
				ts_internal_deboornet_access_result(&first),
				ts_internal_deboornet_access_result(&last),
				ts_bspline_dimension(spline)) <= epsilon ? 1 : 0;
	TS_FINALLY
		ts_deboornet_free(&first);
		ts_deboornet_free(&last);
	TS_END_TRY_RETURN(err)
}



/******************************************************************************
*                                                                             *
* :: Transformation Functions                                                 *
*                                                                             *
******************************************************************************/
tsError ts_bspline_derive(const tsBSpline *spline, size_t n,
	tsBSpline *_derivative_, tsStatus *status)
{
	const size_t sof_real = sizeof(tsReal);
	const size_t dim = ts_bspline_dimension(spline);
	size_t deg = ts_bspline_degree(spline);
	size_t num_ctrlp = ts_bspline_num_control_points(spline);
	size_t num_knots = ts_bspline_num_knots(spline);

	tsBSpline worker; /**< Stores intermediate results. */
	tsReal* ctrlp;    /**< Pointer to the control points of worker. */
	tsReal* knots;    /**< Pointer to the knots of worker. */

	size_t m, i, j, k, l; /**< Used in for loops. */

	tsBSpline swap; /**< Used to swap worker and _derivative_. */
	tsError err;

	INIT_OUT_BSPLINE(spline, _derivative_)
	TS_CALL_ROE(err, ts_bspline_copy(spline, &worker, status))
	ctrlp = ts_internal_bspline_access_ctrlp(&worker);
	knots = ts_internal_bspline_access_knots(&worker);

	TS_TRY(try, err, status)
		for (m = 1; m <= n; m++) { /* from 1st to n'th derivative */
			if (deg == 0) {
				ts_arr_fill(ctrlp, num_ctrlp * dim, 0.f);
				break;
			} else {
				for (i = 0; i < num_ctrlp-1; i++) {
					for (j = 0; j < dim; j++) {
						if (ts_knots_equal(knots[i+deg+1], knots[i+1])) {
							TS_THROW_0(try, err, status,
								   TS_UNDERIVABLE,
								   "unable to derive spline")
						} else {
							k = i    *dim + j;
							l = (i+1)*dim + j;
							ctrlp[k] = ctrlp[l] - ctrlp[k];
							ctrlp[k] *= deg;
							ctrlp[k] /= knots[i+deg+1] - knots[i+1];
						}
					}
				}
				deg       -= 1;
				num_ctrlp -= 1;
				num_knots -= 2;
				knots     += 1;
			}
		}
		TS_CALL(try, err, ts_bspline_new(
			num_ctrlp, dim, deg, TS_OPENED, &swap, status))
		memcpy(ts_internal_bspline_access_ctrlp(&swap), ctrlp,
		       num_ctrlp * dim * sof_real);
		memcpy(ts_internal_bspline_access_knots(&swap), knots,
		       num_knots * sof_real);
		if (spline == _derivative_)
			ts_bspline_free(_derivative_);
		ts_bspline_move(&swap, _derivative_);
	TS_FINALLY
		ts_bspline_free(&worker);
	TS_END_TRY_RETURN(err)
}

tsError ts_internal_bspline_insert_knot(const tsBSpline *spline,
	const tsDeBoorNet *deBoorNet, size_t n, tsBSpline *_result_,
	tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t k = ts_deboornet_index(deBoorNet);
	const size_t s = ts_deboornet_multiplicity(deBoorNet);
	const size_t sof_real = sizeof(tsReal);
	const size_t sof_ctrlp = dim * sof_real;

	size_t N;     /**< Number of affected control points. */
	tsReal* from; /**< Pointer to copy the values from. */
	tsReal* to;   /**< Pointer to copy the values to. */
	int stride;   /**< Stride of the next pointer to copy. */
	size_t i;     /**< Used in for loops. */

	tsReal *ctrlp_spline, *ctrlp_result;
	tsReal *knots_spline, *knots_result;
	size_t num_ctrlp_result;
	size_t num_knots_result;

	tsError err;

	if (n == 0 || s+n > ts_bspline_order(spline))
		return ts_bspline_copy(spline, _result_, status);

	TS_CALL_ROE(err, ts_internal_bspline_resize(
		spline, (int)n, 1, _result_, status));
	ctrlp_spline = ts_internal_bspline_access_ctrlp(spline);
	knots_spline = ts_internal_bspline_access_knots(spline);
	ctrlp_result = ts_internal_bspline_access_ctrlp(_result_);
	knots_result = ts_internal_bspline_access_knots(_result_);
	num_ctrlp_result = ts_bspline_num_control_points(_result_);
	num_knots_result = ts_bspline_num_knots(_result_);

	/* n > 0 implies s <= deg implies a regular evaluation implies h+1 is
	 * valid. */
	N = ts_deboornet_num_insertions(deBoorNet) + 1;

	/* 1. Copy all necessary control points and knots from
	 *    the original B-Spline.
	 * 2. Copy all necessary control points and knots from
	 *    the de Boor net. */

	/* 1.
	 *
	 * a) Copy left hand side control points from original b-spline.
	 * b) Copy right hand side control points from original b-spline.
	 * c) Copy left hand side knots from original b-spline.
	 * d) Copy right hand side knots form original b-spline. */
	/* copy control points */
	memmove(ctrlp_result, ctrlp_spline, (k-deg) * sof_ctrlp);      /* a) */
	from = (tsReal *) ctrlp_spline + dim*(k-deg+N);
	/* n >= 0 implies to >= from */
	to = ctrlp_result + dim*(k-deg+N+n);
	memmove(to, from, (num_ctrlp_result-n-(k-deg+N)) * sof_ctrlp); /* b) */
	/* copy knots */
	memmove(knots_result, knots_spline, (k+1) * sof_real);         /* c) */
	from = (tsReal *) knots_spline + k+1;
	/* n >= 0 implies to >= from */
	to = knots_result + k+1+n;
	memmove(to, from, (num_knots_result-n-(k+1)) * sof_real);      /* d) */

	/* 2.
	 *
	 * a) Copy left hand side control points from de boor net.
	 * b) Copy middle part control points from de boor net.
	 * c) Copy right hand side control points from de boor net.
	 * d) Insert knots with u_k. */
	from = ts_internal_deboornet_access_points(deBoorNet);
	to = ctrlp_result + (k-deg)*dim;
	stride = (int)(N*dim);

	/* copy control points */
	for (i = 0; i < n; i++) {                                      /* a) */
		memcpy(to, from, sof_ctrlp);
		from += stride;
		to += dim;
		stride -= (int)dim;
	}
	memcpy(to, from, (N-n) * sof_ctrlp);                           /* b) */

	from -= dim;
	to += (N-n)*dim;
	/* N = h+1 with h = deg-s (ts_internal_bspline_eval) implies
	 * N = deg-s+1 = order-s. n <= order-s implies
	 * N-n+1 >= order-s - order-s + 1 = 1. Thus, -(int)(N-n+1) <= -1. */
	stride = -(int)(N-n+1) * (int)dim;

	for (i = 0; i < n; i++) {                                      /* c) */
		memcpy(to, from, sof_ctrlp);
		from += stride;
		stride -= (int)dim;
		to += dim;
	}
	/* copy knots */
	to = knots_result + k+1;
	for (i = 0; i < n; i++) {                                      /* d) */
		*to = ts_deboornet_knot(deBoorNet);
		to++;
	}
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_insert_knot(const tsBSpline *spline, tsReal u, size_t n,
	tsBSpline *_result_, size_t* k, tsStatus *status)
{
	tsDeBoorNet net;
	tsError err;
	INIT_OUT_BSPLINE(spline, _result_)
	ts_internal_deboornet_init(&net);
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_bspline_eval(spline, u, &net, status))
		TS_CALL(try, err, ts_internal_bspline_insert_knot(
			spline, &net, n, _result_, status))
		ts_deboornet_free(&net);
		TS_CALL(try, err, ts_bspline_eval(_result_, u, &net, status))
		*k = ts_deboornet_index(&net);
	TS_CATCH(err)
		*k = 0;
	TS_FINALLY
		ts_deboornet_free(&net);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_split(const tsBSpline *spline, tsReal u, tsBSpline *_split_,
	size_t* k, tsStatus *status)
{
	tsDeBoorNet net;
	tsError err;
	INIT_OUT_BSPLINE(spline, _split_)
	ts_internal_deboornet_init(&net);
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_bspline_eval(spline, u, &net, status))
		if (ts_deboornet_multiplicity(&net)
				== ts_bspline_order(spline)) {
			TS_CALL(try, err, ts_bspline_copy(
				spline, _split_, status))
			*k = ts_deboornet_index(&net);
		} else {
			TS_CALL(try, err, ts_internal_bspline_insert_knot(
				spline, &net,
				ts_deboornet_num_insertions(&net) + 1, _split_,
				status))
			*k = ts_deboornet_index(&net) +
				ts_deboornet_num_insertions(&net) + 1;
		}
	TS_CATCH(err)
		*k = 0;
	TS_FINALLY
		ts_deboornet_free(&net);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_buckle(const tsBSpline *spline, tsReal b,
	tsBSpline *_buckled_, tsStatus *status)
{
	const tsReal b_hat  = 1.f-b; /**< The straightening factor. */
	const size_t dim = ts_bspline_dimension(spline);
	const size_t N = ts_bspline_num_control_points(spline);
	const tsReal* p0 = ts_internal_bspline_access_ctrlp(spline);
	const tsReal* pn_1 = p0 + (N-1)*dim;

	tsReal *ctrlp; /**< Pointer to the control points of \p _buckled_. */
	size_t i, d; /**< Used in for loops. */
	tsError err;

	TS_CALL_ROE(err, ts_bspline_copy(spline, _buckled_, status))
	ctrlp = ts_internal_bspline_access_ctrlp(_buckled_);

	for (i = 0; i < N; i++) {
		for (d = 0; d < dim; d++) {
			ctrlp[i*dim + d] =
				b     * ctrlp[i*dim + d] +
				b_hat * (p0[d] + ((tsReal)i / (N-1)) * (pn_1[d] - p0[d]));
		}
	}
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_to_beziers(const tsBSpline *spline, tsBSpline *_beziers_,
	tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = ts_bspline_order(spline);

	int resize;    /**< Number of control points to add/remove. */
	size_t k;      /**< Index of the split knot value. */
	tsReal u_min;  /**< Minimum of the knot values. */
	tsReal u_max;  /**< Maximum of the knot values. */

	tsBSpline tmp;    /**< Temporarily stores the result. */
	tsReal *knots;    /**< Pointer to the knots of tmp. */
	size_t num_knots; /**< Number of knots in tmp. */

	tsError err;

	INIT_OUT_BSPLINE(spline, _beziers_)
	TS_CALL_ROE(err, ts_bspline_copy(spline, &tmp, status))
	knots = ts_internal_bspline_access_knots(&tmp);
	num_knots = ts_bspline_num_knots(&tmp);

	TS_TRY(try, err, status)
		/* DO NOT FORGET TO UPDATE knots AND num_knots AFTER EACH
		 * TRANSFORMATION OF tmp! */

		/* fix first control point if necessary */
		u_min = knots[deg];
		if (!ts_knots_equal(knots[0], u_min)) {
			TS_CALL(try, err, ts_bspline_split(
				&tmp, u_min, &tmp, &k, status))
			resize = (int)(-1*deg + (deg*2 - k));
			TS_CALL(try, err, ts_internal_bspline_resize(
				&tmp, resize, 0, &tmp, status))
			knots = ts_internal_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
		}

		/* fix last control point if necessary */
		u_max = knots[num_knots - order];
		if (!ts_knots_equal(knots[num_knots - 1], u_max)) {
			TS_CALL(try, err, ts_bspline_split(
				&tmp, u_max, &tmp, &k, status))
			num_knots = ts_bspline_num_knots(&tmp);
			resize = (int)(-1*deg + (k - (num_knots - order)));
			TS_CALL(try, err, ts_internal_bspline_resize(
				&tmp, resize, 1, &tmp, status))
			knots = ts_internal_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
		}

		k = order;
		while (k < num_knots - order) {
			TS_CALL(try, err, ts_bspline_split(
				&tmp, knots[k], &tmp, &k, status))
			knots = ts_internal_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
			k++;
		}

		if (spline == _beziers_)
			ts_bspline_free(_beziers_);
		ts_bspline_move(&tmp, _beziers_);
	TS_FINALLY
		ts_bspline_free(&tmp);
	TS_END_TRY_RETURN(err)
}



/******************************************************************************
*                                                                             *
* :: Serialization and Persistence Functions                                  *
*                                                                             *
******************************************************************************/
tsError ts_internal_bspline_to_json(const tsBSpline * spline,
	JSON_Value **value, tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t len_ctrlp = ts_bspline_len_control_points(spline);
	const size_t len_knots = ts_bspline_num_knots(spline);
	const tsReal *ctrlp = ts_internal_bspline_access_ctrlp(spline);
	const tsReal *knots = ts_internal_bspline_access_knots(spline);

	size_t i; /**< Used in loops */
	tsError err;

	JSON_Value  *ctrlp_value;
	JSON_Value  *knots_value;
	JSON_Object *spline_object;
	JSON_Array  *ctrlp_array;
	JSON_Array  *knots_array;

	*value = ctrlp_value = knots_value = NULL;
	TS_TRY(values, err, status)
		/* Init memory. */
		*value = json_value_init_object();
		if (!*value) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}
		ctrlp_value = json_value_init_array();
		if (!ctrlp_value) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}
		knots_value = json_value_init_array();
		if (!knots_value) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}

		/* Although the following functions can't fail, that is, they
		 * won't return NULL or JSONFailure, we nevertheless handle
		 * unexpected return values. */

		/* Init output. */
		spline_object = json_value_get_object(*value);
		if (!spline_object) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}

		/* Set degree and dimension. */
		if (JSONSuccess != json_object_set_number(
				spline_object, "degree", (double) deg)) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}
		if (JSONSuccess != json_object_set_number(
				spline_object, "dimension", (double) dim)) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}

		/* Set control points. */
		if (JSONSuccess != json_object_set_value(spline_object,
				"control_points", ctrlp_value)) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}
		ctrlp_array = json_array(ctrlp_value);
		if (!ctrlp_array) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}
		for (i = 0; i < len_ctrlp; i++) {
			if (JSONSuccess != json_array_append_number(
					ctrlp_array, (double) ctrlp[i])) {
				TS_THROW_0(values, err, status, TS_MALLOC,
					   "out of memory");
			}
		}

		/* Set knots. */
		if (JSONSuccess != json_object_set_value(
				spline_object, "knots", knots_value)) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}
		knots_array = json_array(knots_value);
		if (!knots_array) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				   "out of memory");
		}
		for (i = 0; i < len_knots; i++) {
			if (JSONSuccess != json_array_append_number(
					knots_array, (double) knots[i])) {
				TS_THROW_0(values, err, status, TS_MALLOC,
					   "out of memory");
			}
		}
	TS_CATCH(err)
		if (*value)
			json_value_free(*value);
		if (ctrlp_value && !json_value_get_parent(ctrlp_value))
			json_value_free(ctrlp_value);
		if (knots_value && !json_value_get_parent(knots_value))
			json_value_free(knots_value);
		*value = NULL;
	TS_END_TRY_RETURN(err)
}

tsError ts_internal_bspline_from_json(const JSON_Value *spline_value,
	tsBSpline *_spline_, tsStatus *status)
{
	size_t deg, dim, len_ctrlp, num_knots;
	tsReal *ctrlp, *knots;

	JSON_Object *spline_object;
	JSON_Value *deg_value;
	JSON_Value *dim_value;
	JSON_Value *ctrlp_value;
	JSON_Array *ctrlp_array;
	JSON_Value *knots_value;
	JSON_Array *knots_array;
	JSON_Value *real_value;
	size_t i;
	tsError err;

	ts_internal_bspline_init(_spline_);

	/* Read spline object. */
	if (json_value_get_type(spline_value) != JSONObject)
		TS_RETURN_0(status, TS_PARSE_ERROR, "invalid json input")
	spline_object = json_value_get_object(spline_value);
	if (!spline_object)
		TS_RETURN_0(status, TS_PARSE_ERROR, "invalid json input")

	/* Read degree. */
	deg_value = json_object_get_value(spline_object, "degree");
	if (json_value_get_type(deg_value) != JSONNumber)
		TS_RETURN_0(status, TS_PARSE_ERROR, "degree is not a number")
	if (json_value_get_number(deg_value) < -0.01f) {
		TS_RETURN_1(status, TS_PARSE_ERROR, "degree (%f) < 0",
			   json_value_get_number(deg_value))
	}
	deg = (size_t) json_value_get_number(deg_value);

	/* Read dimension. */
	dim_value = json_object_get_value(spline_object, "dimension");
	if (json_value_get_type(dim_value) != JSONNumber) {
		TS_RETURN_0(status, TS_PARSE_ERROR,
			    "dimension is not a number")
	}
	if (json_value_get_number(dim_value) < 0.99f) {
		TS_RETURN_1(status, TS_PARSE_ERROR, "dimension (%f) < 1",
			   json_value_get_number(deg_value))
	}
	dim = (size_t) json_value_get_number(dim_value);

	/* Read length of control point vector. */
	ctrlp_value = json_object_get_value(spline_object, "control_points");
	if (json_value_get_type(ctrlp_value) != JSONArray) {
		TS_RETURN_0(status, TS_PARSE_ERROR,
			   "control_points is not an array")
	}
	ctrlp_array = json_value_get_array(ctrlp_value);
	len_ctrlp = json_array_get_count(ctrlp_array);
	if (len_ctrlp % dim != 0) {
		TS_RETURN_2(status, TS_PARSE_ERROR,
			   "len(control_points) (%lu) %% dimension (%lu) != 0",
			    (unsigned long) len_ctrlp, (unsigned long) dim)
	}

	/* Read number of knots. */
	knots_value = json_object_get_value(spline_object, "knots");
	if (json_value_get_type(knots_value) != JSONArray) {
		TS_RETURN_0(status, TS_PARSE_ERROR,
			   "knots is not an array")
	}
	knots_array = json_value_get_array(knots_value);
	num_knots = json_array_get_count(knots_array);

	/* Create spline. */
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_bspline_new(len_ctrlp/dim,
				dim, deg, TS_CLAMPED, _spline_, status))
		if (num_knots != ts_bspline_num_knots(_spline_))
			TS_THROW_2(try, err, status, TS_NUM_KNOTS,
				   "unexpected num(knots): (%lu) != (%lu)",
				   (unsigned long) num_knots,
				   (unsigned long) ts_bspline_num_knots(_spline_))

		/* Set control points. */
		ctrlp = ts_internal_bspline_access_ctrlp(_spline_);
		for (i = 0; i < len_ctrlp; i++) {
			real_value = json_array_get_value(ctrlp_array, i);
			if (json_value_get_type(real_value) != JSONNumber)
				TS_THROW_1(try, err, status, TS_PARSE_ERROR,
					   "control_points: value at index %lu is not a number",
					   (unsigned long) i)
			ctrlp[i] = (tsReal) json_value_get_number(real_value);
		}
		TS_CALL(try, err, ts_bspline_set_control_points(
			_spline_, ctrlp, status))

		/* Set knots. */
		knots = ts_internal_bspline_access_knots(_spline_);
		for (i = 0; i < num_knots; i++) {
			real_value = json_array_get_value(knots_array, i);
			if (json_value_get_type(real_value) != JSONNumber)
			TS_THROW_1(try, err, status, TS_PARSE_ERROR,
				   "knots: value at index %lu is not a number",
				   (unsigned long) i)
			knots[i] = (tsReal) json_value_get_number(real_value);
		}
		TS_CALL(try, err, ts_bspline_set_knots(
			_spline_, knots, status))
	TS_CATCH(err)
		ts_bspline_free(_spline_);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_to_json(const tsBSpline *spline, char **_json_,
	tsStatus *status)
{
	tsError err;
	JSON_Value *value = NULL;
	*_json_ = NULL;
	TS_CALL_ROE(err, ts_internal_bspline_to_json(spline, &value, status))
	*_json_ = json_serialize_to_string_pretty(value);
	json_value_free(value);
	if (!*_json_)
		TS_RETURN_0(status, TS_MALLOC, "out of memory");
	TS_RETURN_SUCCESS(status);
}

tsError ts_bspline_from_json(const char *json, tsBSpline *_spline_,
	tsStatus *status)
{
	tsError err;
	JSON_Value *value = NULL;
	ts_internal_bspline_init(_spline_);
	TS_TRY(try, err, status)
		value = json_parse_string(json);
		if (!value) {
			TS_RETURN_0(status, TS_PARSE_ERROR,
				   "invalid json input")
		}
		TS_CALL(try, err, ts_internal_bspline_from_json(
			value, _spline_, status))
	TS_FINALLY
		if (value)
			json_value_free(value);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_save_json(const tsBSpline *spline, const char *path,
	tsStatus *status)
{
	tsError err;
	JSON_Status json_status;
	JSON_Value *value = NULL;
	TS_CALL_ROE(err, ts_internal_bspline_to_json(spline, &value, status))
	json_status = json_serialize_to_file_pretty(value, path);
	json_value_free(value);
	if (json_status != JSONSuccess)
		TS_RETURN_0(status, TS_IO_ERROR, "unexpected io error")
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_load_json(const char *path, tsBSpline *_spline_,
	tsStatus *status)
{
	tsError err;
	FILE *file = NULL;
	JSON_Value *value = NULL;
	ts_internal_bspline_init(_spline_);
	TS_TRY(try, err, status)
		file = fopen(path, "r");
		if (!file) {
			TS_THROW_0(try, err, status, TS_IO_ERROR,
				   "unable to open file")
		}
		value = json_parse_file(path);
		if (!value) {
			TS_RETURN_0(status, TS_PARSE_ERROR,
				   "invalid json input")
		}
		TS_CALL(try, err, ts_internal_bspline_from_json(
			value, _spline_, status))
	TS_FINALLY
		if (file)
			fclose(file);
		if (value)
			json_value_free(value);
	TS_CATCH(err)
		ts_bspline_free(_spline_);
	TS_END_TRY_RETURN(err)
}



/******************************************************************************
*                                                                             *
* :: Utility Functions                                                        *
*                                                                             *
******************************************************************************/
int ts_knots_equal(tsReal x, tsReal y)
{
	return fabs(x-y) <= TS_KNOT_EPSILON ? 1 : 0;
}

void ts_arr_fill(tsReal *arr, size_t num, tsReal val)
{
	size_t i;
	for (i = 0; i < num; i++)
		arr[i] = val;
}

tsReal ts_distance(const tsReal *x, const tsReal *y, size_t dim)
{
	tsReal sum = 0;
	size_t i;
	for (i = 0; i < dim; i++)
		sum += (x[i] - y[i]) * (x[i] - y[i]);
	return (tsReal) sqrt(sum);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
