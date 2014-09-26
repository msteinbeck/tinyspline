#include "bspline.h"

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>

int bspline_new_clamped(
    const size_t deg, const size_t dim, const size_t n_ctrlp, 
    BSpline* bspline
)
{
    // check input parameter
    if (deg >= n_ctrlp) {
        return -1;
    }
    
    const size_t order   = deg + 1;         // <- for convenience
    const size_t n_knots = n_ctrlp + order; // <- for convenience
    
    // setup fields
    bspline->deg     = deg;
    bspline->order   = order;
    bspline->dim     = dim;
    bspline->n_ctrlp = n_ctrlp;
    bspline->n_knots = n_knots;
    
    bspline->ctrlp = (float*) malloc(n_ctrlp * dim * sizeof(float));
    if (bspline->ctrlp == NULL) {
        return -1;
    }
    
    bspline->knots   = (float*) malloc(n_knots * sizeof(float));
    if (bspline->knots == NULL) {
        // do not forget to free already allocated arrays
        free(bspline->ctrlp);
        return -1;
    }
    
    // setup knot vector with:
    // [multiplicity order, uniformly spaced, multiplicity order]
    // to provide a clamped, uniformly spaced b-spline.
    size_t current = 0, end = 0; // <- used by loops

    end = order;
    for (;current < end; current++) {
        bspline->knots[current] = 0.f;
    }
    
    end = n_knots - order;
    size_t numerator = 1;
    size_t dominator = n_knots - (2 * deg) - 1;
    for (;current < end; current++, numerator++) {
        bspline->knots[current] = (float) numerator / dominator;
    }
    
    end = n_knots;
    for (;current < end; current++) {
        bspline->knots[current] = 1.f;
    }
    
    return 0;
}

void bspline_free(BSpline* bspline)
{
    if (bspline->ctrlp != NULL) {
        free(bspline->ctrlp);
    }
    
    if (bspline->knots != NULL) {
        free(bspline->knots);
    }
}

int bspline_evaluate(
    const BSpline* bspline, float u, 
    float* point
)
{
    if (u < 0.0f || u > 1.0f) {
        return -1;
    }
    
    // find [u_k, u_k+1)
    int k = 0; // <- the index
    int s = 0; // <- the multiplicity of u_k
    int h = 0; // <- how many times do we need to insert u
    for (; k < bspline->n_knots; k++) {
        const float uk     = bspline->knots[k];
        const float e_u_uk = fabs(u - uk);

        if (e_u_uk < FLT_EPSILON) { // u == u_k
            s++;
        } else if (u < uk) {
            break;
        }
    }
    k--;
    h = bspline->deg - s; 

    // 1. Check for multiplicity > order.
    //    This is not allowed for any control point.
    // 2. Check for multiplicity = order.
    //    Take the two points k-s and (k-s) + 1. If one of
    //    them doesn't exist, take only the other.
    // 3. Use de boor algorithm to find point P(u).
    if (s > bspline->order) {
        return -2;
    } else if (s == bspline->order) {
        const int ctrlp_i = k-s;
        // spacial case: first control point
        if (ctrlp_i < 0) { 
            memcpy(point, bspline->ctrlp, bspline->dim * sizeof(float));
            return 1;
        // spacial case: last control point
        } else if (ctrlp_i + 1 >= bspline->n_ctrlp) { 
            memcpy(point, &bspline->ctrlp[ctrlp_i], bspline->dim * sizeof(float));
            return 1;
        // inner control points
        } else {
            memcpy(point, bspline->ctrlp, bspline->dim * sizeof(float) * 2);
            return 2;
        }
    } else {
        // s <= bspline->deg
        const int lower = k - bspline->deg;
        const int upper = k-s; // inclusive
        if (lower < 0 || upper >= bspline->n_ctrlp) {
            return -1;
        }
        
        const int amount   = (upper - lower) + 1;
        float* accumulator = (float*) malloc(amount * bspline->deg * sizeof(float));
        memcpy(accumulator, &bspline->ctrlp[lower], amount * sizeof(float));
        
        int r = 1;
        for (;r <= h; r++) {
            int i = (k - bspline->deg) + r;
            for (; i <= upper; i++) {
                float ui = bspline->knots[i];
                float a  = (u - ui) / (bspline->knots[i + bspline->deg - r + 1] - ui);
            }
        }
        return 0;
    }
}