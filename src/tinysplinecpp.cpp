#include "tinysplinecpp.h"

TsBSpline::TsBSpline() 
{
    ts_bspline_default(&bspline);
}

TsBSpline::TsBSpline(TsBSpline& other)
{
    ts_bspline_copy(&other.bspline, &bspline);
}

TsBSpline::TsBSpline(const size_t deg, const size_t dim, const size_t n_ctrlp, const tsBSplineType type) {
    ts_bspline_new(deg, dim, n_ctrlp, type, &bspline);
}

TsBSpline::~TsBSpline()
{
    ts_bspline_free(&bspline);
}

tsBSpline* TsBSpline::data()
{
    return &bspline;
}

size_t TsBSpline::deg()
{
    return bspline.deg;
}

size_t TsBSpline::order()
{
    return bspline.order;
}

size_t TsBSpline::dim()
{
    return bspline.dim;
}

size_t TsBSpline::nCtrlp() 
{
    return bspline.n_ctrlp;
}

size_t TsBSpline::nKnots()
{
    return bspline.n_knots;
}

float* TsBSpline::ctrlp()
{
    return bspline.ctrlp;
}

float* TsBSpline::konts()
{
    return bspline.knots;
}