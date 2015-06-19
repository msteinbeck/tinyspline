#include "tinysplinecpp.h"

TsDeBoorNet::TsDeBoorNet()
{
    ts_deboornet_default(&deBoorNet);
}

TsDeBoorNet::TsDeBoorNet(const TsDeBoorNet &other) {
    const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
    if (err < 0)
        throw err;
}

TsDeBoorNet::~TsDeBoorNet()
{
    ts_deboornet_free(&deBoorNet);
}

TsDeBoorNet &TsDeBoorNet::operator=(const TsDeBoorNet &other) {
    if (this != &other) {
        const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
        if (err < 0)
            throw err;
    }
    return *this;
}

float TsDeBoorNet::u() const
{
    return deBoorNet.u;
}

size_t TsDeBoorNet::k() const
{
    return deBoorNet.k;
}

size_t TsDeBoorNet::s() const
{
    return deBoorNet.s;
}

size_t TsDeBoorNet::h() const
{
    return deBoorNet.h;
}

size_t TsDeBoorNet::dim() const
{
    return deBoorNet.dim;
}

size_t TsDeBoorNet::nPoints() const
{
    return deBoorNet.n_points;
}

float* TsDeBoorNet::points()
{
    return deBoorNet.points;
}

float* TsDeBoorNet::result()
{
    return deBoorNet.result;
}

tsDeBoorNet* TsDeBoorNet::data()
{
    return &deBoorNet;
}




TsBSpline::TsBSpline() 
{
    ts_bspline_default(&bspline);
}

TsBSpline::TsBSpline(const TsBSpline& other)
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

TsBSpline &TsBSpline::operator=(const TsBSpline &other) {
    if (this != &other) {
        const tsError err = ts_bspline_copy(&other.bspline, &bspline);
        if (err < 0)
            throw err;
    }
    return *this;
}

bool TsBSpline::operator ==(const TsBSpline& other) const
{
    return static_cast<bool>(ts_bspline_equals(&bspline, &other.bspline));
}

TsDeBoorNet TsBSpline::operator()(const float u) const
{
    return evaluate(u);
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

float* TsBSpline::ctrlp()
{
    return bspline.ctrlp;
}

float* TsBSpline::knots()
{
    return bspline.knots;
}

tsBSpline* TsBSpline::data()
{
    return &bspline;
}

void TsBSpline::setDeg(const size_t deg)
{
    if (deg >= bspline.n_ctrlp)
        throw TS_DEG_GE_NCTRLP;
    const size_t order = deg+1;
    bspline.deg = deg;
    bspline.order = order;
}

void TsBSpline::setOrder(const size_t order)
{
    const size_t deg = order-1;
    if (deg >= bspline.n_ctrlp)
        throw TS_DEG_GE_NCTRLP;
    bspline.order = order;
    bspline.deg = deg;
}

void TsBSpline::setupKnots(const tsBSplineType type)
{
    const tsError err = ts_bspline_setup_knots(&bspline, type, &bspline);
    if (err < 0)
        throw err;
}

size_t TsBSpline::insertKnot(const float u, const size_t n)
{
    size_t k;
    const tsError err = ts_bspline_insert_knot(&bspline, u, n, &bspline, &k);
    if (err < 0)
        throw err;
    return k;
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

void TsBSpline::toBeziers()
{
    const tsError err = ts_bspline_to_beziers(&bspline, &bspline);
    if (err < 0)
        throw err;
}

TsDeBoorNet TsBSpline::evaluate(const float u) const
{
    TsDeBoorNet deBoorNet;
    ts_bspline_evaluate(&bspline, u, deBoorNet.data());
    return deBoorNet;
}