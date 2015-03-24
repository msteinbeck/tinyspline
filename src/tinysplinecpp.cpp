#include "tinysplinecpp.h"

TsBSpline::TsBSpline() 
{
    ts_bspline_default(&bspline);
}

TsBSpline::TsBSpline(TsBSpline& other)
{
    const tsError err = ts_bspline_copy(&other.bspline, &bspline);
    if (err < 0)
        throw err;
}

TsBSpline::TsBSpline(const size_t deg, const size_t dim, const size_t n_ctrlp, 
                     const tsBSplineType type) {
    const tsError err = ts_bspline_new(deg, dim, n_ctrlp, type, &bspline);
    if (err < 0)
        throw err;
}

TsBSpline::~TsBSpline()
{
    ts_bspline_free(&bspline);
}

tsBSpline* TsBSpline::data()
{
    return &bspline;
}

float* TsBSpline::ctrlp() const
{
    return bspline.ctrlp;
}

float* TsBSpline::knots() const
{
    return bspline.knots;
}

size_t TsBSpline::deg() const
{
    return bspline.deg;
}

size_t TsBSpline::order() const
{
    return bspline.order;
}

size_t TsBSpline::dim() const
{
    return bspline.dim;
}

size_t TsBSpline::nCtrlp() const
{
    return bspline.n_ctrlp;
}

size_t TsBSpline::nKnots() const
{
    return bspline.n_knots;
}

void TsBSpline::setDeg(const size_t deg)
{
    if (deg >= bspline.n_ctrlp)
        throw TS_DEG_GE_NCTRLP;
    const size_t order = deg+1;
    if (order < deg)
        throw TS_OVER_UNDERFLOW;
    bspline.deg = deg;
    bspline.order = order;
}

void TsBSpline::setOrder(const size_t order)
{
    const size_t deg = order-1;
    if (deg > order)
        throw TS_OVER_UNDERFLOW;
    if (deg >= bspline.n_ctrlp)
        throw TS_DEG_GE_NCTRLP;
    bspline.order = order;
    bspline.deg = deg;
}

void TsBSpline::setDim(const size_t dim)
{
    if (dim == 0)
        throw TS_DIM_ZERO;
    bspline.dim = dim;
}

void TsBSpline::setNCtrlp(const size_t nCtrlp)
{
    if (bspline.deg >= nCtrlp)
        throw TS_DEG_GE_NCTRLP;
    bspline.n_ctrlp = nCtrlp;
}

void TsBSpline::setNKnots(const size_t nKnots)
{
    // TODO: add error code to c lib
    bspline.n_knots = nKnots;
}

void TsBSpline::setupKnots(const tsBSplineType type)
{
    const tsError err = ts_bspline_setup_knots(&bspline, type, &bspline);
    if (err < 0)
        throw err;
}

void TsBSpline::insertKnot(const float u, const size_t n)
{
    const tsError err = ts_bspline_insert_knot(&bspline, u, n, &bspline);
    if (err < 0)
        throw err;
}

void TsBSpline::resize(const int n, const int back)
{
    const tsError err = ts_bspline_resize(&bspline, n, back, &bspline);
    if (err < 0)
        throw err;
}

size_t TsBSpline::split(const float u)
{
    size_t k;
    const tsError err = ts_bspline_split(&bspline, u, &bspline, &k);
    if (err < 0)
        throw err;
    return k;
}

void TsBSpline::buckle(const float b)
{
    const tsError err = ts_bspline_buckle(&bspline, b, &bspline);
    if (err < 0)
        throw err;
}

void TsBSpline::toBesziers()
{
    const tsError err = ts_bspline_to_beziers(&bspline, &bspline);
    if (err < 0)
        throw err;
}