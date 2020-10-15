#include "tinysplinecxx.h"
#include <cassert>

int main(int argc, char **argv)
{
	tinyspline::BSpline spline(7, 2, 3, TS_CLAMPED);
	assert(spline.controlPoints().size() == 14);
	return 0;
}
