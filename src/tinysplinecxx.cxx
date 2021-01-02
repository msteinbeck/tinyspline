#include "tinysplinecxx.h"
#include <stdlib.h>
#include <stdexcept>
#include <cstdio>
#include <sstream>

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

#ifdef SWIG
#define std_real_vector_init new std::vector<tinyspline::real>
#define std_real_vector_read(var) var->
#else
#define std_real_vector_init std::vector<tinyspline::real>
#define std_real_vector_read(var) var.
#endif

/******************************************************************************
*                                                                             *
* DeBoorNet                                                                   *
*                                                                             *
******************************************************************************/
tinyspline::DeBoorNet::DeBoorNet(tsDeBoorNet &src)
: net(ts_deboornet_init())
{
	ts_deboornet_move(&src, &net);
}

tinyspline::DeBoorNet::DeBoorNet(const tinyspline::DeBoorNet &other)
: net(ts_deboornet_init())
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

std::string tinyspline::DeBoorNet::toString() const
{
	std::ostringstream oss;
	oss << "DeBoorNet{";
	oss << "knot: " << knot();
	oss << ", index: " << index();
	oss << ", multiplicity: " << multiplicity();
	oss << ", insertions: " << numInsertions();
	oss << ", dimension: " << dimension();
	oss << ", points: " << ts_deboornet_num_points(&net);
	oss << "}";
	return oss.str();
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

std::string tinyspline::Domain::toString() const
{
	std::ostringstream oss;
	oss << "Domain{min: " << _min << ", max: " << _max << "}";
	return oss.str();
}




/******************************************************************************
*                                                                             *
* BSpline                                                                     *
*                                                                             *
******************************************************************************/
tinyspline::BSpline::BSpline()
: spline(ts_bspline_init())
{
	tsReal ctrlp[3] = { 0, 0, 0 };
	tsStatus status;
	if (ts_bspline_new(1, 3, 0, TS_CLAMPED, &spline, &status))
		throw std::runtime_error(status.message);
	if (ts_bspline_set_control_points(&spline, ctrlp, &status))
		throw std::runtime_error(status.message);
}

tinyspline::BSpline::BSpline(const tinyspline::BSpline &other)
: spline(ts_bspline_init())
{
	tsStatus status;
	if (ts_bspline_copy(&other.spline, &spline, &status))
		throw std::runtime_error(status.message);
}

tinyspline::BSpline::BSpline(size_t numControlPoints, size_t dimension,
	size_t degree, tinyspline::BSpline::type type)
: spline(ts_bspline_init())
{
	tsStatus status;
	if (ts_bspline_new(numControlPoints, dimension, degree, type, &spline,
			   &status))
		throw std::runtime_error(status.message);
}

tinyspline::BSpline::~BSpline()
{
	ts_bspline_free(&spline);
}

tinyspline::BSpline tinyspline::BSpline::interpolateCubicNatural(
	const std_real_vector_in points, size_t dimension)
{
	if (dimension == 0)
		throw std::runtime_error("unsupported dimension: 0");
	if (std_real_vector_read(points)size() % dimension != 0)
		throw std::runtime_error("#points % dimension != 0");
	tinyspline::BSpline bspline;
	tsStatus status;
	if (ts_bspline_interpolate_cubic_natural(
			std_real_vector_read(points)data(),
			std_real_vector_read(points)size()/dimension,
			dimension, bspline.data(), &status))
		throw std::runtime_error(status.message);
	return bspline;
}

tinyspline::BSpline tinyspline::BSpline::interpolateCatmullRom(
	const std_real_vector_in points, size_t dimension, tsReal alpha,
	std::vector<tinyspline::real> *first,
	std::vector<tinyspline::real> *last, tsReal epsilon)
{
	if (dimension == 0)
		throw std::runtime_error("unsupported dimension: 0");
	if (std_real_vector_read(points)size() % dimension != 0)
		throw std::runtime_error("#points % dimension != 0");
	tsReal *fst = NULL;
	if (first && first->size() >= dimension)
		fst = first->data();
	tsReal *lst = NULL;
	if (last && last->size() >= dimension)
		lst = last->data();
	tinyspline::BSpline bspline;
	tsStatus status;
	if (ts_bspline_interpolate_catmull_rom(
			std_real_vector_read(points)data(),
			std_real_vector_read(points)size()/dimension,
			dimension, alpha, fst, lst, epsilon, bspline.data(),
			&status))
		throw std::runtime_error(status.message);
	return bspline;
}

tinyspline::BSpline tinyspline::BSpline::fromJson(std::string json)
{
	tinyspline::BSpline bspline;
	tsStatus status;
	if (ts_bspline_from_json(json.c_str(), bspline.data(), &status))
		throw std::runtime_error(status.message);
	return bspline;
}

tinyspline::BSpline tinyspline::BSpline::load(std::string path)
{
	tinyspline::BSpline bspline;
	tsStatus status;
	if (ts_bspline_load(path.c_str(), bspline.data(), &status))
		throw std::runtime_error(status.message);
	return bspline;
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

std_real_vector_out tinyspline::BSpline::controlPointAt(size_t index) const
{
	tsReal *ctrlp;
	tsStatus status;
	if (ts_bspline_control_point_at(&spline, index, &ctrlp, &status))
		throw std::runtime_error(status.message);
	tinyspline::real *begin  = ctrlp;
	tinyspline::real *end = begin + dimension();
	std_real_vector_out vec = std_real_vector_init(begin, end);
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

tinyspline::real tinyspline::BSpline::knotAt(size_t index) const
{
	tsReal knot;
	tsStatus status;
	if (ts_bspline_knot_at(&spline, index, &knot, &status))
		throw std::runtime_error(status.message);
	return knot;
}

tsBSpline * tinyspline::BSpline::data()
{
	return &spline;
}

size_t tinyspline::BSpline::numControlPoints() const
{
	return ts_bspline_num_control_points(&spline);
}

tinyspline::DeBoorNet tinyspline::BSpline::eval(tinyspline::real u) const
{
	tsDeBoorNet net = ts_deboornet_init();
	tsStatus status;
	if (ts_bspline_eval(&spline, u, &net, &status))
		throw std::runtime_error(status.message);
	return tinyspline::DeBoorNet(net);
}

std_real_vector_out tinyspline::BSpline::evalAll(
	const std_real_vector_in us) const
{
	tinyspline::real *points;
	tsStatus status;
	if (ts_bspline_eval_all(&spline, std_real_vector_read(us)data(),
			std_real_vector_read(us)size(), &points, &status)) {
		throw std::runtime_error(status.message);
	}
	tinyspline::real *begin = points;
	tinyspline::real *end = begin +
		std_real_vector_read(us)size() * dimension();
	std_real_vector_out vec = std_real_vector_init(begin, end);
	free(points);
	return vec;
}

std_real_vector_out tinyspline::BSpline::sample(size_t num) const
{
	tinyspline::real *points;
	size_t actualNum;
	tsStatus status;
	if (ts_bspline_sample(&spline, num, &points, &actualNum, &status)) {
		throw std::runtime_error(status.message);
	}
	tinyspline::real *begin = points;
	tinyspline::real *end = begin + actualNum * dimension();
	std_real_vector_out vec = std_real_vector_init(begin, end);
	free(points);
	return vec;
}

tinyspline::DeBoorNet tinyspline::BSpline::bisect(tinyspline::real value,
	tinyspline::real epsilon, bool persnickety, size_t index,
	bool ascending, size_t maxIter) const
{
	tsDeBoorNet net = ts_deboornet_init();
	tsStatus status;
	if (ts_bspline_bisect(&spline, value, epsilon, persnickety, index,
			ascending, maxIter, &net, &status))
		throw std::runtime_error(status.message);
	return DeBoorNet(net);
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

std::string tinyspline::BSpline::toJson() const
{
	char *json;
	tsStatus status;
	if (ts_bspline_to_json(&spline, &json, &status))
		throw std::runtime_error(status.message);
	std::string string(json);
	free(json);
	return string;
}

void tinyspline::BSpline::save(std::string path) const
{
	tsStatus status;
	if (ts_bspline_save(&spline, path.c_str(), &status))
		throw std::runtime_error(status.message);
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
	const std_real_vector_in ctrlp)
{
	size_t actual = std_real_vector_read(ctrlp)size();
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
	tsError err = ts_bspline_set_control_point_at(
		&spline, index, std_real_vector_read(ctrlp)data(), &status);
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

void tinyspline::BSpline::setKnotAt(size_t index, tinyspline::real knot)
{
	tsStatus status;
	if (ts_bspline_set_knot_at(&spline, index, knot, &status))
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

tinyspline::BSpline tinyspline::BSpline::tension(
	tinyspline::real tension) const
{
	tinyspline::BSpline result;
	tsStatus status;
	if (ts_bspline_tension(&spline, tension, &result.spline, &status))
		throw std::runtime_error(status.message);
	return result;
}

tinyspline::BSpline tinyspline::BSpline::toBeziers() const
{
	tinyspline::BSpline bs;
	tsStatus status;
	if (ts_bspline_to_beziers(&spline, &bs.spline, &status))
		throw std::runtime_error(status.message);
	return bs;
}

tinyspline::BSpline tinyspline::BSpline::derive(size_t n, real epsilon) const
{
	tinyspline::BSpline bs;
	tsStatus status;
	if (ts_bspline_derive(&spline, n, epsilon, &bs.spline, &status))
		throw std::runtime_error(status.message);
	return bs;
}

std::string tinyspline::BSpline::toString() const
{
	Domain d = domain();
	std::ostringstream oss;
	oss << "BSpline{";
	oss << "dimension: " << dimension();
	oss << ", degree: " << degree();
	oss << ", domain: [" << d.min() << ", " << d.max() << "]";
	oss << ", control points: " << numControlPoints();
	oss << ", knots: " << ts_bspline_num_knots(&spline);
	oss << "}";
	return oss.str();
}



/******************************************************************************
*                                                                             *
* Utils                                                                       *
*                                                                             *
******************************************************************************/
bool tinyspline::Utils::knotsEqual(tinyspline::real x, tinyspline::real y)
{
	return ts_knots_equal(x, y) == 1;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
