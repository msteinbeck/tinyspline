#include "tinyspline.h"

#include <stdlib.h> /* malloc, free */
#include <math.h> /* fabs */
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


/********************************************************
*                                                       *
* System dependent configuration                        *
*                                                       *
********************************************************/
#define FLT_MAX_ABS_ERROR 1e-5
#define FLT_MAX_REL_ERROR 1e-8


/********************************************************
*                                                       *
* Internal functions                                    *
*                                                       *
********************************************************/
void ts_internal_deboornet_copy(
    const tsDeBoorNet* original,
    tsDeBoorNet* copy, jmp_buf buf
)
{
    const size_t dim = original->dim;
    const size_t n_points = original->n_points;
    const size_t sof_f = sizeof(float);
    const size_t sof_p = n_points * dim * sof_f;

    if (original == copy)
        return;

    copy->u = original->u;
    copy->k = original->k;
    copy->s = original->s;
    copy->h = original->h;
    copy->dim = dim;
    copy->n_points = n_points;
    copy->points = (float*) malloc(sof_p);
    if (copy->points == NULL)
        longjmp(buf, TS_MALLOC);
    memcpy(copy->points, original->points, sof_p);
    copy->result = copy->points + (n_points-1)*dim;
}

void ts_internal_bspline_find_u(
    const tsBSpline* bspline, const float u,
    size_t* k, size_t* s, jmp_buf buf
)
{
    const size_t deg = bspline->deg;
    const size_t order = bspline->order;
    const size_t n_knots = bspline->n_knots;

    *k = *s = 0;
    for (; *k < n_knots; (*k)++) {
        const float uk = bspline->knots[*k];
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
    const tsBSpline* original,
    tsBSpline* copy, jmp_buf buf
)
{
    const size_t dim = original->dim;
    const size_t sof_f = sizeof(float);
    const size_t n_ctrlp = original->n_ctrlp;
    const size_t n_knots = original->n_knots;
    const size_t sof_ck = (n_ctrlp*dim + n_knots) * sof_f;

    /* Nothing to do here. */
    if (original == copy)
        return;

    copy->deg = original->deg;
    copy->order = original->order;
    copy->dim = original->dim;
    copy->n_ctrlp = original->n_ctrlp;
    copy->n_knots = original->n_knots;
    copy->ctrlp = (float*) malloc(sof_ck);
    if (copy->ctrlp == NULL)
        longjmp(buf, TS_MALLOC);
    memcpy(copy->ctrlp, original->ctrlp, sof_ck);
    copy->knots = copy->ctrlp + n_ctrlp*dim;
}

void ts_internal_bspline_setup_knots(
        const tsBSpline* original, const tsBSplineType type,
        const float min, const float max,
        tsBSpline* result, jmp_buf buf
)
{
    const size_t n_knots = original->n_knots;
    const size_t deg = original->deg;
    const size_t order = deg+1; /* Using deg+1 instead of original->order
 * ensures order >= 1. */
    float fac; /* The factor used to calculate the knot values. */
    size_t i; /* Used in for loops. */

    /* order >= 1 implies 2*order >= 2 implies n_knots >= 2 */
    if (n_knots < 2*order)
        longjmp(buf, TS_DEG_GE_NCTRLP);
    if (min > max || ts_fequals(min, max))
        longjmp(buf, TS_KNOTS_DECR);

    /* copy spline even if type is TS_NONE */
    ts_internal_bspline_copy(original, result, buf);
    if (type == TS_NONE)
        return;

    if (type == TS_OPENED) {
        /* ensures that the first knot value is exactly \min */
        result->knots[0] = min; /* n_knots >= 2 */

        fac = (max-min) / (n_knots-1); /* n_knots >= 2 */
        for (i = 1; i < n_knots-1; i++)
            result->knots[i] = min + i*fac;

        /* ensure that the last knot value is exactly \max */
        result->knots[i] = max;
    } else if (type == TS_CLAMPED) {
        /* n_knots >= 2*order == 2*(deg+1) == 2*deg + 2 > 2*deg - 1 */
        fac = (max-min) / (n_knots - 2*deg - 1);

        for (i = 0; i < order; i++)
            result->knots[i] = min;
        for (;i < n_knots-order; i++)
            result->knots[i] = min + (i-deg)*fac;
        for (;i < original->n_knots; i++)
            result->knots[i] = max;
    }
}

void ts_internal_bspline_new(
    const size_t deg, const size_t dim, const size_t n_ctrlp,
    const tsBSplineType type,
    tsBSpline* bspline, jmp_buf buf
)
{
    const size_t order = deg + 1;
    const size_t n_knots = n_ctrlp + order;
    const size_t sof_f = sizeof(float);
    const size_t sof_ck = (n_ctrlp*dim + n_knots) * sof_f;
    tsError e;
    jmp_buf b;

    if (dim < 1)
        longjmp(buf, TS_DIM_ZERO);
    if (deg >= n_ctrlp)
        longjmp(buf, TS_DEG_GE_NCTRLP);

    bspline->deg = deg;
    bspline->order = order;
    bspline->dim = dim;
    bspline->n_ctrlp = n_ctrlp;
    bspline->n_knots = n_knots;
    bspline->ctrlp = (float*) malloc(sof_ck);
    if (bspline->ctrlp == NULL)
        longjmp(buf, TS_MALLOC);
    bspline->knots = bspline->ctrlp + n_ctrlp*dim;

    /* Although this function should never fail because of input == output
     * it might be worthwhile adding error handling in order to prevent
     * memory leaks. */
    TRY(b, e)
        ts_internal_bspline_setup_knots(bspline, type, 0.f, 1.f, bspline, b);
    CATCH
        free(bspline->ctrlp);
        longjmp(buf, e);
    ETRY
}

void ts_internal_bspline_resize(
        const tsBSpline* bspline, const int n, const int back,
        tsBSpline* resized, jmp_buf buf
)
{
    const size_t deg = bspline->deg;
    const size_t dim = bspline->dim;
    const size_t sof_f = sizeof(float);
    const size_t sof_c = dim * sof_f;

    const size_t n_ctrlp = bspline->n_ctrlp;
    const size_t n_knots = bspline->n_knots;
    const size_t nn_ctrlp = n_ctrlp + n; /* The new length of ctrlp. */
    const size_t nn_knots = n_knots + n; /* The new length of knots. */
    const size_t sof_ncnk = (nn_ctrlp*dim + nn_knots) * sof_f;
    const size_t min_n_ctrlp = n < 0 ? nn_ctrlp : n_ctrlp; /* The minimum of
 * the control points old and new size. */
    const size_t min_n_knots = n < 0 ? nn_knots : n_knots; /* the minimum of
 * the knots old and new size. */

    float* from_ctrlp = bspline->ctrlp;
    float* from_knots = bspline->knots;
    float* to_ctrlp = NULL;
    float* to_knots = NULL;

    /* If n is 0 the spline must not be resized. */
    if (n == 0) {
        ts_internal_bspline_copy(bspline, resized, buf);
        return;
    }

    if (bspline != resized) {
        ts_internal_bspline_new(deg, dim, nn_ctrlp, TS_NONE, resized, buf);
        to_ctrlp = resized->ctrlp;
        to_knots = resized->knots;
    } else {
        if (nn_ctrlp <= deg)
            longjmp(buf, TS_DEG_GE_NCTRLP);
        to_ctrlp = (float*) malloc(sof_ncnk);
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
    if (bspline == resized) {
        /* free old memory */
        free(from_ctrlp);
        /* assign new values */
        resized->ctrlp = to_ctrlp;
        resized->knots = to_knots;
        resized->n_ctrlp = nn_ctrlp;
        resized->n_knots = nn_knots;
    }
}

void ts_internal_bspline_insert_knot(
        const tsBSpline* bspline, const tsDeBoorNet* deBoorNet, const size_t n,
        tsBSpline* result, jmp_buf buf
)
{
    const size_t deg = bspline->deg;
    const size_t dim = bspline->dim;
    const size_t k = deBoorNet->k;
    const size_t sof_f = sizeof(float);
    const size_t sof_c = dim * sof_f;
    size_t N; /* The number of affected control points. */
    float* from; /* The pointer to copy the values from. */
    float* to; /* The pointer to copy the values to. */
    int stride; /* The stride of the next pointer to copy. Will be negative
 * later on, thus use int. */
    size_t i; /* Used in for loops. */

    if (deBoorNet->s+n > bspline->order) {
        longjmp(buf, TS_MULTIPLICITY);
    }

    /* Use ::ts_bspline_resize even if \n is 0 to copy
     * the spline if necessary. */
    ts_internal_bspline_resize(bspline, (int)n, 1, result, buf);
    if (n == 0) /* Nothing to insert. */
        return;

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
}

void ts_internal_bspline_evaluate(
    const tsBSpline* bspline, const float u,
    tsDeBoorNet* deBoorNet, jmp_buf buf
)
{
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
    ts_internal_bspline_find_u(bspline, u, &k, &s, buf);
    deBoorNet->k = k;
    deBoorNet->s = s;

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
                longjmp(buf, TS_MALLOC);
            deBoorNet->result = deBoorNet->points;
            deBoorNet->n_points = 1;
            from = k == deg ? 0 : (k-s) * dim;
            memcpy(deBoorNet->points, bspline->ctrlp + from, sof_c);
        } else {
            deBoorNet->points = (float*) malloc(2 * sof_c);
            if (deBoorNet->points == NULL)
                longjmp(buf, TS_MALLOC);
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
            longjmp(buf, TS_MALLOC);
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
}

void ts_internal_bspline_split(
    const tsBSpline* bspline, const float u,
    tsBSpline* split, size_t* k, jmp_buf buf
)
{
    tsDeBoorNet net;
    tsError e;
    jmp_buf b;

    TRY(b, e)
        ts_internal_bspline_evaluate(bspline, u, &net, b);
        if (net.s == bspline->order) {
            ts_internal_bspline_copy(bspline, split, b);
            *k = net.k;
        } else {
            ts_internal_bspline_insert_knot(
                    bspline, &net, net.h+1, split, b);
            *k = net.k + net.h + 1;
        }
    CATCH
        *k = 0;
    ETRY

    ts_deboornet_free(&net);
    if (e < 0)
        longjmp(buf, e);
}

void ts_internal_bspline_thomas_algorithm(
        const float* points, const size_t n, const size_t dim,
        float* output, jmp_buf buf
)
{
    const size_t sof_f = sizeof(float); /* The size of a float. */
    size_t sof_c; /* The size of a single control point. */
    size_t ndsf; /* The size of all (n) control points. */
    size_t len_m; /* The length m. */
    float* m; /* The array of weights. */
    size_t lst; /* The index of the first component of the last control point
 * in \points. */
    size_t i, d; /* Used in for loops. */
    size_t j, k, l; /* Used as temporary indices. */

    /* input validation */
    if (dim == 0)
        longjmp(buf, TS_DIM_ZERO);
    if (n == 0)
        longjmp(buf, TS_DEG_GE_NCTRLP);

    sof_c = dim * sof_f; /* dim > 0 implies sof_c > 0 */
    ndsf = n * sof_c; /* n > 0 and sof_c > 0 implies ndsf > 0 */

    if (n <= 2) {
        memcpy(output, points, ndsf);
        return;
    }

    /* In the following n >= 3 applies. */

    /* m_0 = 1/4, m_{k+1} = 1/(4-m_k), for k = 0,...,n-2 */
    len_m = n-2; /* n >= 3 implies n-2 >= 1 */
    m = (float*) malloc(len_m * sof_f);
    if (m == NULL)
        longjmp(buf, TS_MALLOC);
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
}

void ts_internal_relaxed_uniform_cubic_bspline(
        const float* points, const size_t n, const size_t dim,
        tsBSpline* bspline, jmp_buf buf
)
{
    const size_t order = 4; /* The order of the spline to interpolate. */
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

    sof_c = dim * sizeof(float); /* dim > 0 implies sof_c > 0 */

    /* n >= 2 implies n-1 >= 1 implies (n-1)*4 >= 4 */
    ts_internal_bspline_new(order-1, dim, (n-1)*4, TS_CLAMPED, bspline, buf);

    TRY(b_, e_)
        s = (float*) malloc(n * sof_c);
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
        for (d = 0; d < order; d++) {
            bspline->knots[i*order+d] = i*u;
        }
    }

    free(s);
}

void ts_internal_bspline_interpolate(
    const float* points, const size_t n, const size_t dim,
    tsBSpline* bspline, jmp_buf buf
)
{
    tsError e;
    jmp_buf b;
    float* thomas = (float*) malloc(n*dim*sizeof(float));
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

void ts_internal_bspline_buckle(
    const tsBSpline* bspline, const float b,
    tsBSpline* buckled, jmp_buf buf
)
{
    const float b_hat  = 1.f-b; /* The straightening factor. */
    const size_t dim = bspline->dim;
    const size_t N = bspline->n_ctrlp;
    const float* p0 = bspline->ctrlp; /* The pointer to the first ctrlp. */
    const float* pn_1 = p0 + (N-1)*dim; /* The pointer to the last ctrlp. */
    size_t i, d; /* Used in for loops. */

    ts_internal_bspline_copy(bspline, buckled, buf);

    for (i = 0; i < N; i++) {
        for (d = 0; d < dim; d++) {
            buckled->ctrlp[i*dim + d] =
                b * buckled->ctrlp[i*dim + d] +
                b_hat * (p0[d] + ((float)i / (N-1)) * (pn_1[d] - p0[d]));
        }
    }
}

void ts_internal_bspline_to_beziers(
    const tsBSpline* bspline,
    tsBSpline* beziers, jmp_buf buf
)
{
    const size_t deg = bspline->deg;
    const size_t order = bspline->order;
    int resize; /* The number of control points to add/remove. */
    size_t k; /* The index of the splitted knot value. */
    float u_min; /* The minimum of the knot values. */
    float u_max; /* The maximum of the knot values. */

    ts_internal_bspline_copy(bspline, beziers, buf);

    /* fix first control point if necessary */
    u_min = beziers->knots[deg];
    if (!ts_fequals(beziers->knots[0], u_min)) {
        ts_internal_bspline_split(beziers, u_min, beziers, &k, buf);
        resize = (int)(-1*deg + (deg*2 - k));
        ts_internal_bspline_resize(beziers, resize, 0, beziers, buf);
    }

    /* fix last control point if necessary */
    u_max = beziers->knots[beziers->n_knots - order];
    if (!ts_fequals(beziers->knots[beziers->n_knots-1], u_max)) {
        ts_internal_bspline_split(beziers, u_max, beziers, &k, buf);
        resize = (int)(-1*deg + (k - (beziers->n_knots - order)));
        ts_internal_bspline_resize(beziers, resize, 1, beziers, buf);
    }

    k = order;
    while (k < beziers->n_knots - order) {
        ts_internal_bspline_split(beziers, beziers->knots[k], beziers, &k, buf);
        k++;
    }
}

void ts_internal_bspline_set_ctrlp(
    const tsBSpline* bspline, const float* ctrlp,
    tsBSpline* result, jmp_buf buf
)
{
    const size_t s = bspline->n_ctrlp * bspline->dim * sizeof(float);
    ts_internal_bspline_copy(bspline, result, buf);
    memmove(result->ctrlp, ctrlp, s);
}

void ts_internal_bspline_set_knots(
    const tsBSpline* bspline, const float* knots,
    tsBSpline* result, jmp_buf buf
)
{
    const size_t s = bspline->n_knots * sizeof(float);
    ts_internal_bspline_copy(bspline, result, buf);
    memmove(result->knots, knots, s);
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

tsError ts_bspline_new(
    const size_t deg, const size_t dim,
    const size_t n_ctrlp, const tsBSplineType type,
    tsBSpline* bspline
)
{
    tsError err;
    jmp_buf buf;
    TRY(buf, err)
        ts_internal_bspline_new(deg, dim, n_ctrlp, type, bspline, buf);
    CATCH
        ts_bspline_default(bspline);
    ETRY
    return err;
}

tsError ts_bspline_interpolate(
    const float* points, const size_t n, const size_t dim,
    tsBSpline* bspline
)
{
    tsError err;
    jmp_buf buf;
    TRY(buf, err)
        ts_internal_bspline_interpolate(points, n, dim, bspline, buf);
    CATCH
        ts_bspline_default(bspline);
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
    const tsBSpline* bspline, const float* ctrlp,
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
    const tsBSpline* bspline, const float* knots,
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

tsError ts_bspline_setup_knots(
    const tsBSpline* original, const tsBSplineType type,
    const float min, const float max,
    tsBSpline* result
)
{
    tsError err;
    jmp_buf buf;
    TRY(buf, err)
        ts_internal_bspline_setup_knots(original, type, min, max, result, buf);
    CATCH
        if (original != result)
            ts_bspline_default(result);
    ETRY
    return err;
}

tsError ts_bspline_evaluate(
    const tsBSpline* bspline, const float u,
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
    const tsBSpline* bspline, const float u, const size_t n,
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
    const tsBSpline* bspline, const int n, const int back,
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
    const tsBSpline* bspline, const float u,
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
    const tsBSpline* bspline, const float b,
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

char* ts_enum_str(const tsError err)
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
    return TS_SUCCESS;
}
