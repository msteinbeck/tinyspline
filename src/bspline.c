#include "bspline.h"

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>

int bspline_new(
    const size_t deg, const size_t dim, const size_t n_ctrlp, BSplineType type,
    BSpline* bspline
)
{
    // check input parameter
    if (deg < 1 || n_ctrlp < 2 || deg >= n_ctrlp || dim < 1) {
        return -1;
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
    
    bspline->ctrlp = (float*) malloc(n_ctrlp * dim * sizeof(float));
    if (bspline->ctrlp == NULL) {
        return -2;
    }
    
    bspline->knots   = (float*) malloc(n_knots * sizeof(float));
    if (bspline->knots == NULL) {
        // do not forget to free already allocated arrays
        free(bspline->ctrlp);
        return -2;
    }
    
    // for clamped b-splines setup knot vector with:
    // [multiplicity order, uniformly spaced, multiplicity order]
    // for opened b-splines setup knot vector with:
    // [uniformly spaced]
    size_t current, end; // <- used by loops
    size_t numerator, dominator; // <- to fill uniformly spaced elements
    
    if (type == OPENED) {
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
    const BSpline* bspline, const float u, 
    size_t* n_points, float** points
)
{
    // u must be within [0, 1]
    if (u < 0.0f || u > 1.0f) {
        *n_points = 0;
        return -1;
    }
    
    // for convenience
    const size_t dim = bspline->dim;
    const size_t p   = bspline->deg;
    const size_t size_ctrlp = sizeof(float) * dim; // <- size of one control point
    
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
    h = p-s; 

    // 1. Check for multiplicity > order.
    //    This is not allowed for any control point.
    // 2. Check for multiplicity = order.
    //    Take the two points k-s and k-s + 1. If one of
    //    them doesn't exist, take only the other.
    // 3. Use de boor algorithm to find point P(u).
    if (s > bspline->order) {
        *n_points = 0;
        return -2;
    } else if (s == bspline->order) {
        const int fst = k-s;   // <- the index k-s
        const int snd = fst+1; // <- the index k-s + 1
        // only one of the two control points exists
        if (fst < 0 || snd >= bspline->n_ctrlp) {
            *n_points = 1;
            *points = (float*) malloc(size_ctrlp);
            // error handling
            if (*points == NULL) {
                *n_points = 0;
                return -3;
            }
            // copy only first control point
            if (fst < 0) {
                memcpy(*points, bspline->ctrlp, size_ctrlp);
            // copy only last control point
            } else {
                memcpy(*points, &bspline->ctrlp[fst * dim], size_ctrlp);
            }
            return 1;
        // must be an inner control points, copy both
        } else {
            *n_points = 2;
            *points = (float*) malloc(size_ctrlp * *n_points);
            memcpy(*points, bspline->ctrlp, size_ctrlp * *n_points);
            return 2;
        }
    } else {
        const int fst = k-p; // <- first affected control point, inclusive
        const int lst = k-s; // <- last affected control point, inclusive
        
        // spline is not defined at u
        if (fst < 0 || lst >= bspline->n_ctrlp) {
            *n_points = 0;
            return -1;
        }
        
        const size_t n_affected = lst-fst + 1;
        *n_points = (1.f/2.f) * n_affected * (n_affected + 1);
        *points   = (float*) malloc(*n_points * size_ctrlp);
        // error handling
        if (*points == NULL) {
            *n_points = 0;
            return -3;
        }
        memcpy(*points, &bspline->ctrlp[fst * dim], n_affected * size_ctrlp);
        
        int idx_l  = 0; // <- the current left index
        int idx_r  = idx_l + dim; // <- the current right index
        int idx_to = n_affected * dim; // <- the current to index
        
        int r = 1;
        for (;r <= h; r++) {
            int i = fst + r;
            for (; i <= lst; i++) {
                const float ui = bspline->knots[i];
                const float a  = (u - ui) / (bspline->knots[i+p-r+1] - ui);
                const float a_hat = 1-a;
                size_t counter;
                for (counter = 0; counter < dim; counter++) {
                    (*points)[idx_to++] = 
                            a_hat * (*points)[idx_l++] + 
                                a * (*points)[idx_r++];
                }
            }
            idx_l += dim; 
            idx_r += dim;
        }        
        return 0;
    }
}