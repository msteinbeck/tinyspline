#ifndef DEBUGGING_H
#define	DEBUGGING_H

#include "tinyspline.h"
#include <stdio.h>

void ts_bspline_print(tsBSpline* bspline)
{
    int i, j;
    
    printf("########### control points (%zu) ###########\n", bspline->n_ctrlp);
    for (i = 0; i < bspline->n_ctrlp; i++) {
        for (j = 0; j < bspline->dim; j++) {
            printf("%f\t", bspline->ctrlp[i * bspline->dim + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    printf("########### knots(%zu) ###########\n", bspline->n_knots);
    for (i = 0; i < bspline->n_knots; i++) {
        printf("%f  ", bspline->knots[i]);
    }
    printf("\n\n");
}

#endif	/* DEBUGGING_H */