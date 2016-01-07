#include "tinyspline.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>

/********************************************************
*                                                       *
* System dependent configuration                        *
*                                                       *
********************************************************/
#define FLT_MAX_ABS_ERROR 1e-7
#define FLT_MAX_REL_ERROR 1e-5


/********************************************************
*                                                       *
* Internal functions                                    *
*                                                       *
********************************************************/
tsError ts_internal_bspline_find_u(
    const tsBSpline* bspline, const float u,
    size_t* k, size_t* s
)
{
    const size_t deg = bspline->deg;
    const size_t order = bspline->order;
    const size_t n_knots = bspline->n_knots;

    for (*k = 0; *k < n_knots; (*k)++) {
        const float uk = bspline->knots[*k];
        if (ts_fequals(u, uk)) {
            (*s)++;
        } else if (u < uk) {
            break;
        }
    }

    /* keep in mind that currently k is k+1 */
    if (*s > order)
        return TS_MULTIPLICITY;
    if (*k <= deg)                /* u < u_min */
        return TS_U_UNDEFINED;
    if (*k == n_knots && *s == 0) /* u > u_last */
        return TS_U_UNDEFINED;
    if (*k > n_knots-deg + *s-1)  /* u > u_max */
        return TS_U_UNDEFINED;

    (*k)--; /* k+1 - 1 will never underflow */
    return TS_SUCCESS;
}

/**
 * @return ::TS_SUCCESS, ::TS_MULTIPLICITY, ::TS_OVER_UNDERFLOW
 */
tsError ts_internal_bspline_insert_knot(
    const tsBSpline* bspline, const tsDeBoorNet* deBoorNet, const size_t n,
    tsBSpline* result
)
{
    tsError err;

    /* constant variables */
    const size_t deg = bspline->deg;
    const size_t dim = bspline->dim;
    const size_t k = deBoorNet->k;
    const size_t sof_f = sizeof(float); /* The size of a float. */
    size_t sof_c; /* The size of a single control point. */
    size_t N; /* The number of affected control points. */

    /* mutable variables */
    float* from; /* The pointer to copy the values from. */
    float* to; /* The pointer to copy the values to. */
    int stride; /* The stride of the next pointer to copy. Will be negative
 * later on, thus use int. */
    size_t i; /* Used in for loops. */

    /* input validation */
    if (deBoorNet->s+n > bspline->order)
        goto err_multiplicity;
    /* Use ::ts_bspline_resize even if \n is 0 to copy
     * the spline if necessary. */
    err = ts_bspline_resize(bspline, (int)n, 1, result);
    if (err < 0)
        return err;
    if (n == 0) /* nothing to insert */
        return TS_SUCCESS;

    sof_c = dim * sof_f; /* dim > 0 implies sof_c > 0 */
    N = deBoorNet->h+1; /* n > 0 implies s <= deg implies a regular evaluation
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
    memmove(result->ctrlp, bspline->ctrlp, (k-deg) * sof_c); /* a) */
    from = bspline->ctrlp + dim*(k-deg+N);
    to   = result->ctrlp  + dim*(k-deg+N+n); /* n >= 0 implies to >= from */
    memmove(to, from, (result->n_ctrlp-n-(k-deg+N)) * sof_c); /* b) */
    /* copy knots */
    memmove(result->knots, bspline->knots, (k+1) * sof_f); /* c) */
    from = bspline->knots + k+1;
    to   = result->knots  + k+1+n; /* n >= 0 implies to >= from */
    memmove(to, from, (result->n_knots-n-(k+1)) * sof_f); /* d) */

    /* 2.
     *
     * a) Copy left hand side control points from de boor net.
     * b) Copy middle part control points from de boor net.
     * c) Copy right hand side control points from de boor net.
     * d) Insert knots with u_k. */
    from = deBoorNet->points;
    to = result->ctrlp + (k-deg)*dim;
    stride = (int)(N*dim);

    /* copy control points */
    for (i = 0; i < n; i++) { /* a) */
        memcpy(to, from, sof_c);
        from += stride;
        to += dim;
        stride -= dim;
    }
    memcpy(to, from, (N-n) * sof_c); /* b) */

    from -= dim;
    to += (N-n)*dim;
    stride = (int)(-(N-n+1)*dim);

    for (i = 0; i < n; i++) { /* c) */
        memcpy(to, from, sof_c);
        from += stride;
        stride -= dim;
        to += dim;
    }
    /* copy knots */
    to = result->knots+k+1;
    for (i = 0; i < n; i++) { /* d) */
        *to = deBoorNet->u;
        to++;
    }

    return TS_SUCCESS;

    /* error handling */
    err_multiplicity:
        err = TS_MULTIPLICITY;
        goto cleanup;
    cleanup:
        if (bspline != result)
            ts_bspline_default(result);
        return err;
}

tsError ts_internal_bspline_thomas_algorithm(
    const float* points, const size_t n, const size_t dim,
    float* output
)
{
    /* constant variables */
    const size_t sof_f = sizeof(float); /* The size of a float. */
    size_t sof_c; /* The size of a single control point. */
    size_t ndsf; /* The size of all (n) control points. */
    size_t len_m; /* The length m. */
    float* m; /* The array of weights. */
    size_t lst; /* The index of the first component of the last control point
 * in \points. */

    /* mutable variables */
    size_t i, d; /* Used in for loops. */
    size_t j, k, l; /* Used as temporary indices. */

    /* input validation */
    if (points == output)
        return TS_INPUT_EQ_OUTPUT;
    if (dim == 0)
        return TS_DIM_ZERO;
    if (n == 0)
        return TS_DEG_GE_NCTRLP;

    sof_c = dim * sof_f; /* dim > 0 implies sof_c > 0 */
    ndsf = n * sof_c; /* n > 0 and sof_c > 0 implies ndsf > 0 */

    if (n <= 2) {
        memcpy(output, points, ndsf);
        return TS_SUCCESS;
    }

    /* In the following n >= 3 applies. */

    /* m_0 = 1/4, m_{k+1} = 1/(4-m_k), for k = 0,...,n-2 */
    len_m = n-2; /* n >= 3 implies n-2 >= 1 */
    m = (float*) malloc(len_m * sof_f);
    if (m == NULL)
        return TS_MALLOC;
    m[0] = 0.25f;
    for (i = 1; i < len_m; i++)
        m[i] = 1.f/(4 - m[i-1]);

    lst = (n-1)*dim; /* n >= 3 implies n-1 >= 2 */
    memset(output, 0, ndsf);
    memcpy(output+lst, points+lst, sof_c);

    /* forward sweep */
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
            output[k] -= m[i-2]*output[j];
        }
    }

    /* back substitution */
    memset(output+lst, 0, sof_c);
    for (i = n-2; i >= 1; i--) {
        for (d = 0; d < dim; d++) {
            k = i*dim+d;
            l = (i+1)*dim+d;
            output[k] -= output[l];
            output[k] *= m[i-1];
        }
    }
    memcpy(output, points, sof_c);
    memcpy(output+lst, points+lst, sof_c);

    /* we are done */
    free(m);
    return TS_SUCCESS;
}

tsError ts_internal_relaxed_uniform_cubic_bspline(
    const float* points, const size_t n, const size_t dim,
    tsBSpline* bspline
)
{
    tsError err;

    /* constant variables */
    const float as = 1.f/6.f; /* The value 'a sixth'. */
    const float at = 1.f/3.f; /* The value 'a third'. */
    const float tt = 2.f/3.f; /* The value 'two third'. */
    size_t sof_c; /* The size of a single control point. */
    const float* b = points; /* The array of the b values (used for
 * convenience). */
    float* s; /* The array of the s values. */
    float u0; /* The first knot value of the interpolated spline. */
    float u1; /* The last knot value of the interpolated spline. */
    float u; /* The distance of two successional knot values of the
 * interpolated spline. Used to properly setup the knot vector. */

    /* mutable variables */
    size_t i, d; /* Used in for loops */
    size_t j, k, l; /* Uses as temporary indices. */

    ts_bspline_default(bspline);

    /* input validation */
    if (dim == 0)
        goto err_dim_zero;
    if (n <= 1)
        goto err_deg_ge_nctrlp;
    /* in the following n >= 2 applies */

    /* setup the spline */
    err = ts_bspline_new(3, dim, (n-1)*4, TS_CLAMPED, bspline); /* n >= 2
 * implies n-1 >= 1 implies (n-1)*4 >= 4 */
    if (err < 0)
        return err;

    sof_c = dim * sizeof(float); /* dim > 0 implies sof_c > 0 */
    s = (float*) malloc(n * sof_c);
    if (s == NULL)
        goto err_malloc;

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
            bspline->ctrlp[k] = s[j];
            bspline->ctrlp[k+dim] = tt*b[j] + at*b[l];
            bspline->ctrlp[k+2*dim] = at*b[j] + tt*b[l];
            bspline->ctrlp[k+3*dim] = s[l];
        }
    }

    u0 = bspline->knots[0];
    u1 = bspline->knots[bspline->n_knots-1];
    u = (u1-u0) / (n-1);
    for (i = 1; i < n-1; i++) {
        for (d = 0; d < dim+1; d++) {
            bspline->knots[i*(dim+1)+d] = i*u;
        }
    }

    free(s);
    return TS_SUCCESS;

    /* error handling */
    err_deg_ge_nctrlp:
        err = TS_DEG_GE_NCTRLP;
        goto cleanup;
    err_dim_zero:
        err = TS_DIM_ZERO;
        goto cleanup;
    err_malloc:
        err = TS_MALLOC;
        goto cleanup;
    cleanup:
        ts_bspline_free(bspline);
        return err;
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

void ts_deboornet_free(tsDeBoorNet* deBoorNet)
{
    if (deBoorNet->points != NULL) {
        free(deBoorNet->points);/* automatically frees the field result */
    }
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
    if (bspline->ctrlp != NULL) {
        free(bspline->ctrlp);
    }
    if (bspline->knots != NULL) {
        free(bspline->knots);
    }
    ts_bspline_default(bspline);
}

tsError ts_bspline_new(
    const size_t deg, const size_t dim,
    const size_t n_ctrlp, const tsBSplineType type,
    tsBSpline* bspline
)
{
    tsError err;

    /* constant variables */
    const size_t sof_f = sizeof(float); /* The size of a float. */
    const size_t order = deg + 1; /* The order of the spline to create. */
    size_t n_knots; /* The number of knots. */
    size_t sof_nc; /* the size of all control points. */
    size_t sof_nk; /* the size of all knots. */

    /* In the following order >= 1 applies. */

    ts_bspline_default(bspline);

    /* input validation */
    if (dim < 1)
        goto err_dim_zero;
    if (deg >= n_ctrlp)
        goto err_deg_ge_nctrlp;

    /* In the following dim >= 1 and n_ctrlp >= 1 applies. */

    n_knots = n_ctrlp + order; /* n_ctrlp >= 1 and order >= 1 implies
 * n_knots >= 2. */
    sof_nc = n_ctrlp * dim * sof_f; /* n_ctrlp >= 1 and dim >= 1 implies
 * sof_nc > 0 */
    sof_nk = n_knots * sof_f; /* n_knots >= 1 implies sof_nk > 0 */

    /* setup b-spline */
    bspline->deg     = deg;
    bspline->order   = order;
    bspline->dim     = dim;
    bspline->n_ctrlp = n_ctrlp;
    bspline->n_knots = n_knots;
    bspline->ctrlp   = (float*) malloc(sof_nc);
    if (bspline->ctrlp == NULL)
        goto err_malloc;
    bspline->knots   = (float*) malloc(sof_nk);
    if (bspline->knots == NULL)
        goto err_malloc;
    ts_bspline_setup_knots(bspline, type, bspline);

    return TS_SUCCESS;

    /* error handling */
    err_dim_zero:
        err = TS_DIM_ZERO;
        goto cleanup;
    err_deg_ge_nctrlp:
        err = TS_DEG_GE_NCTRLP;
        goto cleanup;
    err_malloc:
        err = TS_MALLOC;
        goto cleanup;
    cleanup:
        ts_bspline_free(bspline);
        return err;
}

tsError ts_bspline_interpolate(
    const float* points, const size_t n, const size_t dim,
    tsBSpline* bspline
)
{
    tsError err;

    float* thomas = (float*) malloc(n*dim*sizeof(float));
    if (thomas == NULL)
        return TS_MALLOC;

    err = ts_internal_bspline_thomas_algorithm(points, n, dim, thomas);
    if (err < 0)
        goto exit;
    err = ts_internal_relaxed_uniform_cubic_bspline(thomas, n, dim, bspline);
    if (err < 0)
        goto exit;

    err = TS_SUCCESS;
    exit:
        free(thomas);
        return err;
}

tsError ts_deboornet_copy(
    const tsDeBoorNet* original,
    tsDeBoorNet* copy
)
{
    /* The size of all points to copy. */
    const size_t size = original->n_points * original->dim * sizeof(float);

    if (original == copy)
        return TS_INPUT_EQ_OUTPUT;

    copy->u   = original->u;
    copy->k   = original->k;
    copy->s   = original->s;
    copy->h   = original->h;
    copy->dim = original->dim;
    copy->n_points = original->n_points;
    /* copy points */
    copy->points = (float*) malloc(size);
    if (copy->points == NULL)
        goto err_malloc;
    memcpy(copy->points, original->points, size);
    /* set result */
    copy->result = copy->points + (copy->n_points-1)*copy->dim;

    return TS_SUCCESS;

    /* error handling */
    err_malloc:
        ts_deboornet_free(copy);
        return TS_MALLOC;
}

tsError ts_bspline_copy(
    const tsBSpline* original,
    tsBSpline* copy
)
{
    size_t size; /* The size to copy (control points and knots). */

    if (original == copy)
        return TS_INPUT_EQ_OUTPUT;

    copy->deg     = original->deg;
    copy->order   = original->order;
    copy->dim     = original->dim;
    copy->n_ctrlp = original->n_ctrlp;
    copy->n_knots = original->n_knots;
    /* copy control points */
    size = original->n_ctrlp * original->dim * sizeof(float);
    copy->ctrlp = (float*) malloc(size);
    if (copy->ctrlp == NULL)
        goto err_malloc;
    memcpy(copy->ctrlp, original->ctrlp, size);
    /* copy knots */
    size = original->n_knots * sizeof(float);
    copy->knots = (float*) malloc(size);
    if (copy->knots == NULL)
        goto err_malloc;
    memcpy(copy->knots, original->knots, size);

    return TS_SUCCESS;

    /* error handling */
    err_malloc:
        ts_bspline_free(copy);
        return TS_MALLOC;
}

tsError ts_bspline_setup_knots(
    const tsBSpline* original, const tsBSplineType type,
    tsBSpline* result
)
{
    /* constant variables */
    const size_t n_knots = original->n_knots;
    const size_t deg = original->deg;
    const size_t order = original->order;

    /* mutable variables */
    size_t current, end; /* Used by loops. */
    size_t numerator, dominator; /* To fill uniformly spaced elements. */

    if (original != result) {
        const tsError ret = ts_bspline_copy(original, result);
        if (ret < 0)
            return ret;
    }

    if (type == TS_NONE)
        return TS_SUCCESS;

    if (type == TS_OPENED) {
        current = numerator = 0;
        end = n_knots;
        dominator = end-1;
        for (;current < end; current++, numerator++)
            result->knots[current] = (float) numerator / dominator;
    } else {
        current = 0;
        end = order;
        for (;current < end; current++)
            result->knots[current] = 0.f;
        end = n_knots - order;
        numerator = 1;
        dominator = n_knots - (2 * deg) - 1;
        for (;current < end; current++, numerator++)
            result->knots[current] = (float) numerator / dominator;
        end = n_knots;
        for (;current < end; current++)
            result->knots[current] = 1.f;
    }

    return TS_SUCCESS;
}

tsError ts_bspline_evaluate(
    const tsBSpline* bspline, const float u,
    tsDeBoorNet* deBoorNet
)
{
    tsError err;

    /* constant variables */
    const size_t deg = bspline->deg;
    const size_t order = bspline->order;
    const size_t dim = bspline->dim;
    const size_t sof_c = dim * sizeof(float); /* The size of a single
 * control points.*/
    size_t k;
    size_t s;
    float uk; /* The actual used u. */
    size_t from; /* An offset used to copy values. */
    size_t fst; /* The first affected control point, inclusive. */
    size_t lst; /* The last affected control point, inclusive. */
    size_t N; /* The number of affected control points. */

    /* mutable variables */
    /* the following indices are used to create the DeBoor net. */
    int lidx; /* The current left index. */
    int ridx; /* The current right index. */
    int tidx; /* The current to index. */
    size_t r, i, d; /* Used in for loop. */
    float ui; /* The knot value at index i. */
    float a, a_hat; /* The weighting factors of the control points. */

    /* Setup the net with its default values. */
    ts_deboornet_default(deBoorNet);

    /* 1. Find index k such that u is in between [u_k, u_k+1).
     * 2. Setup already known values.
     * 3. Decide by multiplicity of u how to calculate point P(u). */

    /* 1. */
    err = ts_internal_bspline_find_u(bspline, u, &deBoorNet->k, &deBoorNet->s);
    if (err < 0)
        goto cleanup;
    k = deBoorNet->k;
    s = deBoorNet->s;

    /* 2. */
    uk = bspline->knots[k];                    /* Ensures that with any */
    deBoorNet->u = ts_fequals(u, uk) ? uk : u; /* float precision the knot
                                                * vector stays valid. */
    deBoorNet->h = deg < s ? 0 : deg-s; /* prevent underflow */
    deBoorNet->dim = dim;

    /* 3. (by 1. s <= order)
     *
     * 3a) Check for s = order.
     *     Take the two points k-s and k-s + 1. If one of
     *     them doesn't exist, take only the other.
     * 3b) Use de boor algorithm to find point P(u). */
    if (s == order) {
        /* only one of the two control points exists */
        if (k == deg ||                  /* only the first control point */
            k == bspline->n_knots - 1) { /* only the last control point */

            deBoorNet->points = (float*) malloc(sof_c);
            if (deBoorNet->points == NULL)
                goto err_malloc;
            deBoorNet->result = deBoorNet->points;
            deBoorNet->n_points = 1;
            from = k == deg ? 0 : (k-s) * dim;
            memcpy(deBoorNet->points, bspline->ctrlp + from, sof_c);
        } else {
            deBoorNet->points = (float*) malloc(2 * sof_c);
            if (deBoorNet->points == NULL)
                goto err_malloc;
            deBoorNet->result = deBoorNet->points+dim;
            deBoorNet->n_points = 2;
            from = (k-s) * dim;
            memcpy(deBoorNet->points, bspline->ctrlp + from, 2 * sof_c);
        }
    } else { /* by 3a) s <= deg (order = deg+1) */
        fst = k-deg; /* by 1. k >= deg */
        lst = k-s; /* s <= deg <= k */
        N = lst-fst + 1; /* lst <= fst implies N >= 1 */

        deBoorNet->n_points = (size_t)(N * (N+1) * 0.5f); /* always fits */
        deBoorNet->points = (float*) malloc(deBoorNet->n_points * sof_c);
        if (deBoorNet->points == NULL)
            goto err_malloc;
        deBoorNet->result = deBoorNet->points + (deBoorNet->n_points-1)*dim;

        /* copy initial values to output */
        memcpy(deBoorNet->points, bspline->ctrlp + fst*dim, N * sof_c);

        lidx = 0;
        ridx = (int)dim;
        tidx = (int)(N*dim); /* N >= 1 implies tidx > 0 */
        r = 1;
        for (;r <= deBoorNet->h; r++) {
            i = fst + r;
            for (; i <= lst; i++) {
                ui = bspline->knots[i];
                a = (deBoorNet->u - ui) / (bspline->knots[i+deg-r+1] - ui);
                a_hat = 1.f-a;

                for (d = 0; d < dim; d++) {
                    deBoorNet->points[tidx++] =
                        a_hat * deBoorNet->points[lidx++] +
                            a * deBoorNet->points[ridx++];
                }
            }
            lidx += dim;
            ridx += dim;
        }
    }

    return TS_SUCCESS;

    /* error handling */
    err_malloc:
        err = TS_MALLOC;
        goto cleanup;
    cleanup:
        ts_deboornet_free(deBoorNet);
        return err;
}

tsError ts_bspline_insert_knot(
    const tsBSpline* bspline, const float u, const size_t n,
    tsBSpline* result, size_t* k
)
{
    tsDeBoorNet net;
    tsError err = ts_bspline_evaluate(bspline, u, &net);
    if (err < 0) {
        ts_bspline_default(result);
        *k = 0;
    } else {
        err = ts_internal_bspline_insert_knot(bspline, &net, n, result);
        *k = err < 0 ? 0 : net.k+n;
    }
    ts_deboornet_free(&net);
    return err;
}

tsError ts_bspline_resize(
    const tsBSpline* bspline, const int n, const int back,
    tsBSpline* resized
)
{
    tsError err;

    /* constant variables */
    const size_t deg = bspline->deg;
    const size_t dim = bspline->dim;
    const size_t n_ctrlp = bspline->n_ctrlp;
    const size_t n_knots = bspline->n_knots;
    const size_t new_n_ctrlp = n_ctrlp + n; /* The new length of ctrlp. */
    const size_t new_n_knots = n_knots+n; /* The new length of knots. */
    const size_t min_n_ctrlp = n < 0 ? new_n_ctrlp : n_ctrlp; /* The minimum of
 * the control points old and new size. */
    const size_t min_n_knots = n < 0 ? new_n_knots : n_knots; /* the minimum of
 * the knots old and new size. */
    const size_t sof_f = sizeof(float); /* The size of a float. */
    const size_t sof_c = dim*sof_f; /* The  size of a single control points. */

    /* mutable variables */
    float* from_ctrlp = bspline->ctrlp; /* The pointer to copy the
 * control points from. */
    float* from_knots = bspline->knots; /* The pointer to copy the
 * knots from. */
    float* to_ctrlp; /* The pointer to copy the control points to. */
    float* to_knots; /* The pointer to copy the knots to. */

    /* if n is 0 the spline must not be resized */
    if (n == 0 && bspline != resized)
        return ts_bspline_copy(bspline, resized);
    if (n == 0 && bspline == resized)
        return TS_SUCCESS;

    if (new_n_ctrlp <= deg)
        goto err_deg_ge_nctrlp;
    else if (n < 0 && new_n_ctrlp > n_ctrlp)
        goto err_over_underflow;
    else if (n > 0 && new_n_knots < n_knots)
        goto err_over_underflow;

    if (bspline != resized) {
        err = ts_bspline_new(deg, dim, new_n_ctrlp, TS_NONE, resized);
        if (err < 0)
            return err;
        to_ctrlp = resized->ctrlp;
        to_knots = resized->knots;
    } else {
        to_ctrlp = (float*) malloc(new_n_ctrlp * sof_c);
        if (to_ctrlp == NULL)
            goto err_malloc;
        to_knots = (float*) malloc(new_n_knots * sof_f);
        if (to_knots == NULL) {
            free(to_ctrlp); /* prevent memory leak */
            goto err_malloc;
        }
    }

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

    if (bspline == resized) {
        /* free old memory */
        free(from_ctrlp);
        free(from_knots);
        /* assign new values */
        resized->ctrlp = to_ctrlp;
        resized->knots = to_knots;
        resized->n_ctrlp = new_n_ctrlp;
        resized->n_knots = new_n_knots;
    }

    return TS_SUCCESS;

    /* error handling */
    err_deg_ge_nctrlp:
        err = TS_DEG_GE_NCTRLP;
        goto cleanup;
    err_over_underflow:
        err = TS_OVER_UNDERFLOW;
        goto cleanup;
    err_malloc:
        err = TS_MALLOC;
        goto cleanup;
    cleanup:
        if (bspline != resized)
            ts_bspline_default(resized);
        return err;
}

tsError ts_bspline_split(
    const tsBSpline* bspline, const float u,
    tsBSpline* split, size_t* k
)
{
    tsDeBoorNet net;
    tsError err = ts_bspline_evaluate(bspline, u, &net);
    if (err < 0) {
        if (bspline != split)
            ts_bspline_default(split);
        *k = 0;
    } else if (net.s == bspline->order) {
        if (bspline != split)
            err = ts_bspline_copy(bspline, split);
        *k = err < 0 ? 0 : net.k;
    } else {
        err = ts_internal_bspline_insert_knot(bspline, &net, net.h+1, split);
        *k = err < 0 ? 0 : net.k + net.h + 1;
    }
    ts_deboornet_free(&net);
    return err;
}

tsError ts_bspline_buckle(
    const tsBSpline* bspline, const float b,
    tsBSpline* buckled
)
{
    const float b_hat  = 1.f-b; /* The straightening factor. */
    const size_t dim   = bspline->dim;
    const size_t N     = bspline->n_ctrlp;
    const float* p0    = bspline->ctrlp; /* The pointer to the first ctrlp. */
    const float* pn_1  = p0 + (N-1)*dim; /* The pointer to the last ctrlp. */
    size_t i, d; /* Used in for loops. */

    if (bspline != buckled) {
        const tsError err = ts_bspline_copy(bspline, buckled);
        if (err < 0)
            return err;
    }

    for (i = 0; i < N; i++) {
        for (d = 0; d < dim; d++) {
            buckled->ctrlp[i*dim + d] =
                    b * buckled->ctrlp[i*dim + d] +
                b_hat * (p0[d] + ((float)i / (N-1)) * (pn_1[d] - p0[d]));
        }
    }

    return TS_SUCCESS;
}

tsError ts_bspline_to_beziers(
    const tsBSpline* bspline,
    tsBSpline* beziers
)
{
    tsError err;

    const size_t deg = bspline->deg;
    const size_t order = bspline->order;
    int resize; /* The number of control points to add/remove. */
    size_t k; /* The index of the splitted knot value. */
    float u_min; /* The minimum of the knot values. */
    float u_max; /* The maximum of the knot values. */

    if (bspline != beziers) {
        err = ts_bspline_copy(bspline, beziers);
        if (err < 0)
            return err;
    }

    /* fix first control point if necessary */
    u_min = beziers->knots[deg];
    if (!ts_fequals(beziers->knots[0], u_min)) {
        err = ts_bspline_split(beziers, u_min, beziers, &k);
        if (err < 0)
            return err;
        resize = (int)(-1*deg + (deg*2 - k));
        err = ts_bspline_resize(beziers, resize, 0, beziers);
        if (err < 0)
            return err;
    }

    /* fix last control point if necessary */
    u_max = beziers->knots[beziers->n_knots - order];
    if (!ts_fequals(beziers->knots[beziers->n_knots-1], u_max)) {
        err = ts_bspline_split(beziers, u_max, beziers, &k);
        if (err < 0)
            return err;
        resize = (int)(-1*deg + (k - (beziers->n_knots - order)));
        err = ts_bspline_resize(beziers, resize, 1, beziers);
        if (err < 0)
            return err;
    }

    k = order;
    while (k < beziers->n_knots - order) {
        err = ts_bspline_split(beziers, beziers->knots[k], beziers, &k);
        if (err < 0)
            return err;
        k++;
    }

    return TS_SUCCESS;
}

int ts_fequals(const float x, const float y)
{
    if (fabs(x-y) <= FLT_MAX_ABS_ERROR) {
        return 1;
    } else {
        const float r = (float)fabs(x) > (float)fabs(y) ?
            (float)fabs((x-y) / x) : (float)fabs((x-y) / y);
        return r <= FLT_MAX_REL_ERROR;
    }
}

char* ts_enum_str(const tsError err) {
    if (err == TS_MALLOC)
        return "malloc failed";
    else if (err == TS_OVER_UNDERFLOW)
        return "over/underflow detected";
    else if (err == TS_DIM_ZERO)
        return "dim == 0";
    else if (err == TS_DEG_GE_NCTRLP)
        return "deg >= number of control points";
    else if (err == TS_U_UNDEFINED)
        return "spline is undefined at given u";
    else if (err == TS_MULTIPLICITY)
        return "s > order";
    else if (err == TS_INPUT_EQ_OUTPUT)
        return "input == output";
    return "unkown error";
}
