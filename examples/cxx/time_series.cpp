#include <iostream>
#include "tinysplinecxx.h"

int main(int argc, char **argv)
{
	std::vector<tinyspline::real> points;
	// P0
	points.push_back(1600); // time
	points.push_back(10);
	points.push_back(100);
	points.push_back(1);
	// P1
	points.push_back(1650); // time
	points.push_back(20);
	points.push_back(200);
	points.push_back(2);
	// P2
	points.push_back(1700); // time
	points.push_back(30);
	points.push_back(300);
	points.push_back(3);
	// P3
	points.push_back(1800); // time
	points.push_back(40);
	points.push_back(400);
	points.push_back(4);
	// P4
	points.push_back(1900); // time
	points.push_back(80);
	points.push_back(600);
	points.push_back(10);
	// P5
	points.push_back(2000); // time
	points.push_back(40);
	points.push_back(400);
	points.push_back(4);

	tinyspline::BSpline spline = tinyspline::BSpline::
		interpolateCubicNatural(points, 4);

	tinyspline::DeBoorNet net = spline.bisect(1850);
	std::vector<tinyspline::real> result = net.result();
	std::cout << "t = " << result[0] << ", p = (" << result[1] << ", "
			<< result[2] << ", " << result[3] << ")" << std::endl;
}
