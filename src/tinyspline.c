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
    ts_bspline_default(result);
    
    if (deBoorNet->s+n > bspline->order) {
        return TS_MULTIPLICITY;
    }
    
    const tsError ret = ts_bspline_new(
        // It doesn't matter whether clamped or opened is used here,
        // because the knots will be copied from the original b-spline anyway.
        bspline->deg, bspline->dim, bspline->n_ctrlp + n, TS_CLAMPED, result
    );
    if (ret < 0) {
        return ret;
    }
    
    // for convenience
    const size_t deg = bspline->deg; // <- the degree of the original b-spline
    const size_t dim = bspline->dim; // <- dimension of one control point
    const int k = deBoorNet->k;      // <- the index k of the de boor net
    const size_t N = 
        deBoorNet->n_affected;       // <- number of affected conrol points
    const size_t size_ctrlp = 
        dim * sizeof(float);         // <- size of one control point

    // == control points ==
    // 1. copy left hand side control points from original b-spline
    // 2. copy left hand side control points from de boor net
    // 3. copy middle part control points from de boor net
    // 4. copy right hand side control points from de boor net
    // 5. copy right hand side control points from original b-spline
    // == knots ==
    // 6. copy left hand side knots from original b-spline
    // 7. fill insert point with u
    // 8. copy right hand side knots form original b-spline
    
    size_t i;     // <- used in for loops
    int from, to; // <- copy indicies
    int stride, stride_inc; // <- offsets for copy indicies
    
    // 1.
    from = to = 0;
    memcpy(&result->ctrlp[to], &bspline->ctrlp[from], (k-deg) * size_ctrlp);
    to += (k-deg)*dim;
    
    // 2.
    from   = 0;
    stride = N*dim;
    stride_inc = -dim;
    for (i = 0; i < n; i++) {
        memcpy(&result->ctrlp[to], &deBoorNet->points[from], size_ctrlp);
        from   += stride;
        stride += stride_inc;
        to     += dim;
    }
    
    // 3.
    memcpy(&result->ctrlp[to], &deBoorNet->points[from], (N-n) * size_ctrlp);
    to += (N-n)*dim;
    
    // 4.
    from  -= dim;
    stride = -(N-n+1)*dim;
    stride_inc = -dim;
    for (i = 0; i < n; i++) {
        memcpy(&result->ctrlp[to], &deBoorNet->points[from], size_ctrlp);
        from   += stride;
        stride += stride_inc;
        to     += dim;
    }

    // 5.
    from = ((k-deg)+N)*dim;
    memcpy(
        &result->ctrlp[to], 
        &bspline->ctrlp[from], 
        (bspline->n_ctrlp-((k-deg)+N)) * size_ctrlp
    );

    // 6.
    from = to = 0;
    memcpy(&result->knots[0], &bspline->knots[0], (k+1)*sizeof(float));
    from = to = (k+1);
    
    // 7.
    for (i = 0; i < n; i++) {
        result->knots[to] = deBoorNet->u;
        to++;
    }
    
    // 8.
    memcpy(
        &result->knots[to], 
        &bspline->knots[from], 
        (bspline->n_knots-from)*sizeof(float)
    );
    
    return TS_SUCCESS;
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
    deBoorNet->deg        = 0;
    deBoorNet->order      = 0;
    deBoorNet->dim        = 0;
    deBoorNet->n_affected = 0;
    deBoorNet->n_points   = 0;
    deBoorNet->last_idx   = 0;
    deBoorNet->points     = NULL;
}

void ts_deboornet_free(tsDeBoorNet* deBoorNet)
{
    if (deBoorNet->points != NULL) {
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

    // for convenience
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
    
    // for clamped b-splines setup knot vector with:
    // [multiplicity order, uniformly spaced, multiplicity order]
    // for opened b-splines setup knot vector with:
    // [uniformly spaced]
    int current, end; // <- used by loops
    size_t numerator, dominator; // <- to fill uniformly spaced elements
    
    if (type == TS_OPENED) {
        current = numerator = 0;
        end = n_knots;
        dominator = n_knots - 1;
        for (;current < end; current++, numerator++) {
            bspline->knots[current] = (float) numerator / dominator;
        }
    } else {
        current = 0;
        end = order;

        // fill first knots with 0
        for (;current < end; current++) {
            bspline->knots[current] = 0.f;
        }
        
        // uniformly spaced between 0 and 1
        end = n_knots - order;
        numerator = 1;
        dominator = n_knots - (2 * deg) - 1;
        for (;current < end; current++, numerator++) {
            bspline->knots[current] = (float) numerator / dominator;
        }
        
        // fill last knots with 1
        end = n_knots;
        for (;current < end; current++) {
            bspline->knots[current] = 1.f;
        }
    }
    return TS_SUCCESS;
}

tsError ts_bspline_copy(
    const tsBSpline* original,
    tsBSpline* copy
)
{
    const tsError ret = ts_bspline_new(
        original->deg,
        original->dim,
        original->n_ctrlp,
        TS_CLAMPED, /* doesn't really matter because we copy knots anyway. */
        copy
    );
    
    if (ret >= 0) {
        memcpy(
            copy->ctrlp, 
            original->ctrlp, 
            original->n_ctrlp * original->dim * sizeof(float)
        );
        memcpy(
            copy->knots, 
            original->knots, 
            original->n_knots * sizeof(float)
        );
    }
    
    return ret;
}

tsError ts_bspline_evaluate(
    const tsBSpline* bspline, const float u, 
    tsDeBoorNet* deBoorNet
)
{
    ts_deboornet_default(deBoorNet);
    deBoorNet->deg   = bspline->deg;
    deBoorNet->order = bspline->order;
    deBoorNet->dim   = bspline->dim;
    
    // 1. Find index k such that u is in between [u_k, u_k+1).
    // 2. Decide by multiplicity of u how to calculate point P(u).
    
    for (deBoorNet->k = 0; deBoorNet->k < bspline->n_knots; deBoorNet->k++) {
        const float uk = bspline->knots[deBoorNet->k];
        if (ts_fequals(u, uk)) {
            deBoorNet->s++;
        } else if (u < uk) {
            break;
        }
    }
    deBoorNet->k--;
    // ensures that with any float precision the knot vector stays valid
    const float uk = bspline->knots[deBoorNet->k];
    deBoorNet->u = ts_fequals(u, uk) ? uk : u;
    deBoorNet->h = deBoorNet->deg - deBoorNet->s;
    
    // for convenience
    const size_t deg = bspline->deg; // <- the degree of the original b-spline
    const size_t dim = bspline->dim; // <- dimension of one control point
    const int k = deBoorNet->k;      // <- the index k of the de boor net
    const int s = deBoorNet->s;      // <- the multiplicity of u        
    const size_t size_ctrlp = 
        sizeof(float) * dim;         // <- size of one control point

    // 1. Check for multiplicity > order.
    //    This is not allowed for any control point.
    // 2. Check for multiplicity = order.
    //    Take the two points k-s and k-s + 1. If one of
    //    them doesn't exist, take only the other.
    // 3. Use de boor algorithm to find point P(u).
    
    if (s > bspline->order) {
        ts_deboornet_free(deBoorNet);
        return TS_MULTIPLICITY;
    } else if (s == bspline->order) {
        const int fst = k-s;   // <- the index k-s
        const int snd = fst+1; // <- the index k-s + 1
        // only one of the two control points exists
        if (fst < 0 || snd >= bspline->n_ctrlp) {
            deBoorNet->n_affected = deBoorNet->n_points = 1;
            deBoorNet->last_idx = 0;
            deBoorNet->points = (float*) malloc(size_ctrlp);
            // error handling
            if (deBoorNet->points == NULL) {
                ts_deboornet_free(deBoorNet);
                return TS_MALLOC;
            }
            // copy only first control point
            if (fst < 0) {
                memcpy(deBoorNet->points, bspline->ctrlp, size_ctrlp);
            // copy only last control point
            } else {
                memcpy(deBoorNet->points, &bspline->ctrlp[fst * dim], size_ctrlp);
            }
            return 1;
        // must be an inner control points, copy both
        } else {
            deBoorNet->n_affected = deBoorNet->n_points = 2;
            deBoorNet->last_idx = dim;
            deBoorNet->points = (float*) malloc(2 * size_ctrlp);
            // error handling
            if (deBoorNet->points == NULL) {
                ts_deboornet_free(deBoorNet);
                return TS_MALLOC;
            }
            memcpy(deBoorNet->points, &bspline->ctrlp[fst * dim], 2 * size_ctrlp);
            return 2;
        }
    } else {
        const int fst = k-deg;  // <- first affected control point, inclusive
        const size_t lst = k-s; // <- last affected control point, inclusive
        
        // b-spline is not defined at u
        if (fst < 0 || lst >= bspline->n_ctrlp) {
            ts_deboornet_free(deBoorNet);
            return TS_U_UNDEFINED;
        }
        
        deBoorNet->n_affected = lst-fst + 1;
        deBoorNet->n_points = 
                deBoorNet->n_affected * (deBoorNet->n_affected + 1) * 0.5f;
        deBoorNet->last_idx = (deBoorNet->n_points - 1) * dim;
        deBoorNet->points = (float*) malloc(deBoorNet->n_points * size_ctrlp);
        
        // error handling
        if (deBoorNet->points == NULL) {
            ts_deboornet_free(deBoorNet);
            return TS_MALLOC;
        }
        
        // copy initial values to output
        memcpy(
            deBoorNet->points, 
            &bspline->ctrlp[fst * dim], 
            deBoorNet->n_affected * size_ctrlp
        );
        
        int idx_l  = 0;   // <- the current left index
        int idx_r  = dim; // <- the current right index
        int idx_to = deBoorNet->n_affected * dim; // <- the current to index
        
        int r = 1;
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

tsError ts_bspline_split(
    const tsBSpline* bspline, const float u,
    tsBSpline* split
)
{
    tsError ret;
    tsDeBoorNet net;
    ret = ts_bspline_evaluate(bspline, u, &net);
    if (ret >= 0) {
        if (net.h < 0 ||
            ts_fequals(bspline->knots[bspline->deg], u) ||
            ts_fequals(bspline->knots[bspline->n_knots - bspline->order], u)) {
            ret = ts_bspline_copy(bspline, split);
            ret = ret >= 0 ? net.k : ret;
        } else {
            ret = ts_internal_bspline_insert_knot(bspline, &net, net.h+1, split);
            ret = ret >= 0 ? net.k + net.h + 1 : ret;
        }
    } else {
        ts_bspline_default(split);
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
    
    // for convenience
    const float b_hat  = 1.f-b;            // <- 1-b
    const size_t dim   = buckled->dim;     // <- dimension of one control point 
    const size_t N     = buckled->n_ctrlp; // <- number of control points
    const float* p0    = buckled->ctrlp;   // <- pointer to P0
    const float* pn_1  =
        &buckled->ctrlp[(N-1) * dim];      // <- pointer to P_n-1
    
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

tsError ts_bspline_to_bezier(
    const tsBSpline* bspline,
    tsBSpline* sequence
)
{
    ts_bspline_copy(bspline, sequence);
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