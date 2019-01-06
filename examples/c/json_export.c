#include "tinyspline.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    tsReal points[15] = { /**< Data points to interpolate. */
            1, -1, 0,
            -1, 2, 0,
            1, 4, 0,
            4, 3, 0,
            7, 5, 0
    };

    tsBSpline spline; /**< Stores the interpolated spline. */
    char *json; /** < Stores the JSON string. */

    ts_bspline_interpolate_cubic(points, 5, 3, &spline, NULL);
    ts_bspline_to_json(&spline, &json, NULL);

    puts(json);
    return 0;
}