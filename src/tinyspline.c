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
		ts_int_bspline_init(out);



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
void ts_int_bspline_init(tsBSpline *_spline_)
{
	_spline_->pImpl = NULL;
}

size_t ts_int_bspline_sof_state(const tsBSpline *spline)
{
	return sizeof(struct tsBSplineImpl) +
	       ts_bspline_sof_control_points(spline) +
	       ts_bspline_sof_knots(spline);
}

tsReal * ts_int_bspline_access_ctrlp(const tsBSpline *spline)
{
	return (tsReal *) (& spline->pImpl[1]);
}

tsReal * ts_int_bspline_access_knots(const tsBSpline *spline)
{
	return ts_int_bspline_access_ctrlp(spline)
		+ ts_bspline_len_control_points(spline);
}

tsError ts_int_bspline_access_ctrlp_at(const tsBSpline *spline, size_t index,
	tsReal **ctrlp, tsStatus *status)
{
	if (index >= ts_bspline_num_control_points(spline)) {
		TS_RETURN_2(status, TS_INDEX_ERROR,
			"index (%lu) >= num(control_points) (%lu)",
			(unsigned long) index,
			(unsigned long) ts_bspline_num_control_points(spline))
	}
	*ctrlp = ts_int_bspline_access_ctrlp(spline) +
		 index * ts_bspline_dimension(spline);
	TS_RETURN_SUCCESS(status)
}

tsError ts_int_bspline_access_knot_at(const tsBSpline *spline, size_t index,
	tsReal *knot, tsStatus *status)
{
	if (index >= ts_bspline_num_knots(spline)) {
		TS_RETURN_2(status, TS_INDEX_ERROR,
			"index (%lu) >= num(knots) (%lu)",
			(unsigned long) index,
			(unsigned long) ts_bspline_num_knots(spline))
	}
	*knot = ts_int_bspline_access_knots(spline)[index];
	TS_RETURN_SUCCESS(status)
}

void ts_int_deboornet_init(tsDeBoorNet *_deBoorNet_)
{
	_deBoorNet_->pImpl = NULL;
}

size_t ts_int_deboornet_sof_state(const tsDeBoorNet *net)
{
	return sizeof(struct tsDeBoorNetImpl) +
	       ts_deboornet_sof_points(net) +
	       ts_deboornet_sof_result(net);
}

tsReal * ts_int_deboornet_access_points(const tsDeBoorNet *net)
{
	return (tsReal *) (& net->pImpl[1]);
}

tsReal * ts_int_deboornet_access_result(const tsDeBoorNet *net)
{
	if (ts_deboornet_num_result(net) == 2) {
		return ts_int_deboornet_access_points(net);
	} else {
		return ts_int_deboornet_access_points(net) +
			/* Last point in `points`. */
		       (ts_deboornet_len_points(net) -
			ts_deboornet_dimension(net));
	}
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
	memcpy(*ctrlp, ts_int_bspline_access_ctrlp(spline), size);
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_control_point_at(const tsBSpline *spline, size_t index,
	tsReal **ctrlp, tsStatus *status)
{
	tsReal *from;
	size_t size;
	tsError err;
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_int_bspline_access_ctrlp_at(
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
	memmove(ts_int_bspline_access_ctrlp(spline), ctrlp, size);
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_set_control_point_at(tsBSpline *spline, size_t index,
	const tsReal *ctrlp, tsStatus *status)
{
	tsReal *to;
	size_t size;
	tsError err;
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_int_bspline_access_ctrlp_at(
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
	memcpy(*knots, ts_int_bspline_access_knots(spline), size);
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_knot_at(const tsBSpline *spline, size_t index, tsReal *knot,
	tsStatus *status)
{
	return ts_int_bspline_access_knot_at(spline, index, knot, status);
}

tsError ts_bspline_set_knots(tsBSpline *spline, const tsReal *knots,
	tsStatus *status)
{
	const size_t size = ts_bspline_sof_knots(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	const size_t order = ts_bspline_order(spline);
	size_t idx, mult;
	tsReal lst_knot, knot;
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
	memmove(ts_int_bspline_access_knots(spline), knots, size);
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_set_knot_at(tsBSpline *spline, size_t index, tsReal knot,
	tsStatus *status)
{
	tsError err;
	tsReal *knots = NULL;
	/* This is only for initialization. */
	tsReal oldKnot = ts_int_bspline_access_knots(spline)[0];
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_int_bspline_access_knot_at(
			spline, index, &oldKnot, status))
		/* knots must be set after reading oldKnot because the catch
		 * block assumes that oldKnot contains the correct value if
		 * knots is not NULL. */
		knots = ts_int_bspline_access_knots(spline);
		knots[index] = knot;
		TS_CALL(try, err, ts_bspline_set_knots(
			spline, knots, status))
	TS_CATCH(err)
		/* If knots is not NULL, oldKnot contains the correct value. */
		if (knots)
			knots[index] = oldKnot;
	TS_END_TRY_RETURN(err)
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
	memcpy(*points, ts_int_deboornet_access_points(net), size);
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
	memcpy(*result, ts_int_deboornet_access_result(net), size);
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

tsError ts_int_bspline_generate_knots(const tsBSpline *spline,
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

	knots = ts_int_bspline_access_knots(spline);

	if (type == TS_OPENED) {
		knots[0] = TS_DOMAIN_DEFAULT_MIN; /* n_knots >= 2 */
		fac = (TS_DOMAIN_DEFAULT_MAX - TS_DOMAIN_DEFAULT_MIN)
		      / (n_knots - 1); /* n_knots >= 2 */
		for (i = 1; i < n_knots-1; i++)
			knots[i] = TS_DOMAIN_DEFAULT_MIN + i*fac;
		knots[i] = TS_DOMAIN_DEFAULT_MAX; /* n_knots >= 2 */
	} else if (type == TS_CLAMPED) {
		/* n_knots >= 2*order == 2*(deg+1) == 2*deg + 2 > 2*deg - 1 */
		fac = (TS_DOMAIN_DEFAULT_MAX - TS_DOMAIN_DEFAULT_MIN)
		      / (n_knots - 2*deg - 1);
		ts_arr_fill(knots, order, TS_DOMAIN_DEFAULT_MIN);
		for (i = order ;i < n_knots-order; i++)
			knots[i] = TS_DOMAIN_DEFAULT_MIN + (i-deg)*fac;
		ts_arr_fill(knots + i, order, TS_DOMAIN_DEFAULT_MAX);
	} else if (type == TS_BEZIERS) {
		/* n_knots >= 2*order implies n_knots/order >= 2 */
		fac = (TS_DOMAIN_DEFAULT_MAX - TS_DOMAIN_DEFAULT_MIN)
		      / (n_knots/order - 1);
		ts_arr_fill(knots, order, TS_DOMAIN_DEFAULT_MIN);
		for (i = order; i < n_knots-order; i += order)
			ts_arr_fill(knots + i, order,
				    TS_DOMAIN_DEFAULT_MIN + (i/order)*fac);
		ts_arr_fill(knots + i, order, TS_DOMAIN_DEFAULT_MAX);
	}
	TS_RETURN_SUCCESS(status)
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

	ts_int_bspline_init(spline);

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
		TS_CALL(try, err, ts_int_bspline_generate_knots(
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
	ts_int_bspline_init(dest);
	size = ts_int_bspline_sof_state(src);
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
	ts_int_bspline_init(src);
}

void ts_bspline_free(tsBSpline *spline)
{
	if (spline->pImpl)
		free(spline->pImpl);
	ts_int_bspline_init(spline);
}

/* ------------------------------------------------------------------------- */

tsDeBoorNet ts_deboornet_init()
{
	tsDeBoorNet net;
	net.pImpl = NULL;
	return net;
}

tsError ts_int_deboornet_new(const tsBSpline *spline, tsDeBoorNet *net,
	tsStatus *status)
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

	net->pImpl = (struct tsDeBoorNetImpl *) malloc(sof_net);
	if (!net->pImpl)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")

	net->pImpl->u = 0.f;
	net->pImpl->k = 0;
	net->pImpl->s = 0;
	net->pImpl->h = deg;
	net->pImpl->dim = dim;
	net->pImpl->n_points = fixed_num_points;
	TS_RETURN_SUCCESS(status)
}

void ts_deboornet_free(tsDeBoorNet *net)
{
	if (net->pImpl)
		free(net->pImpl);
	ts_int_deboornet_init(net);
}

tsError ts_deboornet_copy(const tsDeBoorNet *src, tsDeBoorNet *dest,
	tsStatus *status)
{
	size_t size;
	if (src == dest)
		TS_RETURN_SUCCESS(status)
	ts_int_deboornet_init(dest);
	size = ts_int_deboornet_sof_state(src);
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
	ts_int_deboornet_init(src);
}



/******************************************************************************
*                                                                             *
* :: Interpolation and Approximation Functions                                *
*                                                                             *
******************************************************************************/
tsError ts_int_thomas_algorithm(const tsReal *a, const tsReal *b,
	const tsReal *c, size_t num, size_t dim, tsReal *d, tsStatus *status)
{
	size_t i, j, k, l;
	tsReal m, *cc = NULL;
	tsError err;

	if (dim == 0) {
		TS_RETURN_0(status, TS_DIM_ZERO,
			"unsupported dimension: 0")
	}
	if (num <= 1) {
		TS_RETURN_1(status, TS_NUM_POINTS,
			"num(points) (%lu) <= 1", (unsigned long) num)
	}
	cc = (tsReal *) malloc(num * sizeof(tsReal));
	if (!cc) TS_RETURN_0(status, TS_MALLOC, "out of memory")

	TS_TRY(try, err, status)
		/* Forward sweep. */
		if (fabs(b[0]) <= fabs(c[0])) {
			TS_THROW_2(try, err, status, TS_NO_RESULT,
				"error: |%f| <= |%f|", b[0], c[0])
		}
		/* |b[i]| > |c[i]| implies that |b[i]| > 0. Thus, the following
		 * statements cannot evaluate to division by zero.*/
		cc[0] = c[0] / b[0];
		for (i = 0; i < dim; i++)
			d[i] = d[i] / b[0];
		for (i = 1; i < num; i++) {
			if (fabs(b[i]) <= fabs(a[i]) + fabs(c[i])) {
				TS_THROW_3(try, err, status, TS_NO_RESULT,
					"error: |%f| <= |%f| + |%f|",
					b[i], a[i], c[i])
			}
			/* |a[i]| < |b[i]| and cc[i - 1] < 1. Therefore, the
			 * following statement cannot evaluate to division by
			 * zero. */
			m = 1.f / (b[i] - a[i] * cc[i - 1]);
			/* |b[i]| > |a[i]| + |c[i]| implies that there must be
			 * an eps > 0 such that |b[i]| = |a[i]| + |c[i]| + eps.
			 * Even if |a[i]| is 0 (by which the result of the
			 * following statement becomes maximum), |c[i]| is less
			 * than |b[i]| by an amount of eps. By substituting the
			 * previous and the following statements (under the
			 * assumption that |a[i]| is 0), we obtain c[i] / b[i],
			 * which must be less than 1. */
			cc[i] = c[i] * m;
			for (j = 0; j < dim; j++) {
				k = i * dim + j;
				l = (i-1) * dim + j;
				d[k] = (d[k] - a[i] * d[l]) * m;
			}
		}

		/* Back substitution. */
		for (i = num-1; i > 0; i--) {
			for (j = 0; j < dim; j++) {
				k = (i-1) * dim + j;
				l = i * dim + j;
				d[k] -= cc[i-1] * d[l];
			}
		}
	TS_FINALLY
		free(cc);
	TS_END_TRY_RETURN(err)
}

tsError ts_int_relaxed_uniform_cubic_bspline(const tsReal *points, size_t n,
	size_t dim, tsBSpline *spline, tsStatus *status)
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
		TS_RETURN_1(status, TS_NUM_POINTS,
			"num(points) (%lu) <= 1", (unsigned long) n)
	}
	/* in the following n >= 2 applies */

	sof_ctrlp = dim * sizeof(tsReal); /* dim > 0 implies sof_ctrlp > 0 */

	s = NULL;
	TS_TRY(try, err, status)
		/* n >= 2 implies n-1 >= 1 implies (n-1)*4 >= 4 */
		TS_CALL(try, err, ts_bspline_new(
			(n-1)*4, dim, order-1, TS_BEZIERS, spline, status))
		ctrlp = ts_int_bspline_access_ctrlp(spline);

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
		ts_bspline_free(spline);
	TS_FINALLY
		if (s)
			free(s);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_interpolate_cubic_natural(const tsReal *points,
	size_t num_points, size_t dimension, tsBSpline *spline,
	tsStatus *status)
{
	const size_t sof_ctrlp = dimension * sizeof(tsReal);
	const size_t len_points = num_points * dimension;
	const size_t num_int_points = num_points - 2;
	const size_t len_int_points = num_int_points * dimension;
	tsReal *thomas, *a, *b, *c, *d;
	size_t i, j, k, l;
	tsError err;

	ts_int_bspline_init(spline);
	if (num_points == 0)
		TS_RETURN_0(status, TS_NUM_POINTS, "num(points) == 0")
	if (num_points == 1) {
		TS_CALL_ROE(err, ts_bspline_new(
			4, dimension, 3, TS_CLAMPED, spline, status))
		for (i = 0; i < ts_bspline_num_control_points(spline); i++) {
			memcpy(ts_int_bspline_access_ctrlp(spline)
			       + i * dimension, points, sof_ctrlp);
		}
		TS_RETURN_SUCCESS(status)
	}
	if (num_points == 2) {
		return ts_int_relaxed_uniform_cubic_bspline(
			points, num_points, dimension, spline, status);
	}
	/* `num_points` >= 3 */
	thomas = NULL;
	TS_TRY(try, err, status)
		thomas = (tsReal *) malloc(3 * num_int_points * sof_ctrlp);
		if (!thomas) {
			TS_THROW_0(try, err, status, TS_MALLOC,
				"out of memory")
		}
		/* The system of linear equations is taken from:
		 *     http://www.bakoma-tex.com/doc/generic/pst-bspline/
		 *     pst-bspline-doc.pdf */
		a = c = thomas;
		ts_arr_fill(a, len_int_points, 1);
		b = a + len_int_points;
		ts_arr_fill(b, len_int_points, 4);
		d = b + len_int_points;
		/* 6 * S_{i+1} */
		for (i = 0; i < num_int_points; i++) {
			for (j = 0; j < dimension; j++) {
				k = i * dimension + j;
				l = (i+1) * dimension + j;
				d[k] = 6 * points[l];
			}
		}
		for (i = 0; i < dimension; i++) {
			/* 6 * S_{1} - S_{0} */
			d[i] -= points[i];
			/* 6 * S_{n-1} - S_{n} */
			k = len_int_points - (i+1);
			l = len_points - (i+1);
			d[k] -= points[l];
		}
		/* The Thomas algorithm requires at least two points. Hence,
		 * `num_int_points` == 1 must be handled separately (let's call
		 * it "Mini Thomas"). */
		if (num_int_points == 1) {
			for (i = 0; i < dimension; i++)
				d[i] *= (tsReal) 0.25f;
		} else {
			TS_CALL(try, err, ts_int_thomas_algorithm(
				a, b, c, num_int_points, dimension, d,
				status))
		}
		memcpy(thomas, points, sof_ctrlp);
		memmove(thomas + dimension, d, num_int_points * sof_ctrlp);
		memcpy(thomas + (num_int_points+1) * dimension,
		       points + (num_points-1) * dimension, sof_ctrlp);
		TS_CALL(try, err, ts_int_relaxed_uniform_cubic_bspline(
			thomas, num_points, dimension, spline, status))
	TS_CATCH(err)
		ts_bspline_free(spline);
	TS_FINALLY
		if (thomas)
			free(thomas);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_interpolate_catmull_rom(const tsReal *points,
	size_t num_points, size_t dimension, tsReal alpha, const tsReal *first,
	const tsReal *last, tsReal epsilon, tsBSpline *spline,
	tsStatus *status)
{
	const size_t sof_real = sizeof(tsReal);
	const size_t sof_ctrlp = dimension * sof_real;
	const tsReal eps = (tsReal) fabs(epsilon);
	tsReal *bs_ctrlp; /* Points to the control points of `spline`. */
	tsReal *cr_ctrlp; /**< The points to interpolate based on `points`. */
	size_t i, d; /**< Used in for loops. */
	tsError err; /**< Local error handling. */
	/* [https://en.wikipedia.org/wiki/
	 * Centripetal_Catmull%E2%80%93Rom_spline] */
	tsReal t0, t1, t2, t3; /**< Catmull-Rom knots. */
	/* [https://stackoverflow.com/questions/30748316/
	 * catmull-rom-interpolation-on-svg-paths/30826434#30826434] */
	tsReal c1, c2, d1, d2, m1, m2; /**< Used to calculate derivatives. */
	tsReal *p0, *p1, *p2, *p3; /**< Processed Catmull-Rom points. */

	ts_int_bspline_init(spline);
	if (dimension == 0)
		TS_RETURN_0(status, TS_DIM_ZERO, "unsupported dimension: 0")
	if (num_points == 0)
		TS_RETURN_0(status, TS_NUM_POINTS, "num(points) == 0")
	if (num_points == 1) {
		TS_CALL_ROE(err, ts_bspline_new(num_points, dimension,
			num_points - 1, TS_CLAMPED, spline, status))
		bs_ctrlp = ts_int_bspline_access_ctrlp(spline);
		memcpy(bs_ctrlp, points, sof_ctrlp);
		TS_RETURN_SUCCESS(status)
	}
	if (alpha < 0.f)
		alpha = (tsReal) 0.f;
	if (alpha > 1.f)
		alpha = (tsReal) 1.f;

	/* Copy `points` to `cr_ctrlp`. Add space for `first` and `last`. */
	cr_ctrlp = (tsReal *) malloc((num_points + 2) * sof_ctrlp);
	if (!cr_ctrlp)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	memcpy(cr_ctrlp + dimension, points, num_points * sof_ctrlp);

	/* Remove redundant points from `cr_ctrlp`. Update `num_points`. */
	for (i = 1 /* 0 (`first`) is not assigned yet */;
			i < num_points /* skip last point (inclusive end) */;
			i++) {
		p0 = cr_ctrlp + (i * dimension);
		p1 = p0 + dimension;
		if (ts_distance(p0, p1, dimension) <= eps) {
			/* Are there any other points (after the one that is
			 * to be removed) that need to be shifted? */
			if (i < num_points - 1) {
				memmove(p1, p1 + dimension,
					(num_points - (i + 1)) * sof_ctrlp);
			}
			num_points--;
			i--;
		}
	}

	/* Check if there are still enough points for interpolation. */
	if (num_points == 1) { /* `num_points` can't be 0 */
		free(cr_ctrlp); /* The point is copied from `points`. */
		TS_CALL_ROE(err, ts_bspline_new(num_points, dimension,
			num_points - 1, TS_CLAMPED, spline, status))
		bs_ctrlp = ts_int_bspline_access_ctrlp(spline);
		memcpy(bs_ctrlp, points, sof_ctrlp);
		TS_RETURN_SUCCESS(status)
	}

	/* Add or generate `first` and `last`. Update `num_points`. */
	p0 = cr_ctrlp + dimension;
	if (first && ts_distance(first, p0, dimension) > eps) {
		memcpy(cr_ctrlp, first, sof_ctrlp);
	} else {
		p1 = p0 + dimension;
		for (d = 0; d < dimension; d++)
			cr_ctrlp[d] = p0[d] + (p0[d] - p1[d]);
	}
	p1 = cr_ctrlp + (num_points * dimension);
	if (last && ts_distance(p1, last, dimension) > eps) {
		memcpy(cr_ctrlp + ((num_points + 1) * dimension),
		       last, sof_ctrlp);
	} else {
		p0 = p1 - dimension;
		for (d = 0; d < dimension; d++) {
			cr_ctrlp[((num_points + 1) * dimension) + d] =
				p1[d] + (p1[d] - p0[d]);
		}
	}
	num_points = num_points + 2;

	/* Transform the sequence of Catmull-Rom splines. */
	bs_ctrlp = NULL;
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_bspline_new(
			(num_points - 3) * 4, dimension, 3,
			TS_BEZIERS, spline, status))
		bs_ctrlp = ts_int_bspline_access_ctrlp(spline);
	TS_CATCH(err)
		free(cr_ctrlp);
	TS_END_TRY_ROE(err)
	for (i = 0; i < ts_bspline_num_control_points(spline) / 4; i++) {
		p0 = cr_ctrlp + ((i+0) * dimension);
		p1 = cr_ctrlp + ((i+1) * dimension);
		p2 = cr_ctrlp + ((i+2) * dimension);
		p3 = cr_ctrlp + ((i+3) * dimension);

		t0 = (tsReal) 0.f;
		t1 = t0 + (tsReal) pow(ts_distance(p0, p1, dimension), alpha);
		t2 = t1 + (tsReal) pow(ts_distance(p1, p2, dimension), alpha);
		t3 = t2 + (tsReal) pow(ts_distance(p2, p3, dimension), alpha);

		c1 = (t2-t1) / (t2-t0);
		c2 = (t1-t0) / (t2-t0);
		d1 = (t3-t2) / (t3-t1);
		d2 = (t2-t1) / (t3-t1);

		for (d = 0; d < dimension; d++) {
			m1 = (t2-t1)*(c1*(p1[d]-p0[d])/(t1-t0)
				+ c2*(p2[d]-p1[d])/(t2-t1));
			m2 = (t2-t1)*(d1*(p2[d]-p1[d])/(t2-t1)
				+ d2*(p3[d]-p2[d])/(t3-t2));
			bs_ctrlp[((i*4 + 0) * dimension) + d] = p1[d];
			bs_ctrlp[((i*4 + 1) * dimension) + d] = p1[d] + m1/3;
			bs_ctrlp[((i*4 + 2) * dimension) + d] = p2[d] - m2/3;
			bs_ctrlp[((i*4 + 3) * dimension) + d] = p2[d];
		}
	}
	free(cr_ctrlp);
	TS_RETURN_SUCCESS(status)
}



/******************************************************************************
*                                                                             *
* :: Query Functions                                                          *
*                                                                             *
******************************************************************************/
tsError ts_int_bspline_find_knot(const tsBSpline *spline, tsReal knot,
	size_t *index, size_t *multiplicity, tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	const tsReal *knots = ts_int_bspline_access_knots(spline);
	tsReal min, max;
	size_t low, high;

	ts_bspline_domain(spline, &min, &max);
	if (knot < min && !ts_knots_equal(knot, min)) {
		TS_RETURN_2(status, TS_U_UNDEFINED,
			"knot (%f) < min(domain) (%f)", knot, min)
	}
	if (knot > max && !ts_knots_equal(knot, max)) {
		TS_RETURN_2(status, TS_U_UNDEFINED,
			"knot (%f) > max(domain) (%f)", knot, max)
	}

	/* Based on 'The NURBS Book' (Les Piegl and Wayne Tiller). */
	if (ts_knots_equal(knot, knots[num_knots - 1])) {
		*index = num_knots - 1;
	} else {
		low = 0;
		high = num_knots - 1;
		*index = (low+high) / 2;
		while (knot < knots[*index] || knot >= knots[*index + 1]) {
			if (knot < knots[*index])
				high = *index;
			else
				low = *index;
			*index = (low+high) / 2;
		}
	}

	/* Handle floating point errors. */
	while (*index < num_knots - 1 && /* there is a next knot */
		ts_knots_equal(knot, knots[*index + 1])) {
		(*index)++;
	}

	/* Calculate knot's multiplicity. */
	for (*multiplicity = deg + 1; *multiplicity > 0 ; (*multiplicity)--) {
		if (ts_knots_equal(knot, knots[*index - (*multiplicity-1)]))
			break;
	}

	TS_RETURN_SUCCESS(status)
}

tsError ts_int_bspline_eval_woa(const tsBSpline *spline, tsReal u,
	tsDeBoorNet *net, tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = ts_bspline_order(spline);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	const size_t sof_ctrlp = dim * sizeof(tsReal);

	const tsReal *ctrlp = ts_int_bspline_access_ctrlp(spline);
	const tsReal *knots = ts_int_bspline_access_knots(spline);
	tsReal *points = NULL;  /**< Pointer to the points of \p net. */

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

	points = ts_int_deboornet_access_points(net);

	/* 1. Find index k such that u is in between [u_k, u_k+1).
	 * 2. Setup already known values.
	 * 3. Decide by multiplicity of u how to calculate point P(u). */

	/* 1. */
	k = s = 0;
	TS_CALL_ROE(err, ts_int_bspline_find_knot(
		spline, u, &k, &s, status))

	/* 2. */
	uk = knots[k];  /* Ensures that with any precision of  */
	net->pImpl->u = /* tsReal the knot vector stays valid. */
		ts_knots_equal(u, uk) ? uk : u;
	net->pImpl->k = k;
	net->pImpl->s = s;
        net->pImpl->h = deg < s ? 0 : deg-s; /* prevent underflow */

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
			net->pImpl->n_points = 1;
			memcpy(points, ctrlp + from, sof_ctrlp);
		} else {
			from = (k-s) * dim;
			net->pImpl->n_points = 2;
			memcpy(points, ctrlp + from, 2 * sof_ctrlp);
		}
	} else { /* by 3a) s <= deg (order = deg+1) */
		fst = k-deg; /* by 1. k >= deg */
		lst = k-s; /* s <= deg <= k */
		N = lst-fst + 1; /* lst <= fst implies N >= 1 */

		net->pImpl->n_points = (size_t)(N * (N+1) * 0.5f);

		/* copy initial values to output */
		memcpy(points, ctrlp + fst*dim, N * sof_ctrlp);

		lidx = 0;
		ridx = dim;
		tidx = N*dim; /* N >= 1 implies tidx > 0 */
		r = 1;
		for (;r <= ts_deboornet_num_insertions(net); r++) {
			i = fst + r;
			for (; i <= lst; i++) {
				ui = knots[i];
				a = (ts_deboornet_knot(net) - ui) /
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

tsError ts_bspline_eval(const tsBSpline *spline, tsReal u, tsDeBoorNet *net,
	tsStatus *status)
{
	tsError err;
	ts_int_deboornet_init(net);
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_int_deboornet_new(
			spline, net, status))
		TS_CALL(try, err, ts_int_bspline_eval_woa(
			spline, u, net, status))
	TS_CATCH(err)
		ts_deboornet_free(net);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_eval_all(const tsBSpline *spline, const tsReal *us,
	size_t num, tsReal **points, tsStatus *status)
{
	const size_t dim = ts_bspline_dimension(spline);
	const size_t sof_point = dim * sizeof(tsReal);
	const size_t sof_points = num * sof_point;
	tsDeBoorNet net = ts_deboornet_init();
	tsReal *result;
	size_t i;
	tsError err;
	TS_TRY(try, err, status)
		*points = (tsReal *) malloc(sof_points);
		if (!*points) {
			TS_THROW_0(try, err, status, TS_MALLOC,
				"out of memory")
		}
		TS_CALL(try, err, ts_int_deboornet_new(
			spline,&net, status))
		for (i = 0; i < num; i++) {
			TS_CALL(try, err, ts_int_bspline_eval_woa(
				spline, us[i], &net, status))
			result = ts_int_deboornet_access_result(&net);
			memcpy((*points) + i * dim, result, sof_point);
		}
	TS_CATCH(err)
		if (*points)
			free(*points);
		*points = NULL;
	TS_FINALLY
		ts_deboornet_free(&net);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_sample(const tsBSpline *spline, size_t num, tsReal **points,
	size_t *actual_num, tsStatus *status)
{
	tsError err;
	tsReal *knots;
	tsReal min, max;
	size_t i;
	if (num == 0)
		num = (ts_bspline_num_control_points(spline) -
			ts_bspline_degree(spline)) * 30;
	*actual_num = num;
	knots = (tsReal *) malloc(num * sizeof(tsReal));
	if (!knots) {
		*points = NULL;
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	}
	ts_bspline_domain(spline, &min, &max);
	for (i = 0; i < num; i++) {
		knots[i] = max - min;
		knots[i] *= (tsReal)i / (num - 1);
		knots[i] += min;
	}
	/* Set knots[0] after knots[num - 1] to ensure that
	 * knots[0] = min if num == 1. */
	knots[num - 1] = max;
	knots[0] = min;
	TS_TRY(try, err, status)
	       TS_CALL(try, err, ts_bspline_eval_all(
			spline, knots, num, points, status))
	TS_FINALLY
		free(knots);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_bisect(const tsBSpline *spline, tsReal value,
	tsReal epsilon, int persnickety, size_t index, int ascending,
	size_t max_iter, tsDeBoorNet *net, tsStatus *status)
{
	tsError err;
	const size_t dim = ts_bspline_dimension(spline);
	const tsReal eps = (tsReal) fabs(epsilon);
	size_t i = 0;
	tsReal dist = 0;
	tsReal min, max, mid;
	tsReal *P;

	ts_int_deboornet_init(net);

	if (dim < index) {
		TS_RETURN_2(status, TS_INDEX_ERROR,
			"dimension (%lu) <= index (%lu)",
			(unsigned long) dim,
			(unsigned long) index)
	}
	if(max_iter == 0)
		TS_RETURN_0(status, TS_NO_RESULT, "0 iterations")

	ts_bspline_domain(spline, &min, &max);
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_int_deboornet_new(
			spline, net, status))
		do {
			mid = (tsReal) ((min + max) / 2.0);
			TS_CALL(try, err, ts_int_bspline_eval_woa(
				spline, mid, net, status))
			P = ts_int_deboornet_access_result(net);
			dist = ts_distance(&P[index], &value, 1);
			if (dist <= eps)
				TS_RETURN_SUCCESS(status)
			if (ascending) {
				if (P[index] < value)
					min = mid;
				else
					max = mid;
			} else {
				if (P[index] < value)
					max = mid;
				else
					min = mid;
			}
		} while (i++ < max_iter);
		if (persnickety) {
			TS_THROW_1(try, err, status, TS_NO_RESULT,
				"maximum iterations (%lu) exceeded",
				(unsigned long) max_iter)
		}
	TS_CATCH(err)
		ts_deboornet_free(net);
	TS_END_TRY_RETURN(err)
}

void ts_bspline_domain(const tsBSpline *spline, tsReal *min, tsReal *max)
{
	*min = ts_int_bspline_access_knots(spline)
		[ts_bspline_degree(spline)];
	*max = ts_int_bspline_access_knots(spline)
		[ts_bspline_num_knots(spline) - ts_bspline_order(spline)];
}

tsError ts_bspline_is_closed(const tsBSpline *spline, tsReal epsilon,
	int *closed, tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t dim = ts_bspline_dimension(spline);
	tsBSpline derivative;
	tsReal min, max;
	tsDeBoorNet first, last;
	size_t i;
	tsError err;

	ts_int_bspline_init(&derivative);
	ts_int_deboornet_init(&first);
	ts_int_deboornet_init(&last);

	TS_TRY(try, err, status)
		for (i = 0; i < deg; i++) {
			TS_CALL(try, err, ts_bspline_derive(
				spline, i, -1.f, &derivative, status))
			ts_bspline_domain(&derivative, &min, &max);
			TS_CALL(try, err, ts_bspline_eval(
				&derivative, min, &first, status))
			TS_CALL(try, err, ts_bspline_eval(
				&derivative, max, &last, status))
			*closed = ts_distance(
				ts_int_deboornet_access_result(&first),
				ts_int_deboornet_access_result(&last),
				dim) <= epsilon ? 1 : 0;
			ts_bspline_free(&derivative);
			ts_deboornet_free(&first);
			ts_deboornet_free(&last);
			if (!*closed)
				TS_RETURN_SUCCESS(status)
		}
	TS_FINALLY
		ts_bspline_free(&derivative);
		ts_deboornet_free(&first);
		ts_deboornet_free(&last);
	TS_END_TRY_RETURN(err)
}



/******************************************************************************
*                                                                             *
* :: Transformation Functions                                                 *
*                                                                             *
******************************************************************************/
tsError ts_int_bspline_resize(const tsBSpline *spline, int n, int back,
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
	const tsReal* from_ctrlp = ts_int_bspline_access_ctrlp(spline);
	const tsReal* from_knots = ts_int_bspline_access_knots(spline);
	tsReal* to_ctrlp = NULL; /**< Pointer to the control points of tmp. */
	tsReal* to_knots = NULL; /**< Pointer to the knots of tmp. */

	tsError err;

	if (n == 0)
		return ts_bspline_copy(spline, _resized_, status);

	INIT_OUT_BSPLINE(spline, _resized_)
	TS_CALL_ROE(err, ts_bspline_new(
		nnum_ctrlp, dim, deg, TS_OPENED, &tmp, status))
	to_ctrlp = ts_int_bspline_access_ctrlp(&tmp);
	to_knots = ts_int_bspline_access_knots(&tmp);

	/* Copy control points and knots. */
	if (!back && n < 0) {
		memcpy(to_ctrlp, from_ctrlp - n*dim, sof_min_num_ctrlp);
		memcpy(to_knots, from_knots - n    , sof_min_num_knots);
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

tsError ts_bspline_derive(const tsBSpline *spline, size_t n, tsReal epsilon,
	tsBSpline *derivative, tsStatus *status)
{
	const size_t sof_real = sizeof(tsReal);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t sof_ctrlp = dim * sof_real;
	size_t deg = ts_bspline_degree(spline);
	size_t num_ctrlp = ts_bspline_num_control_points(spline);
	size_t num_knots = ts_bspline_num_knots(spline);

	tsBSpline worker; /**< Stores the intermediate result. */
	tsReal* ctrlp;    /**< Pointer to the control points of worker. */
	tsReal* knots;    /**< Pointer to the knots of worker. */

	size_t m, i, j, k, l; /**< Used in for loops. */
	tsReal *fst, *snd; /**< Pointer to first and second control point. */
	tsReal dist; /**< Distance between fst and snd. */
	tsReal kid1, ki1; /**< Knots at i+deg+1 and i+1. */
	tsReal span; /**< Distance between kid1 and ki1. */

	tsBSpline swap; /**< Used to swap worker and derivative. */
	tsError err;

	INIT_OUT_BSPLINE(spline, derivative)
	TS_CALL_ROE(err, ts_bspline_copy(spline, &worker, status))
	ctrlp = ts_int_bspline_access_ctrlp(&worker);
	knots = ts_int_bspline_access_knots(&worker);

	TS_TRY(try, err, status)
		for (m = 1; m <= n; m++) { /* from 1st to n'th derivative */
			if (deg == 0) {
				ts_arr_fill(ctrlp, dim, 0.f);
				ts_bspline_domain(spline,
					&knots[0], &knots[1]);
				num_ctrlp = 1;
				num_knots = 2;
				break;
			}
			/* Check and, if possible, fix discontinuity. */
			for (i = 2*deg + 1; i < num_knots - (deg+1); i++) {
				if (!ts_knots_equal(knots[i], knots[i-deg]))
					continue;
				fst = ctrlp + (i - (deg+1)) * dim;
				snd = fst + dim;
				dist = ts_distance(fst, snd, dim);
				if (epsilon >= 0.f && dist > epsilon) {
					TS_THROW_1(try, err, status,
						TS_UNDERIVABLE,
						"discontinuity at knot: %f",
						knots[i])
				}
				memmove(snd, snd + dim,
					(num_ctrlp - (i+1-deg)) * sof_ctrlp);
				memmove(&knots[i], &knots[i+1],
					(num_knots - (i+1)) * sof_real);
				num_ctrlp--;
				num_knots--;
				i += deg-1;
			}
			/* Derive continuous worker. */
			for (i = 0; i < num_ctrlp-1; i++) {
				for (j = 0; j < dim; j++) {
					k = i    *dim + j;
					l = (i+1)*dim + j;
					ctrlp[k] = ctrlp[l] - ctrlp[k];
					kid1 = knots[i+deg+1];
					ki1  = knots[i+1];
					span = kid1 - ki1;
					if (span < TS_KNOT_EPSILON)
						span = TS_KNOT_EPSILON;
					ctrlp[k] *= deg;
					ctrlp[k] /= span;
				}
			}
			deg       -= 1;
			num_ctrlp -= 1;
			num_knots -= 2;
			knots     += 1;
		}
		TS_CALL(try, err, ts_bspline_new(
			num_ctrlp, dim, deg, TS_OPENED, &swap, status))
		memcpy(ts_int_bspline_access_ctrlp(&swap), ctrlp,
		       num_ctrlp * sof_ctrlp);
		memcpy(ts_int_bspline_access_knots(&swap), knots,
		       num_knots * sof_real);
		if (spline == derivative)
			ts_bspline_free(derivative);
		ts_bspline_move(&swap, derivative);
	TS_FINALLY
		ts_bspline_free(&worker);
	TS_END_TRY_RETURN(err)
}

tsError ts_int_bspline_insert_knot(const tsBSpline *spline,
	const tsDeBoorNet *deBoorNet, size_t n, tsBSpline *result,
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
		return ts_bspline_copy(spline, result, status);

	TS_CALL_ROE(err, ts_int_bspline_resize(
		spline, (int)n, 1, result, status))
	ctrlp_spline = ts_int_bspline_access_ctrlp(spline);
	knots_spline = ts_int_bspline_access_knots(spline);
	ctrlp_result = ts_int_bspline_access_ctrlp(result);
	knots_result = ts_int_bspline_access_knots(result);
	num_ctrlp_result = ts_bspline_num_control_points(result);
	num_knots_result = ts_bspline_num_knots(result);

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
	from = ts_int_deboornet_access_points(deBoorNet);
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
	/* N = h+1 with h = deg-s (ts_int_bspline_eval) implies
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

tsError ts_bspline_insert_knot(const tsBSpline *spline, tsReal u, size_t num,
	tsBSpline *result, size_t* k, tsStatus *status)
{
	tsDeBoorNet net;
	tsError err;
	INIT_OUT_BSPLINE(spline, result)
	ts_int_deboornet_init(&net);
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_bspline_eval(spline, u, &net, status))
		TS_CALL(try, err, ts_int_bspline_insert_knot(
			spline, &net, num, result, status))
		ts_deboornet_free(&net);
		TS_CALL(try, err, ts_bspline_eval(result, u, &net, status))
		*k = ts_deboornet_index(&net);
	TS_CATCH(err)
		*k = 0;
	TS_FINALLY
		ts_deboornet_free(&net);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_split(const tsBSpline *spline, tsReal u, tsBSpline *split,
	size_t* k, tsStatus *status)
{
	tsDeBoorNet net;
	tsError err;
	INIT_OUT_BSPLINE(spline, split)
	ts_int_deboornet_init(&net);
	TS_TRY(try, err, status)
		TS_CALL(try, err, ts_bspline_eval(spline, u, &net, status))
		if (ts_deboornet_multiplicity(&net)
				== ts_bspline_order(spline)) {
			TS_CALL(try, err, ts_bspline_copy(
				spline, split, status))
			*k = ts_deboornet_index(&net);
		} else {
			TS_CALL(try, err, ts_int_bspline_insert_knot(
				spline, &net,
				ts_deboornet_num_insertions(&net) + 1, split,
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

tsError ts_bspline_tension(const tsBSpline *spline, tsReal tension,
	tsBSpline *out, tsStatus *status)
{
	const tsReal s  = 1.f - tension; /**< The straightening factor. */
	const size_t dim = ts_bspline_dimension(spline);
	const size_t N = ts_bspline_num_control_points(spline);
	const tsReal* p0 = ts_int_bspline_access_ctrlp(spline);
	const tsReal* pn_1 = p0 + (N-1)*dim;

	tsReal *ctrlp; /**< Pointer to the control points of \p out. */
	size_t i, d; /**< Used in for loops. */
	tsError err;

	TS_CALL_ROE(err, ts_bspline_copy(spline, out, status))
	ctrlp = ts_int_bspline_access_ctrlp(out);

	for (i = 0; i < N; i++) {
		for (d = 0; d < dim; d++) {
			ctrlp[i*dim + d] *= tension;
			ctrlp[i*dim + d] += s * (p0[d] + ((tsReal)i / (N-1)) *
				(pn_1[d] - p0[d]));
		}
	}
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_to_beziers(const tsBSpline *spline, tsBSpline *beziers,
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

	INIT_OUT_BSPLINE(spline, beziers)
	TS_CALL_ROE(err, ts_bspline_copy(spline, &tmp, status))
	knots = ts_int_bspline_access_knots(&tmp);
	num_knots = ts_bspline_num_knots(&tmp);

	TS_TRY(try, err, status)
		/* DO NOT FORGET TO UPDATE knots AND num_knots AFTER EACH
		 * TRANSFORMATION OF tmp! */

		/* Fix first control point if necessary. */
		u_min = knots[deg];
		if (!ts_knots_equal(knots[0], u_min)) {
			TS_CALL(try, err, ts_bspline_split(
				&tmp, u_min, &tmp, &k, status))
			resize = (int)(-1*deg + (deg*2 - k));
			TS_CALL(try, err, ts_int_bspline_resize(
				&tmp, resize, 0, &tmp, status))
			knots = ts_int_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
		}

		/* Fix last control point if necessary. */
		u_max = knots[num_knots - order];
		if (!ts_knots_equal(knots[num_knots - 1], u_max)) {
			TS_CALL(try, err, ts_bspline_split(
				&tmp, u_max, &tmp, &k, status))
			num_knots = ts_bspline_num_knots(&tmp);
			resize = (int)(-1*deg + (k - (num_knots - order)));
			TS_CALL(try, err, ts_int_bspline_resize(
				&tmp, resize, 1, &tmp, status))
			knots = ts_int_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
		}

		/* Split internal knots. */
		k = order;
		while (k < num_knots - order) {
			TS_CALL(try, err, ts_bspline_split(
				&tmp, knots[k], &tmp, &k, status))
			knots = ts_int_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
			k++;
		}

		if (spline == beziers)
			ts_bspline_free(beziers);
		ts_bspline_move(&tmp, beziers);
	TS_FINALLY
		ts_bspline_free(&tmp);
	TS_END_TRY_RETURN(err)
}



/******************************************************************************
*                                                                             *
* :: Serialization and Persistence Functions                                  *
*                                                                             *
******************************************************************************/
tsError ts_int_bspline_to_json(const tsBSpline * spline, JSON_Value **value,
	tsStatus *status)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t len_ctrlp = ts_bspline_len_control_points(spline);
	const size_t len_knots = ts_bspline_num_knots(spline);
	const tsReal *ctrlp = ts_int_bspline_access_ctrlp(spline);
	const tsReal *knots = ts_int_bspline_access_knots(spline);

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
				"out of memory")
		}
		ctrlp_value = json_value_init_array();
		if (!ctrlp_value) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}
		knots_value = json_value_init_array();
		if (!knots_value) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}

		/* Although the following functions can't fail, that is, they
		 * won't return NULL or JSONFailure, we nevertheless handle
		 * unexpected return values. */

		/* Init output. */
		spline_object = json_value_get_object(*value);
		if (!spline_object) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}

		/* Set degree and dimension. */
		if (JSONSuccess != json_object_set_number(
				spline_object, "degree", (double) deg)) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}
		if (JSONSuccess != json_object_set_number(
				spline_object, "dimension", (double) dim)) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}

		/* Set control points. */
		if (JSONSuccess != json_object_set_value(spline_object,
				"control_points", ctrlp_value)) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}
		ctrlp_array = json_array(ctrlp_value);
		if (!ctrlp_array) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}
		for (i = 0; i < len_ctrlp; i++) {
			if (JSONSuccess != json_array_append_number(
					ctrlp_array, (double) ctrlp[i])) {
				TS_THROW_0(values, err, status, TS_MALLOC,
					"out of memory")
			}
		}

		/* Set knots. */
		if (JSONSuccess != json_object_set_value(
				spline_object, "knots", knots_value)) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}
		knots_array = json_array(knots_value);
		if (!knots_array) {
			TS_THROW_0(values, err, status, TS_MALLOC,
				"out of memory")
		}
		for (i = 0; i < len_knots; i++) {
			if (JSONSuccess != json_array_append_number(
					knots_array, (double) knots[i])) {
				TS_THROW_0(values, err, status, TS_MALLOC,
					"out of memory")
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

tsError ts_int_bspline_from_json(const JSON_Value *spline_value,
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

	ts_int_bspline_init(_spline_);

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
		ctrlp = ts_int_bspline_access_ctrlp(_spline_);
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
		knots = ts_int_bspline_access_knots(_spline_);
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

tsError ts_bspline_to_json(const tsBSpline *spline, char **json,
	tsStatus *status)
{
	tsError err;
	JSON_Value *value = NULL;
	*json = NULL;
	TS_CALL_ROE(err, ts_int_bspline_to_json(spline, &value, status))
	*json = json_serialize_to_string_pretty(value);
	json_value_free(value);
	if (!*json)
		TS_RETURN_0(status, TS_MALLOC, "out of memory")
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_from_json(const char *json, tsBSpline *spline,
	tsStatus *status)
{
	tsError err;
	JSON_Value *value = NULL;
	ts_int_bspline_init(spline);
	TS_TRY(try, err, status)
		value = json_parse_string(json);
		if (!value) {
			TS_RETURN_0(status, TS_PARSE_ERROR,
				"invalid json input")
		}
		TS_CALL(try, err, ts_int_bspline_from_json(
			value, spline, status))
	TS_FINALLY
		if (value)
			json_value_free(value);
	TS_END_TRY_RETURN(err)
}

tsError ts_bspline_save(const tsBSpline *spline, const char *path,
	tsStatus *status)
{
	tsError err;
	JSON_Status json_status;
	JSON_Value *value = NULL;
	TS_CALL_ROE(err, ts_int_bspline_to_json(spline, &value, status))
	json_status = json_serialize_to_file_pretty(value, path);
	json_value_free(value);
	if (json_status != JSONSuccess)
		TS_RETURN_0(status, TS_IO_ERROR, "unexpected io error")
	TS_RETURN_SUCCESS(status)
}

tsError ts_bspline_load(const char *path, tsBSpline *spline, tsStatus *status)
{
	tsError err;
	FILE *file = NULL;
	JSON_Value *value = NULL;
	ts_int_bspline_init(spline);
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
		TS_CALL(try, err, ts_int_bspline_from_json(
			value, spline, status))
	TS_FINALLY
		if (file)
			fclose(file);
		if (value)
			json_value_free(value);
	TS_CATCH(err)
		ts_bspline_free(spline);
	TS_END_TRY_RETURN(err)
}



/******************************************************************************
*                                                                             *
* :: Utility Functions                                                        *
*                                                                             *
******************************************************************************/
int ts_knots_equal(tsReal x, tsReal y)
{
	return fabs(x-y) < TS_KNOT_EPSILON ? 1 : 0;
}

void ts_arr_fill(tsReal *arr, size_t num, tsReal val)
{
	size_t i;
	for (i = 0; i < num; i++)
		arr[i] = val;
}

tsReal ts_distance(const tsReal *x, const tsReal *y, size_t dimension)
{
	tsReal sum = 0;
	size_t i;
	for (i = 0; i < dimension; i++)
		sum += (x[i] - y[i]) * (x[i] - y[i]);
	return (tsReal) sqrt(sum);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
