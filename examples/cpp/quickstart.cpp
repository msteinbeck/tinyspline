#include <iostream>
#include "tinysplinecpp.h"

int main( int argc, char** argv )
{
    try
    {
        // t variable
        ts::rational t = 0.0f;

        // Create a clamped spline of degree 3 in 2D consisting of 7 control points.
        ts::BSpline spline(3, 2, 7, TS_CLAMPED);

        // Setup the control points.
        std::vector<ts::rational> ctrlp = spline.ctrlp();
        ctrlp[0] = -1.75f; // x0
        ctrlp[1] = -1.0f;  // y0
        ctrlp[2] = -1.5f;  // x1
        ctrlp[3] = -0.5f;  // y1
        ctrlp[4] = -1.5f;  // x2
        ctrlp[5] = 0.0f;  // y2
        ctrlp[6] = -1.25f; // x3
        ctrlp[7] = 0.5f;  // y3
        ctrlp[8] = -0.75f; // x4
        ctrlp[9] = 0.75f; // y4
        ctrlp[10] = 0.0f;  // x5
        ctrlp[11] = 0.5f;  // y5
        ctrlp[12] = 0.5f;  // x6
        ctrlp[13] = 0.0f;  // y6
        spline.setCtrlp(ctrlp);

        // Evaluate `spline` at t = 0.4
        t = 0.4f;
        std::vector<ts::rational> result = spline.evaluate(t).result();
        std::cout << "evaluate spline at t=" << t << ": (" << result[0] << ", " << result[1] << ")" << std::endl;

        // Derive `spline` and subdivide it into a sequence of Bezier curves.
        ts::BSpline beziers = spline.derive().toBeziers();

        // Evaluate `beziers` at t = 0.3
        t = 0.3f;
        result = beziers(t).result(); // you can use '()' instead of 'evaluate'
        std::cout << "evaluate beziers at t=" << t << ": (" << result[0] << ", " << result[1] << ")" << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}

