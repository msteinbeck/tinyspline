#define TINYSPLINE_EXPORT
#include "tinysplinecxx.h"

#include <stdlib.h>
#include <cstring>
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

/*! @name DeBoorNet
 *
 * @{
 */
tinyspline::DeBoorNet::DeBoorNet(tsDeBoorNet &data)
: net(ts_deboornet_init())
{
	ts_deboornet_move(&data, &net);
}

tinyspline::DeBoorNet::DeBoorNet(const DeBoorNet &other)
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

tinyspline::DeBoorNet &
tinyspline::DeBoorNet::operator=(const DeBoorNet &other)
{
	if (&other != this) {
		tsDeBoorNet data = ts_deboornet_init();
		tsStatus status;
		if (ts_deboornet_copy(&other.net, &data, &status))
			throw std::runtime_error(status.message);
		ts_deboornet_free(&net);
		ts_deboornet_move(&data, &net);
	}
	return *this;
}

tinyspline::real
tinyspline::DeBoorNet::knot() const
{
	return ts_deboornet_knot(&net);
}

size_t
tinyspline::DeBoorNet::index() const
{
	return ts_deboornet_index(&net);
}

size_t
tinyspline::DeBoorNet::multiplicity() const
{
	return ts_deboornet_multiplicity(&net);
}

size_t
tinyspline::DeBoorNet::numInsertions() const
{
	return ts_deboornet_num_insertions(&net);
}

size_t
tinyspline::DeBoorNet::dimension() const
{
	return ts_deboornet_dimension(&net);
}

std::vector<tinyspline::real>
tinyspline::DeBoorNet::points() const
{
	tsReal *points;
	tsStatus status;
	if (ts_deboornet_points(&net, &points, &status))
		throw std::runtime_error(status.message);
	size_t num_points = ts_deboornet_num_points(&net);
	real *begin = points;
	real *end = begin + num_points * dimension();
	std::vector<real> vec = std::vector<real>(begin, end);
	std::free(points);
	return vec;
}

std::vector<tinyspline::real>
tinyspline::DeBoorNet::result() const
{
	tsReal *result;
	tsStatus status;
	if (ts_deboornet_result(&net, &result, &status))
		throw std::runtime_error(status.message);
	size_t num_result = ts_deboornet_num_result(&net);
	real *begin = result;
	real *end = begin + num_result * dimension();
	std::vector<real> vec = std::vector<real>(begin, end);
	std::free(result);
	return vec;
}

tinyspline::Vec2
tinyspline::DeBoorNet::resultVec2(size_t idx) const
{
	Vec3 vec3 = resultVec3(idx);
	return Vec2(vec3.x(), vec3.y());
}

tinyspline::Vec3
tinyspline::DeBoorNet::resultVec3(size_t idx) const
{
	const size_t num = ts_deboornet_num_result(&net);
	if (idx >= num)
		throw std::out_of_range( "idx >= num(result)");
	std::vector<real> res = result();
	const real *res_ptr = res.data() + idx * dimension();
	real vec_data[3];
	ts_vec3_set(vec_data, res_ptr, dimension());
	return Vec3(vec_data[0], vec_data[1], vec_data[2]);
}

std::string
tinyspline::DeBoorNet::toString() const
{
	std::ostringstream oss;
	oss << "DeBoorNet{"
	    << "knot: " << knot()
	    << ", index: " << index()
	    << ", multiplicity: " << multiplicity()
	    << ", insertions: " << numInsertions()
	    << ", dimension: " << dimension()
	    << ", points: " << ts_deboornet_num_points(&net)
	    << "}";
	return oss.str();
}
/*! @} */



/*! @name Domain
 *
 * @{
 */
tinyspline::Domain::Domain(tinyspline::real min,
                           tinyspline::real max)
: m_min(min), m_max(max)
{}

tinyspline::Domain::Domain(const tinyspline::Domain &other)
: m_min(other.m_min), m_max(other.m_max)
{}

tinyspline::Domain &
tinyspline::Domain::operator=(const tinyspline::Domain &other)
{
	if (&other != this) {
		m_min = other.m_min;
		m_max = other.m_max;
	}
	return *this;
}

tinyspline::real
tinyspline::Domain::min() const
{
	return m_min;
}

tinyspline::real
tinyspline::Domain::max() const
{
	return m_max;
}

std::string
tinyspline::Domain::toString() const
{
	std::ostringstream oss;
        oss << "Domain{"
            << "min: " << min()
            << ", max: " << max()
            << "}";
	return oss.str();
}
/*! @} */



/*! @name Vec2
 *
 * @{
 */
tinyspline::Vec2::Vec2()
{
	const real v = (real) 0.0;
	ts_vec2_init(m_vals, v, v);
}

tinyspline::Vec2::Vec2(real x,
                       real y)
{
	ts_vec2_init(m_vals, x, y);
}

tinyspline::Vec2::Vec2(const Vec2 &other)
{
	std::memcpy(m_vals, other.m_vals, sizeof(m_vals));
}

tinyspline::Vec2 &
tinyspline::Vec2::operator=(const tinyspline::Vec2 &other)
{
	if (&other != this)
		std::memcpy(m_vals, other.m_vals, sizeof(m_vals));
	return *this;
}

tinyspline::Vec2
tinyspline::Vec2::operator+(const tinyspline::Vec2 &other)
{
	return add(other);
}

tinyspline::Vec2
tinyspline::Vec2::operator-(const tinyspline::Vec2 &other)
{
	return subtract(other);
}

tinyspline::Vec2
tinyspline::Vec2::operator*(real scalar)
{
	return multiply(scalar);
}

tinyspline::real
tinyspline::Vec2::x() const
{
	return m_vals[0];
}

void
tinyspline::Vec2::setX(real val)
{
	m_vals[0] = val;
}

tinyspline::real
tinyspline::Vec2::y() const
{
	return m_vals[1];
}

void
tinyspline::Vec2::setY(real val)
{
	m_vals[1] = val;
}

tinyspline::Vec2
tinyspline::Vec2::add(const tinyspline::Vec2 &other) const
{
	Vec2 vec;
	ts_vec_add(m_vals, other.m_vals, 2, vec.m_vals);
	return vec;
}

tinyspline::Vec2
tinyspline::Vec2::subtract(const tinyspline::Vec2 &other) const
{
	Vec2 vec;
	ts_vec_sub(m_vals, other.m_vals, 2, vec.m_vals);
	return vec;
}

tinyspline::Vec2
tinyspline::Vec2::multiply(real scalar) const
{
	Vec2 vec;
	ts_vec_mul(m_vals, 2, scalar, vec.m_vals);
	return vec;
}

tinyspline::Vec2
tinyspline::Vec2::norm() const
{
	Vec2 vec;
	ts_vec_norm(m_vals, 2, vec.m_vals);
	return vec;
}

tinyspline::real
tinyspline::Vec2::magnitude() const
{
	return ts_vec_mag(m_vals, 2);
}

tinyspline::real
tinyspline::Vec2::dot(const tinyspline::Vec2 &other) const
{
	return ts_vec_dot(m_vals, other.m_vals, 2);
}

tinyspline::real
tinyspline::Vec2::distance(const tinyspline::Vec2 &other) const
{
	return ts_distance(m_vals, other.m_vals, 2);
}

std::string
tinyspline::Vec2::toString() const
{
	std::ostringstream oss;
	oss << "Vec2{"
	    << "x: " << x()
	    << ", y: " << y()
	    << "}";
	return oss.str();
}
/*! @} */



/*! @name Vec3
 *
 * @{
 */
tinyspline::Vec3::Vec3()
{
	const real v = (real) 0.0;
	ts_vec3_init(m_vals, v, v, v);
}

tinyspline::Vec3::Vec3(real x,
                       real y,
                       real z)
{
	ts_vec3_init(m_vals, x, y, z);
}

tinyspline::Vec3::Vec3(const Vec3 &other)
{
	std::memcpy(m_vals, other.m_vals, sizeof(m_vals));
}

tinyspline::Vec3 &
tinyspline::Vec3::operator=(const tinyspline::Vec3 &other)
{
	if (&other != this)
		std::memcpy(m_vals, other.m_vals, sizeof(m_vals));
	return *this;
}

tinyspline::Vec3
tinyspline::Vec3::operator+(const tinyspline::Vec3 &other)
{
	return add(other);
}

tinyspline::Vec3
tinyspline::Vec3::operator-(const tinyspline::Vec3 &other)
{
	return subtract(other);
}

tinyspline::Vec3
tinyspline::Vec3::operator*(real scalar)
{
	return multiply(scalar);
}

tinyspline::real
tinyspline::Vec3::x() const
{
	return m_vals[0];
}

void
tinyspline::Vec3::setX(real val)
{
	m_vals[0] = val;
}

tinyspline::real
tinyspline::Vec3::y() const
{
	return m_vals[1];
}

void
tinyspline::Vec3::setY(real val)
{
	m_vals[1] = val;
}

tinyspline::real
tinyspline::Vec3::z() const
{
	return m_vals[2];
}

void
tinyspline::Vec3::setZ(real val)
{
	m_vals[2] = val;
}

tinyspline::Vec3
tinyspline::Vec3::add(const tinyspline::Vec3 &other) const
{
	Vec3 vec;
	ts_vec_add(m_vals, other.m_vals, 3, vec.m_vals);
	return vec;
}

tinyspline::Vec3
tinyspline::Vec3::subtract(const tinyspline::Vec3 &other) const
{
	Vec3 vec;
	ts_vec_sub(m_vals, other.m_vals, 3, vec.m_vals);
	return vec;
}

tinyspline::Vec3
tinyspline::Vec3::multiply(real scalar) const
{
	Vec3 vec;
	ts_vec_mul(m_vals, 3, scalar, vec.m_vals);
	return vec;
}

tinyspline::Vec3
tinyspline::Vec3::cross(const tinyspline::Vec3 &other) const
{
	Vec3 vec;
	ts_vec3_cross(m_vals, other.m_vals, vec.m_vals);
	return vec;
}

tinyspline::Vec3
tinyspline::Vec3::norm() const
{
	Vec3 vec;
	ts_vec_norm(m_vals, 3, vec.m_vals);
	return vec;
}

tinyspline::real
tinyspline::Vec3::magnitude() const
{
	return ts_vec_mag(m_vals, 3);
}

tinyspline::real
tinyspline::Vec3::dot(const tinyspline::Vec3 &other) const
{
	return ts_vec_dot(m_vals, other.m_vals, 3);
}

tinyspline::real
tinyspline::Vec3::distance(const tinyspline::Vec3 &other) const
{
	return ts_distance(m_vals, other.m_vals, 3);
}

std::string
tinyspline::Vec3::toString() const
{
	std::ostringstream oss;
	oss << "Vec3{"
	    << "x: " << x()
	    << ", y: " << y()
	    << ", z: " << z()
	    << "}";
	return oss.str();
}
/*! @} */



/*! @name Frame
 *
 * @{
 */
tinyspline::Frame::Frame(Vec3 &position,
                         Vec3 &tangent,
                         Vec3 &normal,
                         Vec3 &binormal)
: m_position(position),
  m_tangent(tangent),
  m_normal(normal),
  m_binormal(binormal)
{}

tinyspline::Frame::Frame(const Frame &other)
: m_position(other.m_position),
  m_tangent(other.m_tangent),
  m_normal(other.m_normal),
  m_binormal(other.m_binormal)
{}

tinyspline::Frame &
tinyspline::Frame::operator=(const tinyspline::Frame &other)
{
	if (&other != this) {
		m_position = other.m_position;
		m_tangent = other.m_tangent;
		m_normal = other.m_normal;
		m_binormal = other.m_binormal;
	}
	return *this;
}

tinyspline::Vec3
tinyspline::Frame::position() const
{
	return m_position;
}

tinyspline::Vec3
tinyspline::Frame::tangent() const
{
	return m_tangent;
}

tinyspline::Vec3
tinyspline::Frame::normal() const
{
	return m_normal;
}

tinyspline::Vec3
tinyspline::Frame::binormal() const
{
	return m_binormal;
}

std::string
tinyspline::Frame::toString() const
{
	std::ostringstream oss;
	oss << "Frame{"
	    << "position: " << position().toString()
	    << ", tangent: " << tangent().toString()
	    << ", normal: " << normal().toString()
	    << ", binormal: " << binormal().toString()
	    << "}";
	return oss.str();
}
/*! @} */



/*! @name FrameSeq
 *
 * @{
 */
tinyspline::FrameSeq::FrameSeq(tsFrame *frames,
                               size_t len)
: m_frames(frames), m_size(len)
{}

tinyspline::FrameSeq::FrameSeq(const FrameSeq &other)
: m_frames(NULL), m_size(other.m_size)
{
	const size_t sf = m_size * sizeof(tsFrame);
	m_frames = (tsFrame *) std::malloc(sf);
	if (!m_frames) throw std::bad_alloc();
	std::memcpy(m_frames, other.m_frames, sf);
}

tinyspline::FrameSeq::~FrameSeq()
{
	std::free(m_frames);
}

tinyspline::FrameSeq &
tinyspline::FrameSeq::operator=(const tinyspline::FrameSeq &other)
{
	if (&other != this) {
		const size_t sf = other.m_size * sizeof(tsFrame);
		tsFrame *data = (tsFrame *) std::malloc(sf);
		if (!data) throw std::bad_alloc();
		std::memcpy(data, other.m_frames, sf);
		std::free(m_frames);
		m_frames = data;
		m_size = other.m_size;
	}
	return *this;
}

size_t
tinyspline::FrameSeq::size() const
{
	return m_size;
}

tinyspline::Frame
tinyspline::FrameSeq::at(size_t idx) const
{
	if (idx >= m_size)
		throw std::out_of_range( "idx >= size");
	tsFrame frame = m_frames[idx];
	Vec3 position = Vec3(frame.position[0],
	                     frame.position[1],
	                     frame.position[2]);
	Vec3 tangent = Vec3(frame.tangent[0],
	                    frame.tangent[1],
	                    frame.tangent[2]);
	Vec3 normal = Vec3(frame.normal[0],
	                   frame.normal[1],
	                   frame.normal[2]);
	Vec3 binormal = Vec3(frame.binormal[0],
	                     frame.binormal[1],
	                     frame.binormal[2]);
	return Frame(position, tangent, normal, binormal);
}

std::string
tinyspline::FrameSeq::toString() const
{
	std::ostringstream oss;
	oss << "FrameSeq{"
	    << "frames: " << size()
	    << "}";
	return oss.str();
}
/*! @} */



/******************************************************************************
*                                                                             *
* BSpline                                                                     *
*                                                                             *
******************************************************************************/
tinyspline::BSpline::BSpline(tsBSpline &data)
: spline(ts_bspline_init())
{
	ts_bspline_move(&data, &spline);
}

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
	tsBSpline data = ts_bspline_init();
	tsStatus status;
	if (ts_bspline_interpolate_cubic_natural(
			std_real_vector_read(points)data(),
			std_real_vector_read(points)size()/dimension,
			dimension, &data, &status))
		throw std::runtime_error(status.message);
	return BSpline(data);
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
	tsBSpline data = ts_bspline_init();
	tsStatus status;
	if (ts_bspline_interpolate_catmull_rom(
			std_real_vector_read(points)data(),
			std_real_vector_read(points)size()/dimension,
			dimension, alpha, fst, lst, epsilon, &data,
			&status))
		throw std::runtime_error(status.message);
	return BSpline(data);
}

tinyspline::BSpline tinyspline::BSpline::parseJson(std::string json)
{
	tsBSpline data = ts_bspline_init();
	tsStatus status;
	if (ts_bspline_parse_json(json.c_str(), &data, &status))
		throw std::runtime_error(status.message);
	return BSpline(data);
}

tinyspline::BSpline tinyspline::BSpline::load(std::string path)
{
	tsBSpline data = ts_bspline_init();
	tsStatus status;
	if (ts_bspline_load(path.c_str(), &data, &status))
		throw std::runtime_error(status.message);
	return BSpline(data);
}

tinyspline::BSpline & tinyspline::BSpline::operator=(
	const tinyspline::BSpline &other)
{
	if (&other != this) {
		tsBSpline data = ts_bspline_init();
		tsStatus status;
		if (ts_bspline_copy(&other.spline, &data, &status))
			throw std::runtime_error(status.message);
		ts_bspline_free(&spline);
		ts_bspline_move(&data, &spline);
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
	std::free(ctrlp);
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
	std::free(ctrlp);
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
	std::free(knots);
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
	std::free(points);
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
	std::free(points);
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

tinyspline::FrameSeq
tinyspline::BSpline::computeRMF(const std_real_vector_in knots,
                                tinyspline::Vec3 *firstNormal) const
{
	tsStatus status;
	size_t size = std_real_vector_read(knots)size();
	const tsReal *data = std_real_vector_read(knots)data();
	tsFrame *frames = (tsFrame *) std::malloc(size * sizeof(tsFrame));
	if (!frames) throw std::bad_alloc();
	if (firstNormal && size > 0) {
		ts_vec3_init(frames[0].normal,
		             firstNormal->x(),
		             firstNormal->y(),
		             firstNormal->z());
	}
	if (ts_bspline_compute_rmf(&spline, data, size,
	                           firstNormal != NULL,
	                           frames, &status))
		throw std::runtime_error(status.message);
	FrameSeq seq = FrameSeq(frames, size);
	return seq;
}

std_real_vector_out
tinyspline::BSpline::uniformKnotSeq(size_t num) const
{
	tsReal *knots = (tsReal *) std::malloc(num * sizeof(tsReal));
	if (!knots) throw std::bad_alloc();
	ts_bspline_uniform_knot_seq(&spline, num, knots);
	std_real_vector_out vec = std_real_vector_init(knots, knots + num);
	std::free(knots);
	return vec;
}

std::string tinyspline::BSpline::toJson() const
{
	char *json;
	tsStatus status;
	if (ts_bspline_to_json(&spline, &json, &status))
		throw std::runtime_error(status.message);
	std::string string(json);
	std::free(json);
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
	tsBSpline data = ts_bspline_init();
	size_t k;
	tsStatus status;
	if (ts_bspline_insert_knot(&spline, u, n, &data, &k, &status))
		throw std::runtime_error(status.message);
	return BSpline(data);
}

tinyspline::BSpline tinyspline::BSpline::split(tinyspline::real u) const
{
	tsBSpline data = ts_bspline_init();
	size_t k;
	tsStatus status;
	if (ts_bspline_split(&spline, u, &data, &k, &status))
		throw std::runtime_error(status.message);
	return BSpline(data);
}

tinyspline::BSpline tinyspline::BSpline::tension(
	tinyspline::real tension) const
{
	tsBSpline data = ts_bspline_init();
	tsStatus status;
	if (ts_bspline_tension(&spline, tension, &data, &status))
		throw std::runtime_error(status.message);
	return BSpline(data);
}

tinyspline::BSpline tinyspline::BSpline::toBeziers() const
{
	tsBSpline data = ts_bspline_init();
	tsStatus status;
	if (ts_bspline_to_beziers(&spline, &data, &status))
		throw std::runtime_error(status.message);
	return BSpline(data);
}

tinyspline::BSpline tinyspline::BSpline::derive(size_t n, real epsilon) const
{
	tsBSpline data = ts_bspline_init();
	tsStatus status;
	if (ts_bspline_derive(&spline, n, epsilon, &data, &status))
		throw std::runtime_error(status.message);
	return BSpline(data);
}

tinyspline::BSpline tinyspline::BSpline::elevateDegree(
	size_t amount, real epsilon) const
{
	tsBSpline data = ts_bspline_init();
	tsStatus status;
	if (ts_bspline_elevate_degree(&spline, amount, epsilon,
		&data, &status)) {
		throw std::runtime_error(status.message);
	}
	return BSpline(data);
}

tinyspline::BSpline tinyspline::BSpline::alignWith(
	const BSpline &other, BSpline &otherAligned, real epsilon) const
{
	tsBSpline data = ts_bspline_init();
	tsBSpline deleteIf_Other_And_OtherAligned_AreDifferent =
		otherAligned.spline;
	tsStatus status;
	if (ts_bspline_align(&spline, &other.spline, epsilon, &data,
		&otherAligned.spline, &status)) {
		throw std::runtime_error(status.message);
	}
	if (&other != &otherAligned)
		ts_bspline_free(&deleteIf_Other_And_OtherAligned_AreDifferent);
	return BSpline(data);
}

tinyspline::Morphism tinyspline::BSpline::morphTo(
	const BSpline &other, real epsilon) const
{
	return Morphism(*this, other, epsilon);
}

std::string tinyspline::BSpline::toString() const
{
	Domain d = domain();
	std::ostringstream oss;
	oss << "BSpline{"
	    << "dimension: " << dimension()
	    << ", degree: " << degree()
	    << ", domain: [" << d.min() << ", " << d.max() << "]"
	    << ", control points: " << numControlPoints()
	    << ", knots: " << ts_bspline_num_knots(&spline)
	    << "}";
	return oss.str();
}



/*! @name Morphism
 *
 * @{
 */
tinyspline::Morphism::Morphism(const tinyspline::BSpline &source,
			       const tinyspline::BSpline &target,
			       real epsilon)
	: _source(source), _target(target), _epsilon(epsilon)
{
	sourceAligned = source.alignWith(target, targetAligned, epsilon);
	// Make buffer compatible by copying one of the aligned splines.
	buffer = sourceAligned;
}

tinyspline::BSpline
tinyspline::Morphism::eval(real t)
{
	tsStatus status;
	if (ts_bspline_morph(&sourceAligned.spline,
			     &targetAligned.spline,
			     t, _epsilon,
			     &buffer.spline, &status)) {
		throw std::runtime_error(status.message);
	}
	return buffer;
}

tinyspline::BSpline
tinyspline::Morphism::source() const
{
	return _source;
}

tinyspline::BSpline
tinyspline::Morphism::target() const
{
	return _target;
}

tinyspline::real
tinyspline::Morphism::epsilon() const
{
	return _epsilon;
}

tinyspline::BSpline
tinyspline::Morphism::operator()(real t)
{
	return eval(t);
}

std::string tinyspline::Morphism::toString() const
{
	std::ostringstream oss;
	oss << "Morphism{"
	    << "buffer: " << buffer.toString()
	    << ", epsilon: " << epsilon()
	    << "}";
	return oss.str();
}
/*! @} */



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
