#include "tinysplinecpp.h"
#include <stdlib.h>
#include <stdexcept>
#include <cstdio>

/* Suppress some useless MSVC warnings. */
#ifdef _MSC_VER
#pragma warning(push)
/* address of dllimport */
#pragma warning(disable:4232)
/* binding rvalues to non-const references */
#pragma warning(disable:4350)
/* unreferenced inline function */
#pragma warning(disable:4514)
/* function not inlined */
#pragma warning(disable:4710)
/* byte padding */
#pragma warning(disable:4820)
/* meaningless deprecation */
#pragma warning(disable:4996)
/* Spectre mitigation */
#pragma warning(disable:5045)
#endif

/******************************************************************************
*                                                                             *
* DeBoorNet                                                                   *
*                                                                             *
******************************************************************************/
tinyspline::DeBoorNet::DeBoorNet()
{
	net = ts_deboornet_init();
}

tinyspline::DeBoorNet::DeBoorNet(const tinyspline::DeBoorNet &other)
{
	tsStatus status;
	if (ts_deboornet_copy(&other.net, &net, &status))
		throw std::runtime_error(status.message);
}

tinyspline::DeBoorNet::~DeBoorNet()
{
	ts_deboornet_free(&net);
}

tinyspline::DeBoorNet & tinyspline::DeBoorNet::operator=(
	const tinyspline::DeBoorNet &other)
{
	if (&other != this) {
		tsStatus status;
		if (ts_deboornet_copy(&other.net, &net, &status))
			throw std::runtime_error(status.message);
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
	tsReal *points;
	tsStatus status;
	if (ts_deboornet_points(&net, &points, &status))
		throw std::runtime_error(status.message);
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
	tsReal *result;
	tsStatus status;
	if (ts_deboornet_result(&net, &result, &status))
		throw std::runtime_error(status.message);
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



/******************************************************************************
*                                                                             *
* Domain                                                                      *
*                                                                             *
******************************************************************************/
tinyspline::Domain::Domain(tinyspline::real min, tinyspline::real max)
{
	_min = min;
	_max = max;
}

tinyspline::Domain::Domain(const tinyspline::Domain &other)
{
	_min = other._min;
	_max = other._max;
}

tinyspline::Domain & tinyspline::Domain::operator=(
	const tinyspline::Domain &other)
{
	if (&other != this) {
		_min = other.min();
		_max = other.max();
	}
	return *this;
}

tinyspline::real tinyspline::Domain::min() const
{
	return _min;
}

tinyspline::real tinyspline::Domain::max() const
{
	return _max;
}




/******************************************************************************
*                                                                             *
* BSpline                                                                     *
*                                                                             *
******************************************************************************/
tinyspline::BSpline::BSpline()
{
	tsReal ctrlp[3] = { 0, 0, 0 };
	tsStatus status;
	if (ts_bspline_new(1, 3, 0, TS_CLAMPED, &spline, &status))
		throw std::runtime_error(status.message);
	if (ts_bspline_set_control_points(&spline, ctrlp, &status))
		throw std::runtime_error(status.message);
}

tinyspline::BSpline::BSpline(const tinyspline::BSpline &other)
{
	tsStatus status;
	if (ts_bspline_copy(&other.spline, &spline, &status))
		throw std::runtime_error(status.message);
}

tinyspline::BSpline::BSpline(size_t nCtrlp, size_t dim, size_t deg,
	tinyspline::BSpline::type type)
{
	tsStatus status;
	if (ts_bspline_new(nCtrlp, dim, deg, type, &spline, &status))
		throw std::runtime_error(status.message);
}

tinyspline::BSpline::~BSpline()
{
	ts_bspline_free(&spline);
}

tinyspline::BSpline & tinyspline::BSpline::operator=(
	const tinyspline::BSpline &other)
{
	if (&other != this) {
		tsStatus status;
		if (ts_bspline_copy(&other.spline, &spline, &status))
			throw std::runtime_error(status.message);
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
	tsReal *ctrlp;
	tsStatus status;
	if (ts_bspline_control_points(&spline, &ctrlp, &status))
		throw std::runtime_error(status.message);
	size_t num_ctrlp = ts_bspline_num_control_points(&spline);
	tinyspline::real *begin  = ctrlp;
	tinyspline::real *end = begin + num_ctrlp * dimension();
	std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
	free(ctrlp);
	return vec;
}

#ifdef SWIG
std::vector<tinyspline::real> *
#else
std::vector<tinyspline::real>
#endif
tinyspline::BSpline::controlPointAt(size_t index) const
{
	tsReal *ctrlp;
	tsStatus status;
	if (ts_bspline_control_point_at(&spline, index, &ctrlp, &status))
		throw std::runtime_error(status.message);
	tinyspline::real *begin  = ctrlp;
	tinyspline::real *end = begin + dimension();
#ifdef SWIG
	std::vector<tinyspline::real> * vec =
		new std::vector<tinyspline::real>(begin, end);
#else
	std::vector<tinyspline::real> vec =
		std::vector<tinyspline::real>(begin, end);
#endif
	free(ctrlp);
	return vec;
}

std::vector<tinyspline::real> tinyspline::BSpline::knots() const
{
	tsReal *knots;
	tsStatus status;
	if (ts_bspline_knots(&spline, &knots, &status))
		throw std::runtime_error(status.message);
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
	tsStatus status;
	if (ts_bspline_eval(&spline, u, deBoorNet.data(), &status))
		throw std::runtime_error(status.message);
	return deBoorNet;
}

tinyspline::Domain tinyspline::BSpline::domain() const
{
	real min, max;
	ts_bspline_domain(&spline, &min, &max);
	return Domain(min, max);
}

bool tinyspline::BSpline::isClosed(tinyspline::real epsilon) const
{
	int closed = 0;
	tsStatus status;
	if (ts_bspline_is_closed(&spline, epsilon, &closed, &status))
		throw std::runtime_error(status.message);
	return closed == 1;
}

std::string tinyspline::BSpline::toJSON()
{
	char *json;
	tsStatus status;
	if (ts_bspline_to_json(&spline, &json, &status))
		throw std::runtime_error(status.message);
	std::string string(json);
	free(json);
	return string;
}

void tinyspline::BSpline::fromJSON(std::string json)
{
	tsBSpline s;
	tsStatus status;
	if (ts_bspline_from_json(json.c_str(), &s, &status))
		throw std::runtime_error(status.message);
	ts_bspline_free(&spline);
	ts_bspline_move(&s, &spline);
}

void tinyspline::BSpline::save(std::string path)
{
	tsStatus status;
	if (ts_bspline_save_json(&spline, path.c_str(), &status))
		throw std::runtime_error(status.message);
}

void tinyspline::BSpline::load(std::string path)
{
	tsBSpline s;
	tsStatus status;
	if (ts_bspline_load_json(path.c_str(), &s, &status))
		throw std::runtime_error(status.message);
	ts_bspline_free(&spline);
	ts_bspline_move(&s, &spline);
}

void tinyspline::BSpline::setControlPoints(
	const std::vector<tinyspline::real> &ctrlp)
{
	size_t expected = ts_bspline_len_control_points(&spline);
	size_t actual = ctrlp.size();
	if (expected != actual) {
		char expected_str[32];
		char actual_str[32];
		sprintf(expected_str, "%lu", (unsigned long) expected);
		sprintf(actual_str, "%lu", (unsigned long) actual);
		throw std::runtime_error(
			"Expected size: " + std::string(expected_str) +
			", Actual size: " + std::string(actual_str));
	}
	tsStatus status;
	if (ts_bspline_set_control_points(&spline, ctrlp.data(), &status))
		throw std::runtime_error(status.message);
}

void tinyspline::BSpline::setControlPointAt(size_t index,
#ifdef SWIG
	const std::vector<tinyspline::real> *ctrlp)
{
	size_t actual = ctrlp->size();
#else
	const std::vector<tinyspline::real> &ctrlp)
{
	size_t actual = ctrlp.size();
#endif
	size_t expected = dimension();
	if (expected != actual) {
		char expected_str[32];
		char actual_str[32];
		sprintf(expected_str, "%lu", (unsigned long) expected);
		sprintf(actual_str, "%lu", (unsigned long) actual);
		throw std::runtime_error(
			"Expected size: " + std::string(expected_str) +
			", Actual size: " + std::string(actual_str));
	}
	tsStatus status;
#ifdef SWIG
	tsError err = ts_bspline_set_control_point_at(
		&spline, index, ctrlp->data(), &status);
#else
	tsError err = ts_bspline_set_control_point_at(
		&spline, index, ctrlp.data(), &status);
#endif
	if (err < 0)
		throw std::runtime_error(status.message);
}

void tinyspline::BSpline::setKnots(const std::vector<tinyspline::real> &knots)
{
	size_t expected = ts_bspline_num_knots(&spline);
	size_t actual = knots.size();
	if (expected != actual) {
		char expected_str[32];
		char actual_str[32];
		sprintf(expected_str, "%lu", (unsigned long) expected);
		sprintf(actual_str, "%lu", (unsigned long) actual);
		throw std::runtime_error(
			"Expected size: " + std::string(expected_str) +
			", Actual size: " + std::string(actual_str));
	}
	tsStatus status;
	if (ts_bspline_set_knots(&spline, knots.data(), &status))
		throw std::runtime_error(status.message);
}

tinyspline::BSpline tinyspline::BSpline::insertKnot(tinyspline::real u,
	size_t n) const
{
	tinyspline::BSpline bs;
	size_t k;
	tsStatus status;
	if (ts_bspline_insert_knot(&spline, u, n, &bs.spline, &k, &status))
		throw std::runtime_error(status.message);
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::split(tinyspline::real u) const
{
	tinyspline::BSpline bs;
	size_t k;
	tsStatus status;
	if (ts_bspline_split(&spline, u, &bs.spline, &k, &status))
		throw std::runtime_error(status.message);
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::buckle(tinyspline::real b) const
{
	tinyspline::BSpline bs;
	tsStatus status;
	if (ts_bspline_buckle(&spline, b, &bs.spline, &status))
		throw std::runtime_error(status.message);
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::toBeziers() const
{
	tinyspline::BSpline bs;
	tsStatus status;
	if (ts_bspline_to_beziers(&spline, &bs.spline, &status))
		throw std::runtime_error(status.message);
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::derive(size_t n) const
{
	tinyspline::BSpline bs;
	tsStatus status;
	if (ts_bspline_derive(&spline, n, &bs.spline, &status))
		throw std::runtime_error(status.message);
	return bs;
}



/******************************************************************************
*                                                                             *
* Utils                                                                       *
*                                                                             *
******************************************************************************/
tinyspline::BSpline tinyspline::Utils::interpolateCubic(
	const std::vector<tinyspline::real> *points, size_t dim)
{
	if (dim == 0)
		throw std::runtime_error("unsupported dimension: 0");
	if (points->size() % dim != 0)
		throw std::runtime_error("#points % dim == 0 failed");
	tinyspline::BSpline bspline;
	tsStatus status;
	if (ts_bspline_interpolate_cubic(points->data(), points->size()/dim,
			dim, bspline.data(), &status))
		throw std::runtime_error(status.message);
	return bspline;
}

bool tinyspline::Utils::knotsEqual(tinyspline::real x, tinyspline::real y)
{
	return ts_knots_equal(x, y) == 1;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
