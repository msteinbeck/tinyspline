#pragma once

#include "tinyspline.h"
#include <vector>
#include <string>

#ifndef TINYSPLINECXX_API
#define TINYSPLINECXX_API TINYSPLINE_API
#endif



/*! @name Swig Type Mapping
 *
 * Methods that do not return or set the value of a class attribute (let's call
 * these methods `non-accessor methods') must return/take instances of
 * std::vector as pointer. Otherwise, they aren't type mapped by Swig to the
 * std::vector representation of the target language.
 *
 * @{
 */
#ifdef SWIG
#define std_real_vector_in std::vector<tinyspline::real> *
#define std_real_vector_out std::vector<tinyspline::real> *
#else
#define std_real_vector_in std::vector<tinyspline::real> &
#define std_real_vector_out std::vector<tinyspline::real>
#endif
/*! @} */



/*! @name Emscripten Extensions
 *
 * @{
 */
#ifdef TINYSPLINE_EMSCRIPTEN
#include <stdexcept>
void inline cannotWrite() {
	throw std::runtime_error("cannot write read-only property");
}

std::string exceptionMessage(int ptr) {
	return std::string(reinterpret_cast<std::exception *>(ptr)->what());
}
#endif
/*! @} */



namespace tinyspline {

/*! @name API Configuration
 *
 * @{
 */
typedef tsReal real;
/*! @} */



/*! @name Vector Math
 *
 * Wrapper classes for TinySpline's vector math.
 *
 * @{
 */
class TINYSPLINECXX_API Vec2 {
public:
	Vec2();
	Vec2(real x, real y);
	Vec2(const Vec2 &other);

	Vec2 &operator=(const Vec2 &other);
	Vec2 operator+(const Vec2 &other);
	Vec2 operator-(const Vec2 &other);
	Vec2 operator*(real scalar);

	real x() const;
	void setX(real val);
	real y() const;
	void setY(real val);

	Vec2 add(const Vec2 &other) const;
	Vec2 subtract(const Vec2 &other) const;
	Vec2 multiply(real scalar) const;
	Vec2 norm() const;
	real magnitude() const;
	real dot(const Vec2 &other) const;
	real angle(const Vec2 &other) const;
	real distance(const Vec2 &other) const;

	std::string toString() const;

private:
	real m_vals[2];
};

class TINYSPLINECXX_API Vec3 {
public:
	Vec3();
	Vec3(real x, real y, real z);
	Vec3(const Vec3 &other);

	Vec3 &operator=(const Vec3 &other);
	Vec3 operator+(const Vec3 &other);
	Vec3 operator-(const Vec3 &other);
	Vec3 operator*(real scalar);

	real x() const;
	void setX(real val);
	real y() const;
	void setY(real val);
	real z() const;
	void setZ(real val);

	Vec3 add(const Vec3 &other) const;
	Vec3 subtract(const Vec3 &other) const;
	Vec3 multiply(real scalar) const;
	Vec3 cross(const Vec3 &other) const;
	Vec3 norm() const;
	real magnitude() const;
	real dot(const Vec3 &other) const;
	real angle(const Vec3 &other) const;
	real distance(const Vec3 &other) const;

	std::string toString() const;

private:
	real m_vals[3];
};

class TINYSPLINECXX_API Vec4 {
public:
	Vec4();
	Vec4(real x, real y, real z, real w);
	Vec4(const Vec4 &other);

	Vec4 &operator=(const Vec4 &other);
	Vec4 operator+(const Vec4 &other);
	Vec4 operator-(const Vec4 &other);
	Vec4 operator*(real scalar);

	real x() const;
	void setX(real val);
	real y() const;
	void setY(real val);
	real z() const;
	void setZ(real val);
	real w() const;
	void setW(real val);

	Vec4 add(const Vec4 &other) const;
	Vec4 subtract(const Vec4 &other) const;
	Vec4 multiply(real scalar) const;
	Vec4 norm() const;
	real magnitude() const;
	real dot(const Vec4 &other) const;
	real angle(const Vec4 &other) const;
	real distance(const Vec4 &other) const;

	std::string toString() const;

private:
	real m_vals[4];
};
/*! @} */



/*! @name Spline Framing
 *
 * Object-oriented representation of ::tsFrame (::Frame) and sequences of
 * ::tsFrame (::FrameSeq). Instances of ::FrameSeq are created by
 * ::BSpline::computeRMF.
 *
 * @{
 */
class TINYSPLINECXX_API Frame {
public:
	Frame(Vec3 &position,
	      Vec3 &tangent,
	      Vec3 &normal,
	      Vec3 &binormal);
	Frame(const Frame &other);
	Frame &operator=(const Frame &other);

	Vec3 position() const;
	Vec3 tangent() const;
	Vec3 normal() const;
	Vec3 binormal() const;

	std::string toString() const;

private:
	Vec3 m_position,
	     m_tangent,
	     m_normal,
	     m_binormal;
};

class TINYSPLINECXX_API FrameSeq {
public:
	FrameSeq(const FrameSeq &other);
	virtual ~FrameSeq();
	FrameSeq &operator=(const FrameSeq &other);

	size_t size() const;
	Frame at(size_t idx) const;

	std::string toString() const;

private:
	tsFrame *m_frames;
	size_t m_size;
	FrameSeq(tsFrame *frames,
	         size_t len);
	friend class BSpline;
};
/*! @} */



/*! @name Utility Classes
 *
 * Little helper classes, such as value classes or classes with only static
 * methods. These are primarily classes that do not really fit into another
 * group or are too small to form their own group.
 *
 * @{
 */
class TINYSPLINECXX_API Domain {
public:
	/* Constructors & Destructors */
	Domain(tsReal min, tsReal max);
	Domain(const Domain &other);

	/* Operators */
	Domain &operator=(const Domain &other);

	/* Accessors */
	real min() const;
	real max() const;

	/* Debug */
	std::string toString() const;

private:
	real m_min,
	     m_max;

#ifdef TINYSPLINE_EMSCRIPTEN
public:
	Domain()
	: m_min(TS_DOMAIN_DEFAULT_MIN),
	  m_max(TS_DOMAIN_DEFAULT_MAX)
	{}
	void setMin(real) { cannotWrite(); }
	void setMax(real) { cannotWrite(); }
#endif
};
/*! @} */



class TINYSPLINECXX_API DeBoorNet {
public:
	/* Constructors & Destructors */
	DeBoorNet(const DeBoorNet &other);
	~DeBoorNet();

	/* Operators */
	DeBoorNet & operator=(const DeBoorNet &other);

	/* Accessors */
	real knot() const;
	size_t index() const;
	size_t multiplicity() const;
	size_t numInsertions() const;
	size_t dimension() const;
	std::vector<real> points() const;
	std::vector<real> result() const;

	/**
	* Returns the result at \p idx as ::Vec2. Note that, by design, \p idx
	* cannot be greater than \c 1. It is safe to call this method even if
	* ::dimension is less than \c 2. In this case, the missing components
	* are set to \c 0. If ::dimension is greater than \c 2, the excess
	* values are ignored.
	*
	* @return
	* 	The result at \p idx as ::Vec2.
	* @throws std::out_of_range
	* 	If \p idx is greater than \c 1, or if \p idx is \c 1, but there
	* 	is only one result.
	*/
	Vec2 resultVec2(size_t idx = 0) const;

	/**
	* Returns the result at \p idx as ::Vec3. Note that, by design, \p idx
	* cannot be greater than \c 1. It is safe to call this method even if
	* ::dimension is less than \c 3. In this case, the missing components
	* are set to \c 0. If ::dimension is greater than \c 3, the excess
	* values are ignored.
	*
	* @return
	* 	The result at \p idx as ::Vec3.
	* @throws std::out_of_range
	* 	If \p idx is greater than \c 1, or if \p idx is \c 1, but there
	* 	is only one result.
	*/
	Vec3 resultVec3(size_t idx = 0) const;

	/**
	* Returns the result at \p idx as ::Vec4. Note that, by design, \p idx
	* cannot be greater than \c 1. It is safe to call this method even if
	* ::dimension is less than \c 4. In this case, the missing components
	* are set to \c 0. If ::dimension is greater than \c 4, the excess
	* values are ignored.
	*
	* @return
	* 	The result at \p idx as ::Vec4.
	* @throws std::out_of_range
	* 	If \p idx is greater than \c 1, or if \p idx is \c 1, but there
	* 	is only one result.
	*/
	Vec4 resultVec4(size_t idx = 0) const;

	/* Debug */
	std::string toString() const;

private:
	tsDeBoorNet net;

	/* Constructors & Destructors */
	explicit DeBoorNet(tsDeBoorNet &data);

	friend class BSpline;

#ifdef TINYSPLINE_EMSCRIPTEN
public:
	DeBoorNet() : net(ts_deboornet_init()) {}
	void setKnot(real) { cannotWrite(); }
	void setIndex(size_t) { cannotWrite(); }
	void setMultiplicity(size_t) { cannotWrite(); }
	void setNumInsertions(size_t) { cannotWrite(); }
	void setDimension(size_t) { cannotWrite(); }
	void setPoints(std::vector<real>) { cannotWrite(); }
	void setResult(std::vector<real>) { cannotWrite(); }
#endif
};



class Morphism;
class TINYSPLINECXX_API BSpline {
public:
	typedef tsBSplineType type;

	/* Constructors & Destructors */
	BSpline();
	BSpline(const BSpline &other);
	explicit BSpline(size_t numControlPoints, size_t dimension = 2,
		size_t degree = 3,
		tinyspline::BSpline::type type = TS_CLAMPED);
	~BSpline();

	/* Create from static method */
	static BSpline interpolateCubicNatural(const std_real_vector_in points,
		size_t dimension);
	static BSpline interpolateCatmullRom(const std_real_vector_in points,
		size_t dimension, tsReal alpha = (tsReal) 0.5f,
		std::vector<tinyspline::real> *first = NULL,
		std::vector<tinyspline::real> *last = NULL,
		tsReal epsilon = TS_CONTROL_POINT_EPSILON);
	static BSpline parseJson(std::string json);
	static BSpline load(std::string path);

	static bool knotsEqual(real x, real y);

	/* Operators */
	BSpline & operator=(const BSpline &other);
	DeBoorNet operator()(real u) const;

	/* Accessors */
	size_t degree() const;
	size_t order() const;
	size_t dimension() const;
	std::vector<real> controlPoints() const;
	Vec2 controlPointVec2At(size_t idx) const;
	Vec3 controlPointVec3At(size_t idx) const;
	Vec4 controlPointVec4At(size_t idx) const;
	std::vector<real> knots() const;
	real knotAt(size_t index) const;

	/* Query */
	size_t numControlPoints() const;
	DeBoorNet eval(real u) const;
	std_real_vector_out evalAll(const std_real_vector_in us) const;
	std_real_vector_out sample(size_t num = 0) const;
	DeBoorNet bisect(real value, real epsilon = TS_CONTROL_POINT_EPSILON,
		bool persnickety = false, size_t index = 0,
		bool ascending = true, size_t maxIter = 30) const;
	Domain domain() const;
	bool isClosed(real epsilon = TS_CONTROL_POINT_EPSILON) const;
	FrameSeq computeRMF(const std_real_vector_in knots,
	                    Vec3 *firstNormal = NULL) const;
	std_real_vector_out uniformKnotSeq(size_t num = 100) const;

	/* Serialization */
	std::string toJson() const;
	void save(std::string path) const;

	/* Modifications */
	void setControlPoints(const std::vector<real> &ctrlp);
	void setControlPointVec2At(size_t idx, Vec2 &cp);
	void setControlPointVec3At(size_t idx, Vec3 &cp);
	void setControlPointVec4At(size_t idx, Vec4 &cp);
	void setKnots(const std::vector<real> &knots);
	void setKnotAt(size_t index, real knot);

	/* Transformations */
	BSpline insertKnot(real u, size_t n) const;
	BSpline split(real u) const;
	BSpline tension(real tension) const;
	BSpline toBeziers() const;
	BSpline derive(size_t n = 1,
		real epsilon = TS_CONTROL_POINT_EPSILON) const;
	BSpline elevateDegree(size_t amount,
		real epsilon = TS_CONTROL_POINT_EPSILON) const;
	BSpline alignWith(const BSpline &other, BSpline &otherAligned,
		real epsilon = TS_CONTROL_POINT_EPSILON) const;
	Morphism morphTo(const BSpline &other,
		real epsilon = TS_CONTROL_POINT_EPSILON) const;

	/* Debug */
	std::string toString() const;

private:
	tsBSpline spline;

	/* Constructors & Destructors */
	explicit BSpline(tsBSpline &data);

	/* Needs to access ::spline. */
	friend class Morphism;

#ifdef TINYSPLINE_EMSCRIPTEN
public:
	std_real_vector_out sample0() const { return sample(); }
	std_real_vector_out sample1(size_t num) const { return sample(num); }
	BSpline derive0() const { return derive(); }
	BSpline derive1(size_t n) const { return derive(n); }
	BSpline derive2(size_t n, real eps) const { return derive(n, eps); }
#endif
};



/*! @name Spline Morphing
 *
 * @{
 */
class TINYSPLINECXX_API Morphism {
public:
	Morphism(const BSpline &origin,
		 const BSpline &target,
		 real epsilon = TS_CONTROL_POINT_EPSILON);

	BSpline origin() const;
	BSpline target() const;
	real epsilon() const;

	BSpline eval(real t);
	BSpline operator()(real t);

	std::string toString() const;
private:
	BSpline m_origin, m_target;
	real m_epsilon;
	BSpline m_originAligned, m_targetAligned;
	BSpline m_buffer;
};
/*! @} */

}



#ifdef TINYSPLINE_EMSCRIPTEN
// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
// https://emscripten.org/docs/api_reference/bind.h.html
#include <emscripten/bind.h>
using namespace emscripten;
using namespace tinyspline;
EMSCRIPTEN_BINDINGS(tinyspline) {
	function("exceptionMessage", &exceptionMessage);

	value_object<DeBoorNet>("DeBoorNet")
	        .field("knot", &DeBoorNet::knot, &DeBoorNet::setKnot)
	        .field("index", &DeBoorNet::index, &DeBoorNet::setIndex)
	        .field("multiplicity", &DeBoorNet::multiplicity,
	        	&DeBoorNet::setMultiplicity)
	        .field("numInsertions", &DeBoorNet::numInsertions,
	        	&DeBoorNet::setNumInsertions)
	        .field("dimension", &DeBoorNet::dimension,
	        	&DeBoorNet::setDimension)
	        .field("points", &DeBoorNet::points, &DeBoorNet::setPoints)
	        .field("result", &DeBoorNet::result, &DeBoorNet::setResult)
	;

	value_object<Domain>("Domain")
	        .field("min", &Domain::min, &Domain::setMin)
	        .field("max", &Domain::max, &Domain::setMax)
	;

	class_<BSpline>("BSpline")
	        .constructor<>()
	        //.constructor<BSpline>()
	        .constructor<size_t>()
	        .constructor<size_t, size_t>()
	        .constructor<size_t, size_t, size_t>()
	        .constructor<size_t, size_t, size_t, BSpline::type>()

	        .class_function("interpolateCubicNatural",
			&BSpline::interpolateCubicNatural)
	        .class_function("interpolateCatmullRom",
			&BSpline::interpolateCatmullRom,
			allow_raw_pointers())
	        .class_function("parseJson", &BSpline::parseJson)

	        .property("degree", &BSpline::degree)
	        .property("order", &BSpline::order)
	        .property("dimension", &BSpline::dimension)
	        .property("controlPoints", &BSpline::controlPoints,
			&BSpline::setControlPoints)
	        .property("knots", &BSpline::knots, &BSpline::setKnots)
	        .property("domain", &BSpline::domain)

	        /* Property by index */
	        .function("knotAt", &BSpline::knotAt)
	        .function("setKnotAt", &BSpline::setKnotAt)

	        /* Query */
	        .function("numControlPoints", &BSpline::numControlPoints)
	        .function("eval", &BSpline::eval)
	        .function("evalAll", &BSpline::evalAll)
	        .function("sample",
			select_overload<std_real_vector_out() const>
			(&BSpline::sample0))
	        .function("sample",
			select_overload<std_real_vector_out(size_t) const>
			(&BSpline::sample1))
	        .function("sample", &BSpline::sample)
	        .function("bisect", &BSpline::bisect)
	        .function("isClosed", &BSpline::isClosed)

		/* Serialization */
	        .function("toJson", &BSpline::toJson)

	        /* Transformations */
	        .function("insertKnot", &BSpline::insertKnot)
	        .function("split", &BSpline::split)
	        .function("tension", &BSpline::tension)
	        .function("toBeziers", &BSpline::toBeziers)
	        .function("derive",
			select_overload<BSpline() const>
			(&BSpline::derive0))
	        .function("derive",
			select_overload<BSpline(size_t) const>
			(&BSpline::derive1))
	        .function("derive",
			select_overload<BSpline(size_t, real) const>
			(&BSpline::derive2))

	        /* Debug */
	        .function("toString", &BSpline::toString)
	;

	enum_<BSpline::type>("BSplineType")
	        .value("OPENED", BSpline::type::TS_OPENED)
	        .value("CLAMPED", BSpline::type::TS_CLAMPED)
	        .value("BEZIERS", BSpline::type::TS_BEZIERS)
	;
}

// Map: std::vector <--> JS array
// https://github.com/emscripten-core/emscripten/issues/11070#issuecomment-717675128
namespace emscripten {
namespace internal {
	template <typename T, typename Allocator>
	struct BindingType<std::vector<T, Allocator>> {
		using ValBinding = BindingType<val>;
		using WireType = ValBinding::WireType;

		static WireType toWireType(
				const std::vector<T, Allocator> &vec) {
			return ValBinding::toWireType(val::array(vec));
		}

		static std::vector<T, Allocator> fromWireType(WireType value) {
			return vecFromJSArray<T>(
				ValBinding::fromWireType(value));
		}
	};

	template <typename T>
	struct TypeID<T,
		typename std::enable_if_t<std::is_same<
		        typename Canonicalized<T>::type,
		std::vector<typename Canonicalized<T>::type::value_type,
		typename Canonicalized<T>::type::allocator_type>>::value>> {

		static constexpr TYPEID get() {
			return TypeID<val>::get();
		}
	};
}  // namespace internal
}  // namespace emscripten
#endif
