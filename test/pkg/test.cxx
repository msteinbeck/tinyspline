#include "tinysplinecxx.h"
#include <cassert>

using namespace tinyspline;

int main(int argc, char **argv)
{
	BSpline spline(7, 2, 3, BSpline::Type::CLAMPED);
	assert(spline.controlPoints().size() == 14);
	return 0;
}
