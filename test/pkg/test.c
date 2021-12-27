#include "tinyspline.h"
#include <assert.h>

int main(int argc, char **argv)
{
	tsBSpline spline = ts_bspline_init();
	assert(!spline.pImpl);
	ts_bspline_new(7, 2, 3, TS_CLAMPED, &spline, NULL);
	assert(spline.pImpl);
	ts_bspline_free(&spline);
	assert(!spline.pImpl);
	return 0;
}
