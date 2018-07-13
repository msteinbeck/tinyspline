#include "tinysplinecpp.h"
#include <stdexcept>

/********************************************************
*                                                       *
* DeBoorNet                                             *
*                                                       *
********************************************************/
tinyspline::DeBoorNet::DeBoorNet()
{
	ts_deboornet_default(&deBoorNet);
}

tinyspline::DeBoorNet::DeBoorNet(const tinyspline::DeBoorNet &other)
{
	const tsError err = ts_deboornet_copy(&other.deBoorNet, &deBoorNet);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

tinyspline::DeBoorNet::~DeBoorNet()
{
	ts_deboornet_free(&deBoorNet);
}

tinyspline::DeBoorNet & tinyspline::DeBoorNet::operator=(
	const tinyspline::DeBoorNet &other)
{
	if (&other != this) {
		const tsError err = ts_deboornet_copy(
			&other.deBoorNet, &deBoorNet);
		if (err < 0)
			throw std::runtime_error(ts_enum_str(err));
	}
	return *this;
}

tinyspline::real tinyspline::DeBoorNet::u() const
{
	return ts_deboornet_knot(&deBoorNet);
}

size_t tinyspline::DeBoorNet::k() const
{
	return ts_deboornet_index(&deBoorNet);
}

size_t tinyspline::DeBoorNet::s() const
{
	return ts_deboornet_multiplicity(&deBoorNet);
}

size_t tinyspline::DeBoorNet::h() const
{
	return ts_deboornet_num_insertions(&deBoorNet);
}

size_t tinyspline::DeBoorNet::dim() const
{
	return ts_deboornet_dimension(&deBoorNet);
}

size_t tinyspline::DeBoorNet::nPoints() const
{
	return ts_deboornet_num_points(&deBoorNet);
}

std::vector<tinyspline::real> tinyspline::DeBoorNet::points() const
{
	tsReal *points = ts_deboornet_points(&deBoorNet);
	if (!points)
		throw std::runtime_error(ts_enum_str(TS_MALLOC));
	const tinyspline::real *begin = points;
	const tinyspline::real *end = begin + nPoints() * dim();
	const std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(points);
	return vec;
}

std::vector<tinyspline::real> tinyspline::DeBoorNet::result() const
{
	tsReal *result = ts_deboornet_result(&deBoorNet);
	if (!result)
		throw std::runtime_error(ts_enum_str(TS_MALLOC));
	const tinyspline::real *begin = result;
	const tinyspline::real *end = begin + dim();
	const std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(result);
	return vec;
}

tsDeBoorNet * tinyspline::DeBoorNet::data()
{
	return &deBoorNet;
}

#ifndef TINYSPLINE_DISABLE_CXX11_FEATURES
tinyspline::DeBoorNet::DeBoorNet(tinyspline::DeBoorNet &&other) noexcept
{
	ts_deboornet_default(&deBoorNet);
	swap(other);
}

tinyspline::DeBoorNet & tinyspline::DeBoorNet::operator=(
	tinyspline::DeBoorNet &&other) noexcept
{
	if (&other != this) {
		ts_deboornet_free(&deBoorNet);
		swap(other);
	}
	return *this;
}

void tinyspline::DeBoorNet::swap(tinyspline::DeBoorNet &other)
{
	if (&other != this) {
		std::swap(deBoorNet.pImpl, other.deBoorNet.pImpl);
	}
}
#endif


/********************************************************
*                                                       *
* BSpline                                               *
*                                                       *
********************************************************/
tinyspline::BSpline::BSpline()
{
	ts_bspline_default(&bspline);
}

tinyspline::BSpline::BSpline(const tinyspline::BSpline &other)
{
	const tsError err = ts_bspline_copy(&other.bspline, &bspline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

tinyspline::BSpline::BSpline(const size_t nCtrlp, const size_t dim,
	const size_t deg, const tinyspline::BSpline::type type)
{
	const tsError err = ts_bspline_new(nCtrlp, dim, deg, type, &bspline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

tinyspline::BSpline::~BSpline()
{
	ts_bspline_free(&bspline);
}

tinyspline::BSpline & tinyspline::BSpline::operator=(
	const tinyspline::BSpline &other)
{
	if (&other != this) {
		const tsError err = ts_bspline_copy(&other.bspline, &bspline);
		if (err < 0)
			throw std::runtime_error(ts_enum_str(err));
	}
	return *this;
}

tinyspline::DeBoorNet tinyspline::BSpline::operator()(
	const tinyspline::real u) const
{
	return evaluate(u);
}

size_t tinyspline::BSpline::deg() const
{
	return ts_bspline_degree(&bspline);
}

size_t tinyspline::BSpline::order() const
{
	return ts_bspline_order(&bspline);
}

size_t tinyspline::BSpline::dim() const
{
	return ts_bspline_dimension(&bspline);
}

size_t tinyspline::BSpline::nCtrlp() const
{
	return ts_bspline_num_control_points(&bspline);
}

size_t tinyspline::BSpline::nKnots() const
{
	return ts_bspline_num_knots(&bspline);
}

std::vector<tinyspline::real> tinyspline::BSpline::ctrlp() const
{
	tsReal *ctrlp = ts_bspline_control_points(&bspline);
	if (!ctrlp)
		throw std::runtime_error(ts_enum_str(TS_MALLOC));
	const tinyspline::real *begin  = ctrlp;
	const tinyspline::real *end = begin + nCtrlp() * dim();
	const std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(ctrlp);
	return vec;
}

std::vector<tinyspline::real> tinyspline::BSpline::knots() const
{
	tsReal *knots = ts_bspline_knots(&bspline);
	if (!knots)
		throw std::runtime_error(ts_enum_str(TS_MALLOC));
	const tinyspline::real *begin = knots;
	const tinyspline::real *end = begin + nKnots();
	const std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(knots);
	return vec;
}

tsBSpline * tinyspline::BSpline::data()
{
	return &bspline;
}

tinyspline::DeBoorNet tinyspline::BSpline::evaluate(
	const tinyspline::real u) const
{
	tinyspline::DeBoorNet deBoorNet;
	const tsError err = ts_bspline_eval(&bspline, u, deBoorNet.data());
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return deBoorNet;
}

void tinyspline::BSpline::setCtrlp(
	const std::vector<tinyspline::real> &ctrlp)
{
	if (ctrlp.size() != nCtrlp() * dim()) {
		throw std::runtime_error("The number of values must be equals"
			"to the spline's number of control points multiplied"
			"by the dimension of each control point.");
	}
	const tsError err = ts_bspline_set_control_points(
		&bspline, ctrlp.data());
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

void tinyspline::BSpline::setKnots(
	const std::vector<tinyspline::real> &knots)
{
	if (knots.size() != nKnots()) {
		throw std::runtime_error("The number of values must be equals"
			"to the spline's number of knots.");
	}
	const tsError err = ts_bspline_set_knots(
		&bspline, knots.data());
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

tinyspline::BSpline tinyspline::BSpline::fillKnots(
	const tsBSplineType type, const tinyspline::real min,
	const tinyspline::real max) const
{
	tinyspline::BSpline bs;
	const tsError err = ts_bspline_fill_knots(
		&bspline, type, min, max, &bs.bspline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::insertKnot(
	const tinyspline::real u, const size_t n) const
{
	tinyspline::BSpline bs;
	size_t k;
	const tsError err = ts_bspline_insert_knot(
		&bspline, u, n, &bs.bspline, &k);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::resize(
	const int n, const int back) const
{
	tinyspline::BSpline bs;
	const tsError err = ts_bspline_resize(&bspline, n, back, &bs.bspline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::split(
	const tinyspline::real u) const
{
	tinyspline::BSpline bs;
	size_t k;
	const tsError err = ts_bspline_split(&bspline, u, &bs.bspline, &k);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::buckle(
	const tinyspline::real b) const
{
	tinyspline::BSpline bs;
	const tsError err = ts_bspline_buckle(&bspline, b, &bs.bspline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::toBeziers() const
{
	tinyspline::BSpline bs;
	const tsError err = ts_bspline_to_beziers(&bspline, &bs.bspline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::derive() const
{
	tinyspline::BSpline bs;
	const tsError err = ts_bspline_derive(&bspline, &bs.bspline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

#ifndef TINYSPLINE_DISABLE_CXX11_FEATURES
tinyspline::BSpline::BSpline(tinyspline::BSpline &&other) noexcept
{
	ts_bspline_default(&bspline);
	swap(other);
}

tinyspline::BSpline & tinyspline::BSpline::operator=(
	tinyspline::BSpline &&other) noexcept
{
	if (&other != this) {
		ts_bspline_free(&bspline);
		swap(other);
	}
	return *this;
}

void tinyspline::BSpline::swap(tinyspline::BSpline &other)
{
	if (&other != this) {
		std::swap(bspline.pImpl, other.bspline.pImpl);
	}
}
#endif


/********************************************************
*                                                       *
* Utils                                                 *
*                                                       *
********************************************************/
tinyspline::BSpline tinyspline::Utils::interpolateCubic(
	const std::vector<tinyspline::real> *points, const size_t dim)
{
	if (dim == 0)
		throw std::runtime_error(ts_enum_str(TS_DIM_ZERO));
	if (points->size() % dim != 0)
		throw std::runtime_error("#points % dim == 0 failed");
	tinyspline::BSpline bspline;
	const tsError err = ts_bspline_interpolate_cubic(
		points->data(), points->size()/dim, dim, bspline.data());
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bspline;
}

bool tinyspline::Utils::fequals(
	const tinyspline::real x, const tinyspline::real y)
{
	return ts_fequals(x, y) == 1;
}

std::string tinyspline::Utils::enum_str(const tsError err)
{
	return std::string(ts_enum_str(err));
}

tsError tinyspline::Utils::str_enum(const std::string str)
{
	return ts_str_enum(str.c_str());
}
