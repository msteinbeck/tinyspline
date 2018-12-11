#include "tinyspline.h"
#include "parson.h" /* serialization */

#include <stdlib.h> /* malloc, free */
#include <math.h>   /* fabs, sqrt */
#include <string.h> /* memcpy, memmove, strcmp */
#include <setjmp.h> /* setjmp, longjmp */
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
void ts_internal_bspline_fill_knots(const tsBSpline *spline,
	tsBSplineType type, tsReal min, tsReal max, tsBSpline *_result_,
	jmp_buf buf);

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

void ts_internal_bspline_find_u(const tsBSpline *spline, tsReal u, size_t *k,
	size_t *s, jmp_buf buf)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	const tsReal *knots = ts_internal_bspline_access_knots(spline);

	*k = *s = 0;
	for (; *k < num_knots; (*k)++) {
		const tsReal uk = knots[*k];
		if (ts_fequals(u, uk)) {
			(*s)++;
		} else if (u < uk) {
			break;
		}
	}

	/* keep in mind that currently k is k+1 */
	if (*k <= deg)                /* u < u_min */
		longjmp(buf, TS_U_UNDEFINED);
	if (*k == num_knots && *s == 0) /* u > u_last */
		longjmp(buf, TS_U_UNDEFINED);
	if (*k > num_knots-deg + *s-1)  /* u > u_max */
		longjmp(buf, TS_U_UNDEFINED);

	(*k)--; /* k+1 - 1 will never underflow */
}

tsReal * ts_internal_bspline_access_ctrlp_at(const tsBSpline *spline,
	size_t index, jmp_buf buf)
{
	if (index >= ts_bspline_num_control_points(spline))
		longjmp(buf, TS_INDEX_ERROR);
	return ts_internal_bspline_access_ctrlp(spline) +
	       index * ts_bspline_dimension(spline);
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

tsError ts_bspline_control_points(const tsBSpline *spline, tsReal **ctrlp)
{
	const size_t size = ts_bspline_sof_control_points(spline);
	*ctrlp = (tsReal*) malloc(size);
	if (!*ctrlp) {
		return TS_MALLOC;
	} else {
		memcpy(*ctrlp,
		       ts_internal_bspline_access_ctrlp(spline),
		       size);
		return TS_SUCCESS;
	}
}

tsError ts_bspline_control_point_at(const tsBSpline *spline, size_t index,
	tsReal **ctrlp)
{

	const size_t size = ts_bspline_dimension(spline) * sizeof(tsReal);
	tsReal * to_copy;
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		to_copy = ts_internal_bspline_access_ctrlp_at(
			spline, index, buf);
		*ctrlp = (tsReal*) malloc(size);
		if (!*ctrlp)
			longjmp(buf, TS_MALLOC);
		memcpy(*ctrlp, to_copy, size);
	CATCH
		*ctrlp = NULL;
	ETRY
	return err;
}

tsError ts_bspline_set_control_points(tsBSpline *spline, const tsReal *ctrlp)
{
	const size_t size = ts_bspline_sof_control_points(spline);
	memmove(ts_internal_bspline_access_ctrlp(spline), ctrlp, size);
	return TS_SUCCESS;
}

tsError ts_bspline_set_control_point_at(tsBSpline *spline, size_t index,
	const tsReal *ctrlp)
{
	const size_t size = ts_bspline_dimension(spline) * sizeof(tsReal);
	tsReal * target;
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		target = ts_internal_bspline_access_ctrlp_at(
			spline, index, buf);
		memcpy(target, ctrlp, size);
	ETRY
	return err;
}

size_t ts_bspline_num_knots(const tsBSpline *spline)
{
	return spline->pImpl->n_knots;
}

size_t ts_bspline_sof_knots(const tsBSpline *spline)
{
	return ts_bspline_num_knots(spline) * sizeof(tsReal);
}

tsError ts_bspline_knots(const tsBSpline *spline, tsReal **knots)
{
	const size_t size = ts_bspline_sof_knots(spline);
	*knots = (tsReal*) malloc(size);
	if (!*knots) {
		return TS_MALLOC;
	} else {
		memcpy(*knots,
		       ts_internal_bspline_access_knots(spline),
		       size);
		return TS_SUCCESS;
	}
}

tsError ts_bspline_set_knots(tsBSpline *spline, const tsReal *knots)
{
	size_t order, size, idx, mult;
	tsReal lst_knot, knot;
	order = ts_bspline_order(spline);
	lst_knot = ts_internal_bspline_access_knots(spline)[0];
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
	memmove(ts_internal_bspline_access_knots(spline), knots, size);
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

tsError ts_deboornet_points(const tsDeBoorNet *net, tsReal **points)
{
	const size_t size = ts_deboornet_sof_points(net);
	*points = (tsReal*) malloc(size);
	if (!*points) {
		return TS_MALLOC;
	} else {
		memcpy(*points,
		       ts_internal_deboornet_access_points(net),
		       size);
		return TS_SUCCESS;
	}
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

tsError ts_deboornet_result(const tsDeBoorNet *net, tsReal **result)
{
	const size_t size = ts_deboornet_sof_result(net);
	*result = (tsReal*) malloc(size);
	if (!*result) {
		return TS_MALLOC;
	} else {
		memcpy(*result,
		       ts_internal_deboornet_access_result(net),
		       size);
		return TS_SUCCESS;
	}
}



/******************************************************************************
*                                                                             *
* :: Constructors, Destructors, Copy, and Move Functions                      *
*                                                                             *
******************************************************************************/
void ts_internal_bspline_new(size_t num_control_points, size_t dimension,
	size_t degree, tsBSplineType type, tsBSpline *_spline_, jmp_buf buf)
{
	const size_t order = degree + 1;
	const size_t num_knots = num_control_points + order;
	const size_t len_ctrlp = num_control_points * dimension;

	const size_t sof_real = sizeof(tsReal);
	const size_t sof_impl = sizeof(struct tsBSplineImpl);
	const size_t sof_ctrlp_vec = len_ctrlp * sof_real;
	const size_t sof_knots_vec = num_knots * sof_real;
	const size_t sof_spline = sof_impl + sof_ctrlp_vec + sof_knots_vec;

	tsError e;
	jmp_buf b;

	if (dimension < 1)
		longjmp(buf, TS_DIM_ZERO);
	if (degree >= num_control_points)
		longjmp(buf, TS_DEG_GE_NCTRLP);

	_spline_->pImpl = (struct tsBSplineImpl *) malloc(sof_spline);
	if (!_spline_->pImpl)
		longjmp(buf, TS_MALLOC);

	_spline_->pImpl->deg = degree;
	_spline_->pImpl->dim = dimension;
	_spline_->pImpl->n_ctrlp = num_control_points;
	_spline_->pImpl->n_knots = num_knots;

	TRY(b, e)
		ts_internal_bspline_fill_knots(
			_spline_, type, 0.f, 1.f, _spline_, b);
	CATCH
		ts_bspline_free(_spline_);
		longjmp(buf, e);
	ETRY
}

tsError ts_bspline_new(size_t num_control_points, size_t dimension,
	size_t degree, tsBSplineType type, tsBSpline *_spline_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_new(
			num_control_points, dimension,
			degree, type, _spline_, buf);
	CATCH
		ts_internal_bspline_init(_spline_);
	ETRY
	return err;
}

void ts_internal_bspline_copy(const tsBSpline *original, tsBSpline *_copy_,
	jmp_buf buf)
{
	size_t size;
	if (original == _copy_)
		return;
	size = ts_internal_bspline_sof_state(original);
	_copy_->pImpl = (struct tsBSplineImpl *) malloc(size);
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
		if (original != _copy_)
			ts_internal_bspline_init(_copy_);
	ETRY
	return err;
}

void ts_bspline_move(tsBSpline *from, tsBSpline *_to_)
{
	if (from == _to_)
		return;
	_to_->pImpl = from->pImpl;
	ts_internal_bspline_init(from);
}

void ts_bspline_free(tsBSpline *_spline_)
{
	if (_spline_->pImpl)
		free(_spline_->pImpl);
	ts_internal_bspline_init(_spline_);
}

/* ------------------------------------------------------------------------- */

void ts_internal_deboornet_new(const tsBSpline *spline,
	tsDeBoorNet *_deBoorNet_, jmp_buf buf)
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
		longjmp(buf, TS_MALLOC);

	_deBoorNet_->pImpl->u = 0.f;
	_deBoorNet_->pImpl->k = 0;
	_deBoorNet_->pImpl->s = 0;
	_deBoorNet_->pImpl->h = deg;
	_deBoorNet_->pImpl->dim = dim;
	_deBoorNet_->pImpl->n_points = fixed_num_points;
}

void ts_deboornet_free(tsDeBoorNet *_net_)
{
	if (_net_->pImpl)
		free(_net_->pImpl);
	ts_internal_deboornet_init(_net_);
}

void ts_internal_deboornet_copy(const tsDeBoorNet *original,
	tsDeBoorNet *_copy, jmp_buf buf)
{
	size_t size;
	if (original == _copy)
		return;
	size = ts_internal_deboornet_sof_state(original);
	_copy->pImpl = (struct tsDeBoorNetImpl *) malloc(size);
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
		if (original != _copy_)
			ts_internal_deboornet_init(_copy_);
	ETRY
	return err;
}

void ts_deboornet_move(tsDeBoorNet *from, tsDeBoorNet *_to_)
{
	if (from == _to_)
		return;
	_to_->pImpl = from->pImpl;
	ts_internal_deboornet_init(from);
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

	tsReal *ctrlp; /**< Pointer to the control points of \p _spline_. */

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
	ctrlp = ts_internal_bspline_access_ctrlp(_spline_);

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
			ctrlp[k] = s[j];
			ctrlp[k+dim] = tt*b[j] + at*b[l];
			ctrlp[k+2*dim] = at*b[j] + tt*b[l];
			ctrlp[k+3*dim] = s[l];
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
		ts_internal_bspline_init(_spline_);
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

	/* Setup the net with its default values. */
	ts_internal_deboornet_new(spline, _deBoorNet_, buf);
	points = ts_internal_deboornet_access_points(_deBoorNet_);

	/* 1. Find index k such that u is in between [u_k, u_k+1).
	 * 2. Setup already known values.
	 * 3. Decide by multiplicity of u how to calculate point P(u). */

	/* 1. */
	ts_internal_bspline_find_u(spline, u, &k, &s, buf);

	/* 2. */
	uk = knots[k];          /* Ensures that with any precision of  */
	_deBoorNet_->pImpl->u = /* tsReal the knot vector stays valid. */
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
}

tsError ts_bspline_eval(const tsBSpline *spline, tsReal u,
	tsDeBoorNet *_deBoorNet_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_eval(spline, u, _deBoorNet_, buf);
	CATCH
		ts_internal_deboornet_init(_deBoorNet_);
	ETRY
	return err;
}

tsReal ts_bspline_domain_min(const tsBSpline *spline)
{
	return ts_internal_bspline_access_knots(spline)
		[ts_bspline_degree(spline)];
}

tsReal ts_bspline_domain_max(const tsBSpline *spline)
{
	return ts_internal_bspline_access_knots(spline)
		[ts_bspline_num_knots(spline) - ts_bspline_order(spline)];
}

tsError ts_bspline_is_closed(const tsBSpline *spline, tsReal epsilon,
	int *closed)
{
	const tsReal min = ts_bspline_domain_min(spline);
	const tsReal max = ts_bspline_domain_max(spline);
	const size_t dim = ts_bspline_dimension(spline);
	tsDeBoorNet first, last;
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_deboornet_init(&first);
		ts_internal_deboornet_init(&last);
		ts_internal_bspline_eval(spline, min, &first, buf);
		ts_internal_bspline_eval(spline, max, &last, buf);
		*closed = ts_ctrlp_dist2(
			ts_internal_deboornet_access_result(&first),
			ts_internal_deboornet_access_result(&last),
			dim) <= epsilon ? 1 : 0;
	CATCH
	ETRY
	ts_deboornet_free(&first);
	ts_deboornet_free(&last);
	return err;
}



/******************************************************************************
*                                                                             *
* :: Transformation Functions                                                 *
*                                                                             *
******************************************************************************/
void ts_internal_bspline_derive(const tsBSpline *spline, size_t n,
	tsBSpline *_derivative_, jmp_buf buf)
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
	jmp_buf b;
	tsError err;

	ts_internal_bspline_copy(spline, &worker, buf);
	ctrlp = ts_internal_bspline_access_ctrlp(&worker);
	knots = ts_internal_bspline_access_knots(&worker);

	for (m = 1; m <= n; m++) { /* from 1st to n'th derivative */
		if (deg == 0) {
			ts_arr_fill(ctrlp, num_ctrlp * dim, 0.f);
			break;
		} else {
			for (i = 0; i < num_ctrlp-1; i++) {
				for (j = 0; j < dim; j++) {
					if (ts_fequals(knots[i+deg+1], knots[i+1])) {
						ts_bspline_free(&worker);
						longjmp(buf, TS_UNDERIVABLE);
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

	TRY(b, err)
		ts_internal_bspline_new(num_ctrlp, dim, deg, TS_NONE, &swap, b);
	CATCH
		ts_bspline_free(&worker);
		longjmp(buf, err);
	ETRY
	memcpy(ts_internal_bspline_access_ctrlp(&swap), ctrlp,
	       num_ctrlp * dim * sof_real);
	memcpy(ts_internal_bspline_access_knots(&swap), knots,
	       num_knots * sof_real);
	ts_bspline_free(&worker);
	if (spline == _derivative_)
		ts_bspline_free(_derivative_);
	ts_bspline_move(&swap, _derivative_);
}

tsError ts_bspline_derive(const tsBSpline *spline, size_t n,
	tsBSpline *_derivative_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_derive(spline, n, _derivative_, buf);
	CATCH
		if (spline != _derivative_)
			ts_internal_bspline_init(_derivative_);
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

	tsReal *knots; /**< Pointer to the knots of \p _result_. */

	/* order >= 1 implies 2*order >= 2 implies n_knots >= 2 */
	if (type == TS_BEZIERS && n_knots % order != 0)
		longjmp(buf, TS_NUM_KNOTS);
	if (min > max || ts_fequals(min, max))
		longjmp(buf, TS_KNOTS_DECR);

	/* copy spline even if type is TS_NONE */
	ts_internal_bspline_copy(spline, _result_, buf);
	knots = ts_internal_bspline_access_knots(_result_);

	if (type == TS_OPENED) {
		/* ensures that the first knot value is exactly \min */
		knots[0] = min; /* n_knots >= 2 */

		fac = (max-min) / (n_knots-1); /* n_knots >= 2 */
		for (i = 1; i < n_knots-1; i++)
			knots[i] = min + i*fac;

		/* ensure that the last knot value is exactly \max */
		knots[i] = max;
	} else if (type == TS_CLAMPED) {
		/* n_knots >= 2*order == 2*(deg+1) == 2*deg + 2 > 2*deg - 1 */
		fac = (max-min) / (n_knots - 2*deg - 1);

		ts_arr_fill(knots, order, min);
		for (i = order ;i < n_knots-order; i++)
			knots[i] = min + (i-deg)*fac;
		ts_arr_fill(knots + i, order, max);
	} else if (type == TS_BEZIERS) {
		/* n_knots >= 2*order implies n_knots/order >= 2 */
		fac = (max-min) / (n_knots/order - 1);

		ts_arr_fill(knots, order, min);
		for (i = order; i < n_knots-order; i += order)
			ts_arr_fill(knots + i,
				    order, min + (i/order)*fac);
		ts_arr_fill(knots + i, order, max);
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
		if (spline != _result_)
			ts_internal_bspline_init(_result_);
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

	tsBSpline tmp; /**< Temporarily stores the result. */
	const tsReal* from_ctrlp = ts_internal_bspline_access_ctrlp(spline);
	const tsReal* from_knots = ts_internal_bspline_access_knots(spline);
	tsReal* to_ctrlp = NULL; /**< Pointer to the control points of tmp. */
	tsReal* to_knots = NULL; /**< Pointer to the knots of tmp. */

	/* If n is 0 the spline must not be resized. */
	if (n == 0) {
		ts_internal_bspline_copy(spline, _resized_, buf);
		return;
	}

	ts_internal_bspline_new(nnum_ctrlp, dim, deg, TS_NONE, &tmp, buf);
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
}

tsError ts_bspline_resize(const tsBSpline *spline, int n, int back,
	tsBSpline *_resized_)
{
	tsError err;
	jmp_buf buf;
	TRY(buf, err)
		ts_internal_bspline_resize(spline, n, back, _resized_, buf);
	CATCH
		if (spline != _resized_)
			ts_internal_bspline_init(_resized_);
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

	tsReal *ctrlp_spline, *ctrlp_result;
	tsReal *knots_spline, *knots_result;
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
		if (spline != _result_)
			ts_internal_bspline_init(_result_);
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
		if (spline != _split_)
			ts_internal_bspline_init(_split_);
	ETRY
	return err;
}

void ts_internal_bspline_buckle(const tsBSpline *spline, tsReal b,
	tsBSpline *_buckled_, jmp_buf buf)
{
	const tsReal b_hat  = 1.f-b; /**< The straightening factor. */
	const size_t dim = ts_bspline_dimension(spline);
	const size_t N = ts_bspline_num_control_points(spline);
	const tsReal* p0 = ts_internal_bspline_access_ctrlp(spline);
	const tsReal* pn_1 = p0 + (N-1)*dim;

	tsReal *ctrlp; /**< Pointer to the control points of \p _buckled_. */
	size_t i, d; /**< Used in for loops. */

	ts_internal_bspline_copy(spline, _buckled_, buf);
	ctrlp = ts_internal_bspline_access_ctrlp(_buckled_);

	for (i = 0; i < N; i++) {
		for (d = 0; d < dim; d++) {
			ctrlp[i*dim + d] =
				b     * ctrlp[i*dim + d] +
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
		if (spline != _buckled_)
			ts_internal_bspline_init(_buckled_);
	ETRY
	return err;
}

void ts_internal_bspline_to_beziers(const tsBSpline *spline,
	tsBSpline *_beziers_, jmp_buf buf)
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

	tsError e;
	jmp_buf b;

	ts_internal_bspline_copy(spline, &tmp, buf);
	knots = ts_internal_bspline_access_knots(&tmp);
	num_knots = ts_bspline_num_knots(&tmp);

	TRY(b, e)
		/* DO NOT FORGET TO UPDATE knots AND num_knots AFTER EACH
		 * TRANSFORMATION OF tmp! */

		/* fix first control point if necessary */
		u_min = knots[deg];
		if (!ts_fequals(knots[0], u_min)) {
			ts_internal_bspline_split(&tmp, u_min, &tmp, &k, b);
			resize = (int)(-1*deg + (deg*2 - k));
			ts_internal_bspline_resize(&tmp, resize, 0, &tmp, b);
			knots = ts_internal_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
		}

		/* fix last control point if necessary */
		u_max = knots[num_knots - order];
		if (!ts_fequals(knots[num_knots - 1], u_max)) {
			ts_internal_bspline_split(&tmp, u_max, &tmp, &k, b);
			num_knots = ts_bspline_num_knots(&tmp);
			resize = (int)(-1*deg + (k - (num_knots - order)));
			ts_internal_bspline_resize(&tmp, resize, 1, &tmp, b);
			knots = ts_internal_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
		}

		k = order;
		while (k < num_knots - order) {
			ts_internal_bspline_split(&tmp, knots[k], &tmp, &k, b);
			knots = ts_internal_bspline_access_knots(&tmp);
			num_knots = ts_bspline_num_knots(&tmp);
			k++;
		}

		if (spline == _beziers_)
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
		if (spline != _beziers_)
			ts_internal_bspline_init(_beziers_);
	ETRY
	return err;
}



/******************************************************************************
*                                                                             *
* :: Serialization and Persistence Functions                                  *
*                                                                             *
******************************************************************************/
JSON_Value * ts_internal_bspline_to_json(const tsBSpline * spline, jmp_buf buf)
{
	const size_t deg = ts_bspline_degree(spline);
	const size_t dim = ts_bspline_dimension(spline);
	const size_t len_ctrlp = ts_bspline_len_control_points(spline);
	const size_t len_knots = ts_bspline_num_knots(spline);
	const tsReal *ctrlp = ts_internal_bspline_access_ctrlp(spline);
	const tsReal *knots = ts_internal_bspline_access_knots(spline);

	size_t i; /**< Used in loops */

	JSON_Value  *spline_value;
	JSON_Object *spline_object;
	JSON_Value  *ctrlp_value;
	JSON_Array  *ctrlp_array;
	JSON_Value  *knots_value;
	JSON_Array  *knots_array;
	JSON_Status status; /**< Status of last json operation. */

	/* Initialize parson structs. */
	spline_value = json_value_init_object();
	if (!spline_value) {
		longjmp(buf, TS_MALLOC);
	}
	ctrlp_value = json_value_init_array();
	if (!ctrlp_value) {
		json_value_free(spline_value);
		longjmp(buf, TS_MALLOC);
	}
	knots_value = json_value_init_array();
	if (!knots_value) {
		json_value_free(spline_value);
		json_value_free(ctrlp_value);
		longjmp(buf, TS_MALLOC);
	}
	spline_object = json_value_get_object(spline_value);
	if (!spline_object) {
		json_value_free(spline_value);
		json_value_free(ctrlp_value);
		json_value_free(knots_value);
		longjmp(buf, TS_MALLOC);
	}

	/* Set degree and dimension. */
	status = json_object_set_number(
		spline_object, "degree", (double) deg);
	if (status != JSONSuccess) {
		json_value_free(spline_value);
		json_value_free(ctrlp_value);
		json_value_free(knots_value);
		longjmp(buf, TS_MALLOC);
	}
	status = json_object_set_number(
		spline_object, "dimension", (double) dim);
	if (status != JSONSuccess) {
		json_value_free(spline_value);
		json_value_free(ctrlp_value);
		json_value_free(knots_value);
		longjmp(buf, TS_MALLOC);
	}

	/* Set control points. */
	status = json_object_set_value(
		spline_object, "control_points", ctrlp_value);
	if (status != JSONSuccess) {
		json_value_free(spline_value);
		json_value_free(ctrlp_value);
		json_value_free(knots_value);
		longjmp(buf, TS_MALLOC);
	}
	ctrlp_array = json_array(ctrlp_value);
	if (!ctrlp_array){
		json_value_free(spline_value);
		json_value_free(knots_value);
		longjmp(buf, TS_MALLOC);
	}
	for (i = 0; i < len_ctrlp; i++) {
		status = json_array_append_number(
			ctrlp_array, (double) ctrlp[i]);
		if (status != JSONSuccess) {
			json_value_free(spline_value);
			json_value_free(knots_value);
			longjmp(buf, TS_MALLOC);
		}
	}

	/* Set knots. */
	status = json_object_set_value(
		spline_object, "knots", knots_value);
	if (status != JSONSuccess) {
		json_value_free(spline_value);
		json_value_free(knots_value);
		longjmp(buf, TS_MALLOC);
	}
	knots_array = json_array(knots_value);
	if (!knots_array) {
		json_value_free(spline_value);
		longjmp(buf, TS_MALLOC);
	}
	for (i = 0; i < len_knots; i++) {
		status = json_array_append_number(
			knots_array, (double) knots[i]);
		if (status != JSONSuccess) {
			json_value_free(spline_value);
			longjmp(buf, TS_MALLOC);
		}
	}

	/* Done. */
	return spline_value;
}

void ts_internal_bspline_from_json(const JSON_Value *spline_value,
	tsBSpline *_spline_, jmp_buf buf)
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
	jmp_buf b;

	/* Read spline object. */
	if (json_value_get_type(spline_value) != JSONObject)
		longjmp(buf, TS_PARSE_ERROR);
	spline_object = json_value_get_object(spline_value);
	if (!spline_object)
		longjmp(buf, TS_PARSE_ERROR);

	/* Read degree. */
	deg_value = json_object_get_value(spline_object, "degree");
	if (json_value_get_type(deg_value) != JSONNumber)
		longjmp(buf, TS_PARSE_ERROR);
	deg = (size_t) json_value_get_number(deg_value);

	/* Read dimension. */
	dim_value = json_object_get_value(spline_object, "dimension");
	if (json_value_get_type(dim_value) != JSONNumber)
		longjmp(buf, TS_PARSE_ERROR);
	dim = (size_t) json_value_get_number(dim_value);
	if (dim == 0)
		longjmp(buf, TS_DIM_ZERO);

	/* Read length of control point vector. */
	ctrlp_value = json_object_get_value(spline_object, "control_points");
	if (json_value_get_type(ctrlp_value) != JSONArray)
		longjmp(buf, TS_PARSE_ERROR);
	ctrlp_array = json_value_get_array(ctrlp_value);
	len_ctrlp = json_array_get_count(ctrlp_array);
	if (len_ctrlp % dim != 0)
		longjmp(buf, TS_LCTRLP_DIM_MISMATCH);

	/* Read number of knots. */
	knots_value = json_object_get_value(spline_object, "knots");
	if (json_value_get_type(knots_value) != JSONArray)
		longjmp(buf, TS_PARSE_ERROR);
	knots_array = json_value_get_array(knots_value);
	num_knots = json_array_get_count(knots_array);

	/* Create spline. */
	ts_internal_bspline_new(
		len_ctrlp/dim, dim, deg, TS_CLAMPED, _spline_, buf);
	TRY(b, err)
		if (num_knots != ts_bspline_num_knots(_spline_))
			longjmp(b, TS_NUM_KNOTS);

		/* Set control points. */
		ctrlp = ts_internal_bspline_access_ctrlp(_spline_);
		for (i = 0; i < len_ctrlp; i++) {
			real_value = json_array_get_value(ctrlp_array, i);
			if (json_value_get_type(real_value) != JSONNumber)
				longjmp(b, TS_PARSE_ERROR);
			ctrlp[i] = (tsReal) json_value_get_number(real_value);
		}
		err = ts_bspline_set_control_points(_spline_, ctrlp);
		if (err)
			longjmp(b, err);

		/* Set knots. */
		knots = ts_internal_bspline_access_knots(_spline_);
		for (i = 0; i < num_knots; i++) {
			real_value = json_array_get_value(knots_array, i);
			if (json_value_get_type(real_value) != JSONNumber)
				longjmp(b, TS_PARSE_ERROR);
			knots[i] = (tsReal) json_value_get_number(real_value);
		}
		err = ts_bspline_set_knots(_spline_, knots);
		if (err)
			longjmp(b, err);
	CATCH
		ts_bspline_free(_spline_);
		longjmp(buf, err);
	ETRY
}

tsError ts_bspline_to_json(const tsBSpline *spline, char **_json_)
{
	tsError err;
	jmp_buf buf;
	JSON_Value *value = NULL;
	*_json_ = NULL;
	TRY(buf, err)
		value = ts_internal_bspline_to_json(spline, buf);
	CATCH
		return err;
	ETRY
	*_json_ = json_serialize_to_string_pretty(value);
	json_value_free(value);
	if (!*_json_)
		return TS_MALLOC;
	return TS_SUCCESS;
}

tsError ts_bspline_from_json(const char *json, tsBSpline *_spline_)
{
	tsError err;
	jmp_buf buf;
	JSON_Value *value = NULL;
	ts_internal_bspline_init(_spline_);
	TRY(buf, err)
		value = json_parse_string(json);
		if (!value)
			longjmp(buf, TS_PARSE_ERROR);
		ts_internal_bspline_from_json(value, _spline_, buf);
	ETRY
	if (value)
		json_value_free(value);
	return err;
}

tsError ts_bspline_save_json(const tsBSpline *spline, const char *path)
{
	tsError err;
	jmp_buf buf;
	JSON_Status status;
	JSON_Value *value = NULL;
	TRY(buf, err)
		value = ts_internal_bspline_to_json(spline, buf);
	CATCH
		return err;
	ETRY
	status = json_serialize_to_file_pretty(value, path);
	json_value_free(value);
	if (status != JSONSuccess)
		return TS_IO_ERROR;
	return TS_SUCCESS;
}

tsError ts_bspline_load_json(const char *path, tsBSpline *_spline_)
{
	tsError err;
	jmp_buf buf;
	FILE *file = NULL;
	JSON_Value *value = NULL;
	TRY(buf, err)
		file = fopen(path, "r");
		if (!file)
			longjmp(buf, TS_IO_ERROR);
		value = json_parse_file(path);
		if (!value)
			longjmp(buf, TS_PARSE_ERROR);
		ts_internal_bspline_from_json(value, _spline_, buf);
	CATCH
		return err;
	ETRY
	if (file)
		fclose(file);
	if (value)
		json_value_free(value);
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
		return "deg >= num(control_points)";
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
	else if (err == TS_LCTRLP_DIM_MISMATCH)
		return "len(control_points) % dim != 0";
	else if (err == TS_IO_ERROR)
		return "io error";
	else if (err == TS_PARSE_ERROR)
		return "parse error";
	else if (err == TS_INDEX_ERROR)
		return "index does not exist";
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
	else if (!strcmp(str, ts_enum_str(TS_IO_ERROR)))
		return TS_IO_ERROR;
	else if (!strcmp(str, ts_enum_str(TS_PARSE_ERROR)))
		return TS_PARSE_ERROR;
	else if (!strcmp(str, ts_enum_str(TS_INDEX_ERROR)))
		return TS_INDEX_ERROR;
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

#ifdef _MSC_VER
#pragma warning(pop)
#endif
