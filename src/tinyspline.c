#include "tinyspline.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>

/********************************************************
*                                                       *
* Internal functions                                    *
*                                                       *
********************************************************/
tsError ts_internal_bspline_insert_knot(
    const tsBSpline* bspline, const tsDeBoorNet* deBoorNet, const size_t n, 
    tsBSpline* result
)
{
    if (deBoorNet->s+n > bspline->order)
        goto err_multiplicity;
    if (((int)n) < 0) // ensure n fits into an int without getting negative
        goto err_over_underflow;
    
    const tsError ret = ts_bspline_resize_back(bspline, n, result);
    if (ret < 0)
        return ret;
    
    const size_t deg = bspline->deg;
    const size_t dim = bspline->dim;  
    const size_t k = deBoorNet->k;
    const size_t N = deBoorNet->n_affected;
    const size_t size_ctrlp = dim * sizeof(float);

    // 1. Copy all necessary control points and knots from 
    //    the original B-Spline.
    // 2. Copy all necessary control points and knots from
    //    the de Boor net.
    
    // 1.
    //
    // a) copy left hand side control points from original b-spline
    // b) copy right hand side control points from original b-spline
    // c) copy left hand side knots from original b-spline
    // d) copy right hand side knots form original b-spline
    const size_t cidx = k-deg+N;
    const size_t kidx = k+1;
    // copy control points
    memmove(result->ctrlp, bspline->ctrlp, (k-deg) * size_ctrlp);
    memmove(
        result->ctrlp + (cidx+n)*dim, // n >= 0 implies cidx+n >= cidx
        bspline->ctrlp + cidx*dim, 
        (result->n_ctrlp-n-cidx) * size_ctrlp
    );
    // copy knots
    memmove(result->knots, bspline->knots, (k+1) * sizeof(float));
    memmove(
        result->knots+kidx+n, // n >= 0 implies kidx+n >= kidx
        bspline->knots+kidx, 
        (bspline->n_knots-n-kidx) * sizeof(float)
    );
    
    // 2.
    //
    // a) copy left hand side control points from de boor net
    // b) copy middle part control points from de boor net
    // c) copy right hand side control points from de boor net
    // d) insert knots with u_k
    size_t i;     // <- used in for loops
    // copy control points
    int from = 0;
    int to = (k-deg)*dim;
    int stride = N*dim;
    int stride_inc = -dim;
    for (i = 0; i < n; i++) {
        memcpy(result->ctrlp+to, deBoorNet->points+from, size_ctrlp);
        from   += stride;
        stride += stride_inc;
        to     += dim;
    }
    memcpy(result->ctrlp+to, deBoorNet->points+from, (N-n) * size_ctrlp);
    from  -= dim;
    to += (N-n)*dim;
    stride = -(N-n+1)*dim;
    stride_inc = -dim;
    for (i = 0; i < n; i++) {
        memcpy(result->ctrlp+to, deBoorNet->points+from, size_ctrlp);
        from   += stride;
        stride += stride_inc;
        to     += dim;
    }
    // copy knots
    from = to = (k+1);
    for (i = 0; i < n; i++) {
        result->knots[to] = deBoorNet->u;
        to++;
    }
    return TS_SUCCESS;
    
    // error handling
    tsError err;
    err_multiplicity:
        err = TS_MULTIPLICITY;
        goto cleanup;
    err_over_underflow:
        err = TS_OVER_UNDERFLOW;
        goto cleanup;
    cleanup:
        if (bspline != result)
            ts_bspline_default(result);
        return err;
}

tsError ts_internal_bspline_resize(
    const tsBSpline* bspline, const int n,
    tsBSpline* resized
)
{
    if (n == 0 && bspline != resized)
        return ts_bspline_copy(bspline, resized);
    
    const size_t deg = bspline->deg;
    const size_t n_ctrlp = bspline->n_ctrlp;
    const size_t new_n_ctrlp = n_ctrlp+n;
    
    if (new_n_ctrlp <= deg)
        goto err_deg_ge_nctrlp;
    else if (n < 0 && new_n_ctrlp > n_ctrlp)
        goto err_over_underflow;
    else if (n > 0 && new_n_ctrlp < n_ctrlp)
        goto err_over_underflow;
    
    const size_t dim = bspline->dim;
    const size_t size_ctrlp = dim * sizeof(float);
    const size_t n_knots = bspline->n_knots;
    const size_t new_n_knots = n_knots+n;

    if (bspline != resized) {
        return ts_bspline_new(deg, dim, new_n_ctrlp, TS_CLAMPED, resized);
    } else {
        float* tmp; // <- used to not loose pointer if realloc fails
        // resize control points
        tmp = (float*) realloc(resized->ctrlp, new_n_ctrlp * size_ctrlp);
        if (tmp == NULL)
            goto err_malloc;
        resized->ctrlp = tmp;
        resized->n_ctrlp = new_n_ctrlp;
        // resize knots
        tmp = (float*) realloc(resized->knots, new_n_knots * sizeof(float));
        if (tmp == NULL)
            goto err_malloc;
        resized->knots = tmp;
        resized->n_knots = new_n_knots;
        return TS_SUCCESS;
    }
    
    // error handling
    tsError err;
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

/********************************************************
*                                                       *
* Interface implementation                              *
*                                                       *
********************************************************/
void ts_deboornet_default(tsDeBoorNet* deBoorNet)
{
    deBoorNet->u          = 0.f;
    deBoorNet->k          = 0;
    deBoorNet->s          = 0;
    deBoorNet->h          = 0;
    deBoorNet->dim        = 0;
    deBoorNet->n_affected = 0;
    deBoorNet->n_points   = 0;
    deBoorNet->points     = NULL;
    deBoorNet->result     = NULL;
}

void ts_deboornet_free(tsDeBoorNet* deBoorNet)
{
    if (deBoorNet->points != NULL) {
        // automatically frees the field result
        free(deBoorNet->points);
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
    const size_t deg, const size_t dim, const size_t n_ctrlp, const tsBSplineType type,
    tsBSpline* bspline
)
{
    ts_bspline_default(bspline);
    
    // check input parameter
    if (dim < 1) {
        return TS_DIM_ZERO;
    }
    if (deg >= n_ctrlp) {
        return TS_DEG_GE_NCTRLP;
    }

    const size_t order   = deg + 1;
    const size_t n_knots = n_ctrlp + order;
    
    // setup fields
    bspline->deg     = deg;
    bspline->order   = order;
    bspline->dim     = dim;
    bspline->n_ctrlp = n_ctrlp;
    bspline->n_knots = n_knots;
    bspline->ctrlp   = (float*) malloc(n_ctrlp * dim * sizeof(float));
    if (bspline->ctrlp == NULL) {
        ts_bspline_free(bspline);
        return TS_MALLOC;
    }
    bspline->knots   = (float*) malloc(n_knots * sizeof(float));
    if (bspline->knots == NULL) {
        ts_bspline_free(bspline);
        return TS_MALLOC;
    }
    
    ts_bspline_fill_knots(bspline, type, bspline);
    
    return TS_SUCCESS;
}

tsError ts_bspline_fill_knots(
    const tsBSpline* original, const tsBSplineType type,
    tsBSpline* result
)
{
    if (original != result) {
        const tsError ret = ts_bspline_copy(original, result);
        if (ret < 0)
            return ret;
    }
    
    // for clamped b-splines setup knot vector with:
    // [multiplicity order, uniformly spaced, multiplicity order]
    // 
    // for opened b-splines setup knot vector with:
    // [uniformly spaced]
    const size_t n_knots = result->n_knots;
    const size_t deg = result->deg;
    const size_t order = result->order;
    
    size_t current, end; // <- used by loops
    size_t numerator, dominator; // <- to fill uniformly spaced elements
    
    if (type == TS_OPENED) {
        current = numerator = 0;
        end = n_knots;
        dominator = end-1;
        for (;current < end; current++, numerator++) {
            result->knots[current] = (float) numerator / dominator;
        }
    } else {
        current = 0;
        end = order;

        // fill first knots with 0
        for (;current < end; current++) {
            result->knots[current] = 0.f;
        }
        
        // uniformly spaced between 0 and 1
        end = n_knots - order;
        numerator = 1;
        dominator = n_knots - (2 * deg) - 1;
        for (;current < end; current++, numerator++) {
            result->knots[current] = (float) numerator / dominator;
        }
        
        // fill last knots with 1
        end = n_knots;
        for (;current < end; current++) {
            result->knots[current] = 1.f;
        }
    }
    
    return TS_SUCCESS;
}

tsError ts_bspline_copy(
    const tsBSpline* original,
    tsBSpline* copy
)
{
    if (original == copy)
        return TS_INPUT_EQ_OUTPUT;
    
    const size_t deg = original->deg;
    const size_t dim = original->dim;
    const size_t n_ctrlp = original->n_ctrlp;
    const size_t sf = sizeof(float);
    
    const tsError ret = ts_bspline_new(deg, dim, n_ctrlp, TS_CLAMPED, copy);
    if (ret >= 0) {
        memcpy(copy->ctrlp, original->ctrlp, n_ctrlp*dim*sf);
        memcpy(copy->knots, original->knots, original->n_knots*sf);
    }
    return ret;
}

tsError ts_bspline_evaluate(
    const tsBSpline* bspline, const float u, 
    tsDeBoorNet* deBoorNet
)
{
    ts_deboornet_default(deBoorNet);
    
    // 1. Find index k such that u is in between [u_k, u_k+1).
    // 2. Check whether b-spline is defined at u
    // 3. Setup already known values
    // 4. Decide by multiplicity of u how to calculate point P(u).
    
    const size_t n_knots = bspline->n_knots; // <- the number of knots
    
    // 1. find index k
    for (deBoorNet->k = 0; deBoorNet->k < n_knots; deBoorNet->k++) {
        const float uk = bspline->knots[deBoorNet->k];
        if (ts_fequals(u, uk)) {
            deBoorNet->s++;
        } else if (u < uk) {
            break;
        }
    }
    
    const size_t s   = deBoorNet->s; // <- the multiplicity of u
    const size_t deg = bspline->deg; // <- the degree of the b-spline
    
    // 2. check u
    //
    // 2a) Check for u < u_0
    // 2b) Check for u > u_max
    // 2c) Check whether b-spline is defined at u for opened b-splines
    if (deBoorNet->k == 0) { // u < u_0
        goto err_u_undefined;
    } else if (deBoorNet->k == n_knots && s == 0) { // u > u_max
        goto err_u_undefined;
    } else if (s <= deg) { // we may have an opened b-spline
        // (keep in mind that currently k is k+1)
        if (deBoorNet->k <= deg || deBoorNet->k > n_knots-deg) {
            goto err_u_undefined;
        }
    }
    
    // 3. setup already known values
    deBoorNet->dim   = bspline->dim;
    deBoorNet->k--; // by 2. k must be >= 1, thus -1 will never underflow
    const float uk = bspline->knots[deBoorNet->k]; // ensures that with any 
                                                   // float precision the knot
    deBoorNet->u = ts_fequals(u, uk) ? uk : u;     // vector stays valid
    deBoorNet->h = deg < s ? 0 : deg-s; // prevent underflow
 
    const size_t order = bspline->order; // <- the order of the b-spline
    const size_t dim   = bspline->dim;   // <- the dimension of one point
    const size_t k     = deBoorNet->k;   // <- the index k of the de boor net
    const size_t size_ctrlp = sizeof(float) * dim; // <- size of one ctrlp
    
    // 4. decide how to calculate P(u)
    //
    // 4a) Check for multiplicity > order.
    //     This is not allowed for any control point.
    // 4b) Check for multiplicity = order.
    //     Take the two points k-s and k-s + 1. If one of
    //     them doesn't exist, take only the other.
    // 4c) Use de boor algorithm to find point P(u). 
    if (s > order) {
        goto err_multiplicity;
    } else if (s == order) {
        // only one of the two control points exists
        if (k == deg ||                  // only the first control point
            k == bspline->n_knots - 1) { // only the last control point
            
            deBoorNet->points = (float*) malloc(size_ctrlp);
            if (deBoorNet->points == NULL) {
                goto err_malloc;
            }
            deBoorNet->result = deBoorNet->points;
            deBoorNet->n_affected = deBoorNet->n_points = 1;
            
            // first control point
            if (k == deg) {
                memcpy(deBoorNet->points, bspline->ctrlp, size_ctrlp);
            // last control point
            } else {
                const size_t from = (k-s) * dim;
                memcpy(deBoorNet->points, bspline->ctrlp+from, size_ctrlp);
            }
            return 1;
        } else {
            deBoorNet->points = (float*) malloc(2 * size_ctrlp);
            if (deBoorNet->points == NULL) {
                goto err_malloc;
            }
            deBoorNet->result = deBoorNet->points+dim;
            deBoorNet->n_affected = deBoorNet->n_points = 2;
            
            // copy both control points
            const size_t from = (k-s) * dim;
            memcpy(deBoorNet->points, bspline->ctrlp+from, 2 * size_ctrlp);
            return 2;
        }
    } else { // by 4a) and 4b) s <= deg (order = deg+1)
        const size_t fst = k-deg; // <- first affected control point, inclusive
                                  //    by 2c) s <= deg implies k > deg
        const size_t lst = k-s;   // <- last affected control point, inclusive
                                  //    s <= deg < k
        const size_t N   = lst-fst + 1; // <- the number of affected ctrlps
        
        deBoorNet->n_affected = N;
        deBoorNet->n_points = N * (N+1) * 0.5f;
        deBoorNet->points = (float*) malloc(deBoorNet->n_points * size_ctrlp);
        if (deBoorNet->points == NULL) {
            goto err_malloc;
        }
        deBoorNet->result = deBoorNet->points + (deBoorNet->n_points-1)*dim;
        
        // copy initial values to output
        memcpy(deBoorNet->points, bspline->ctrlp + fst*dim, N * size_ctrlp);
        
        int idx_l  = 0;       // <- the current left index
        int idx_r  = dim;     // <- the current right index
        int idx_to = N * dim; // <- the current to index
        
        size_t r = 1;
        for (;r <= deBoorNet->h; r++) {
            size_t i = fst + r;
            for (; i <= lst; i++) {
                const float ui    = bspline->knots[i];
                const float a     = (deBoorNet->u - ui) / 
                                    (bspline->knots[i+deg-r+1] - ui);
                const float a_hat = 1.f-a;
                
                size_t d;
                for (d = 0; d < dim; d++) {
                    deBoorNet->points[idx_to++] = 
                        a_hat * deBoorNet->points[idx_l++] + 
                            a * deBoorNet->points[idx_r++];
                }
            }
            idx_l += dim; 
            idx_r += dim;
        }
        
        return 0;
    }
    
    err_malloc:
        ts_deboornet_free(deBoorNet);
        return TS_MALLOC;
    err_u_undefined:
        ts_deboornet_free(deBoorNet);
        return TS_U_UNDEFINED;
    err_multiplicity:
        ts_deboornet_free(deBoorNet);
        return TS_MULTIPLICITY;
}

tsError ts_bspline_insert_knot(
    const tsBSpline* bspline, const float u, const size_t n,
    tsBSpline* result
)
{
    tsError ret;
    tsDeBoorNet net;
    ret = ts_bspline_evaluate(bspline, u, &net);
    if (ret >= 0) {
        ret = ts_internal_bspline_insert_knot(bspline, &net, n, result);
    } else {
        ts_bspline_default(result);
    }
    ts_deboornet_free(&net);
    return ret;
}

tsError ts_bspline_resize_back(
    const tsBSpline* bspline, const int n,
    tsBSpline* resized
)
{
    const tsError ret = ts_internal_bspline_resize(bspline, n, resized);
    if (ret < 0) {
        return ret;
    } else if (bspline != resized) {
        const size_t min_nc = n < 0 ? resized->n_ctrlp : bspline->n_ctrlp;
        const size_t min_nk = n < 0 ? resized->n_knots : bspline->n_knots;
        const size_t sf = sizeof(float);
        memcpy(resized->ctrlp, bspline->ctrlp, min_nc * bspline->dim * sf);
        memcpy(resized->knots, bspline->knots, min_nk * sf);
    }
    return TS_SUCCESS;
}

tsError ts_bspline_resize_front(
    const tsBSpline* bspline, const int n,
    tsBSpline* resized
)
{
    const tsError ret = ts_internal_bspline_resize(bspline, n, resized);
    if (ret < 0) {
        return ret;
    } 
    
    const size_t sf = sizeof(float);
    const size_t dim = bspline->dim;
    if (n < 0) {
        memmove(resized->ctrlp, bspline->ctrlp+n*dim, resized->n_ctrlp*dim*sf);
        memmove(resized->knots, bspline->knots+n, resized->n_knots*sf);
    } else {
        memmove(resized->ctrlp+n*dim, bspline->ctrlp, bspline->n_ctrlp*dim*sf);
        memmove(resized->knots+n, bspline->knots, bspline->n_knots*sf);
    }
    return TS_SUCCESS;
}

tsError ts_bspline_split(
    const tsBSpline* bspline, const float u,
    tsBSpline* split, size_t* k
)
{
    tsError ret;
    tsDeBoorNet net;
    ret = ts_bspline_evaluate(bspline, u, &net);
    if (ret >= 1) {
        if (bspline != split)
            ret = ts_bspline_copy(bspline, split);
        *k = ret >= 0 ? net.k : 0;
    } else if (ret == 0) {
        ret = ts_internal_bspline_insert_knot(bspline, &net, net.h+1, split);
        *k = ret >= 0 ? net.k + net.h + 1 : 0;
    } else {
        if (bspline != split)
            ts_bspline_default(split);
        *k = 0;
    }
    ts_deboornet_free(&net);
    return ret;
}

tsError ts_bspline_buckle(
    const tsBSpline* original, const float b,
    tsBSpline* buckled
)
{
    const tsError ret = ts_bspline_copy(original, buckled);
    if (ret < 0) {
        return ret;
    }
    
    const float b_hat  = 1.f-b;            // <- 1-b
    const size_t dim   = buckled->dim;     // <- dimension of one control point 
    const size_t N     = buckled->n_ctrlp; // <- number of control points
    const float* p0    = buckled->ctrlp;   // <- pointer to P0
    const float* pn_1  = p0 + (N-1)*dim;   // <- pointer to P_n-1
    
    size_t i, d;
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
    ts_bspline_copy(bspline, beziers);
    
    // fix first control point if necessary
    const float u_min = beziers->knots[beziers->deg];
    if (!ts_fequals(beziers->knots[0], u_min)) {
        tsBSpline fixed;
        size_t k;
        float* tmp;
        
        ts_bspline_split(beziers, u_min, &fixed, &k);
        ts_bspline_free(beziers);

        fixed.n_ctrlp -= fixed.deg;
        fixed.n_knots -= fixed.deg;
        memmove(
            fixed.ctrlp, 
            fixed.ctrlp + (fixed.deg * fixed.dim), 
            fixed.n_ctrlp * fixed.dim * sizeof(float)
        );
        memmove(
            fixed.knots, 
            fixed.knots + fixed.deg, 
            fixed.n_knots * sizeof(float)
        );
        
        // if realloc fails
        if (!(tmp = (float*) realloc(
            fixed.ctrlp, 
            fixed.n_ctrlp * fixed.dim * sizeof(float))
        )) {
            ts_bspline_free(&fixed);
            return TS_MALLOC;
        }
        fixed.ctrlp = tmp;

        // if realloc fail  
        if (!(tmp = (float*) realloc(
            fixed.knots, 
            fixed.n_knots * sizeof(float))
        )) {
            ts_bspline_free(&fixed);
            return TS_MALLOC;
        }
        fixed.knots = tmp;

        ts_bspline_copy(&fixed, beziers);
        ts_bspline_free(&fixed);
    }
    
    // fix last control point if necessary
    const float u_max = beziers->knots[beziers->n_knots - beziers->order];
    if (!ts_fequals(beziers->knots[beziers->n_knots-1], u_max)) {
        tsBSpline fixed;
        size_t k;
        float* tmp;
        
        ts_bspline_split(beziers, u_max, &fixed, &k);
        ts_bspline_free(beziers);
        
        fixed.n_ctrlp -= fixed.deg;
        fixed.n_knots -= fixed.deg;
        
        // if realloc fails
        if (!(tmp = (float*) realloc(
            fixed.ctrlp, 
            fixed.n_ctrlp * fixed.dim * sizeof(float))
        )) {
            ts_bspline_free(&fixed);
            return TS_MALLOC;
        }
        fixed.ctrlp = tmp;

        // if realloc fail  
        if (!(tmp = (float*) realloc(
            fixed.knots, 
            fixed.n_knots * sizeof(float))
        )) {
            ts_bspline_free(&fixed);
            return TS_MALLOC;
        }
        fixed.knots = tmp;

        ts_bspline_copy(&fixed, beziers);
        ts_bspline_free(&fixed);
    }
    
    tsBSpline current, next;
    current = *beziers;
    size_t k = bspline->order;

    while (k < current.n_knots - bspline->order) {
        ts_bspline_split(&current, current.knots[k], &next, &k);
        ts_bspline_free(&current);
        current = next;
        k++;
    }
    ts_bspline_copy(&current, beziers);
    ts_bspline_free(&current);
    
    return TS_SUCCESS;
}

int ts_fequals(const float x, const float y)
{
    if (fabs(x-y) <= FLT_MAX_ABS_ERROR) {
        return 1;
    } else {
        const float r = fabs(x) > fabs(y) ? 
            fabs((x-y) / x) : fabs((x-y) / y);
        return r <= FLT_MAX_REL_ERROR;
    }
}