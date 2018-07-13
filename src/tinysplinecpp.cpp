#include "tinysplinecpp.h"
#include <stdexcept>

/******************************************************************************
*                                                                             *
* DeBoorNet                                                                   *
*                                                                             *
******************************************************************************/
tinyspline::DeBoorNet::DeBoorNet()
{
	ts_deboornet_default(&net);
}

tinyspline::DeBoorNet::DeBoorNet(const tinyspline::DeBoorNet &other)
{
	tsError err = ts_deboornet_copy(&other.net, &net);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

tinyspline::DeBoorNet::~DeBoorNet()
{
	ts_deboornet_free(&net);
}

tinyspline::DeBoorNet & tinyspline::DeBoorNet::operator=(
	const tinyspline::DeBoorNet &other)
{
	if (&other != this) {
		tsError err = ts_deboornet_copy(&other.net, &net);
		if (err < 0)
			throw std::runtime_error(ts_enum_str(err));
	}
	return *this;
}

tinyspline::real tinyspline::DeBoorNet::knot() const
{
	return ts_deboornet_knot(&net);
}

size_t tinyspline::DeBoorNet::index() const
{
	return ts_deboornet_index(&net);
}

size_t tinyspline::DeBoorNet::multiplicity() const
{
	return ts_deboornet_multiplicity(&net);
}

size_t tinyspline::DeBoorNet::numInsertions() const
{
	return ts_deboornet_num_insertions(&net);
}

size_t tinyspline::DeBoorNet::dimension() const
{
	return ts_deboornet_dimension(&net);
}

std::vector<tinyspline::real> tinyspline::DeBoorNet::points() const
{
	tsReal *points = ts_deboornet_points(&net);
	if (!points)
		throw std::runtime_error(ts_enum_str(TS_MALLOC));
	size_t num_points = ts_deboornet_num_points(&net);
	tinyspline::real *begin = points;
	tinyspline::real *end = begin + num_points * dimension();
	std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(points);
	return vec;
}

std::vector<tinyspline::real> tinyspline::DeBoorNet::result() const
{
	tsReal *result = ts_deboornet_result(&net);
	if (!result)
		throw std::runtime_error(ts_enum_str(TS_MALLOC));
	size_t num_result = ts_deboornet_num_result(&net);
	tinyspline::real *begin = result;
	tinyspline::real *end = begin + num_result * dimension();
	std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(result);
	return vec;
}

tsDeBoorNet * tinyspline::DeBoorNet::data()
{
	return &net;
}

#ifndef TINYSPLINE_DISABLE_CXX11_FEATURES
tinyspline::DeBoorNet::DeBoorNet(tinyspline::DeBoorNet &&other) noexcept
{
	ts_deboornet_default(&net);
	swap(other);
}

tinyspline::DeBoorNet & tinyspline::DeBoorNet::operator=(
	tinyspline::DeBoorNet &&other) noexcept
{
	if (&other != this) {
		ts_deboornet_free(&net);
		swap(other);
	}
	return *this;
}

void tinyspline::DeBoorNet::swap(tinyspline::DeBoorNet &other)
{
	if (&other != this) {
		std::swap(net.pImpl, other.net.pImpl);
	}
}
#endif


/******************************************************************************
*                                                                             *
* BSpline                                                                     *
*                                                                             *
******************************************************************************/
tinyspline::BSpline::BSpline()
{
	ts_bspline_default(&spline);
}

tinyspline::BSpline::BSpline(const tinyspline::BSpline &other)
{
	tsError err = ts_bspline_copy(&other.spline, &spline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

tinyspline::BSpline::BSpline(size_t nCtrlp, size_t dim, size_t deg,
	tinyspline::BSpline::type type)
{
	tsError err = ts_bspline_new(nCtrlp, dim, deg, type, &spline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

tinyspline::BSpline::~BSpline()
{
	ts_bspline_free(&spline);
}

tinyspline::BSpline & tinyspline::BSpline::operator=(
	const tinyspline::BSpline &other)
{
	if (&other != this) {
		tsError err = ts_bspline_copy(&other.spline, &spline);
		if (err < 0)
			throw std::runtime_error(ts_enum_str(err));
	}
	return *this;
}

tinyspline::DeBoorNet tinyspline::BSpline::operator()(tinyspline::real u) const
{
	return eval(u);
}

size_t tinyspline::BSpline::degree() const
{
	return ts_bspline_degree(&spline);
}

size_t tinyspline::BSpline::order() const
{
	return ts_bspline_order(&spline);
}

size_t tinyspline::BSpline::dimension() const
{
	return ts_bspline_dimension(&spline);
}

std::vector<tinyspline::real> tinyspline::BSpline::controlPoints() const
{
	tsReal *ctrlp = ts_bspline_control_points(&spline);
	if (!ctrlp)
		throw std::runtime_error(ts_enum_str(TS_MALLOC));
	size_t num_ctrlp = ts_bspline_num_control_points(&spline);
	tinyspline::real *begin  = ctrlp;
	tinyspline::real *end = begin + num_ctrlp * dimension();
	std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(ctrlp);
	return vec;
}

std::vector<tinyspline::real> tinyspline::BSpline::knots() const
{
	tsReal *knots = ts_bspline_knots(&spline);
	if (!knots)
		throw std::runtime_error(ts_enum_str(TS_MALLOC));
	size_t num_knots = ts_bspline_num_knots(&spline);
	tinyspline::real *begin = knots;
	tinyspline::real *end = begin + num_knots;
	std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(knots);
	return vec;
}

tsBSpline * tinyspline::BSpline::data()
{
	return &spline;
}

tinyspline::DeBoorNet tinyspline::BSpline::eval(tinyspline::real u) const
{
	tinyspline::DeBoorNet deBoorNet;
	tsError err = ts_bspline_eval(&spline, u, deBoorNet.data());
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return deBoorNet;
}

void tinyspline::BSpline::setControlPoints(
	const std::vector<tinyspline::real> &ctrlp)
{
	size_t expected = ts_bspline_len_control_points(&spline);
	if (ctrlp.size() != expected) {
		throw std::runtime_error(
			"Expected size: " + std::to_string(expected) +
			", Actual size: " + std::to_string(ctrlp.size()));
	}
	tsError err = ts_bspline_set_control_points(&spline, ctrlp.data());
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

void tinyspline::BSpline::setKnots(const std::vector<tinyspline::real> &knots)
{
	size_t expected = ts_bspline_num_knots(&spline);
	if (knots.size() != expected) {
		throw std::runtime_error(
			"Expected size: " + std::to_string(expected) +
			", Actual size: " + std::to_string(knots.size()));
	}
	tsError err = ts_bspline_set_knots(&spline, knots.data());
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
}

tinyspline::BSpline tinyspline::BSpline::fillKnots(tsBSplineType type,
	tinyspline::real min, tinyspline::real max) const
{
	tinyspline::BSpline bs;
	tsError err = ts_bspline_fill_knots(
		&spline, type, min, max, &bs.spline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::insertKnot(tinyspline::real u,
	size_t n) const
{
	tinyspline::BSpline bs;
	size_t k;
	tsError err = ts_bspline_insert_knot(&spline, u, n, &bs.spline, &k);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::resize(int n, int back) const
{
	tinyspline::BSpline bs;
	tsError err = ts_bspline_resize(&spline, n, back, &bs.spline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::split(tinyspline::real u) const
{
	tinyspline::BSpline bs;
	size_t k;
	tsError err = ts_bspline_split(&spline, u, &bs.spline, &k);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::buckle(tinyspline::real b) const
{
	tinyspline::BSpline bs;
	tsError err = ts_bspline_buckle(&spline, b, &bs.spline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::toBeziers() const
{
	tinyspline::BSpline bs;
	tsError err = ts_bspline_to_beziers(&spline, &bs.spline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::derive() const
{
	tinyspline::BSpline bs;
	tsError err = ts_bspline_derive(&spline, &bs.spline);
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bs;
}

#ifndef TINYSPLINE_DISABLE_CXX11_FEATURES
tinyspline::BSpline::BSpline(tinyspline::BSpline &&other) noexcept
{
	ts_bspline_default(&spline);
	swap(other);
}

tinyspline::BSpline & tinyspline::BSpline::operator=(
	tinyspline::BSpline &&other) noexcept
{
	if (&other != this) {
		ts_bspline_free(&spline);
		swap(other);
	}
	return *this;
}

void tinyspline::BSpline::swap(tinyspline::BSpline &other)
{
	if (&other != this) {
		std::swap(spline.pImpl, other.spline.pImpl);
	}
}
#endif


/********************************************************
*                                                       *
* Utils                                                 *
*                                                       *
********************************************************/
tinyspline::BSpline tinyspline::Utils::interpolateCubic(
	const std::vector<tinyspline::real> *points, size_t dim)
{
	if (dim == 0)
		throw std::runtime_error(ts_enum_str(TS_DIM_ZERO));
	if (points->size() % dim != 0)
		throw std::runtime_error("#points % dim == 0 failed");
	tinyspline::BSpline bspline;
	tsError err = ts_bspline_interpolate_cubic(
		points->data(), points->size()/dim, dim, bspline.data());
	if (err < 0)
		throw std::runtime_error(ts_enum_str(err));
	return bspline;
}

bool tinyspline::Utils::fequals(tinyspline::real x, tinyspline::real y)
{
	return ts_fequals(x, y) == 1;
}

std::string tinyspline::Utils::enum_str(tsError err)
{
	return std::string(ts_enum_str(err));
}

tsError tinyspline::Utils::str_enum(std::string str)
{
	return ts_str_enum(str.c_str());
}
