#include "tinysplinecpp.h"
#include <stdexcept>

ts::DeBoorNet::DeBoorNet()
{
    ts_deboornet_default(&deBoorNet);
}

ts::DeBoorNet::DeBoorNet(const ts::DeBoorNet& other)
{
    const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

ts::DeBoorNet::~DeBoorNet()
{
    ts_deboornet_free(&deBoorNet);
}

ts::DeBoorNet& ts::DeBoorNet::operator=(const ts::DeBoorNet& other)
{
    const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return *this;
}

float ts::DeBoorNet::u() const
{
    return deBoorNet.u;
}

size_t ts::DeBoorNet::k() const
{
    return deBoorNet.k;
}

size_t ts::DeBoorNet::s() const
{
    return deBoorNet.s;
}

size_t ts::DeBoorNet::h() const
{
    return deBoorNet.h;
}

size_t ts::DeBoorNet::dim() const
{
    return deBoorNet.dim;
}

size_t ts::DeBoorNet::nPoints() const
{
    return deBoorNet.n_points;
}

std::vector<float> ts::DeBoorNet::points() const
{
    const float* begin = deBoorNet.points;
    const float* end = begin + deBoorNet.n_points*deBoorNet.dim;
    return std::vector<float>(begin, end);
}

std::vector<float> ts::DeBoorNet::result() const
{
    const float* begin = deBoorNet.result;
    const float* end = begin + deBoorNet.dim;
    return std::vector<float>(begin, end);
}

tsDeBoorNet* ts::DeBoorNet::data()
{
    return &deBoorNet;
}




ts::BSpline::BSpline()
{
    ts_bspline_default(&bspline);
}

ts::BSpline::BSpline(const ts::BSpline& other)
{
    const tsError err = ts_bspline_copy(&other.bspline, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

ts::BSpline::BSpline(const size_t deg, const size_t dim, const size_t nCtrlp,
                     const tsBSplineType type)
{
    const tsError err = ts_bspline_new(deg, dim, nCtrlp, type, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

ts::BSpline::BSpline(const std::vector<float> points, const size_t dim)
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

ts::BSpline::~BSpline()
{
    ts_bspline_free(&bspline);
}

ts::BSpline& ts::BSpline::operator=(const ts::BSpline& other)
{
    const tsError err = ts_bspline_copy(&other.bspline, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return *this;
}

ts::DeBoorNet ts::BSpline::operator()(const float u) const
{
    return evaluate(u);
}

size_t ts::BSpline::deg() const
{
    return bspline.deg;
}

size_t ts::BSpline::order() const
{
    return bspline.order;
}

size_t ts::BSpline::dim() const
{
    return bspline.dim;
}

size_t ts::BSpline::nCtrlp() const
{
    return bspline.n_ctrlp;
}

size_t ts::BSpline::nKnots() const
{
    return bspline.n_knots;
}

std::vector<float> ts::BSpline::ctrlp() const
{
    const float* begin  = bspline.ctrlp;
    const float* end = begin + bspline.n_ctrlp*bspline.dim;
    return std::vector<float>(begin, end);
}

std::vector<float> ts::BSpline::knots() const
{
    const float* begin = bspline.knots;
    const float* end = begin + bspline.n_knots;
    return std::vector<float>(begin, end);
}

tsBSpline* ts::BSpline::data()
{
    return &bspline;
}

void ts::BSpline::setCtrlp(const std::vector<float> ctrlp)
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

void ts::BSpline::setKnots(const std::vector<float> knots)
{
    if (knots.size() != nKnots()) {
        throw std::runtime_error("The number of values must be equals to the"
        " spline's number of knots.");
    }
    const tsError err = ts_bspline_set_knots(&bspline, knots.data(), &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

void ts::BSpline::setupKnots(const tsBSplineType type,
                             const float min, const float max)
{
    const tsError err = ts_bspline_setup_knots(
            &bspline, type, min, max, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

size_t ts::BSpline::insertKnot(const float u, const size_t n)
{
    size_t k;
    const tsError err = ts_bspline_insert_knot(&bspline, u, n, &bspline, &k);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return k;
}

void ts::BSpline::resize(const int n, const int back)
{
    const tsError err = ts_bspline_resize(&bspline, n, back, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

size_t ts::BSpline::split(const float u)
{
    size_t k;
    const tsError err = ts_bspline_split(&bspline, u, &bspline, &k);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return k;
}

void ts::BSpline::buckle(const float b)
{
    const tsError err = ts_bspline_buckle(&bspline, b, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

void ts::BSpline::toBeziers()
{
    const tsError err = ts_bspline_to_beziers(&bspline, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

ts::DeBoorNet ts::BSpline::evaluate(const float u) const
{
    DeBoorNet deBoorNet;
    const tsError err = ts_bspline_evaluate(&bspline, u, deBoorNet.data());
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return deBoorNet;
}



bool ts::Utils::fequals(const float x, const float y)
{
    return ts_fequals(x, y) == 1;
}

std::string ts::Utils::enum_str(const tsError err)
{
    return std::string(ts_enum_str(err));
}

tsError ts::Utils::str_enum(const std::string str)
{
    return ts_str_enum(str.c_str());
}
