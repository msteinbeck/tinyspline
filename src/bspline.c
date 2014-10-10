#include "bspline.h"

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>

// =============================
// De Boor
// =============================
void deboornet_free(DeBoorNet* deBoorNet)
{
    if (deBoorNet->points != NULL) {
        free(deBoorNet->points);
    }
}

// =============================
// B-Spline
// =============================
int bspline_new(
    const size_t deg, const size_t dim, const size_t n_ctrlp, const BSplineType type,
    BSpline* bspline
)
{
    // check input parameter
    if (deg < 0 || n_ctrlp < 1 || deg >= n_ctrlp || dim < 1) {
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
    DeBoorNet* deBoorNet
)
{
    // set default values
    deBoorNet->k = 0;
    deBoorNet->s = 0;
    deBoorNet->h = 0;
    deBoorNet->deg = bspline->deg;
    deBoorNet->dim = bspline->dim;
    deBoorNet->n_affected = 0;
    deBoorNet->n_points = 0;
    deBoorNet->last_idx = 0;
    
    // u must be within [0, 1]
    if (u < 0.0f || u > 1.0f) {
        return -1;
    }
    
    // for convenience
    const size_t size_ctrlp = 
        sizeof(float) * deBoorNet->dim; // <- size of one control point
    
    // find [u_k, u_k+1)
    for (; deBoorNet->k < bspline->n_knots; deBoorNet->k++) {
        const float uk     = bspline->knots[deBoorNet->k];
        const float e_u_uk = fabs(u - uk);

        if (e_u_uk < FLT_EPSILON) { // u == u_k
            deBoorNet->s++;
        } else if (u < uk) {
            break;
        }
    }
    deBoorNet->k--;
    deBoorNet->h = deBoorNet->dim - deBoorNet->s;

    // 1. Check for multiplicity > order.
    //    This is not allowed for any control point.
    // 2. Check for multiplicity = order.
    //    Take the two points k-s and k-s + 1. If one of
    //    them doesn't exist, take only the other.
    // 3. Use de boor algorithm to find point P(u).
    
    if (deBoorNet->s > bspline->order) {
        return -2;
    } else if (deBoorNet->s == bspline->order) {
        const int fst = deBoorNet->k - deBoorNet->s; // <- the index k-s
        const int snd = fst+1; // <- the index k-s + 1
        // only one of the two control points exists
        if (fst < 0 || snd >= bspline->n_ctrlp) {
            deBoorNet->points = (float*) malloc(size_ctrlp);
            // error handling
            if (deBoorNet->points == NULL) {
                return -3;
            }
            deBoorNet->n_affected = deBoorNet->n_points = 1;
            deBoorNet->last_idx = 0;
            // copy only first control point
            if (fst < 0) {
                memcpy(deBoorNet->points, bspline->ctrlp, size_ctrlp);
            // copy only last control point
            } else {
                memcpy(deBoorNet->points, &bspline->ctrlp[fst * deBoorNet->dim], size_ctrlp);
            }
            return 1;
        // must be an inner control points, copy both
        } else {
            deBoorNet->points = (float*) malloc(deBoorNet->n_points * size_ctrlp);
            // error handling
            if (deBoorNet->points == NULL) {
                return -3;
            }
            deBoorNet->n_affected = deBoorNet->n_points = 2;
            deBoorNet->last_idx = 1;
            memcpy(deBoorNet->points, bspline->ctrlp, deBoorNet->n_points * size_ctrlp);
            return 2;
        }
    } else {
        const int fst = deBoorNet->k - deBoorNet->deg; // <- first affected control point, inclusive
        const int lst = deBoorNet->k - deBoorNet->s;   // <- last affected control point, inclusive
        
        // b-spline is not defined at u
        if (fst < 0 || lst >= bspline->n_ctrlp) {
            return -1;
        }
        
        deBoorNet->n_affected = lst-fst + 1;
        deBoorNet->n_points = 
                deBoorNet->n_affected * (deBoorNet->n_affected + 1) * 0.5f;
        deBoorNet->last_idx = (deBoorNet->n_points - 1) * deBoorNet->dim;
        deBoorNet->points = (float*) malloc(deBoorNet->n_points * size_ctrlp);
        
        // error handling
        if (deBoorNet->points == NULL) {
            // unset already assigned values
            deBoorNet->n_affected = 0;
            deBoorNet->n_points = 0;
            deBoorNet->last_idx = 0;
            return -3;
        }
        
        // copy initial values to output
        memcpy(
            deBoorNet->points, 
            &bspline->ctrlp[fst * deBoorNet->dim], 
            deBoorNet->n_affected * size_ctrlp
        );
        
        int idx_l  = 0; // <- the current left index
        int idx_r  = idx_l + deBoorNet->dim; // <- the current right index
        int idx_to = deBoorNet->n_affected * deBoorNet->dim; // <- the current to index
        
        int r = 1;
        for (;r <= deBoorNet->h; r++) {
            int i = fst + r;
            for (; i <= lst; i++) {
                const float ui = bspline->knots[i];
                const float a  = (u - ui) / (bspline->knots[i + deBoorNet->deg - r + 1] - ui);
                const float a_hat = 1-a;
                size_t counter;
                for (counter = 0; counter < deBoorNet->dim; counter++) {
                    deBoorNet->points[idx_to++] = 
                            a_hat * deBoorNet->points[idx_l++] + 
                                a * deBoorNet->points[idx_r++];
                }
            }
            idx_l += deBoorNet->dim; 
            idx_r += deBoorNet->dim;
        }
        
        return 0;
    }
}