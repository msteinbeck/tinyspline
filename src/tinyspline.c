#include "tinyspline.h"

#include <stdlib.h> /* malloc, free */
#include <math.h> /* fabs, sqrt */
#include <string.h> /* memcpy, memmove, strcmp */
#include <setjmp.h> /* setjmp, longjmp */



/******************************************************************************
*                                                                             *
* :: Error handling                                                           *
*                                                                             *
******************************************************************************/
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
* :: Forward Declarations & Internal Utility Functions                        *
*                                                                             *
******************************************************************************/
void ts_internal_bspline_fill_knots(const tsBSpline *spline,
	tsBSplineType type, tsReal min, tsReal max, tsBSpline *_result_,
	jmp_buf buf);

size_t ts_internal_bspline_sof_state(const tsBSpline *spline)
{
	return sizeof(struct tsBSplineImpl) +
	       ts_bspline_sof_control_points(spline) +
	       ts_bspline_sof_knots(spline);
}

size_t ts_internal_deboornet_sof_state(const tsDeBoorNet *net)
{
	return sizeof(struct tsDeBoorNetImpl) +
	       ts_deboornet_sof_points(net) +
	       ts_deboornet_sof_result(net);
}

void ts_internal_bspline_find_u(const tsBSpline *spline, tsReal u, size_t *k,
	size_t *s, jmp_buf buf)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = ts_bspline_order(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);

	*k = *s = 0;
	for (; *k < num_knots; (*k)++) {
		const tsReal uk = spline->pImpl->knots[*k];
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
	if (*k == num_knots && *s == 0) /* u > u_last */
		longjmp(buf, TS_U_UNDEFINED);
	if (*k > num_knots-deg + *s-1)  /* u > u_max */
		longjmp(buf, TS_U_UNDEFINED);

	(*k)--; /* k+1 - 1 will never underflow */
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

tsError ts_bspline_set_degree(tsBSpline *spline, size_t deg)
{
	if (deg >= ts_bspline_num_control_points(spline))
		return TS_DEG_GE_NCTRLP;
	spline->pImpl->deg = deg;
	return TS_SUCCESS;
}

size_t ts_bspline_order(const tsBSpline *spline)
{
	return ts_bspline_degree(spline) + 1;
}

tsError ts_bspline_set_order(tsBSpline *spline, size_t order)
{
	return ts_bspline_set_degree(spline, order - 1);
}

size_t ts_bspline_dimension(const tsBSpline *spline)
{
	return spline->pImpl->dim;
}

tsError ts_bspline_set_dimension(tsBSpline *spline, size_t dim)
{
	if (dim == 0)
		return TS_DIM_ZERO;
	if (ts_bspline_len_control_points(spline) % dim != 0)
		return TS_LCTRLP_DIM_MISMATCH;
	spline->pImpl->dim = dim;
	return TS_SUCCESS;
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

tsReal * ts_bspline_control_points(const tsBSpline *spline)
{
	size_t size;
	tsReal *ctrlp;
	size = ts_bspline_sof_control_points(spline);
	ctrlp = malloc(size);
	if (ctrlp)
		memcpy(ctrlp, spline->pImpl->ctrlp, size);
	return ctrlp;
}

tsError ts_bspline_set_control_points(tsBSpline *spline, const tsReal *ctrlp)
{
	const size_t size = ts_bspline_sof_control_points(spline);
	memcpy(spline->pImpl->ctrlp, ctrlp, size);
	return TS_SUCCESS;
}

size_t ts_bspline_num_knots(const tsBSpline *spline)
{
	return spline->pImpl->n_knots;
}

size_t ts_bspline_sof_knots(const tsBSpline *spline)
{
	return ts_bspline_num_knots(spline) * sizeof(tsReal);
}

tsReal * ts_bspline_knots(const tsBSpline *spline)
{
	size_t size;
	tsReal *knots;
	size = ts_bspline_sof_knots(spline);
	knots = malloc(size);
	if (knots)
		memcpy(knots, spline->pImpl->knots, size);
	return knots;
}

tsError ts_bspline_set_knots(tsBSpline *spline, const tsReal *knots)
{
	size_t order, size, idx, mult;
	tsReal lst_knot, knot;
	order = ts_bspline_order(spline);
	lst_knot = spline->pImpl->knots[0];
	mult = 1;
	for (idx = 1; idx < ts_bspline_num_knots(spline); idx++)
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
	memcpy(spline->pImpl->knots, knots, size);
	return TS_SUCCESS;
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

tsReal * ts_deboornet_points(const tsDeBoorNet *net)
{
	size_t size;
	tsReal *points;
	size = ts_deboornet_sof_points(net);
	points = malloc(size);
	if (points)
		memcpy(points, net->pImpl->points, size);
	return points;
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

tsReal * ts_deboornet_result(const tsDeBoorNet *net)
{
	size_t size;
	tsReal *result;
	size = ts_deboornet_sof_result(net);
	result = malloc(size);
	if (result)
		memcpy(result, net->pImpl->result, size);
	return result;
}



/******************************************************************************
*                                                                             *
* :: Constructors, Destructors, Copy, and Move Functions                      *
*                                                                             *
******************************************************************************/
void ts_bspline_default(tsBSpline *_spline_)
{
	_spline_->pImpl = NULL;
}

void ts_internal_bspline_new(size_t n_ctrlp, size_t dim, size_t deg,
	tsBSplineType type, tsBSpline *_spline_, jmp_buf buf)
{
	const size_t order = deg + 1;
	const size_t num_knots = n_ctrlp + order;
	const size_t len_ctrlp = n_ctrlp * dim;

	const size_t sof_real = sizeof(tsReal);
	const size_t sof_impl = sizeof(struct tsBSplineImpl);
	const size_t sof_ctrlp_vec = len_ctrlp * sof_real;
	const size_t sof_knots_vec = num_knots * sof_real;
	const size_t sof_spline = sof_impl + sof_ctrlp_vec + sof_knots_vec;

	tsError e;
	jmp_buf b;

	if (dim < 1)
		longjmp(buf, TS_DIM_ZERO);
	if (deg >= n_ctrlp)
		longjmp(buf, TS_DEG_GE_NCTRLP);

	_spline_->pImpl = (struct tsBSplineImpl *) malloc(sof_spline);
	if (!_spline_->pImpl)
		longjmp(buf, TS_MALLOC);

	_spline_->pImpl->deg = deg;
	_spline_->pImpl->dim = dim;
	_spline_->pImpl->n_ctrlp = n_ctrlp;
	_spline_->pImpl->n_knots = num_knots;
	_spline_->pImpl->ctrlp = (tsReal *) (& _spline_->pImpl[1]);
	_spline_->pImpl->knots = _spline_->pImpl->ctrlp + len_ctrlp;

	TRY(b, e)
		ts_internal_bspline_fill_knots(
			_spline_, type, 0.f, 1.f, _spline_, b);
	CATCH
		ts_bspline_free(_spline_);
		longjmp(buf, e);
	ETRY
}

tsError ts_bspline_new(size_t n_ctrlp, size_t dim, size_t deg,
	tsBSplineType type, tsBSpline *_spline_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_new(
			n_ctrlp, dim, deg, type, _spline_, buf);
	CATCH
		ts_bspline_default(_spline_);
	ETRY
	return err;
}

void ts_internal_bspline_copy(const tsBSpline *original, tsBSpline *_copy_,
	jmp_buf buf)
{
	size_t size;
	if (original->pImpl == _copy_->pImpl)
		return;
	size = ts_internal_bspline_sof_state(original);
	_copy_->pImpl = malloc(size);
	if (!_copy_->pImpl)
		longjmp(buf, TS_MALLOC);
	memcpy(_copy_->pImpl, original->pImpl, size);
}

tsError ts_bspline_copy(const tsBSpline *original, tsBSpline *_copy_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_copy(original, _copy_, buf);
	CATCH
		if (original->pImpl != _copy_->pImpl)
			ts_bspline_default(_copy_);
	ETRY
	return err;
}

void ts_bspline_move(tsBSpline *from, tsBSpline *_to_)
{
	if (from == _to_)
		return;
	_to_->pImpl = from->pImpl;
	ts_bspline_default(from);
}

void ts_bspline_free(tsBSpline *_spline_)
{
	if (_spline_->pImpl)
		free(_spline_->pImpl);
	ts_bspline_default(_spline_);
}

/* ------------------------------------------------------------------------- */

void ts_deboornet_default(tsDeBoorNet *_deBoorNet_)
{
	_deBoorNet_->pImpl = NULL;
}

void ts_internal_deboornet_new(const tsBSpline *spline,
	tsDeBoorNet *_deBoorNet_, jmp_buf buf)
{
	const size_t dim = ts_bspline_dimension(spline);
	const size_t deg = ts_bspline_degree(spline);
	const size_t num_points = (size_t)(deg * (deg+1) * 0.5f);

	const size_t sof_real = sizeof(tsReal);
	const size_t sof_impl = sizeof(struct tsDeBoorNetImpl);
	const size_t sof_points_vec = num_points * dim * sof_real;
	const size_t sof_net = sof_impl * sof_points_vec;

	_deBoorNet_->pImpl = (struct tsDeBoorNetImpl *) malloc(sof_net);
	if (!_deBoorNet_->pImpl)
		longjmp(buf, TS_MALLOC);

	_deBoorNet_->pImpl->u = 0.f;
	_deBoorNet_->pImpl->k = 0;
	_deBoorNet_->pImpl->s = 0;
	_deBoorNet_->pImpl->h = deg;
	_deBoorNet_->pImpl->dim = dim;
	_deBoorNet_->pImpl->n_points = num_points;
	_deBoorNet_->pImpl->points = (tsReal *) (& _deBoorNet_->pImpl[1]);
	_deBoorNet_->pImpl->result = _deBoorNet_->pImpl->points +
		(num_points-1)*dim; /* last point in result */
}

void ts_deboornet_free(tsDeBoorNet *_deBoorNet_)
{
	if (_deBoorNet_->pImpl)
		free(_deBoorNet_->pImpl);
	ts_deboornet_default(_deBoorNet_);
}

void ts_internal_deboornet_copy(const tsDeBoorNet *original,
	tsDeBoorNet *_copy, jmp_buf buf)
{
	size_t size;
	if (original->pImpl == _copy->pImpl)
		return;
	size = ts_internal_deboornet_sof_state(original);
	_copy->pImpl = malloc(size);
	if (!_copy->pImpl)
		longjmp(buf, TS_MALLOC);
	memcpy(_copy->pImpl, original->pImpl, size);
}

tsError ts_deboornet_copy(const tsDeBoorNet *original, tsDeBoorNet *_copy_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_deboornet_copy(original, _copy_, buf);
	CATCH
		if (original->pImpl != _copy_->pImpl)
			ts_deboornet_default(_copy_);
	ETRY
	return err;
}

void ts_deboornet_move(tsDeBoorNet *from, tsDeBoorNet *_to_)
{
	if (from == _to_)
		return;
	_to_->pImpl = from->pImpl;
	ts_deboornet_default(from);
}



/******************************************************************************
*                                                                             *
* :: Interpolation and Approximation Functions                                *
*                                                                             *
******************************************************************************/
void ts_internal_bspline_thomas_algorithm(const tsReal *points, size_t n,
	size_t dim, tsReal *_result_, jmp_buf buf)
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
		longjmp(buf, TS_DIM_ZERO);
	if (n == 0)
		longjmp(buf, TS_DEG_GE_NCTRLP);

	if (n <= 2) {
		memcpy(_result_, points, n * sof_ctrlp);
		return;
	}

	/* In the following n >= 3 applies... */
	len_m = n-2; /* ... len_m >= 1 */
	lst = (n-1)*dim; /* ... lst >= 2*dim */

	/* m_0 = 1/4, m_{k+1} = 1/(4-m_k), for k = 0,...,n-2 */
	m = (tsReal*) malloc(len_m * sof_real);
	if (m == NULL)
		longjmp(buf, TS_MALLOC);
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
}

void ts_internal_relaxed_uniform_cubic_bspline(const tsReal *points, size_t n,
	size_t dim, tsBSpline *_spline_, jmp_buf buf)
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

	tsError e_;
	jmp_buf b_;

	/* input validation */
	if (dim == 0)
		longjmp(buf, TS_DIM_ZERO);
	if (n <= 1)
		longjmp(buf, TS_DEG_GE_NCTRLP);
	/* in the following n >= 2 applies */

	sof_ctrlp = dim * sizeof(tsReal); /* dim > 0 implies sof_ctrlp > 0 */

	/* n >= 2 implies n-1 >= 1 implies (n-1)*4 >= 4 */
	ts_internal_bspline_new(
		(n-1)*4, dim, order-1, TS_BEZIERS, _spline_, buf);

	TRY(b_, e_)
		s = (tsReal*) malloc(n * sof_ctrlp);
		if (!s)
			longjmp(b_, TS_MALLOC);
	CATCH
		ts_bspline_free(_spline_);
		longjmp(buf, e_);
	ETRY

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
			_spline_->pImpl->ctrlp[k] = s[j];
			_spline_->pImpl->ctrlp[k+dim] = tt*b[j] + at*b[l];
			_spline_->pImpl->ctrlp[k+2*dim] = at*b[j] + tt*b[l];
			_spline_->pImpl->ctrlp[k+3*dim] = s[l];
		}
	}

	/* we are done */
	free(s);
}

void ts_internal_bspline_interpolate_cubic(const tsReal *points, size_t n,
	size_t dim, tsBSpline *_spline_, jmp_buf buf)
{
	tsError e;
	jmp_buf b;

	tsReal* thomas = (tsReal*) malloc(n*dim*sizeof(tsReal));
	if (!thomas)
		longjmp(buf, TS_MALLOC);

	TRY(b, e)
		ts_internal_bspline_thomas_algorithm(
			points, n, dim, thomas, b);
		ts_internal_relaxed_uniform_cubic_bspline(
			thomas, n, dim, _spline_, b);
	ETRY

	free(thomas);
	if (e < 0)
		longjmp(buf, e);
}

tsError ts_bspline_interpolate_cubic(const tsReal *points, size_t n,
	size_t dim, tsBSpline *_spline_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_interpolate_cubic(
			points, n, dim, _spline_, buf);
	CATCH
		ts_bspline_default(_spline_);
	ETRY
	return err;
}



/******************************************************************************
*                                                                             *
* :: Query Functions                                                          *
*                                                                             *
******************************************************************************/
void ts_internal_bspline_eval(const tsBSpline *spline, tsReal u,
	tsDeBoorNet *_deBoorNet_, jmp_buf buf)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = ts_bspline_order(spline);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	const size_t sof_ctrlp = dim * sizeof(tsReal);

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

	/* Setup the net with its default values. */
	ts_internal_deboornet_new(spline, _deBoorNet_, buf);

	/* 1. Find index k such that u is in between [u_k, u_k+1).
	 * 2. Setup already known values.
	 * 3. Decide by multiplicity of u how to calculate point P(u). */

	/* 1. */
	ts_internal_bspline_find_u(spline, u, &k, &s, buf);

	/* 2. */
	uk = spline->pImpl->knots[k]; /* Ensures that with any precision of  */
	_deBoorNet_->pImpl->u =       /* tsReal the knot vector stays valid. */
		ts_fequals(u, uk) ? uk : u;
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
			memcpy(_deBoorNet_->pImpl->points,
			       spline->pImpl->ctrlp + from,
			       sof_ctrlp);
			_deBoorNet_->pImpl->result =
				_deBoorNet_->pImpl->points;
		} else {
			from = (k-s) * dim;
			_deBoorNet_->pImpl->n_points = 2;
			memcpy(_deBoorNet_->pImpl->points,
			       spline->pImpl->ctrlp + from,
			       2 * sof_ctrlp);
			_deBoorNet_->pImpl->result =
				_deBoorNet_->pImpl->points+dim;
		}
	} else { /* by 3a) s <= deg (order = deg+1) */
		fst = k-deg; /* by 1. k >= deg */
		lst = k-s; /* s <= deg <= k */
		N = lst-fst + 1; /* lst <= fst implies N >= 1 */

		_deBoorNet_->pImpl->n_points = (size_t)(N * (N+1) * 0.5f);
		_deBoorNet_->pImpl->result = _deBoorNet_->pImpl->points +
			(_deBoorNet_->pImpl->n_points-1)*dim;

		/* copy initial values to output */
		memcpy(_deBoorNet_->pImpl->points,
		       spline->pImpl->ctrlp + fst*dim,
		       N * sof_ctrlp);

		lidx = 0;
		ridx = dim;
		tidx = N*dim; /* N >= 1 implies tidx > 0 */
		r = 1;
		for (;r <= ts_deboornet_num_insertions(_deBoorNet_); r++) {
			i = fst + r;
			for (; i <= lst; i++) {
				ui = spline->pImpl->knots[i];
				a = (ts_deboornet_knot(_deBoorNet_) - ui) /
					(spline->pImpl->knots[i+deg-r+1] - ui);
				a_hat = 1.f-a;

				for (d = 0; d < dim; d++) {
					_deBoorNet_->pImpl->points[tidx++] =
						a_hat * _deBoorNet_->pImpl->points[lidx++] +
						a * _deBoorNet_->pImpl->points[ridx++];
				}
			}
			lidx += dim;
			ridx += dim;
		}
	}
}

tsError ts_bspline_eval(const tsBSpline *spline, tsReal u,
	tsDeBoorNet *_deBoorNet_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_eval(spline, u, _deBoorNet_, buf);
	CATCH
		ts_deboornet_default(_deBoorNet_);
	ETRY
	return err;
}



/******************************************************************************
*                                                                             *
* :: Transformation Functions                                                 *
*                                                                             *
******************************************************************************/
void ts_internal_bspline_derive(const tsBSpline *spline,
	tsBSpline *_derivative_, jmp_buf buf)
{
	const size_t sof_f = sizeof(tsReal);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t deg = ts_bspline_degree(spline);
	const size_t nc = ts_bspline_num_control_points(spline);
	const size_t nk = ts_bspline_num_knots(spline);
	tsReal* from_ctrlp = spline->pImpl->ctrlp;
	tsReal* from_knots = spline->pImpl->knots;
	tsReal* to_ctrlp = NULL;
	tsReal* to_knots = NULL;
	size_t i, j, k;

	if (deg < 1 || nc < 2)
		longjmp(buf, TS_UNDERIVABLE);

	if (spline->pImpl != _derivative_->pImpl) {
		ts_internal_bspline_new(
			nc-1, dim, deg-1, TS_NONE, _derivative_, buf);
		to_ctrlp = _derivative_->pImpl->ctrlp;
		to_knots = _derivative_->pImpl->knots;
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

	if (spline->pImpl == _derivative_->pImpl) {
		/* free old memory */
		free(from_ctrlp);
		/* assign new values */
		_derivative_->pImpl->deg = deg-1;
		_derivative_->pImpl->n_ctrlp = nc-1;
		_derivative_->pImpl->n_knots = nk-2;
		_derivative_->pImpl->ctrlp = to_ctrlp;
		_derivative_->pImpl->knots = to_knots;
	}
}

tsError ts_bspline_derive(const tsBSpline *spline, tsBSpline *_derivative_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_derive(spline, _derivative_, buf);
	CATCH
		if (spline->pImpl != _derivative_->pImpl)
			ts_bspline_default(_derivative_);
	ETRY
	return err;
}

void ts_internal_bspline_fill_knots(const tsBSpline *spline,
	tsBSplineType type, tsReal min, tsReal max, tsBSpline *_result_,
	jmp_buf buf)
{
	const size_t n_knots = ts_bspline_num_knots(spline);
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = deg + 1; /**< ensures order >= 1. */
	tsReal fac; /**< Factor used to calculate the knot values. */
	size_t i; /**< Used in for loops. */

	/* order >= 1 implies 2*order >= 2 implies n_knots >= 2 */
	if (n_knots < 2*order)
		longjmp(buf, TS_DEG_GE_NCTRLP);
	if (type == TS_BEZIERS && n_knots % order != 0)
		longjmp(buf, TS_NUM_KNOTS);
	if (min > max || ts_fequals(min, max))
		longjmp(buf, TS_KNOTS_DECR);

	/* copy spline even if type is TS_NONE */
	ts_internal_bspline_copy(spline, _result_, buf);

	if (type == TS_OPENED) {
		/* ensures that the first knot value is exactly \min */
		_result_->pImpl->knots[0] = min; /* n_knots >= 2 */

		fac = (max-min) / (n_knots-1); /* n_knots >= 2 */
		for (i = 1; i < n_knots-1; i++)
			_result_->pImpl->knots[i] = min + i*fac;

		/* ensure that the last knot value is exactly \max */
		_result_->pImpl->knots[i] = max;
	} else if (type == TS_CLAMPED) {
		/* n_knots >= 2*order == 2*(deg+1) == 2*deg + 2 > 2*deg - 1 */
		fac = (max-min) / (n_knots - 2*deg - 1);

		ts_arr_fill(_result_->pImpl->knots, order, min);
		for (i = order ;i < n_knots-order; i++)
			_result_->pImpl->knots[i] = min + (i-deg)*fac;
		ts_arr_fill(_result_->pImpl->knots + i, order, max);
	} else if (type == TS_BEZIERS) {
		/* n_knots >= 2*order implies n_knots/order >= 2 */
		fac = (max-min) / (n_knots/order - 1);

		ts_arr_fill(_result_->pImpl->knots, order, min);
		for (i = order; i < n_knots-order; i += order)
			ts_arr_fill(_result_->pImpl->knots + i,
				    order, min + (i/order)*fac);
		ts_arr_fill(_result_->pImpl->knots + i, order, max);
	}
}

tsError ts_bspline_fill_knots(const tsBSpline *spline, tsBSplineType type,
	tsReal min, tsReal max, tsBSpline *_result_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_fill_knots(
			spline, type, min, max, _result_, buf);
	CATCH
		if (spline->pImpl != _result_->pImpl)
			ts_bspline_default(_result_);
	ETRY
	return err;
}

void ts_internal_bspline_resize(const tsBSpline *spline, int n, int back,
	tsBSpline *_resized_, jmp_buf buf)
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
	tsBSpline tmp;

	tsReal* from_ctrlp = spline->pImpl->ctrlp;
	tsReal* from_knots = spline->pImpl->knots;
	tsReal* to_ctrlp = NULL;
	tsReal* to_knots = NULL;

	/* If n is 0 the spline must not be resized. */
	if (n == 0) {
		ts_internal_bspline_copy(spline, _resized_, buf);
		return;
	}

	ts_internal_bspline_new(nnum_ctrlp, dim, deg, TS_NONE, &tmp, buf);
	to_ctrlp = tmp.pImpl->ctrlp;
	to_knots = tmp.pImpl->knots;

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

	if (spline->pImpl == _resized_->pImpl)
		ts_bspline_free(_resized_);
	ts_bspline_move(&tmp, _resized_);
}

tsError ts_bspline_resize(const tsBSpline *spline, int n, int back,
	tsBSpline *_resized_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_resize(spline, n, back, _resized_, buf);
	CATCH
		if (spline->pImpl != _resized_->pImpl)
			ts_bspline_default(_resized_);
	ETRY
	return err;
}

void ts_internal_bspline_insert_knot(const tsBSpline *spline,
	const tsDeBoorNet *deBoorNet, size_t n, tsBSpline *_result_,
	jmp_buf buf)
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

	size_t num_ctrlp_result;
	size_t num_knots_result;

	if (s+n > ts_bspline_order(spline)) {
		longjmp(buf, TS_MULTIPLICITY);
	}

	/* Use ::ts_bspline_resize even if \p n is 0 to copy the spline if
	 * necessary. */
	ts_internal_bspline_resize(spline, (int)n, 1, _result_, buf);
	if (n == 0) /* Nothing to insert. */
		return;
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
	memmove(_result_->pImpl->ctrlp,                    /* a) */
		spline->pImpl->ctrlp,
		(k-deg) * sof_ctrlp);
	from = spline->pImpl->ctrlp + dim*(k-deg+N);
	/* n >= 0 implies to >= from */
	to = _result_->pImpl->ctrlp  + dim*(k-deg+N+n);
	memmove(to,                                        /* b) */
		from,
		(num_ctrlp_result-n-(k-deg+N)) * sof_ctrlp);
	/* copy knots */
	memmove(_result_->pImpl->knots,                    /* c) */
		spline->pImpl->knots,
		(k+1) * sof_real);
	from = spline->pImpl->knots + k+1;
	/* n >= 0 implies to >= from */
	to   = _result_->pImpl->knots  + k+1+n;
	memmove(to,                                        /* d) */
		from,
		(num_knots_result-n-(k+1)) * sof_real);

	/* 2.
	 *
	 * a) Copy left hand side control points from de boor net.
	 * b) Copy middle part control points from de boor net.
	 * c) Copy right hand side control points from de boor net.
	 * d) Insert knots with u_k. */
	from = deBoorNet->pImpl->points;
	to = _result_->pImpl->ctrlp + (k-deg)*dim;
	stride = (int)(N*dim);

	/* copy control points */
	for (i = 0; i < n; i++) {                          /* a) */
		memcpy(to, from, sof_ctrlp);
		from += stride;
		to += dim;
		stride -= (int)dim;
	}
	memcpy(to, from, (N-n) * sof_ctrlp);               /* b) */

	from -= dim;
	to += (N-n)*dim;
	/* N = h+1 with h = deg-s (ts_internal_bspline_eval) implies
	 * N = deg-s+1 = order-s. n <= order-s implies
	 * N-n+1 >= order-s - order-s + 1 = 1. Thus, -(int)(N-n+1) <= -1. */
	stride = -(int)(N-n+1) * (int)dim;

	for (i = 0; i < n; i++) {                          /* c) */
		memcpy(to, from, sof_ctrlp);
		from += stride;
		stride -= (int)dim;
		to += dim;
	}
	/* copy knots */
	to = _result_->pImpl->knots+k+1;
	for (i = 0; i < n; i++) {                          /* d) */
		*to = ts_deboornet_knot(deBoorNet);
		to++;
	}
}

tsError ts_bspline_insert_knot(const tsBSpline *spline, tsReal u, size_t n,
	tsBSpline *_result_, size_t* k)
{
	tsDeBoorNet net;
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_eval(spline, u, &net, buf);
		ts_internal_bspline_insert_knot(
			spline, &net, n, _result_, buf);
		*k = ts_deboornet_index(&net) + n;
	CATCH
		if (spline->pImpl != _result_->pImpl)
			ts_bspline_default(_result_);
		*k = 0;
	ETRY

	ts_deboornet_free(&net);
	return err;
}

void ts_internal_bspline_split(const tsBSpline *spline, tsReal u,
	tsBSpline *_split_, size_t* k, jmp_buf buf)
{
	tsDeBoorNet net;
	tsError e;
	jmp_buf b;

	TRY(b, e)
		ts_internal_bspline_eval(spline, u, &net, b);
		if (ts_deboornet_multiplicity(&net)
		    		== ts_bspline_order(spline)) {
			ts_internal_bspline_copy(spline, _split_, b);
			*k = ts_deboornet_index(&net);
		} else {
			ts_internal_bspline_insert_knot(spline, &net,
				ts_deboornet_num_insertions(&net) + 1,
				_split_, b);
			*k = ts_deboornet_index(&net) +
				ts_deboornet_num_insertions(&net) + 1;
		}
	CATCH
		*k = 0;
	ETRY

	ts_deboornet_free(&net);
	if (e < 0)
		longjmp(buf, e);
}

tsError ts_bspline_split(const tsBSpline *spline, tsReal u, tsBSpline *_split_,
	size_t* k)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_split(spline, u, _split_, k, buf);
	CATCH
		if (spline->pImpl != _split_->pImpl)
			ts_bspline_default(_split_);
	ETRY
	return err;
}

void ts_internal_bspline_buckle(const tsBSpline *spline, tsReal b,
	tsBSpline *_buckled_, jmp_buf buf)
{
	const tsReal b_hat  = 1.f-b; /**< The straightening factor. */
	const size_t dim = ts_bspline_dimension(spline);
	const size_t N = ts_bspline_num_control_points(spline);
	const tsReal* p0 = spline->pImpl->ctrlp; /**< First ctrlp. */
	const tsReal* pn_1 = p0 + (N-1)*dim; /**< Last ctrlp. */
	size_t i, d; /**< Used in for loops. */

	ts_internal_bspline_copy(spline, _buckled_, buf);

	for (i = 0; i < N; i++) {
		for (d = 0; d < dim; d++) {
			_buckled_->pImpl->ctrlp[i*dim + d] =
				b * _buckled_->pImpl->ctrlp[i*dim + d] +
				b_hat * (p0[d] + ((tsReal)i / (N-1)) * (pn_1[d] - p0[d]));
		}
	}
}

tsError ts_bspline_buckle(const tsBSpline *spline, tsReal b,
	tsBSpline *_buckled_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_buckle(spline, b, _buckled_, buf);
	CATCH
		if (spline->pImpl != _buckled_->pImpl)
			ts_bspline_default(_buckled_);
	ETRY
	return err;
}

void ts_internal_bspline_to_beziers(const tsBSpline *spline,
	tsBSpline *_beziers_, jmp_buf buf)
{
	tsError e;
	jmp_buf b;
	const size_t deg = ts_bspline_degree(spline);
	const size_t order = ts_bspline_order(spline);
	tsBSpline tmp;
	int resize;   /* Number of control points to add/remove. */
	size_t k;     /* Index of the split knot value. */
	tsReal u_min; /* Minimum of the knot values. */
	tsReal u_max; /* Maximum of the knot values. */

	ts_internal_bspline_copy(spline, &tmp, buf);

	TRY(b, e)
		/* fix first control point if necessary */
		u_min = tmp.pImpl->knots[deg];
		if (!ts_fequals(tmp.pImpl->knots[0], u_min)) {
			ts_internal_bspline_split(&tmp, u_min, &tmp, &k, b);
			resize = (int)(-1*deg + (deg*2 - k));
			ts_internal_bspline_resize(&tmp, resize, 0, &tmp, b);
		}

		/* fix last control point if necessary */
		u_max = tmp.pImpl->knots[ts_bspline_num_knots(&tmp) - order];
		if (!ts_fequals(tmp.pImpl->knots[ts_bspline_num_knots(&tmp) - 1], u_max)) {
			ts_internal_bspline_split(&tmp, u_max, &tmp, &k, b);
			resize = (int)(-1*deg + (k - (ts_bspline_num_knots(&tmp) - order)));
			ts_internal_bspline_resize(&tmp, resize, 1, &tmp, b);
		}

		k = order;
		while (k < ts_bspline_num_knots(&tmp) - order) {
			ts_internal_bspline_split(
				&tmp, tmp.pImpl->knots[k], &tmp, &k, b);
			k++;
		}

		if (spline->pImpl == _beziers_->pImpl)
			ts_bspline_free(_beziers_);
		ts_bspline_move(&tmp, _beziers_);
	ETRY

	ts_bspline_free(&tmp);
	if (e < 0)
		longjmp(buf, e);
}

tsError ts_bspline_to_beziers(const tsBSpline *spline, tsBSpline *_beziers_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_to_beziers(spline, _beziers_, buf);
	CATCH
		if (spline->pImpl != _beziers_->pImpl)
			ts_bspline_default(_beziers_);
	ETRY
	return err;
}



/******************************************************************************
*                                                                             *
* :: Utility Functions                                                        *
*                                                                             *
******************************************************************************/
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

tsError ts_str_enum(const char *str)
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

tsReal ts_ctrlp_dist2(const tsReal *x, const tsReal *y, size_t dim)
{
	tsReal sum = 0;
	size_t i;
	for (i = 0; i < dim; i++)
		sum += (x[i] - y[i]) * (x[i] - y[i]);
	return (tsReal) sqrt(sum);
}
