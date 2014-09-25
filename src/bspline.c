#include "bspline.h"
#include <stdlib.h>

int bspline_new_clamped(
    const size_t deg, const size_t dim, const size_t n_ctrlp, 
    BSpline* bspline
)
{
    // check input parameter
    if (deg >= n_ctrlp) {
        return -1;
    }
    
    const size_t n_knots = n_ctrlp + deg + 1; // <- for convenience
    const size_t order = deg + 1;             // <- for convenience
    
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