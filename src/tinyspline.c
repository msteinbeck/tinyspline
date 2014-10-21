#include "tinyspline.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>

void ts_deboornet_default(tsDeBoorNet* deBoorNet)
{
    deBoorNet->k          = 0;
    deBoorNet->s          = 0;
    deBoorNet->h          = 0;
    deBoorNet->deg        = 0;
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

void ts_bsplinesequence_default(tsBSplineSequence* sequence)
{
    sequence->n_bsplines = 0;
    sequence->bsplines   = NULL;
}

void ts_bsplinesequence_free(tsBSplineSequence* sequence)
{
    int n = 0;
    for (; n < sequence->n_bsplines; n++) {
        ts_bspline_free(&sequence->bsplines[n]);
    }
    if (sequence->bsplines != NULL) {
        free(sequence->bsplines);
    }
    ts_bsplinesequence_default(sequence);
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
    size_t current, end; // <- used by loops
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
    ts_bspline_default(copy);
    
    const tsError val = ts_bspline_new(
        original->deg,
        original->dim,
        original->n_ctrlp,
        TS_CLAMPED, /* doesn't really matter because we copy knots anyway. */
        copy
    );
    
    if (val >= 0) {
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
    
    return val;
}

tsError ts_bspline_evaluate(
    const tsBSpline* bspline, const float u, 
    tsDeBoorNet* deBoorNet
)
{
    ts_deboornet_default(deBoorNet);
    deBoorNet->deg = bspline->deg;
    deBoorNet->dim = bspline->dim;
    
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
                return TS_MALLOC;
            }
            memcpy(deBoorNet->points, &bspline->ctrlp[fst * dim], 2 * size_ctrlp);
            return 2;
        }
    } else {
        const int fst = k-deg; // <- first affected control point, inclusive
        const int lst = k-s;   // <- last affected control point, inclusive
        
        // b-spline is not defined at u
        if (fst < 0 || lst >= bspline->n_ctrlp) {
            return TS_U_UNDEFINED;
        }
        
        deBoorNet->n_affected = lst-fst + 1;
        deBoorNet->n_points = 
                deBoorNet->n_affected * (deBoorNet->n_affected + 1) * 0.5f;
        deBoorNet->last_idx = (deBoorNet->n_points - 1) * dim;
        deBoorNet->points = (float*) malloc(deBoorNet->n_points * size_ctrlp);
        
        // error handling
        if (deBoorNet->points == NULL) {
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
            int i = fst + r;
            for (; i <= lst; i++) {
                const float ui = bspline->knots[i];
                const float a  = (u - ui) / (bspline->knots[i+deg-r+1] - ui);
                const float a_hat = 1-a;
                size_t counter;
                for (counter = 0; counter < dim; counter++) {
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

tsError ts_bspline_split(
    const tsBSpline* bspline, const float u,
    tsBSplineSequence* split
)
{
    const tsError defVal = ts_bsplinesequence_new(2, split);
    if (defVal < 0) {
        return defVal;
    }
    
    // split b-spline at P(u).
    tsDeBoorNet net;
    const tsError evalVal = ts_bspline_evaluate(bspline, u, &net);
    if (evalVal < 0) {
        ts_bsplinesequence_free(split);
        return evalVal;
    }
    
    // for convenience
    const size_t deg = bspline->deg; // <- the degree of the original b-spline
    const size_t dim = bspline->dim; // <- dimension of one control point
    const size_t N = net.n_affected; // <- number of affected conrol points
    const int k = net.k;             // <- the index k of the de boor net
    const int s = net.s;             // <- the multiplicity of u
    const size_t size_ctrlp = 
        dim * sizeof(float);         // <- size of one control point

    if (evalVal == 0) {
        tsError newVal;
        const size_t n_ctrlp[2] = {k-deg+N, bspline->n_ctrlp-(k-s)+N-1};
        
        newVal = ts_bspline_new(deg, dim, n_ctrlp[0], TS_CLAMPED, &split->bsplines[0]);
        if (newVal < 0) {
            ts_bsplinesequence_free(split);
            ts_deboornet_free(&net);
            return newVal;
        }
        
        newVal = ts_bspline_new(deg, dim, n_ctrlp[1], TS_CLAMPED, &split->bsplines[1]);
        if (newVal < 0) {
            ts_bsplinesequence_free(split);
            ts_deboornet_free(&net);
            return newVal;
        }
        
        // the offsets to use while copying control points
        // from the original b-spline to the new one
        const size_t from_b[2] = {0, (k-s + 1) * dim};
        const size_t to_b[2]   = {0, N * dim};
        
        // the offsets to use while copying control points
        // from the de boor net to the new b-splines
        size_t from_n[2] = {0, (net.n_points - 1) * dim};
        size_t to_n[2]   = {(n_ctrlp[0] - N) * dim, 0};
        int stride[2]    = {N * dim, -dim}; // <- the next index to use
        const int stride_inc = -dim;
        
        // the offsets to use while copying knots
        // from the original b-spline to the new one
        size_t from_k[2] = {0, k+1};
        size_t to_k[2]   = {0, bspline->order};
        const size_t amount_k[2] = {k-s + 1, bspline->n_knots - (k+1)};
        
        // the offset to use while adding u to 
        // the knot vector of the new b-spline
        size_t to_u[2] = {k-s + 1, 0};
        const size_t amount_u = bspline->order;
        
        // for both parts of the split
        int idx, n;
        for (idx = 0; idx < 2; idx++) {
            // copy the necessary control points from the original b-spline
            memcpy(
                &split->bsplines[idx].ctrlp[to_b[idx]], 
                &bspline->ctrlp[from_b[idx]], 
                (n_ctrlp[idx] - N) * size_ctrlp
            );
            
            // copy the remaining control points from the de boor net
            for (n = 0; n < N; n++) {
                memcpy(
                    &(split->bsplines[idx].ctrlp[to_n[idx]]), 
                    &net.points[from_n[idx]], 
                    size_ctrlp
                );
                
                from_n[idx] += stride[idx];
                stride[idx] += stride_inc;
                to_n[idx]   += dim;
            }
            
            // copy the necessary knots from the original b-spline
            memcpy(
                &split->bsplines[idx].knots[to_k[idx]], 
                &bspline->knots[from_k[idx]], 
                amount_k[idx] * sizeof(float)
            );
            
            // adding u to the knot vector
            for (n = 0; n < amount_u; n++) {
                split->bsplines[idx].knots[to_u[idx]] = u;
                to_u[idx]++;
            }
        }
    } else if (evalVal == 1) {
        const tsError copyVal = ts_bspline_copy(bspline, &split->bsplines[0]);
        if (copyVal < 0) {
            ts_bsplinesequence_free(split);
            ts_deboornet_free(&net);
            return copyVal;
        }
    } else {
        tsError newVal;
        const size_t n_ctrlp[2] = {k-s + 1, bspline->n_ctrlp - (k-s + 1)};
        
        newVal = ts_bspline_new(deg, dim, n_ctrlp[0], TS_CLAMPED, &split->bsplines[0]);
        if (newVal < 0) {
            ts_bsplinesequence_free(split);
            ts_deboornet_free(&net);
            return newVal;
        }
        
        newVal = ts_bspline_new(deg, dim, n_ctrlp[1], TS_CLAMPED, &split->bsplines[1]);
        if (newVal < 0) {
            ts_bsplinesequence_free(split);
            ts_deboornet_free(&net);
            return newVal;
        }
        
        const size_t n_knots[2] = 
            {split->bsplines[0].n_knots, split->bsplines[1].n_knots};
        
        memcpy(
            split->bsplines[0].ctrlp, 
            bspline->ctrlp, 
            n_ctrlp[0] * size_ctrlp
        );
        memcpy(
            split->bsplines[0].knots, 
            bspline->knots, 
            n_knots[0] * sizeof(float)
        );
        memcpy(
            split->bsplines[1].ctrlp, 
            &bspline->ctrlp[n_ctrlp[0] * dim], 
            n_ctrlp[1] * size_ctrlp
        );
        memcpy(
            split->bsplines[1].knots, 
            &bspline->knots[bspline->n_knots - n_knots[1]], 
            n_knots[1] * sizeof(float)
        );
    }
    
    ts_deboornet_free(&net);
    return TS_SUCCESS;
}

tsError ts_bsplinesequence_new(
    const size_t n, 
    tsBSplineSequence* sequence
)
{
    ts_bsplinesequence_default(sequence);

    if (n > 0) {
        sequence->bsplines = (tsBSpline*) malloc(n * sizeof(tsBSpline));
        if (sequence->bsplines == NULL) {
            return TS_MALLOC;
        }
        
        int i = 0;
        for (; i < n; i++) {
            ts_bspline_default(&sequence->bsplines[i]);
        }
        
        sequence->n_bsplines = n;
    }

    return TS_SUCCESS;
}

int ts_fequals(const float x, const float y)
{
    if (fabs(x-y) < FLT_MAX_ABS_ERROR) {
        return 1;
    } else {
        const float r = fabs(x) > fabs(y) ? 
            fabs((x-y) / x) : fabs((x-y) / y);
        return r <= FLT_MAX_REL_ERROR;
    }
}
