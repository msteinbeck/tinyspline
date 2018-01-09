#include "tinyspline.h"

#include <stdlib.h> /* malloc, free */
#include <math.h> /* fabs, sqrt */
#include <string.h> /* memcpy, memmove, strcmp */
#include <setjmp.h> /* setjmp, longjmp */


/********************************************************
*                                                       *
* Error handling                                        *
*                                                       *
********************************************************/
#define TRY(x, y) y = (tsError) setjmp(x); if (y == 0) {
#define CATCH } else {
#define ETRY }

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
	tsReal *ctrlp; /**< The control points of a spline. */
	tsReal *knots; /**< The Knot vector of a spline. */
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
	tsReal *points; /**< The calculated points of a net. */
	tsReal *result; /**< Points to the result in 'points'. */
};



/******************************************************************************
*                                                                             *
* :: Field Access Functions                                                   *
*                                                                             *
******************************************************************************/
size_t ts_bspline_degree(tsBSpline spline)
{
	return spline.pImpl->deg;
}

tsError ts_bspline_set_degree(tsBSpline spline, size_t deg)
{
	if (deg >= spline.pImpl->n_ctrlp)
		return TS_DEG_GE_NCTRLP;
	spline.pImpl->deg = deg;
	return TS_SUCCESS;
}

size_t ts_bspline_order(tsBSpline spline)
{
	return ts_bspline_degree(spline) + 1;
}

tsError ts_bspline_set_order(tsBSpline spline, size_t order)
{
	return ts_bspline_set_degree(spline, order - 1);
}

size_t ts_bspline_dimension(tsBSpline spline)
{
	return spline.pImpl->dim;
}

tsError ts_bspline_set_dimension(tsBSpline spline, size_t dim)
{
	const size_t len = ts_bspline_len_ctrlp(spline);
	if (dim == 0)
		return TS_DIM_ZERO;
	if (len % dim != 0)
		return TS_LCTRLP_DIM_MISMATCH;
	spline.pImpl->dim = dim;
	return TS_SUCCESS;
}

size_t ts_bspline_len_ctrlp(tsBSpline spline)
{
	return ts_bspline_num_ctrlp(spline) *
		ts_bspline_dimension(spline);
}

size_t ts_bspline_num_ctrlp(tsBSpline spline)
{
	return spline.pImpl->n_ctrlp;
}

size_t ts_bspline_sof_ctrlp(tsBSpline spline)
{
	return ts_bspline_len_ctrlp(spline) *
		sizeof(tsReal);
}

tsReal * ts_bspline_ctrlp(tsBSpline spline)
{
	size_t size;
	tsReal *ctrlp;
	size = ts_bspline_sof_ctrlp(spline);
	ctrlp = malloc(size);
	if (ctrlp)
		memcpy(ctrlp, spline.pImpl->ctrlp, size);
	return ctrlp;
}

tsError ts_bspline_set_ctrlp(tsBSpline spline, const tsReal *ctrlp)
{
	const size_t size = ts_bspline_sof_ctrlp(spline);
	memcpy(spline.pImpl->ctrlp, ctrlp, size);
	return TS_SUCCESS;
}

size_t ts_bspline_num_knots(tsBSpline spline)
{
	return spline.pImpl->n_knots;
}

size_t ts_bspline_sof_knots(tsBSpline spline)
{
	return ts_bspline_num_knots(spline) *
		sizeof(tsReal);
}

tsReal * ts_bspline_knots(tsBSpline spline)
{
	size_t size;
	tsReal *knots;
	size = ts_bspline_sof_knots(spline);
	knots = malloc(size);
	if (knots)
		memcpy(knots, spline.pImpl->ctrlp, size);
	return knots;
}

tsError ts_bspline_set_knots(tsBSpline spline, const tsReal *knots)
{
	size_t order, size, idx, mult;
	tsReal lst_knot, knot;
	order = ts_bspline_order(spline);
	lst_knot = spline.pImpl->knots[0];
	mult = 1;
	for (idx = 1; idx < spline.pImpl->n_knots; idx++)
	{
		knot = knots[idx];
		if (ts_fequals(lst_knot, knot))
			mult++;
		else if (lst_knot > knot)
			return TS_KNOTS_DECR;
		else
			mult = 0;
		if (mult > order)
			return TS_MULTIPLICITY;
		lst_knot = knot;
	}
	size = ts_bspline_sof_knots(spline);
	memcpy(spline.pImpl->knots, knots, size);
	return TS_SUCCESS;
}

/* ------------------------------------------------------------------------- */

tsReal ts_deboornet_knot(tsDeBoorNet net)
{
	return net.pImpl->u;
}

size_t ts_deboornet_index(tsDeBoorNet net)
{
	return net.pImpl->k;
}

size_t ts_deboornet_multiplicity(tsDeBoorNet net)
{
	return net.pImpl->s;
}

size_t ts_deboornet_num_insertions(tsDeBoorNet net)
{
	return net.pImpl->h;
}

size_t ts_deboornet_dimension(tsDeBoorNet net)
{
	return net.pImpl->dim;
}

size_t ts_deboornet_len_points(tsDeBoorNet net)
{
	return ts_deboornet_num_points(net) *
		ts_deboornet_dimension(net);
}

size_t ts_deboornet_num_points(tsDeBoorNet net)
{
	return net.pImpl->n_points;
}

size_t ts_deboornet_sof_points(tsDeBoorNet net)
{
	return ts_deboornet_len_points(net) *
		sizeof(tsReal);
}

tsReal * ts_deboornet_points(tsDeBoorNet net)
{
	size_t size;
	tsReal *points;
	size = ts_deboornet_sof_points(net);
	points = malloc(size);
	if (points)
		memcpy(points, net.pImpl->points, size);
	return points;
}

size_t ts_deboornet_len_result(tsDeBoorNet net)
{
	return ts_deboornet_num_result(net) *
		ts_deboornet_dimension(net);
}

size_t ts_deboornet_num_result(tsDeBoorNet net)
{
	return ts_deboornet_num_points(net) == 2 ? 2 : 1;
}

size_t ts_deboornet_sof_result(tsDeBoorNet net)
{
	return ts_deboornet_len_result(net) *
		sizeof(tsReal);
}

tsReal * ts_deboornet_result(tsDeBoorNet net)
{
	size_t size;
	tsReal *result;
	size = ts_deboornet_sof_result(net);
	result = malloc(size);
	if (result)
		memcpy(result, net.pImpl->result, size);
	return result;
}



/********************************************************
*                                                       *
* Internal functions                                    *
*                                                       *
********************************************************/
void ts_internal_deboornet_copy(
	tsDeBoorNet original,
	tsDeBoorNet* copy, jmp_buf buf
)
{
	const size_t dim = original.pImpl->dim;
	const size_t n_points = original.pImpl->n_points;
	const size_t sof_f = sizeof(tsReal);
	const size_t sof_p = n_points * dim * sof_f;

	if (original.pImpl == copy->pImpl)
		return;

	copy->pImpl->u = original.pImpl->u;
	copy->pImpl->k = original.pImpl->k;
	copy->pImpl->s = original.pImpl->s;
	copy->pImpl->h = original.pImpl->h;
	copy->pImpl->dim = dim;
	copy->pImpl->n_points = n_points;
	copy->pImpl->points = (tsReal*) malloc(sof_p);
	if (copy->pImpl->points == NULL)
		longjmp(buf, TS_MALLOC);
	memcpy(copy->pImpl->points, original.pImpl->points, sof_p);
	copy->pImpl->result = copy->pImpl->points + (n_points-1)*dim;
}

void ts_internal_bspline_find_u(
	tsBSpline bspline, const tsReal u,
	size_t* k, size_t* s, jmp_buf buf
)
{
	const size_t deg = bspline.pImpl->deg;
	const size_t order = bspline.pImpl->deg + 1;
	const size_t n_knots = bspline.pImpl->n_knots;

	*k = *s = 0;
	for (; *k < n_knots; (*k)++) {
		const tsReal uk = bspline.pImpl->knots[*k];
		if (ts_fequals(u, uk)) {
			(*s)++;
		} else if (u < uk) {
			break;
		}
	}

	/* keep in mind that currently k is k+1 */
	if (*s > order)
		longjmp(buf, TS_MULTIPLICITY);
	if (*k <= deg)                /* u < u_min */
		longjmp(buf, TS_U_UNDEFINED);
	if (*k == n_knots && *s == 0) /* u > u_last */
		longjmp(buf, TS_U_UNDEFINED);
	if (*k > n_knots-deg + *s-1)  /* u > u_max */
		longjmp(buf, TS_U_UNDEFINED);

	(*k)--; /* k+1 - 1 will never underflow */
}

void ts_internal_bspline_copy(
	tsBSpline original,
	tsBSpline* copy, jmp_buf buf
)
{
	const size_t dim = original.pImpl->dim;
	const size_t sof_f = sizeof(tsReal);
	const size_t n_ctrlp = original.pImpl->n_ctrlp;
	const size_t n_knots = original.pImpl->n_knots;
	const size_t sof_ck = (n_ctrlp*dim + n_knots) * sof_f;

	/* Nothing to do here. */
	if (original.pImpl == copy->pImpl)
		return;

	copy->pImpl->deg = original.pImpl->deg;
	copy->pImpl->dim = original.pImpl->dim;
	copy->pImpl->n_ctrlp = original.pImpl->n_ctrlp;
	copy->pImpl->n_knots = original.pImpl->n_knots;
	copy->pImpl->ctrlp = (tsReal*) malloc(sof_ck);
	if (copy->pImpl->ctrlp == NULL)
		longjmp(buf, TS_MALLOC);
	memcpy(copy->pImpl->ctrlp, original.pImpl->ctrlp, sof_ck);
	copy->pImpl->knots = copy->pImpl->ctrlp + n_ctrlp*dim;
}

void ts_internal_bspline_fill_knots(
		tsBSpline original, const tsBSplineType type,
		const tsReal min, const tsReal max,
		tsBSpline* result, jmp_buf buf
)
{
	const size_t n_knots = original.pImpl->n_knots;
	const size_t deg = original.pImpl->deg;
	const size_t order = deg+1; /* Using deg+1 instead of original->order
 * ensures order >= 1. */
	tsReal fac; /* The factor used to calculate the knot values. */
	size_t i; /* Used in for loops. */

	/* order >= 1 implies 2*order >= 2 implies n_knots >= 2 */
	if (n_knots < 2*order)
		longjmp(buf, TS_DEG_GE_NCTRLP);
	if (type == TS_BEZIERS && n_knots % order != 0)
		longjmp(buf, TS_NUM_KNOTS);
	if (min > max || ts_fequals(min, max))
		longjmp(buf, TS_KNOTS_DECR);

	/* copy spline even if type is TS_NONE */
	ts_internal_bspline_copy(original, result, buf);

	if (type == TS_OPENED) {
		/* ensures that the first knot value is exactly \min */
		result->pImpl->knots[0] = min; /* n_knots >= 2 */

		fac = (max-min) / (n_knots-1); /* n_knots >= 2 */
		for (i = 1; i < n_knots-1; i++)
			result->pImpl->knots[i] = min + i*fac;

		/* ensure that the last knot value is exactly \max */
		result->pImpl->knots[i] = max;
	} else if (type == TS_CLAMPED) {
		/* n_knots >= 2*order == 2*(deg+1) == 2*deg + 2 > 2*deg - 1 */
		fac = (max-min) / (n_knots - 2*deg - 1);

		ts_arr_fill(result->pImpl->knots, order, min);
		for (i = order ;i < n_knots-order; i++)
			result->pImpl->knots[i] = min + (i-deg)*fac;
		ts_arr_fill(result->pImpl->knots + i, order, max);
	} else if (type == TS_BEZIERS) {
		/* n_knots >= 2*order implies n_knots/order >= 2 */
		fac = (max-min) / (n_knots/order - 1);

		ts_arr_fill(result->pImpl->knots, order, min);
		for (i = order; i < n_knots-order; i += order)
			ts_arr_fill(result->pImpl->knots + i, order, min + (i/order)*fac);
		ts_arr_fill(result->pImpl->knots + i, order, max);
	}
}

void ts_internal_bspline_new(
	const size_t n_ctrlp, const size_t dim, const size_t deg,
	const tsBSplineType type, tsBSpline *bspline, jmp_buf buf
)
{
	const size_t order = deg + 1;
	const size_t n_knots = n_ctrlp + order;
	const size_t sof_f = sizeof(tsReal);
	const size_t sof_ck = (n_ctrlp*dim + n_knots) * sof_f;
	tsError e;
	jmp_buf b;

	if (dim < 1)
		longjmp(buf, TS_DIM_ZERO);
	if (deg >= n_ctrlp)
		longjmp(buf, TS_DEG_GE_NCTRLP);

	bspline->pImpl->deg = deg;
	bspline->pImpl->dim = dim;
	bspline->pImpl->n_ctrlp = n_ctrlp;
	bspline->pImpl->n_knots = n_knots;
	bspline->pImpl->ctrlp = (tsReal*) malloc(sof_ck);
	if (bspline->pImpl->ctrlp == NULL)
		longjmp(buf, TS_MALLOC);
	bspline->pImpl->knots = bspline->pImpl->ctrlp + n_ctrlp*dim;

	TRY(b, e)
		ts_internal_bspline_fill_knots(*bspline, type, 0.f, 1.f, bspline, b);
	CATCH
		free(bspline->pImpl->ctrlp);
		longjmp(buf, e);
	ETRY
}

void ts_internal_bspline_resize(
		tsBSpline bspline, const int n, const int back,
		tsBSpline* resized, jmp_buf buf
)
{
	const size_t deg = bspline.pImpl->deg;
	const size_t dim = bspline.pImpl->dim;
	const size_t sof_f = sizeof(tsReal);
	const size_t sof_c = dim * sof_f;

	const size_t n_ctrlp = bspline.pImpl->n_ctrlp;
	const size_t n_knots = bspline.pImpl->n_knots;
	const size_t nn_ctrlp = n_ctrlp + n; /* The new length of ctrlp. */
	const size_t nn_knots = n_knots + n; /* The new length of knots. */
	const size_t sof_ncnk = (nn_ctrlp*dim + nn_knots) * sof_f;
	const size_t min_n_ctrlp = n < 0 ? nn_ctrlp : n_ctrlp; /* The minimum of
 * the control points old and new size. */
	const size_t min_n_knots = n < 0 ? nn_knots : n_knots; /* the minimum of
 * the knots old and new size. */

	tsReal* from_ctrlp = bspline.pImpl->ctrlp;
	tsReal* from_knots = bspline.pImpl->knots;
	tsReal* to_ctrlp = NULL;
	tsReal* to_knots = NULL;

	/* If n is 0 the spline must not be resized. */
	if (n == 0) {
		ts_internal_bspline_copy(bspline, resized, buf);
		return;
	}

	if (bspline.pImpl != resized->pImpl) {
		ts_internal_bspline_new(nn_ctrlp, dim, deg, TS_NONE, resized, buf);
		to_ctrlp = resized->pImpl->ctrlp;
		to_knots = resized->pImpl->knots;
	} else {
		if (nn_ctrlp <= deg)
			longjmp(buf, TS_DEG_GE_NCTRLP);
		to_ctrlp = (tsReal*) malloc(sof_ncnk);
		if (to_ctrlp == NULL)
			longjmp(buf, TS_MALLOC);
		to_knots = to_ctrlp + nn_ctrlp*dim;
	}

	/* Copy control points and knots. */
	if (!back && n < 0) {
		memcpy(to_ctrlp, from_ctrlp - n*dim, min_n_ctrlp * sof_c);
		memcpy(to_knots, from_knots - n, min_n_knots * sof_f);
	} else if (!back && n > 0) {
		memcpy(to_ctrlp + n*dim, from_ctrlp, min_n_ctrlp * sof_c);
		memcpy(to_knots + n, from_knots, min_n_knots * sof_f);
	} else {
		/* n != 0 implies back == true */
		memcpy(to_ctrlp, from_ctrlp, min_n_ctrlp * sof_c);
		memcpy(to_knots, from_knots, min_n_knots * sof_f);
	}

	/* Cleanup if necessary. */
	if (bspline.pImpl == resized->pImpl) {
		/* free old memory */
		free(from_ctrlp);
		/* assign new values */
		resized->pImpl->ctrlp = to_ctrlp;
		resized->pImpl->knots = to_knots;
		resized->pImpl->n_ctrlp = nn_ctrlp;
		resized->pImpl->n_knots = nn_knots;
	}
}

void ts_internal_bspline_insert_knot(
		tsBSpline bspline, tsDeBoorNet deBoorNet, const size_t n,
		tsBSpline* result, jmp_buf buf
)
{
	const size_t deg = bspline.pImpl->deg;
	const size_t dim = bspline.pImpl->dim;
	const size_t k = deBoorNet.pImpl->k;
	const size_t sof_f = sizeof(tsReal);
	const size_t sof_c = dim * sof_f;
	size_t N; /* The number of affected control points. */
	tsReal* from; /* The pointer to copy the values from. */
	tsReal* to; /* The pointer to copy the values to. */
	int stride; /* The stride of the next pointer to copy. Will be negative
 * later on, thus use int. */
	size_t i; /* Used in for loops. */

	if (deBoorNet.pImpl->s+n > bspline.pImpl->deg + 1) {
		longjmp(buf, TS_MULTIPLICITY);
	}

	/* Use ::ts_bspline_resize even if \n is 0 to copy
	 * the spline if necessary. */
	ts_internal_bspline_resize(bspline, (int)n, 1, result, buf);
	if (n == 0) /* Nothing to insert. */
		return;

	N = deBoorNet.pImpl->h+1; /* n > 0 implies s <= deg implies a regular evaluation
 * implies h+1 is valid. */

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
	memmove(result->pImpl->ctrlp, bspline.pImpl->ctrlp, (k-deg) * sof_c); /* a) */
	from = bspline.pImpl->ctrlp + dim*(k-deg+N);
	to   = result->pImpl->ctrlp  + dim*(k-deg+N+n); /* n >= 0 implies to >= from */
	memmove(to, from, (result->pImpl->n_ctrlp-n-(k-deg+N)) * sof_c); /* b) */
	/* copy knots */
	memmove(result->pImpl->knots, bspline.pImpl->knots, (k+1) * sof_f); /* c) */
	from = bspline.pImpl->knots + k+1;
	to   = result->pImpl->knots  + k+1+n; /* n >= 0 implies to >= from */
	memmove(to, from, (result->pImpl->n_knots-n-(k+1)) * sof_f); /* d) */

	/* 2.
	 *
	 * a) Copy left hand side control points from de boor net.
	 * b) Copy middle part control points from de boor net.
	 * c) Copy right hand side control points from de boor net.
	 * d) Insert knots with u_k. */
	from = deBoorNet.pImpl->points;
	to = result->pImpl->ctrlp + (k-deg)*dim;
	stride = (int)(N*dim);

	/* copy control points */
	for (i = 0; i < n; i++) { /* a) */
		memcpy(to, from, sof_c);
		from += stride;
		to += dim;
		stride -= (int)dim;
	}
	memcpy(to, from, (N-n) * sof_c); /* b) */

	from -= dim;
	to += (N-n)*dim;
	stride = -(int)(N-n+1) * (int)dim; /* N = h+1 with h = deg-s
 * (ts_internal_bspline_evaluate) implies N = deg-s+1 = order-s.
 * n <= order-s implies N-n+1 >= order-s - order-s + 1 = 1. Thus,
 * -(int)(N-n+1) <= -1. */

	for (i = 0; i < n; i++) { /* c) */
		memcpy(to, from, sof_c);
		from += stride;
		stride -= (int)dim;
		to += dim;
	}
	/* copy knots */
	to = result->pImpl->knots+k+1;
	for (i = 0; i < n; i++) { /* d) */
		*to = deBoorNet.pImpl->u;
		to++;
	}
}

void ts_internal_bspline_evaluate(
	tsBSpline bspline, const tsReal u,
	tsDeBoorNet* deBoorNet, jmp_buf buf
)
{
	const size_t deg = bspline.pImpl->deg;
	const size_t order = bspline.pImpl->deg + 1;
	const size_t dim = bspline.pImpl->dim;
	const size_t sof_c = dim * sizeof(tsReal); /* The size of a single
 * control points.*/
	size_t k;
	size_t s;
	tsReal uk; /* The actual used u. */
	size_t from; /* An offset used to copy values. */
	size_t fst; /* The first affected control point, inclusive. */
	size_t lst; /* The last affected control point, inclusive. */
	size_t N; /* The number of affected control points. */
	/* the following indices are used to create the DeBoor net. */
	size_t lidx; /* The current left index. */
	size_t ridx; /* The current right index. */
	size_t tidx; /* The current to index. */
	size_t r, i, d; /* Used in for loop. */
	tsReal ui; /* The knot value at index i. */
	tsReal a, a_hat; /* The weighting factors of the control points. */

	/* Setup the net with its default values. */
	ts_deboornet_default(deBoorNet);

	/* 1. Find index k such that u is in between [u_k, u_k+1).
	 * 2. Setup already known values.
	 * 3. Decide by multiplicity of u how to calculate point P(u). */

	/* 1. */
	ts_internal_bspline_find_u(bspline, u, &k, &s, buf);
	deBoorNet->pImpl->k = k;
	deBoorNet->pImpl->s = s;

	/* 2. */
	uk = bspline.pImpl->knots[k];                    /* Ensures that with any */
	deBoorNet->pImpl->u = ts_fequals(u, uk) ? uk : u; /* tsReal precision the
												* knot vector stays valid. */
	deBoorNet->pImpl->h = deg < s ? 0 : deg-s; /* prevent underflow */
	deBoorNet->pImpl->dim = dim;

	/* 3. (by 1. s <= order)
	 *
	 * 3a) Check for s = order.
	 *     Take the two points k-s and k-s + 1. If one of
	 *     them doesn't exist, take only the other.
	 * 3b) Use de boor algorithm to find point P(u). */
	if (s == order) {
		/* only one of the two control points exists */
		if (k == deg ||                  /* only the first control point */
			k == bspline.pImpl->n_knots - 1) { /* only the last control point */

			deBoorNet->pImpl->points = (tsReal*) malloc(sof_c);
			if (deBoorNet->pImpl->points == NULL)
				longjmp(buf, TS_MALLOC);
			deBoorNet->pImpl->result = deBoorNet->pImpl->points;
			deBoorNet->pImpl->n_points = 1;
			from = k == deg ? 0 : (k-s) * dim;
			memcpy(deBoorNet->pImpl->points, bspline.pImpl->ctrlp + from, sof_c);
		} else {
			deBoorNet->pImpl->points = (tsReal*) malloc(2 * sof_c);
			if (deBoorNet->pImpl->points == NULL)
				longjmp(buf, TS_MALLOC);
			deBoorNet->pImpl->result = deBoorNet->pImpl->points+dim;
			deBoorNet->pImpl->n_points = 2;
			from = (k-s) * dim;
			memcpy(deBoorNet->pImpl->points, bspline.pImpl->ctrlp + from, 2 * sof_c);
		}
	} else { /* by 3a) s <= deg (order = deg+1) */
		fst = k-deg; /* by 1. k >= deg */
		lst = k-s; /* s <= deg <= k */
		N = lst-fst + 1; /* lst <= fst implies N >= 1 */

		deBoorNet->pImpl->n_points = (size_t)(N * (N+1) * 0.5f); /* always fits */
		deBoorNet->pImpl->points = (tsReal*) malloc(deBoorNet->pImpl->n_points * sof_c);
		if (deBoorNet->pImpl->points == NULL)
			longjmp(buf, TS_MALLOC);
		deBoorNet->pImpl->result = deBoorNet->pImpl->points + (deBoorNet->pImpl->n_points-1)*dim;

		/* copy initial values to output */
		memcpy(deBoorNet->pImpl->points, bspline.pImpl->ctrlp + fst*dim, N * sof_c);

		lidx = 0;
		ridx = dim;
		tidx = N*dim; /* N >= 1 implies tidx > 0 */
		r = 1;
		for (;r <= deBoorNet->pImpl->h; r++) {
			i = fst + r;
			for (; i <= lst; i++) {
				ui = bspline.pImpl->knots[i];
				a = (deBoorNet->pImpl->u - ui) / (bspline.pImpl->knots[i+deg-r+1] - ui);
				a_hat = 1.f-a;

				for (d = 0; d < dim; d++) {
					deBoorNet->pImpl->points[tidx++] =
							a_hat * deBoorNet->pImpl->points[lidx++] +
							a * deBoorNet->pImpl->points[ridx++];
				}
			}
			lidx += dim;
			ridx += dim;
		}
	}
}

void ts_internal_bspline_split(
	tsBSpline bspline, const tsReal u,
	tsBSpline* split, size_t* k, jmp_buf buf
)
{
	tsDeBoorNet net;
	tsError e;
	jmp_buf b;

	TRY(b, e)
		ts_internal_bspline_evaluate(bspline, u, &net, b);
		if (net.pImpl->s == bspline.pImpl->deg + 1) {
			ts_internal_bspline_copy(bspline, split, b);
			*k = net.pImpl->k;
		} else {
			ts_internal_bspline_insert_knot(
					bspline, net, net.pImpl->h+1, split, b);
			*k = net.pImpl->k + net.pImpl->h + 1;
		}
	CATCH
		*k = 0;
	ETRY

	ts_deboornet_free(&net);
	if (e < 0)
		longjmp(buf, e);
}

void ts_internal_bspline_thomas_algorithm(
		const tsReal* points, const size_t n, const size_t dim,
		tsReal* output, jmp_buf buf
)
{
	const size_t sof_f = sizeof(tsReal); /* The size of a tsReal. */
	const size_t sof_c = dim * sof_f; /* The size of a single control point. */
	size_t len_m; /* The length m. */
	tsReal* m; /* The array of weights. */
	size_t lst; /* The index of the last control point in \points. */
	size_t i, d; /* Used in for loops. */
	size_t j, k, l; /* Used as temporary indices. */

	/* input validation */
	if (dim == 0)
		longjmp(buf, TS_DIM_ZERO);
	if (n == 0)
		longjmp(buf, TS_DEG_GE_NCTRLP);

	if (n <= 2) {
		memcpy(output, points, n * sof_c);
		return;
	}

	/* In the following n >= 3 applies... */
	len_m = n-2; /* ... len_m >= 1 */
	lst = (n-1)*dim; /* ... lst >= 2*dim */

	/* m_0 = 1/4, m_{k+1} = 1/(4-m_k), for k = 0,...,n-2 */
	m = (tsReal*) malloc(len_m * sof_f);
	if (m == NULL)
		longjmp(buf, TS_MALLOC);
	m[0] = 0.25f;
	for (i = 1; i < len_m; i++)
		m[i] = 1.f/(4 - m[i-1]);

	/* forward sweep */
	ts_arr_fill(output, n*dim, 0.f);
	memcpy(output, points, sof_c);
	memcpy(output+lst, points+lst, sof_c);
	for (d = 0; d < dim; d++) {
		k = dim+d;
		output[k] = 6*points[k];
		output[k] -= points[d];
	}
	for (i = 2; i <= n-2; i++) {
		for (d = 0; d < dim; d++) {
			j = (i-1)*dim+d;
			k = i*dim+d;
			l = (i+1)*dim+d;
			output[k] = 6*points[k];
			output[k] -= output[l];
			output[k] -= m[i-2]*output[j]; /* i >= 2 */
		}
	}

	/* back substitution */
	if (n > 3)
		ts_arr_fill(output+lst, dim, 0.f);
	for (i = n-2; i >= 1; i--) {
		for (d = 0; d < dim; d++) {
			k = i*dim+d;
			l = (i+1)*dim+d;
			/* The following line is the reason why it's important to not fill
			 * output with 0 if n = 3. On the other hand, if n > 3 subtracting
			 * 0 is exactly what we want. */
			output[k] -= output[l];
			output[k] *= m[i-1]; /* i >= 1 */
		}
	}
	if (n > 3)
		memcpy(output+lst, points+lst, sof_c);

	/* we are done */
	free(m);
}

void ts_internal_relaxed_uniform_cubic_bspline(
		const tsReal* points, const size_t n, const size_t dim,
		tsBSpline* bspline, jmp_buf buf
)
{
	const size_t order = 4; /* The order of the spline to interpolate. */
	const tsReal as = 1.f/6.f; /* The value 'a sixth'. */
	const tsReal at = 1.f/3.f; /* The value 'a third'. */
	const tsReal tt = 2.f/3.f; /* The value 'two third'. */
	size_t sof_c; /* The size of a single control point. */
	const tsReal* b = points; /* The array of the b values. */
	tsReal* s; /* The array of the s values. */
	size_t i, d; /* Used in for loops */
	size_t j, k, l; /* Uses as temporary indices. */
	tsError e_;
	jmp_buf b_;

	/* input validation */
	if (dim == 0)
		longjmp(buf, TS_DIM_ZERO);
	if (n <= 1)
		longjmp(buf, TS_DEG_GE_NCTRLP);
	/* in the following n >= 2 applies */

	sof_c = dim * sizeof(tsReal); /* dim > 0 implies sof_c > 0 */

	/* n >= 2 implies n-1 >= 1 implies (n-1)*4 >= 4 */
	ts_internal_bspline_new((n-1)*4, dim, order-1, TS_BEZIERS, bspline, buf);

	TRY(b_, e_)
		s = (tsReal*) malloc(n * sof_c);
		if (s == NULL)
			longjmp(b_, TS_MALLOC);
	CATCH
		ts_bspline_free(bspline);
		longjmp(buf, e_);
	ETRY

	/* set s_0 to b_0 and s_n = b_n */
	memcpy(s, b, sof_c);
	memcpy(s + (n-1)*dim, b + (n-1)*dim, sof_c);

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
			bspline->pImpl->ctrlp[k] = s[j];
			bspline->pImpl->ctrlp[k+dim] = tt*b[j] + at*b[l];
			bspline->pImpl->ctrlp[k+2*dim] = at*b[j] + tt*b[l];
			bspline->pImpl->ctrlp[k+3*dim] = s[l];
		}
	}

	free(s);
}

void ts_internal_bspline_interpolate_cubic(
	const tsReal* points, const size_t n, const size_t dim,
	tsBSpline* bspline, jmp_buf buf
)
{
	tsError e;
	jmp_buf b;
	tsReal* thomas = (tsReal*) malloc(n*dim*sizeof(tsReal));
	if (thomas == NULL)
		longjmp(buf, TS_MALLOC);

	TRY(b, e)
		ts_internal_bspline_thomas_algorithm(points, n, dim, thomas, b);
		ts_internal_relaxed_uniform_cubic_bspline(thomas, n, dim, bspline, b);
	ETRY

	free(thomas);
	if (e < 0)
		longjmp(buf, e);
}

void ts_internal_bspline_derive(
	tsBSpline original,
	tsBSpline* derivative, jmp_buf buf
)
{
	const size_t sof_f = sizeof(tsReal);
	const size_t dim = original.pImpl->dim;
	const size_t deg = original.pImpl->deg;
	const size_t nc = original.pImpl->n_ctrlp;
	const size_t nk = original.pImpl->n_knots;
	tsReal* from_ctrlp = original.pImpl->ctrlp;
	tsReal* from_knots = original.pImpl->knots;
	tsReal* to_ctrlp = NULL;
	tsReal* to_knots = NULL;
	size_t i, j, k;

	if (deg < 1 || nc < 2)
		longjmp(buf, TS_UNDERIVABLE);

	if (original.pImpl != derivative->pImpl) {
		ts_internal_bspline_new(nc-1, dim, deg-1, TS_NONE, derivative, buf);
		to_ctrlp = derivative->pImpl->ctrlp;
		to_knots = derivative->pImpl->knots;
	} else {
		to_ctrlp = (tsReal*) malloc( ((nc-1)*dim + (nk-2)) * sof_f );
		if (to_ctrlp == NULL)
			longjmp(buf, TS_MALLOC);
		to_knots = to_ctrlp + (nc-1)*dim;
	}

	for (i = 0; i < nc-1; i++) {
		for (j = 0; j < dim; j++) {
			if (ts_fequals(from_knots[i+deg+1], from_knots[i+1])) {
				free(to_ctrlp);
				longjmp(buf, TS_UNDERIVABLE);
			} else {
				k = i*dim + j;
				to_ctrlp[k] = from_ctrlp[(i+1)*dim + j] - from_ctrlp[k];
				to_ctrlp[k] *= deg;
				to_ctrlp[k] /= from_knots[i+deg+1] - from_knots[i+1];
			}
		}
	}
	memcpy(to_knots, from_knots+1, (nk-2)*sof_f);

	if (original.pImpl == derivative->pImpl) {
		/* free old memory */
		free(from_ctrlp);
		/* assign new values */
		derivative->pImpl->deg = deg-1;
		derivative->pImpl->n_ctrlp = nc-1;
		derivative->pImpl->n_knots = nk-2;
		derivative->pImpl->ctrlp = to_ctrlp;
		derivative->pImpl->knots = to_knots;
	}
}

void ts_internal_bspline_buckle(
	tsBSpline bspline, const tsReal b,
	tsBSpline* buckled, jmp_buf buf
)
{
	const tsReal b_hat  = 1.f-b; /* The straightening factor. */
	const size_t dim = bspline.pImpl->dim;
	const size_t N = bspline.pImpl->n_ctrlp;
	const tsReal* p0 = bspline.pImpl->ctrlp; /* Pointer to first ctrlp. */
	const tsReal* pn_1 = p0 + (N-1)*dim; /* Pointer to the last ctrlp. */
	size_t i, d; /* Used in for loops. */

	ts_internal_bspline_copy(bspline, buckled, buf);

	for (i = 0; i < N; i++) {
		for (d = 0; d < dim; d++) {
			buckled->pImpl->ctrlp[i*dim + d] =
				b * buckled->pImpl->ctrlp[i*dim + d] +
				b_hat * (p0[d] + ((tsReal)i / (N-1)) * (pn_1[d] - p0[d]));
		}
	}
}

void ts_internal_bspline_to_beziers(
	tsBSpline bspline,
	tsBSpline* beziers, jmp_buf buf
)
{
	tsError e;
	jmp_buf b;
	const size_t deg = bspline.pImpl->deg;
	const size_t order = bspline.pImpl->deg + 1;
	tsBSpline tmp;
	int resize; /* The number of control points to add/remove. */
	size_t k; /* The index of the splitted knot value. */
	tsReal u_min; /* The minimum of the knot values. */
	tsReal u_max; /* The maximum of the knot values. */

	ts_internal_bspline_copy(bspline, &tmp, buf);

	TRY(b, e)
		/* fix first control point if necessary */
		u_min = tmp.pImpl->knots[deg];
		if (!ts_fequals(tmp.pImpl->knots[0], u_min)) {
			ts_internal_bspline_split(tmp, u_min, &tmp, &k, b);
			resize = (int)(-1*deg + (deg*2 - k));
			ts_internal_bspline_resize(tmp, resize, 0, &tmp, b);
		}

		/* fix last control point if necessary */
		u_max = tmp.pImpl->knots[tmp.pImpl->n_knots - order];
		if (!ts_fequals(tmp.pImpl->knots[tmp.pImpl->n_knots-1], u_max)) {
			ts_internal_bspline_split(tmp, u_max, &tmp, &k, b);
			resize = (int)(-1*deg + (k - (tmp.pImpl->n_knots - order)));
			ts_internal_bspline_resize(tmp, resize, 1, &tmp, b);
		}

		k = order;
		while (k < tmp.pImpl->n_knots - order) {
			ts_internal_bspline_split(tmp, tmp.pImpl->knots[k], &tmp, &k, b);
			k++;
		}

		if (bspline.pImpl == beziers->pImpl)
			ts_bspline_free(beziers);
		ts_bspline_move(&tmp, beziers);
	ETRY

	ts_bspline_free(&tmp);
	if (e < 0)
		longjmp(buf, e);
}

void ts_internal_bspline_set_ctrlp(
	tsBSpline bspline, const tsReal* ctrlp,
	tsBSpline* result, jmp_buf buf
)
{
	const size_t s = bspline.pImpl->n_ctrlp * bspline.pImpl->dim * sizeof(tsReal);
	ts_internal_bspline_copy(bspline, result, buf);
	memmove(result->pImpl->ctrlp, ctrlp, s);
}

void ts_internal_bspline_set_knots(
	tsBSpline bspline, const tsReal* knots,
	tsBSpline* result, jmp_buf buf
)
{
	const size_t s = bspline.pImpl->n_knots * sizeof(tsReal);
	ts_internal_bspline_copy(bspline, result, buf);
	memmove(result->pImpl->knots, knots, s);
}


/********************************************************
*                                                       *
* Interface implementation                              *
*                                                       *
********************************************************/
void ts_deboornet_default(tsDeBoorNet* deBoorNet)
{
	deBoorNet->u        = 0.f;
	deBoorNet->k        = 0;
	deBoorNet->s        = 0;
	deBoorNet->h        = 0;
	deBoorNet->dim      = 0;
	deBoorNet->n_points = 0;
	deBoorNet->points   = NULL;
	deBoorNet->result   = NULL;
}

void ts_deboornet_move(tsDeBoorNet *from, tsDeBoorNet *to)
{
	if (from == to)
		return;
	to->u = from->u;
	to->k = from->k;
	to->s = from->s;
	to->h = from->h;
	to->dim = from->dim;
	to->n_points = from->n_points;
	to->points = from->points;
	to->result = from->result;
	ts_deboornet_default(from);
}

void ts_deboornet_free(tsDeBoorNet* deBoorNet)
{
	if (deBoorNet->points != NULL)
		free(deBoorNet->points);/* automatically frees the field result */
	ts_deboornet_default(deBoorNet);
}

void ts_bspline_default(tsBSpline* bspline)
{
	bspline->deg     = 0;
	bspline->order   = 0;
	bspline->dim     = 0;
	bspline->n_ctrlp = 0;
	bspline->n_knots = 0;
	bspline->ctrlp   = NULL;
	bspline->knots   = NULL;
}

void ts_bspline_free(tsBSpline* bspline)
{
	if (bspline->ctrlp != NULL)
		free(bspline->ctrlp);
	ts_bspline_default(bspline);
}

void ts_bspline_move(tsBSpline* from, tsBSpline* to)
{
	if (from == to)
		return;
	to->deg = from->deg;
	to->order = from->order;
	to->dim = from->dim;
	to->n_ctrlp = from->n_ctrlp;
	to->n_knots = from->n_knots;
	to->ctrlp = from->ctrlp;
	to->knots = from->knots;
	ts_bspline_default(from);
}

tsError ts_bspline_new(
	size_t n_ctrlp, size_t dim, size_t deg, tsBSplineType type,
	tsBSpline *bspline
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_new(n_ctrlp, dim, deg, type, bspline, buf);
	CATCH
		ts_bspline_default(bspline);
	ETRY
	return err;
}

tsError ts_bspline_interpolate_cubic(
	const tsReal* points, size_t n, size_t dim,
	tsBSpline* bspline
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_interpolate_cubic(points, n, dim, bspline, buf);
	CATCH
		ts_bspline_default(bspline);
	ETRY
	return err;
}

tsError ts_bspline_derive(
	const tsBSpline* original,
	tsBSpline* derivative
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_derive(original, derivative, buf);
	CATCH
		if (original != derivative)
			ts_bspline_default(derivative);
	ETRY
	return err;
}

tsError ts_deboornet_copy(
	const tsDeBoorNet* original,
	tsDeBoorNet* copy
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_deboornet_copy(original, copy, buf);
	CATCH
		if (original != copy)
			ts_deboornet_default(copy);
	ETRY
	return err;
}

tsError ts_bspline_copy(
	const tsBSpline* original,
	tsBSpline* copy
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_copy(original, copy, buf);
	CATCH
		if (original != copy)
			ts_bspline_default(copy);
	ETRY
	return err;
}

tsError ts_bspline_set_ctrlp(
	const tsBSpline* bspline, const tsReal* ctrlp,
	tsBSpline* result
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_set_ctrlp(bspline, ctrlp, result, buf);
	CATCH
		if (bspline != result)
			ts_bspline_default(result);
	ETRY
	return err;
}

tsError ts_bspline_set_knots(
	const tsBSpline* bspline, const tsReal* knots,
	tsBSpline* result
)
{
	tsError err;
	jmp_buf  buf;
	TRY(buf, err)
		ts_internal_bspline_set_knots(bspline, knots, result, buf);
	CATCH
		if (bspline != result)
			ts_bspline_default(result);
	ETRY
	return err;
}

tsError ts_bspline_fill_knots(
	const tsBSpline* original, tsBSplineType type, tsReal min, tsReal max,
	tsBSpline* result
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_fill_knots(original, type, min, max, result, buf);
	CATCH
		if (original != result)
			ts_bspline_default(result);
	ETRY
	return err;
}

tsError ts_bspline_evaluate(
	const tsBSpline* bspline, tsReal u,
	tsDeBoorNet* deBoorNet
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_evaluate(bspline, u, deBoorNet, buf);
	CATCH
		ts_deboornet_default(deBoorNet);
	ETRY
	return err;
}

tsError ts_bspline_insert_knot(
	const tsBSpline* bspline, tsReal u, size_t n,
	tsBSpline* result, size_t* k
)
{
	tsDeBoorNet net;
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_evaluate(bspline, u, &net, buf);
		ts_internal_bspline_insert_knot(bspline, &net, n, result, buf);
		*k = net.k+n;
	CATCH
		if (bspline != result)
			ts_bspline_default(result);
		*k = 0;
	ETRY

	ts_deboornet_free(&net);
	return err;
}

tsError ts_bspline_resize(
	const tsBSpline* bspline, int n, int back,
	tsBSpline* resized
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_resize(bspline, n, back, resized, buf);
	CATCH
		if (bspline != resized)
			ts_bspline_default(resized);
	ETRY
	return err;
}

tsError ts_bspline_split(
	const tsBSpline* bspline, tsReal u,
	tsBSpline* split, size_t* k
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_split(bspline, u, split, k, buf);
	CATCH
		if (bspline != split)
			ts_bspline_default(split);
	ETRY
	return err;
}

tsError ts_bspline_buckle(
	const tsBSpline* bspline, tsReal b,
	tsBSpline* buckled
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_buckle(bspline, b, buckled, buf);
	CATCH
		if (bspline != buckled)
			ts_bspline_default(buckled);
	ETRY
	return err;
}

tsError ts_bspline_to_beziers(
	const tsBSpline* bspline,
	tsBSpline* beziers
)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_to_beziers(bspline, beziers, buf);
	CATCH
		if (bspline != beziers)
			ts_bspline_default(beziers);
	ETRY
	return err;
}

int ts_fequals(tsReal x, tsReal y)
{
	if (fabs(x-y) <= FLT_MAX_ABS_ERROR) {
		return 1;
	} else {
		const tsReal r = (tsReal)fabs(x) > (tsReal)fabs(y) ?
			(tsReal)fabs((x-y) / x) : (tsReal)fabs((x-y) / y);
		return r <= FLT_MAX_REL_ERROR;
	}
}

const char* ts_enum_str(tsError err)
{
	if (err == TS_MALLOC)
		return "malloc failed";
	else if (err == TS_DIM_ZERO)
		return "dim == 0";
	else if (err == TS_DEG_GE_NCTRLP)
		return "deg >= #ctrlp";
	else if (err == TS_U_UNDEFINED)
		return "spline is undefined at given u";
	else if (err == TS_MULTIPLICITY)
		return "s > order";
	else if (err == TS_KNOTS_DECR)
		return "decreasing knot vector";
	else if (err == TS_NUM_KNOTS)
		return "unexpected number of knots";
	else if (err == TS_UNDERIVABLE)
		return "spline is not derivable";
	return "unknown error";
}

tsError ts_str_enum(const char* str)
{
	if (!strcmp(str, ts_enum_str(TS_MALLOC)))
		return TS_MALLOC;
	else if (!strcmp(str, ts_enum_str(TS_DIM_ZERO)))
		return TS_DIM_ZERO;
	else if (!strcmp(str, ts_enum_str(TS_DEG_GE_NCTRLP)))
		return TS_DEG_GE_NCTRLP;
	else if (!strcmp(str, ts_enum_str(TS_U_UNDEFINED)))
		return TS_U_UNDEFINED;
	else if (!strcmp(str, ts_enum_str(TS_MULTIPLICITY)))
		return TS_MULTIPLICITY;
	else if (!strcmp(str, ts_enum_str(TS_KNOTS_DECR)))
		return TS_KNOTS_DECR;
	else if (!strcmp(str, ts_enum_str(TS_NUM_KNOTS)))
		return TS_NUM_KNOTS;
	else if (!strcmp(str, ts_enum_str(TS_UNDERIVABLE)))
		return TS_UNDERIVABLE;
	return TS_SUCCESS;
}

void ts_arr_fill(tsReal *arr, size_t num, tsReal val)
{
	size_t i;
	for (i = 0; i < num; i++)
		arr[i] = val;
}

tsReal ts_ctrlp_dist2(
	const tsReal *x, const tsReal *y, size_t dim
)
{
	tsReal sum = 0;
	size_t i;
	for (i = 0; i < dim; i++)
		sum += (x[i] - y[i]) * (x[i] - y[i]);
	return (tsReal) sqrt(sum);
}
