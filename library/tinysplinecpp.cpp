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

#ifdef WITH_MOVE_SEMANTICS
ts::DeBoorNet::DeBoorNet(DeBoorNet&& other)
{
    ts_deboornet_default(&deBoorNet);
    swap(other);
}
#endif

ts::DeBoorNet::~DeBoorNet()
{
    ts_deboornet_free(&deBoorNet);
}

ts::DeBoorNet& ts::DeBoorNet::operator=(const ts::DeBoorNet& other)
{
    if (&other != this) {
        const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
        if (err < 0)
            throw std::runtime_error(ts_enum_str(err));
    }
    return *this;
}

#ifdef WITH_MOVE_SEMANTICS
ts::DeBoorNet& ts::DeBoorNet::operator=(ts::DeBoorNet&& other)
{
    if (&other != this) {
        ts_deboornet_free(&deBoorNet);
        swap(other);
    }
    return *this;
}

void ts::DeBoorNet::swap(ts::DeBoorNet& other)
{
    if (&other != this) {
        std::swap(deBoorNet.u, other.deBoorNet.u);
        std::swap(deBoorNet.k, other.deBoorNet.k);
        std::swap(deBoorNet.s, other.deBoorNet.s);
        std::swap(deBoorNet.h, other.deBoorNet.h);
        std::swap(deBoorNet.dim, other.deBoorNet.dim);
        std::swap(deBoorNet.n_points, other.deBoorNet.n_points);
        std::swap(deBoorNet.points, other.deBoorNet.points);
        std::swap(deBoorNet.result, other.deBoorNet.result);
    }
}
#endif

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

#ifdef WITH_MOVE_SEMANTICS
ts::BSpline::BSpline(ts::BSpline&& other)
{
    ts_bspline_default(&bspline);
    swap(other);
}
#endif

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
            &points.front(), points.size()/dim, dim, &bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
}

ts::BSpline::~BSpline()
{
    ts_bspline_free(&bspline);
}

ts::BSpline& ts::BSpline::operator=(const ts::BSpline& other)
{
    if (&other != this) {
        const tsError err = ts_bspline_copy(&other.bspline, &bspline);
        if (err < 0)
            throw std::runtime_error(ts_enum_str(err));
    }
    return *this;
}

#ifdef WITH_MOVE_SEMANTICS
ts::BSpline& ts::BSpline::operator=(ts::BSpline&& other)
{
    if (&other != this) {
        ts_bspline_free(&bspline);
        swap(other);
    }
    return *this;
}
#endif

ts::DeBoorNet ts::BSpline::operator()(const float u) const
{
    return evaluate(u);
}

#ifdef WITH_MOVE_SEMANTICS
void ts::BSpline::swap(ts::BSpline &other)
{
    if (&other != this) {
        std::swap(bspline.deg, other.bspline.deg);
        std::swap(bspline.order, other.bspline.order);
        std::swap(bspline.dim, other.bspline.dim);
        std::swap(bspline.n_ctrlp, other.bspline.n_ctrlp);
        std::swap(bspline.n_knots, other.bspline.n_knots);
        std::swap(bspline.ctrlp, other.bspline.ctrlp);
        std::swap(bspline.knots, other.bspline.knots);
    }
}
#endif

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

ts::DeBoorNet ts::BSpline::evaluate(const float u) const
{
    DeBoorNet deBoorNet;
    const tsError err = ts_bspline_evaluate(&bspline, u, deBoorNet.data());
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return deBoorNet;
}

void ts::BSpline::setCtrlp(const std::vector<float> ctrlp)
{
    if (ctrlp.size() != nCtrlp() * dim()) {
        throw std::runtime_error("The number of values must be equals to the"
            " spline's number of control points multiplied by the dimension"
            " of each control point.");
    }
    const tsError err = ts_bspline_set_ctrlp(&bspline, &ctrlp.front(), &bspline);
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

ts::BSpline ts::BSpline::setupKnots(const tsBSplineType type,
                             const float min, const float max) const
{
    ts::BSpline bs;
    const tsError err = ts_bspline_setup_knots(
            &bspline, type, min, max, &bs.bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return bs;
}

ts::BSpline ts::BSpline::insertKnot(const float u, const size_t n) const
{
    ts::BSpline bs;
    size_t k;
    const tsError err = ts_bspline_insert_knot(&bspline, u, n, &bs.bspline, &k);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return bs;
}

ts::BSpline ts::BSpline::resize(const int n, const int back) const
{
    ts::BSpline bs;
    const tsError err = ts_bspline_resize(&bspline, n, back, &bs.bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return bs;
}

ts::BSpline ts::BSpline::split(const float u) const
{
    ts::BSpline bs;
    size_t k;
    const tsError err = ts_bspline_split(&bspline, u, &bs.bspline, &k);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return bs;
}

ts::BSpline ts::BSpline::buckle(const float b) const
{
    ts::BSpline bs;
    const tsError err = ts_bspline_buckle(&bspline, b, &bs.bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return bs;
}

ts::BSpline ts::BSpline::toBeziers() const
{
    ts::BSpline bs;
    const tsError err = ts_bspline_to_beziers(&bspline, &bs.bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return bs;
}

ts::BSpline ts::BSpline::derive() const
{
    ts::BSpline bs;
    const tsError err = ts_bspline_derive(&bspline, &bs.bspline);
    if (err < 0)
        throw std::runtime_error(ts_enum_str(err));
    return bs;
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
