#include "tinysplinecpp.h"
#include <stdexcept>

TsDeBoorNet::TsDeBoorNet()
{
    ts_deboornet_default(&deBoorNet);
}

TsDeBoorNet::TsDeBoorNet(const TsDeBoorNet &other) {
    const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

TsDeBoorNet::~TsDeBoorNet()
{
    ts_deboornet_free(&deBoorNet);
}

TsDeBoorNet &TsDeBoorNet::operator=(const TsDeBoorNet &other) {
    const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
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

std::vector<float> TsDeBoorNet::points() const
{
    const float* begin = deBoorNet.points;
    const float* end = begin + (deBoorNet.n_points-1)*deBoorNet.dim;
    return std::vector<float>(begin, end);
}

std::vector<float> TsDeBoorNet::result() const
{
    const float* begin = deBoorNet.result;
    const float* end = begin + deBoorNet.dim;
    return std::vector<float>(begin, end);
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
        throw std::runtime_error(ts_enum_str(err));
}

TsBSpline::TsBSpline(const size_t deg, const size_t dim, const size_t nCtrlp,
                     const tsBSplineType type)
{
    const tsError err = ts_bspline_new(deg, dim, nCtrlp, type, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

TsBSpline::TsBSpline(const std::vector<float> points, const size_t dim)
{
    if (dim == 0)
        throw std::runtime_error(ts_enum_str(TS_DIM_ZERO));
    if (points.size() % dim != 0)
        throw std::runtime_error("#points % dim == 0 failed");
    const tsError err = ts_bspline_interpolate(
            points.data(), points.size()/dim, dim, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

TsBSpline::~TsBSpline()
{
    ts_bspline_free(&bspline);
}

TsBSpline &TsBSpline::operator=(const TsBSpline &other) {
    const tsError err = ts_bspline_copy(&other.bspline, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return *this;
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

std::vector<float> TsBSpline::ctrlp() const
{
    const float* begin  = bspline.ctrlp;
    const float* end = begin + (bspline.n_ctrlp-1)*bspline.dim;
    return std::vector<float>(begin, end);
}

std::vector<float> TsBSpline::knots() const
{
    const float* begin = bspline.knots;
    const float* end = begin + (bspline.n_knots-1);
    return std::vector<float>(begin, end);
}

tsBSpline* TsBSpline::data()
{
    return &bspline;
}

void TsBSpline::setDeg(const size_t deg)
{
    setOrder(deg+1);
}

void TsBSpline::setOrder(const size_t order)
{
    const size_t deg = order-1;
    if (deg >= bspline.n_ctrlp)
        throw std::runtime_error(ts_enum_str(TS_DEG_GE_NCTRLP));
    bspline.order = order;
    bspline.deg = deg;
}

void TsBSpline::setCtrlp(const std::vector<float> ctrlp)
{
    if (ctrlp.size() != nCtrlp() * dim()) {
        throw std::runtime_error("The number of values must be equals to the"
            " spline's number of control points multiplied by the dimension"
            " of each control point.");
    }
    const tsError err = ts_bspline_set_ctrlp(&bspline, ctrlp.data(), &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

void TsBSpline::setKnots(const std::vector<float> knots)
{
    if (knots.size() != nKnots()) {
        throw std::runtime_error("The number of values must be equals to the"
        " spline's number of knots.");
    }
    const tsError err = ts_bspline_set_knots(&bspline, knots.data(), &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

void TsBSpline::setupKnots(const tsBSplineType type)
{
    const tsError err = ts_bspline_setup_knots(&bspline, type, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

size_t TsBSpline::insertKnot(const float u, const size_t n)
{
    size_t k;
    const tsError err = ts_bspline_insert_knot(&bspline, u, n, &bspline, &k);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return k;
}

void TsBSpline::resize(const int n, const int back)
{
    const tsError err = ts_bspline_resize(&bspline, n, back, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

size_t TsBSpline::split(const float u)
{
    size_t k;
    const tsError err = ts_bspline_split(&bspline, u, &bspline, &k);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return k;
}

void TsBSpline::buckle(const float b)
{
    const tsError err = ts_bspline_buckle(&bspline, b, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

void TsBSpline::toBeziers()
{
    const tsError err = ts_bspline_to_beziers(&bspline, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

TsDeBoorNet TsBSpline::evaluate(const float u) const
{
    TsDeBoorNet deBoorNet;
    const tsError err = ts_bspline_evaluate(&bspline, u, deBoorNet.data());
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return deBoorNet;
}