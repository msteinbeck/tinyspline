#ifndef DEBUGGING_H
#define	DEBUGGING_H

#include "tinyspline.h"
#include <stdio.h>

void ts_bspline_print(const tsBSpline *spline)
{
	tsReal *ctrlp, *knots;
	size_t i, j;
	const size_t dim = ts_bspline_dimension(spline);
	const size_t num_ctrlp = ts_bspline_num_control_points(spline);
	const size_t num_knots = ts_bspline_num_knots(spline);
	ts_bspline_control_points(spline, &ctrlp, NULL);
	ts_bspline_knots(spline, &knots, NULL);
	
	printf("########### control points (%lu) ###########\n",
		   (unsigned long) num_ctrlp);
	for (i = 0; i < num_ctrlp; i++) {
		for (j = 0; j < dim; j++) {
			printf("%f\t", ctrlp[i * dim + j]);
		}
		printf("\n");
	}
	printf("\n");
	
	printf("########### knots(%lu) ###########\n",
		   (unsigned long) num_knots);
	for (i = 0; i < num_knots; i++) {
		printf("%f  ", knots[i]);
	}
	printf("\n\n");

	free(ctrlp);
	free(knots);
}

void ts_deboornet_print(const tsDeBoorNet *net)
{
	tsReal *points;
	size_t i, j;
	const size_t dim = ts_deboornet_dimension(net);
	const size_t num_points = ts_deboornet_num_points(net);
	ts_deboornet_points(net, &points, NULL);
	
	printf("########### points (%lu) ###########\n",
		   (unsigned long) num_points);
	for (i = 0; i < num_points; i++) {
		for (j = 0; j < dim; j++) {
			printf("%f\t", points[i * dim + j]);
		}
		printf("\n");
	}
	printf("\n\n");

	free(points);
}

#endif	/* DEBUGGING_H */